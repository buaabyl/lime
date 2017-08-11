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
 *
 * TODO:
 *  1. fuzzy matching
 *  2. similar matching
 * 
 */
#ifndef LIME_SEARCH_H_052C995A_3719_7E27_B76E_1440A1DABA89_INCLUDED_
#define LIME_SEARCH_H_052C995A_3719_7E27_B76E_1440A1DABA89_INCLUDED_

//max number of dicts, and equal to max length of words support
#define MAX_DICTS_COUNT             15

//when pinyin pair below this is uniq-match
#define MAX_UNIQ_SEARCH_THRESHOLD   3

#define MAX_FREQUENCE_VALUE         8000

struct lime_search_handle_t;
typedef struct lime_search_handle_t lime_search_handle_t;

//maximal length of UTF-8 encoded char is 4 bytes.
typedef struct _lime_candidate_t{
    struct _lime_candidate_t*   next;
    char                        str[MAX_DICTS_COUNT * 4 + 1];
    int                         freq;
}lime_candidate_t;

lime_search_handle_t* lime_search_open(const char* directory);
void lime_search_close(lime_search_handle_t* h);

lime_candidate_t* lime_search_candidates(lime_search_handle_t* dicts, const lime_syllable_t* sequences);
void lime_candidates_destroy(lime_candidate_t* candidates);
void lime_candidates_dump(const lime_candidate_t* candidates);


#endif

