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
#include "lime_pinyin.h"
#include "lime_syllable.h"


////////////////////////////////////////////////////////////////////////////////
//@brief    pair consonant with vowel
//@retval   0   untouch
//@retval   1   modify
static int _lime_parser(lime_syllable_t* sequences);


////////////////////////////////////////////////////////////////////////////////
lime_syllable_t* lime_lexical_analysis_positive(const char* keys)
{
    int i;
    int j;
    int n;
    int pronounce_id;
    lime_syllable_t* result = NULL;
    lime_syllable_t* p = NULL;

    n = strlen(keys);

    // range: [i, j)
    i = 0;
    while (i < n) {
        j = i + MAX_PRONOUNCE_LENGTH;
        j = (j > n)?n:j;

        pronounce_id = -1;
        for (;i < j;j--) {
            pronounce_id = lime_map_by_pronounce(&keys[i], j - i);
            if (pronounce_id >= 0) {
                //append to list
                if (result == NULL) {
                    p = (lime_syllable_t*)malloc(sizeof(lime_syllable_t));
                    result = p;
                } else {
                    p->next = (lime_syllable_t*)malloc(sizeof(lime_syllable_t));
                    p = p->next;
                }
                memset(p, 0, sizeof(lime_syllable_t));

                if (pronounce_id <= MAX_CONSONANT_INDEX) {
                    p->consonant_id = pronounce_id;
                    p->vowel_id     = -1;
                    strncpy(p->consonant_str, &keys[i], j - i);
                } else {
                    p->consonant_id = -1;
                    p->vowel_id     = pronounce_id;
                    strncpy(p->vowel_str, &keys[i], j - i);
                }

                break;
            }
        }
        if (pronounce_id >= 0) {
            i = j;
        } else {
            i++;
        }
    }

    return result;
}

lime_syllable_t* lime_lexical_analysis_reverse(const char* keys)
{
    int i;
    int j;
    int n;
    int pronounce_id;
    lime_syllable_t* result = NULL;
    lime_syllable_t* p = NULL;

    n = strlen(keys);

    // range: [j, i)
    i = n;
    while (i >= 0) {
        j = i - MAX_PRONOUNCE_LENGTH;
        j = (j < 0)?0:j;

        pronounce_id = -1;
        for (;j < i;j++) {
            pronounce_id = lime_map_by_pronounce(&keys[j], i - j);
            if (pronounce_id >= 0) {
                //insert head to list
                if (result == NULL) {
                    p = (lime_syllable_t*)malloc(sizeof(lime_syllable_t));
                    memset(p, 0, sizeof(lime_syllable_t));
                } else {
                    p = (lime_syllable_t*)malloc(sizeof(lime_syllable_t));
                    memset(p, 0, sizeof(lime_syllable_t));
                    p->next = result;
                }
                result = p;

                if (pronounce_id <= MAX_CONSONANT_INDEX) {
                    p->consonant_id = pronounce_id;
                    p->vowel_id     = -1;
                    strncpy(p->consonant_str, &keys[j], i - j);
                } else {
                    p->consonant_id = -1;
                    p->vowel_id     = pronounce_id;
                    strncpy(p->vowel_str, &keys[j], i - j);
                }

                break;
            }
        }
        if (pronounce_id >= 0) {
            i = j;
        } else {
            i--;
        }
    }

    return result;
}

int lime_parser(lime_syllable_t* sequences)
{
    int i;

    for (i = 0;i < MAX_PARSER_PASS;i++) {
        if (!_lime_parser(sequences)) {
            break;
        }
    }

    return 1;
}

void lime_syllable_dump(lime_syllable_t* result)
{
    lime_syllable_t* p;

    for (p = result;p != NULL;p = p->next) {
        if (p->consonant_id >= 0) {
            printf("%s'", p->consonant_str);
        }
        if (p->vowel_id >= 0) {
            printf("%s", p->vowel_str);
        }
        printf("| ");
    }
    printf("\n");
}

void lime_syllable_destroy(lime_syllable_t* root)
{
    lime_syllable_t* p;

    while (root != NULL) {
        p = root;
        root = root->next;
        free(p);
    }
}

int _lime_parser(lime_syllable_t* sequences)
{
    lime_syllable_t* p;
    lime_syllable_t* p_sub;
    lime_syllable_t* tmp;
    int vowel_id = -1;
    const char* s;
    char tmpstr[10];
    int n;
    int len;
    int is_modify = 0;

    p = sequences;
    while (p != NULL) {
        if (p->next) {
            if ((p->consonant_id != -1) && (p->vowel_id == -1)) {
                if (p->next->vowel_id != -1) {
                    //pair normally, merge consonant and vowel node
                    //
                    //  [this] ------> [next] ---> [others]
                    //  consonant      vowel
                    //
                    //  [this] ------------------> [others]
                    //  consonant
                    //  vowel
                    //
                    if (lime_is_paired_by_id(p->consonant_id, p->next->vowel_id)) {
                        p->vowel_id = p->next->vowel_id;
                        strcpy(p->vowel_str, p->next->vowel_str);

                        tmp = p->next;
                        p->next = p->next->next;
                        free(tmp);

                        is_modify = 1;

                    } else {
                        //split vowel, and try minimal length match first
                        //
                        //  [this] ------> [next] ---> [others]
                        //  consonant      vowel
                        //
                        //  [this] ------------------> [others]
                        //  consonant
                        //
                        //  [this] ------> [splited.0] ---> ... ---> [splited.n-1] ---> [others]
                        //  consonant      
                        //  vowel'
                        //
                        strcpy(tmpstr, p->next->vowel_str);
                        len = strlen(tmpstr);

                        for (n = 1;n < len;n++) {
                            vowel_id = lime_map_by_pronounce(tmpstr, n);
                            if (vowel_id > MAX_CONSONANT_INDEX) {
                                if (lime_is_paired_by_id(p->consonant_id, vowel_id)) {
                                    break;
                                }
                            }
                        }

                        if (n < len) {
                            p->vowel_id = p->next->vowel_id;
                            memset(p->vowel_str, 0, sizeof(p->next->vowel_str));
                            memcpy(p->vowel_str, tmpstr, n);
                            memcpy(tmpstr, tmpstr + n, len-n);
                            tmpstr[len-n] = '\0';

                            tmp = p->next;
                            p->next = p->next->next;
                            free(tmp);

                            is_modify = 1;

                            p_sub = lime_lexical_analysis_positive(tmpstr);
                            if (p_sub != NULL) {
                                tmp = p_sub;
                                while (tmp->next != NULL) {
                                    tmp = tmp->next;
                                }
                                tmp->next = p->next->next;
                                p->next->next = p_sub;
                            }
                        }
                    }
                }
            }
        }
        p = p->next;
    }

    return is_modify;
}

