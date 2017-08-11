/* vim: set fileencoding=utf-8:
 *
 *                   GNU GENERAL PUBLIC LICENSE
 *                       Version 2, June 1991
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "libsgscel_inl.h"
#include "libsgscel.h"

///////////////////////////////////////////////////////////////////////////
#define ENDIAN_B2E_U16(u16) \
    ((((u16) & 0x00FFu) << 8) | (((u16) & 0xFF00u) >> 8))

///Convert pinyin id array to pinyin string
static void _decoder_pinyin_id_to_str(
        const sgscel_pinyin_lookup_list_t* pinyins_tables,
        uint16_t* pinyin_ids,
        uint16_t size, 
        uint8_t* out);

///Convert unicode wchar_t to utf-8 chars
static int _encoder_ucs2_to_utf8(
        unsigned short in,
        unsigned char * out);

///Convert unicode string to utf-8 string
static int _encoder_ucs2str_to_utf8str(
        uint8_t* in, 
        int insize,
        uint8_t * out);

///Read uint8_t from file
static int _fread_u8(uint8_t* u8, FILE* fp);

///Read uint16_t from file
static int _fread_u16(uint16_t* u16, FILE* fp);

///Read uint8_t * size from file
static int _fread_str(uint8_t* str, uint16_t size, FILE* fp);

static sgscel_pinyin_lookup_list_t* _load_pinyin_list(FILE* fp);
static void _pinyin_list_destroy(sgscel_pinyin_lookup_list_t* root);

static sgscel_word_t* _load_words(FILE* fp, const sgscel_pinyin_lookup_list_t* lppinyin_list);

///////////////////////////////////////////////////////////////////////////
void _decoder_pinyin_id_to_str(
        const sgscel_pinyin_lookup_list_t* pinyins_tables,
        uint16_t* pinyin_ids,
        uint16_t size, 
        uint8_t* out)
{
    int i = 0;
    int empty_target = 1;
    const sgscel_pinyin_lookup_list_t* p = NULL;
    
    for (i = 0;i < size;i++) {
        for (p = pinyins_tables;p != NULL; p = p->next) {
            if (p->id == pinyin_ids[i]) {
                break;
            }
        }

        if (p != NULL) {
            if (empty_target) {
                empty_target = 0;
            } else {
                strcat(out, ",");
            }
            strcat(out, p->str);
        } else {
            break;
        }
    }
}

int _encoder_ucs2_to_utf8(unsigned short in, unsigned char * out)
{
    if (in <= 0x007fu) {
        *out=in;
        return 0;
    } else if (in >= 0x0080u && in <= 0x07ffu) {
        *out = 0xc0 | (in >> 6);
        out ++;
        *out = 0x80 | (in & (0xff >> 2));
        return 0;
    } else if (in >= 0x0800u) {
        *out = 0xe0 | (in >> 12);
        out ++;
        *out = 0x80 | (in >> 6 & 0x003f);
        out ++;
        *out = 0x80 | (in & (0xff >> 2));
        return 0;
    }
    return 0;
}

int _encoder_ucs2str_to_utf8str(
        uint8_t* in, 
        int insize,
        uint8_t * out)
{
	uint8_t     str[16]={0};
    uint16_t*   tmp = (uint16_t*)malloc(insize);

	int i;
	
	*out='\0';
	memcpy(tmp,in,insize);
	
	for (i = 0;i < insize / 2;i++) {
		memset(str,0,sizeof(str));
		_encoder_ucs2_to_utf8(tmp[i], str);
		strcat(out,str);
	}
	return 0;	
}

int _fread_u8(uint8_t* u8, FILE* fp)
{
    return fread(u8, 1, 1, fp);
}

int _fread_u16(uint16_t* u16, FILE* fp)
{
    return fread(u16, 1, 2, fp);
}

int _fread_str(uint8_t* str, uint16_t size, FILE* fp)
{
    return fread(str, 1, size, fp);
}

///////////////////////////////////////////////////////////////////////////
sgscel_pinyin_lookup_list_t* _load_pinyin_list(FILE* fp)
{
    sgscel_bin_data_header_t    data;
    sgscel_bin_pinyin_lookup_t           bin_word;
    sgscel_pinyin_lookup_t               word;
    sgscel_pinyin_lookup_list_t*              pinyins_tables = NULL;
    sgscel_pinyin_lookup_list_t*              tail = NULL;

    int id = 0;
    uint8_t buffer[64];
    int nr_bytes = 0;

    fseek(fp, OFFSET_BIN_DATA, SEEK_SET);
    fread(&data, SIZEOF_SGSCEL_BIN_DATA_MAGIC, 1, fp);
    if (memcmp(data.magic, SGSCEL_BIN_DATA_MAGIC, SIZEOF_SGSCEL_BIN_DATA_MAGIC) != 0) {
        printf("Error: Not support this dict type!\n");
        return NULL;
    }

    fseek(fp, OFFSET_PINYINS_MAPING, SEEK_SET);
    pinyins_tables = (sgscel_pinyin_lookup_list_t*)malloc(sizeof(sgscel_pinyin_lookup_list_t));
    tail = pinyins_tables;
    tail->next = NULL;

    //Read pinyins_tables
    while(1) {
        memset(&bin_word, 0, sizeof(bin_word));
        memset(&word, 0, sizeof(word));

        //Read info
        nr_bytes += _fread_u16(&bin_word.code, fp);
        nr_bytes += _fread_u16(&bin_word.size, fp);

        if ((nr_bytes <= 0) || (feof(fp))) {
            break;
        }

        //Read word
        nr_bytes = 0;
        nr_bytes += _fread_str(bin_word.pinyin, bin_word.size, fp);

        if ((nr_bytes <= 0) || (feof(fp))) {
            break;
        }

        //Convert
        word.code = bin_word.code;
        _encoder_ucs2str_to_utf8str(bin_word.pinyin, MAX_PINYIN_STRING_LENGTH, word.pinyin);
        word.size = strlen(word.pinyin);

        tail->id    = id;
        tail->code  = word.code;
        tail->str   = strdup(word.pinyin);

        if (strcmp(word.pinyin, "zuo") == 0) {
            break;
        } else {
            //printf("%s\n",word.pinyin);
            tail->next = (sgscel_pinyin_lookup_list_t*)malloc(sizeof(sgscel_pinyin_lookup_list_t));
            tail = tail->next;
            tail->next = NULL;

            id++;
        }
    }

    return pinyins_tables;
}

void _pinyin_list_destroy(sgscel_pinyin_lookup_list_t* root)
{
    sgscel_pinyin_lookup_list_t* p;

    while (root != NULL) {
        p = root;
        root = root->next;

        if (p->str) {
            free(p->str);
        }
        free(p);
    }
}


sgscel_word_t* _load_words(FILE* fp, const sgscel_pinyin_lookup_list_t* pinyins_tables)
{
    sgscel_bin_words_header_t rawword;
    sgscel_word_inl_t word;
    sgscel_word_t* root = NULL;
    sgscel_word_t* p;
    int nr_bytes = 0;
    int i = 0;
    int j = 0;
    int freq_offs = 0;

    while (!feof(fp)) {
        memset(&rawword, 0, sizeof(sgscel_bin_words_header_t));
        memset(&word, 0, sizeof(sgscel_word_t));

        nr_bytes = _fread_u16(&rawword.words_count, fp);
        nr_bytes = _fread_u16(&rawword.pinyins_count, fp);
        if (feof(fp)) {
            break;
        }

        rawword.pinyins_count = (rawword.pinyins_count & 0xFF) / 2;
        if (rawword.pinyins_count > sizeof(rawword.pinyins_id)) {
            break;
        }

        for (i = 0;i < rawword.pinyins_count;i++) {
            nr_bytes = _fread_u16(rawword.pinyins_id + i, fp);
        }
        if (feof(fp)) {
            break;
        }
        _decoder_pinyin_id_to_str(pinyins_tables, rawword.pinyins_id, rawword.pinyins_count, word.pinyin);

        //Read word data
        for (j = 0;j < rawword.words_count;j++) {
            nr_bytes = _fread_u16(&(rawword.words_len), fp);
            if (feof(fp)) {
                break;
            }

            rawword.words_len = (rawword.words_len & 0xFF);
            if (rawword.words_count > sizeof(rawword.words_data)) {
                break;
            }
            nr_bytes = _fread_str(rawword.words_data, rawword.words_len, fp);
            if (feof(fp)) {
                break;
            }

            nr_bytes = _fread_str(rawword.words_freq, sizeof(rawword.words_freq), fp);
            if (feof(fp)) {
                break;
            }

            word.frequency = 0;
            for (freq_offs = 11;freq_offs >= 0;freq_offs--) {
                word.frequency = word.frequency * 256 + rawword.words_freq[freq_offs];
            }

            _encoder_ucs2str_to_utf8str(rawword.words_data, rawword.words_len, word.string);

            //append to list
            if (root == NULL) {
                root = (sgscel_word_t*)malloc(sizeof(sgscel_word_t));
                p = root;
            } else {
                p->next = (sgscel_word_t*)malloc(sizeof(sgscel_word_t));
                p = p->next;
            }
            memset(p, 0, sizeof(sgscel_word_t));
            p->pinyin = strdup(word.pinyin);
            p->string = strdup(word.string);
            p->frequency = word.frequency;
        }
    }

    return root;

ERROR:
    while (root) {
        p = root;
        root = root->next;
        if (p->pinyin) {
            free(p->pinyin);
        }
        if (p->string) {
            free(p->string);
        }
        free(p);
    }

    return NULL;
}

sgscel_db_t* sgscel_load(const char* dbname)
{
    FILE* fp;
    sgscel_db_t*                    result;
    sgscel_bin_filehdr_t            scel_hdr;
    sgscel_bin_fileinfo_t           scel_info;
    sgscel_fileinfo_t               info;
    sgscel_pinyin_lookup_list_t*    pinyins = NULL;
    sgscel_word_t*                  words = NULL;
    uint16_t hz_offset = 0;

    fp = fopen(dbname, "rb");
    if (fp == NULL) {
        perror("Can't open file.");
        return NULL;
    }

    fseek(fp, OFFSET_BIN_FILE_HEADER, SEEK_SET);
    fread(&scel_hdr, SIZEOF_FILE_MAGIC, 1, fp);
    if ((memcmp(scel_hdr.magic, SGSCEL_FILE_MAGIC1, SIZEOF_FILE_MAGIC) != 0) &&
        (memcmp(scel_hdr.magic, SGSCEL_FILE_MAGIC2, SIZEOF_FILE_MAGIC) != 0)) {
        printf("Error: not sougou dict...\n");
        fclose(fp);
        fp = NULL;
        return NULL;
    }
    
    fseek(fp, OFFSET_BIN_INFO_NAME, SEEK_SET);
    fread(&scel_info.name, SIZEOF_BIN_INFO_NAME, 1, fp);
    _encoder_ucs2str_to_utf8str(scel_info.name, SIZEOF_BIN_INFO_NAME, info.name);

    fseek(fp, OFFSET_BIN_INFO_CATEGORY, SEEK_SET);
    fread(&scel_info.category, SIZEOF_BIN_INFO_CATEGORY, 1, fp);
    _encoder_ucs2str_to_utf8str(scel_info.category, SIZEOF_BIN_INFO_CATEGORY, info.category);

    fseek(fp, OFFSET_BIN_INFO_DESCRIPTION, SEEK_SET);
    fread(&scel_info.description, SIZEOF_BIN_INFO_DESCRIPTION, 1, fp);
    _encoder_ucs2str_to_utf8str(scel_info.description, SIZEOF_BIN_INFO_DESCRIPTION, info.description);

    fseek(fp, OFFSET_BIN_INFO_EXAMPLE, SEEK_SET);
    fread(&scel_info.example, SIZEOF_BIN_INFO_EXAMPLE, 1, fp);
    _encoder_ucs2str_to_utf8str(scel_info.example, SIZEOF_BIN_INFO_EXAMPLE, info.example);

    ///////////////////////////////////////////////////////////////////////////
    pinyins = _load_pinyin_list(fp);
    if (pinyins == NULL) {
        printf("Error: load_pinyin_list\n");
        fclose(fp);
        fp = NULL;
        return NULL;
    }

    if (scel_hdr.magic[4] == 0x44) {
        hz_offset = 0x2628;
    } else if (scel_hdr.magic[4] == 0x45) {
        hz_offset = 0x26c4;
    }

    fseek(fp, hz_offset, SEEK_SET);
    words = _load_words(fp, pinyins);
    _pinyin_list_destroy(pinyins);
    if (words == NULL) {
        printf("Error: nothing load\n");
        fclose(fp);
        fp = NULL;
        return NULL;
    }

    result = (sgscel_db_t*)malloc(sizeof(sgscel_db_t));
    result->file        = strdup(dbname);
    result->name        = strdup(info.name);
    result->category    = strdup(info.category);
    result->description = strdup(info.description);
    result->example     = strdup(info.example);
    result->words = words;

    fclose(fp);
    fp = NULL;
    return result;
}

void sgscel_destroy(sgscel_db_t* db)
{
    sgscel_word_t* p;

    while (db->words != NULL) {
        p = db->words;
        db->words = db->words->next;
        if (p->pinyin) {
            free(p->pinyin);
        }
        if (p->string) {
            free(p->string);
        }
        free(p);
    }
    if (db->name) {
        free(db->name);
    }
    if (db->category) {
        free(db->category);
    }
    if (db->description) {
        free(db->description);
    }
    if (db->example) {
        free(db->example);
    }
    free(db);
}

