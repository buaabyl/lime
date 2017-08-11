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
 * SCEL format:
 *  @0x0000_0000
 *  uint8_t magic       [8];
 *  uint8_t unknown     [296];
 *
 *  @0x0000_0130
 *  uint8_t name        [520];
 *  uint8_t category    [520];
 *  uint8_t description [2048];
 *  uint8_t example     [2048];
 *
 *  @0x0000_1540
 *  struct pinyin_maping {
 *      uint8_t magic   [4];    // low->high: 0x9D,0x01,0x00,0x00
 *
 *      @0x0000_1544
 *      struct words {
 *          uint16_t code;
 *          uint16_t size;
 *          uint8_t  data[size];
 *      } [N]
 *  }
 *
 *  @0x0000_26C4 or @0x0000_2628
 *  struct words {
 *               offset  label
 *      -------------------------------
 *      header   0x00    words_count 
 *               0x01    (reserved)
 *               0x02    pinyins_count
 *               0x03    (reserved)
 *  
 *      // this pinyins_id value is pinyin_lookup.code
 *      pinyin   0x04    pinyins_id[0]
 *                       ...
 *                       pinyins_id[pinyins_count-1]
 *  
 *      word[0]  0x?0    words_len
 *               0x?1    (reserved)
 *               0x?2    words_data
 *                       ...
 *               0x?0    words_freq[12]
 *  
 *      word[1]  0x?0    words_len
 *               0x?1    (reserved)
 *               0x?2    words_data
 *                       ...
 *               0x?0    words_freq[12]
 *  }[M]
 */
#ifndef LIBSGSCEL_INL_H_23CA74D9_DF44_2574_425F_6389E62CE181_INCLUDED_
#define LIBSGSCEL_INL_H_23CA74D9_DF44_2574_425F_6389E62CE181_INCLUDED_


///////////////////////////////////////////////////////////////////////////
#define MAX_UTF8_CHAR_SIZE 4

///////////////////////////////////////////////////////////////////////////
#define OFFSET_BIN_FILE_HEADER  0x00
#define SIZEOF_FILE_MAGIC       8

static const uint8_t SGSCEL_FILE_MAGIC1[SIZEOF_FILE_MAGIC] = 
    {0x40,0x15,0x00,0x00,0x44,0x43,0x53,0x01};

static const uint8_t SGSCEL_FILE_MAGIC2[SIZEOF_FILE_MAGIC] = 
    {0x40,0x15,0x00,0x00,0x45,0x43,0x53,0x01};

typedef struct {
    uint8_t magic[SIZEOF_FILE_MAGIC];
}sgscel_bin_filehdr_t;



//Library Header
#define OFFSET_BIN_INFO_NAME        0x130
#define SIZEOF_BIN_INFO_NAME        0x208

//Library Category
#define OFFSET_BIN_INFO_CATEGORY    0x338
#define SIZEOF_BIN_INFO_CATEGORY    0x208

//Library Information
#define OFFSET_BIN_INFO_DESCRIPTION 0x540
#define SIZEOF_BIN_INFO_DESCRIPTION 0x800

//Library Example
#define OFFSET_BIN_INFO_EXAMPLE     0xd40
#define SIZEOF_BIN_INFO_EXAMPLE     0x800

//Struct read from file
typedef struct {
    //encode in unicode
    uint8_t name[SIZEOF_BIN_INFO_NAME];
    uint8_t category[SIZEOF_BIN_INFO_CATEGORY];
    uint8_t description[SIZEOF_BIN_INFO_DESCRIPTION];
    uint8_t example[SIZEOF_BIN_INFO_EXAMPLE];
}sgscel_bin_fileinfo_t;



#define OFFSET_BIN_DATA                 0x1540
#define SIZEOF_SGSCEL_BIN_DATA_MAGIC    4

static const uint8_t SGSCEL_BIN_DATA_MAGIC[SIZEOF_SGSCEL_BIN_DATA_MAGIC] = 
    {0x9D,0x01,0x00,0x00};

typedef struct {
    uint8_t magic[SIZEOF_SGSCEL_BIN_DATA_MAGIC];
}sgscel_bin_data_header_t;



//Struct have decode string.
typedef struct {
    //encode in unicode
    uint8_t name[SIZEOF_BIN_INFO_NAME * MAX_UTF8_CHAR_SIZE];
    uint8_t category[SIZEOF_BIN_INFO_CATEGORY * MAX_UTF8_CHAR_SIZE];
    uint8_t description[SIZEOF_BIN_INFO_DESCRIPTION * MAX_UTF8_CHAR_SIZE];
    uint8_t example[SIZEOF_BIN_INFO_EXAMPLE * MAX_UTF8_CHAR_SIZE];
}sgscel_fileinfo_t;



#define OFFSET_PINYINS_MAPING      0x1544
#define MAX_PINYIN_STRING_LENGTH     64

typedef struct {
    uint16_t    code;
    uint16_t    size;
    uint8_t     pinyin[MAX_PINYIN_STRING_LENGTH];
}sgscel_bin_pinyin_lookup_t;

typedef struct {
    uint16_t    code;
    uint16_t    size;
    uint8_t     pinyin[MAX_PINYIN_STRING_LENGTH * MAX_UTF8_CHAR_SIZE];
}sgscel_pinyin_lookup_t;

//Pinyin list
typedef struct sgscel_pinyin_lookup_list_t {
    struct sgscel_pinyin_lookup_list_t*   next;
    uint16_t                id;
    uint16_t                code;
    char*                   str;
}sgscel_pinyin_lookup_list_t;


///////////////////////////////////////////////////////////////////////////
#define MAX_WORDS_COUNT    256

typedef struct {
    ///Number of words
    ///When words_count > 1 means more than one words for this pinyin.
    uint16_t words_count;

    ///Entry_pinyin
    /// This entry is appear just one for each words record.
    uint16_t pinyins_count;
    uint16_t pinyins_id[MAX_WORDS_COUNT];
    ///End of Entry_pinyin.

    ///Entry_word
    /// This entry have words_count number.
    uint16_t words_len;
    uint8_t  words_data[MAX_WORDS_COUNT];
    uint8_t  words_freq[12];
    ///End of Entry_word.
}sgscel_bin_words_header_t;

typedef struct {
    uint8_t                 pinyin[MAX_WORDS_COUNT * MAX_UTF8_CHAR_SIZE + 1];
    uint8_t                 string[MAX_WORDS_COUNT * MAX_UTF8_CHAR_SIZE + 1];
    double                  frequency;
}sgscel_word_inl_t;



#endif

