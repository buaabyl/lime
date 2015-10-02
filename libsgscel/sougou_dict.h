#ifndef SOUGOU_DICT_H
#define SOUGOU_DICT_H
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
///////////////////////////////////////////////////////////////////////////
//System depend type
typedef unsigned char   uint8_t;
typedef unsigned short  uint16_t;
typedef unsigned long   uint32_t;

#define MAX_UTF8_CHAR_SIZE 4

///////////////////////////////////////////////////////////////////////////
//Sougou dict header///////////////////////////////////////////////////////
//File Header
#define SG_OFFSET_FILE_HEADER   0x00
#define SG_HEADER_MAGIC_SIZE    8

const uint8_t SG_HEADER_MAGIC_DATA1[SG_HEADER_MAGIC_SIZE] = 
    {0x40,0x15,0x00,0x00,0x44,0x43,0x53,0x01};

const uint8_t SG_HEADER_MAGIC_DATA2[SG_HEADER_MAGIC_SIZE] = 
    {0x40,0x15,0x00,0x00,0x45,0x43,0x53,0x01};

typedef struct
{
    uint8_t magic[SG_HEADER_MAGIC_SIZE];
}SG_FILE_HEADER;

//Library Header
#define SG_OFFSET_LIBRARY_HEADER        0x130
#define SG_LIBRARY_HEADER_SIZE          0x208

//Library Category
#define SG_OFFSET_LIBRARY_CATEGORY      0x338
#define SG_LIBRARY_CATEGORY_SIZE        0x208

//Library Information
#define SG_OFFSET_LIBRARY_INFORMATION   0x540
#define SG_LIBRARY_INFORMATION_SIZE     0x800

//Library Example
#define SG_OFFSET_LIBRARY_EXAMPLE       0xd40
#define SG_LIBRARY_EXAMPLE_SIZE         0x800

//Struct read from file
typedef struct
{
    //encode in unicode
    uint8_t library_name[SG_LIBRARY_HEADER_SIZE];
    uint8_t library_category[SG_LIBRARY_CATEGORY_SIZE];
    uint8_t library_information[SG_LIBRARY_INFORMATION_SIZE];
    uint8_t library_example[SG_LIBRARY_EXAMPLE_SIZE];
}SG_LIBRARY_HEADER;

//Struct have decode string.
typedef struct
{
    //encode in unicode
    uint8_t library_name[SG_LIBRARY_HEADER_SIZE * MAX_UTF8_CHAR_SIZE];
    uint8_t library_category[SG_LIBRARY_CATEGORY_SIZE * MAX_UTF8_CHAR_SIZE];
    uint8_t library_information[SG_LIBRARY_INFORMATION_SIZE * MAX_UTF8_CHAR_SIZE];
    uint8_t library_example[SG_LIBRARY_EXAMPLE_SIZE * MAX_UTF8_CHAR_SIZE];
}SG_LIBRARY_HEADER_DECODER;

///////////////////////////////////////////////////////////////////////////
//Sougou dict library//////////////////////////////////////////////////////
#define SG_OFFSET_PINYINS_HEADER      0x1540
#define SG_PINYINS_HEADER_MAGIC_SIZE  4

const uint8_t SG_PINYINS_HEADER_MAGIC_DATA[SG_PINYINS_HEADER_MAGIC_SIZE] = 
    {0x9D,0x01,0x00,0x00};

typedef struct
{
    uint8_t magic[SG_PINYINS_HEADER_MAGIC_SIZE];
}SG_PINYINS_HEADER;

//Pinyin table define
#define SG_OFFSET_PINYIN_BEGIN    0x1544
/*
 *         |header             |words
 * offset   0x00 0x01 0x02 0x03 0x04...
 * means    mark      size      data...
 *
 */
#define SG_PINYIN_MAX     64

typedef struct
{
    uint16_t code;
    uint16_t size;
    uint8_t pinyin[SG_PINYIN_MAX];
}SG_PINYIN;

typedef struct
{
    uint16_t code;
    uint16_t size;
    uint8_t pinyin[SG_PINYIN_MAX * MAX_UTF8_CHAR_SIZE];
}SG_PINYIN_DEC;

//Pinyin list
typedef struct _pinyin_list
{
    uint16_t id;
    uint8_t* data;
    struct _pinyin_list* pnext;
}pinyin_list;


///////////////////////////////////////////////////////////////////////////
//Sougou word library//////////////////////////////////////////////////////
#define MAX_WORD_LEN    256

//Record in file:
// block    offset  label
// -------------------------------
// header   0x00    words_count 
//          0x01    (reserved)
//          0x02    pinyins_count
//          0x03    (reserved)
//
// pinyin   0x04    pinyins_id
//          0xN_    ...
//
// word[0]  0xN0    words_len
//          0xN1    (reserved)
//          0xN2    words_data
//          0xM_    ...
//          0xM0    words_freq[12]
//
// word[1]  0xO0    words_len
//          0xO1    (reserved)
//          0xO2    words_data
//          0xP_    ...
//          0xP0    words_freq[12]
//
//                  ...
//
// struct
// {
//      entry   header
//      entry   pinyin
//      entry   word[header.words_count]
// }
//
//
typedef struct
{
    ///Number of words
    ///When words_count > 1 means more than one words for this pinyin.
    uint16_t words_count;

    ///Entry_pinyin
    /// This entry is appear just one for each words record.
    uint16_t pinyins_count;
    uint16_t pinyins_id[MAX_WORD_LEN];
    ///End of Entry_pinyin.

    ///Entry_word
    /// This entry have words_count number.
    uint16_t words_len;
    uint8_t  words_data[MAX_WORD_LEN];
    uint8_t  words_freq[12];
    ///End of Entry_word.
}SG_WORDS_HEADER;

typedef struct
{
    uint16_t words_count;

    uint16_t pinyins_count;
    uint8_t  pinyins_id[MAX_WORD_LEN * MAX_UTF8_CHAR_SIZE * 2];

    uint16_t words_len;
    uint8_t  words_data[MAX_WORD_LEN * MAX_UTF8_CHAR_SIZE];

    uint8_t  words_freq[12];
}SG_WORDS_HEADER_DEC;


///////////////////////////////////////////////////////////////////////////
//Helpher
#define ENDIAN_B2E_U16(u16) \
    ((((u16) & 0x00FFu) << 8) | (((u16) & 0xFF00u) >> 8))


///////////////////////////////////////////////////////////////////////////
//Global interface declare
///Decode sougou cell dict pinyin table
pinyin_list* DecodeSGWordPinyin(FILE* fp);

///Decode sougou cell dict words
void DecodeSGWordWords(
        FILE* fp,
        uint16_t hz_offset,
        pinyin_list* lppinyin_list);

///Decode sougou cell dict interface
void DecodeSGDict(FILE* fp);
#endif

