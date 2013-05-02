//Generate by python.
/**
 * @file
 * @brief
 *
 * @details 
 * @author  
 * @date    
 * @version 
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sougou_dict.h"

///////////////////////////////////////////////////////////////////////////
//Helpher interface declare
///Print string and rip '\r'
void helpher_puts(uint8_t* str);

///Convert pinyin id array to pinyin string
void helpher_convert_pinyin_ids_to_string(
        pinyin_list* lppinyin_list,
        uint16_t* pinyin_ids,
        uint16_t size, 
        uint8_t* out);

///Convert unicode wchar_t to utf-8 chars
int helpher_ucs2_to_utf8_char(
        unsigned short in,
        unsigned char * out);

///Convert unicode string to utf-8 string
int helpher_ucs2_to_utf8_str(
        uint8_t* in, 
        int insize,
        uint8_t * out);

///Read uint8_t from file
int helpher_fread_u8(uint8_t* u8, FILE* fp);

///Read uint16_t from file
int helpher_fread_u16(uint16_t* u16, FILE* fp);

///Read uint8_t * size from file
int helpher_fread_str(uint8_t* str, uint16_t size, FILE* fp);

///////////////////////////////////////////////////////////////////////////
//Helpher interface define
void helpher_puts(uint8_t* str)
{
    putchar(';');
    putchar(' ');
    for (;*str != '\0';str++)
    {
        if (*str == '\n')
        {
            putchar(*str);
            putchar(';');
        }
        else if ((*str == '\r') && (*(str + 1) == '\n'))
        {
            continue;
        }
        else if ((*str == '\r') && (*(str + 1) != '\n'))
        {
            putchar('\n');
            putchar(';');
        }
        else
        {
            putchar(*str);
        }
    }
    putchar('\n');
}

void helpher_convert_pinyin_ids_to_string(
        pinyin_list* lppinyin_list,
        uint16_t* pinyin_ids,
        uint16_t size, 
        uint8_t* out)
{
    int i = 0;
    pinyin_list* lppinyin_tmp = NULL;
    
    for (i = 0;i < size;i++)
    {
        for (lppinyin_tmp = lppinyin_list;
            lppinyin_tmp != NULL;
            lppinyin_tmp = lppinyin_tmp->pnext)
        {
            if (lppinyin_tmp->id == pinyin_ids[i])
            {
                break;
            }
        }

        if (lppinyin_tmp != NULL)
        {
            strcat(out, "'");
            strcat(out, lppinyin_tmp->data);
        }
        else
        {
            break;
        }
    }
}

int helpher_ucs2_to_utf8_char(unsigned short in, unsigned char * out)
{
    if (in <= 0x007fu)
    {
        *out=in;
        return 0;
    }
    else if (in >= 0x0080u && in <= 0x07ffu)
    {
        *out = 0xc0 | (in >> 6);
        out ++;
        *out = 0x80 | (in & (0xff >> 2));
        return 0;
    }
    else if (in >= 0x0800u)
    {
        *out = 0xe0 | (in >> 12);
        out ++;
        *out = 0x80 | (in >> 6 & 0x003f);
        out ++;
        *out = 0x80 | (in & (0xff >> 2));
        return 0;
    }
    return 0;
}

int helpher_ucs2_to_utf8_str(
        uint8_t* in, 
        int insize,
        uint8_t * out)
{
	uint8_t     str[16]={0};
    uint16_t*   tmp = (uint16_t*)malloc(insize);

	int i;
	
	*out='\0';
	memcpy(tmp,in,insize);
	
	for (i = 0;i < insize / 2;i++)
	{
		memset(str,0,sizeof(str));
		helpher_ucs2_to_utf8_char(tmp[i], str);
		strcat(out,str);
	}
	return 0;	
}

int helpher_fread_u8(uint8_t* u8, FILE* fp)
{
    return fread(u8, 1, 1, fp);
}

int helpher_fread_u16(uint16_t* u16, FILE* fp)
{
    return fread(u16, 1, 2, fp);
}

int helpher_fread_str(uint8_t* str, uint16_t size, FILE* fp)
{
    return fread(str, 1, size, fp);
}

///////////////////////////////////////////////////////////////////////////
//Global interface define
pinyin_list* DecodeSGWordPinyin(FILE* fp)
{
    SG_PINYINS_HEADER sg_pinyins_header;
    SG_PINYIN         sg_pinyin;
    SG_PINYIN_DEC     sg_pinyin_dec;
    pinyin_list*      lppinyin_list = NULL;
    pinyin_list*      lppinyin_next = NULL;

    int id = 0;

    uint8_t buffer[64];

    int nrOfBytes = 0;

    fseek(fp, SG_OFFSET_PINYINS_HEADER, SEEK_SET);
    fread(&sg_pinyins_header, SG_PINYINS_HEADER_MAGIC_SIZE, 1, fp);
    if (memcmp(sg_pinyins_header.magic,
                SG_PINYINS_HEADER_MAGIC_DATA,
                SG_PINYINS_HEADER_MAGIC_SIZE) != 0)
    {
        printf("Not Sougou words...\n");
        return NULL;
    }
    else
    {
        //printf(" * library_pinyin_table:\n");
    }

    fseek(fp, SG_OFFSET_PINYIN_BEGIN, SEEK_SET);

    lppinyin_list = (pinyin_list*)malloc(sizeof(pinyin_list));
    lppinyin_next = lppinyin_list;
    lppinyin_next->pnext = NULL;

    //Read words
    while(1)
    {
        memset(&sg_pinyin, 0, sizeof(sg_pinyin));
        memset(&sg_pinyin_dec, 0, sizeof(sg_pinyin_dec));

        //Read info
        nrOfBytes += helpher_fread_u16(&sg_pinyin.code, fp);
        nrOfBytes += helpher_fread_u16(&sg_pinyin.size, fp);

        if ((nrOfBytes <= 0) || (feof(fp)))
        {
            break;
        }

        //Read word
        nrOfBytes = 0;
        nrOfBytes += helpher_fread_str(sg_pinyin.pinyin, 
                sg_pinyin.size,
                fp);

        if ((nrOfBytes <= 0) || (feof(fp)))
        {
            break;
        }

        //Convert
        sg_pinyin_dec.code = sg_pinyin.code;
        helpher_ucs2_to_utf8_str(sg_pinyin.pinyin, 
                SG_PINYIN_MAX,
                sg_pinyin_dec.pinyin);
        sg_pinyin_dec.size = strlen(sg_pinyin_dec.pinyin);

        lppinyin_next->id    = id;
        lppinyin_next->data  = strdup(sg_pinyin_dec.pinyin);

        if (strcmp(sg_pinyin_dec.pinyin, "zuo") == 0)
        {
            break;
        }
        else
        {
            //printf("%s\n",sg_pinyin_dec.pinyin);
            lppinyin_next->pnext = (pinyin_list*)malloc(sizeof(pinyin_list));
            lppinyin_next = lppinyin_next->pnext;
            lppinyin_next->pnext = NULL;

            id++;
        }
    }

    return lppinyin_list;
}

//#define DEBUG_STEP
#define CHECK_EOF_ERROR(fp) \
    if (feof(fp)) \
    { \
        break;\
    }

void DecodeSGWordWords(FILE* fp, uint16_t hz_offset, pinyin_list* lppinyin_list)
{
    int nrOfBytes = 0;
    SG_WORDS_HEADER* sg_words_header = 
        (SG_WORDS_HEADER*)malloc(sizeof(SG_WORDS_HEADER));
    SG_WORDS_HEADER_DEC* sg_words_header_dec = 
        (SG_WORDS_HEADER_DEC*)malloc(sizeof(SG_WORDS_HEADER_DEC));

    int i = 0;
    int j = 0;

    //Jump to words
    fseek(fp, hz_offset, SEEK_SET);

    printf("; * library_words_table:\n");

    while(!feof(fp))
    {
#ifdef DEBUG_STEP
        printf("0x%08x\n", ftell(fp));
        fflush(stdout);
#endif
        memset(sg_words_header, 0, sizeof(SG_WORDS_HEADER));
        memset(sg_words_header_dec, 0, sizeof(SG_WORDS_HEADER_DEC));

        nrOfBytes = helpher_fread_u16(
                &sg_words_header->words_count,
                fp);
        CHECK_EOF_ERROR(fp);

        nrOfBytes = helpher_fread_u16(
                &sg_words_header->pinyins_count,
                fp);
        CHECK_EOF_ERROR(fp);

        sg_words_header->pinyins_count = 
            (sg_words_header->pinyins_count & 0xFF) / 2;

#ifdef DEBUG_STEP
        printf("words_count   %d\n", sg_words_header->words_count);
        printf("pinyins_count %d\n", sg_words_header->pinyins_count);
        fflush(stdout);
#endif

        if (sg_words_header->pinyins_count > sizeof(sg_words_header->pinyins_id))
        {
            printf("pinyins_id overflow %d", sg_words_header->pinyins_count);
            break;
        }

        //Read pinyin
        for (i = 0;i < sg_words_header->pinyins_count;i++)
        {
            nrOfBytes = helpher_fread_u16(
                    sg_words_header->pinyins_id + i,
                    fp);
            CHECK_EOF_ERROR(fp);
        }

        //Convert
        helpher_convert_pinyin_ids_to_string(lppinyin_list,
                sg_words_header->pinyins_id,
                sg_words_header->pinyins_count,
                sg_words_header_dec->pinyins_id);

        //Read word data
        for (j = 0;j < sg_words_header->words_count;j++)
        {
            nrOfBytes = helpher_fread_u16(
                    &(sg_words_header->words_len),
                    fp);
            CHECK_EOF_ERROR(fp);

            sg_words_header->words_len = 
                (sg_words_header->words_len & 0xFF);

#ifdef DEBUG_STEP
            printf("words_len     %d\n", sg_words_header->words_len);
            fflush(stdout);
#endif

            if (sg_words_header->words_count > sizeof(sg_words_header->words_data))
            {
                printf("words_data overflow %d", sg_words_header->words_count);
                break;
            }

            nrOfBytes = helpher_fread_str(
                    sg_words_header->words_data,
                    sg_words_header->words_len,
                    fp);
            CHECK_EOF_ERROR(fp);


#ifdef DEBUG_STEP
            printf("Read freq\n");
            fflush(stdout);
#endif

            nrOfBytes = helpher_fread_str(
                    sg_words_header->words_freq,
                    sizeof(sg_words_header->words_freq),
                    fp);
            CHECK_EOF_ERROR(fp);

            //Convert
            helpher_ucs2_to_utf8_str(sg_words_header->words_data,
                    sg_words_header->words_len,
                    sg_words_header_dec->words_data);
            sg_words_header_dec->words_len = strlen(sg_words_header_dec->words_data);


            //Print
            printf("%s ", sg_words_header_dec->pinyins_id);
            printf("%s\n", sg_words_header_dec->words_data);
        }
    }

    free(sg_words_header);
    free(sg_words_header_dec);
}

void DecodeSGDict(FILE* fp)
{
    SG_FILE_HEADER              sg_header;
    SG_LIBRARY_HEADER           sg_lib_header;
    SG_LIBRARY_HEADER_DECODER   sg_lib_header_dec;
    pinyin_list*                lppinyin_list = NULL;
    pinyin_list*                lppinyin_next = NULL;
    uint16_t hz_offset = 0;

    //File header
    fseek(fp, SG_OFFSET_FILE_HEADER, SEEK_SET);
    fread(&sg_header, SG_HEADER_MAGIC_SIZE, 1, fp);
    if ((memcmp(sg_header.magic, SG_HEADER_MAGIC_DATA1, SG_HEADER_MAGIC_SIZE) != 0) &&
        (memcmp(sg_header.magic, SG_HEADER_MAGIC_DATA2, SG_HEADER_MAGIC_SIZE) != 0))
    {
        printf("Not Sougou dict...\n");
        return;
    }
    
    ///////////////////////////////////////////////////////////////////////////
    //Library name
    fseek(fp, SG_OFFSET_LIBRARY_HEADER, SEEK_SET);
    fread(&sg_lib_header.library_name, SG_LIBRARY_HEADER_SIZE, 1, fp);

    //Library category
    fseek(fp, SG_OFFSET_LIBRARY_CATEGORY, SEEK_SET);
    fread(&sg_lib_header.library_category, SG_LIBRARY_CATEGORY_SIZE, 1, fp);

    //Library information
    fseek(fp, SG_OFFSET_LIBRARY_INFORMATION, SEEK_SET);
    fread(&sg_lib_header.library_information, SG_LIBRARY_INFORMATION_SIZE, 1, fp);

    //Library example
    fseek(fp, SG_OFFSET_LIBRARY_EXAMPLE, SEEK_SET);
    fread(&sg_lib_header.library_example, SG_LIBRARY_EXAMPLE_SIZE, 1, fp);


    //Convert to utf-8
    helpher_ucs2_to_utf8_str(
            sg_lib_header.library_name,
            SG_LIBRARY_HEADER_SIZE,
            sg_lib_header_dec.library_name);

    helpher_ucs2_to_utf8_str(
            sg_lib_header.library_category,
            SG_LIBRARY_CATEGORY_SIZE,
            sg_lib_header_dec.library_category);

    helpher_ucs2_to_utf8_str(
            sg_lib_header.library_information,
            SG_LIBRARY_INFORMATION_SIZE,
            sg_lib_header_dec.library_information);

    helpher_ucs2_to_utf8_str(
            sg_lib_header.library_example,
            SG_LIBRARY_EXAMPLE_SIZE,
            sg_lib_header_dec.library_example);


    printf("; * library_name        : %s\n", sg_lib_header_dec.library_name);
    printf("; * library_category    : %s\n", sg_lib_header_dec.library_category);
    printf("; * library_information : %s\n", sg_lib_header_dec.library_information);
    printf("; * library_example:\n");
    
    helpher_puts(sg_lib_header_dec.library_example);


    lppinyin_list = DecodeSGWordPinyin(fp);
    if (lppinyin_list == NULL)
    {
        return;
    }

    if (sg_header.magic[4] == 0x44)
    {
        hz_offset = 0x2628;
    }
    else if (sg_header.magic[4] == 0x45)
    {
        hz_offset = 0x26c4;
    }
    else
    {
        return;
    }

    DecodeSGWordWords(fp, hz_offset, lppinyin_list);

    if (lppinyin_list->pnext == NULL)
    {
        free(lppinyin_list->data);
        free(lppinyin_list);
    }
    else
    {
        lppinyin_next = lppinyin_list->pnext;
        while(lppinyin_next != NULL)
        {
            free(lppinyin_list->data);
            free(lppinyin_list);

            lppinyin_list = lppinyin_next;
            lppinyin_next = lppinyin_next->pnext;
        }
    }
}

/**
 * @brief program entry.
 * @details
 *        The first function call by systm.
 * @param[in]   argc number of command line arguments.
 * @param[in]   argv array of command line arguments.
 * @return      any int value.
 */
int main(int argc, const char* argv[])
{
    FILE* fp = NULL;
    //Adding source below.
    if (argc != 2)
    {
        printf("Need input file!\n");
        return -1;
    }
    else
    {
        fp = fopen(argv[1], "rb");
        if (fp == NULL)
        {
            perror("Can't open file.");
            return -1;
        }
        else
        {
            DecodeSGDict(fp);
            fclose(fp);
            fp = NULL;
        }
    }

    return 0;
}

