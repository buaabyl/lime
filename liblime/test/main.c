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
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include "lime_pinyin.h"
#include "lime_syllable.h"
#include "lime_search.h"
#include "lime_conv.h"


int test_parser(const char* s)
{
    lime_syllable_t* result;

    printf("input: %s\n", s);

    result = lime_lexical_analysis_positive(s);
    //result = lime_lexical_analysis_reverse(s);
    printf("lexical: ");
    lime_syllable_dump(result);

    lime_parser(result);
    printf("parser : ");
    lime_syllable_dump(result);

    lime_syllable_destroy(result);

    return 0;
}

int test_dict(const char* keys)
{
    lime_syllable_t* sequences;
    lime_search_handle_t* dicts;
    lime_candidate_t* candidates;

    dicts = lime_search_open("data");
    if (dicts == NULL) {
        printf("Missing database\n");
        return -1;
    }

    sequences = lime_lexical_analysis_positive(keys);
    lime_parser(sequences);
    lime_syllable_dump(sequences);

    candidates = lime_search_candidates(dicts, sequences);
    if (candidates != NULL) {
        lime_candidates_dump(candidates);
        lime_candidates_destroy(candidates);
        candidates = NULL;
    }

    lime_syllable_destroy(sequences);
    sequences = NULL;

    lime_search_close(dicts);
    dicts = NULL;

    printf("\n");

    return 0;
}

int main(int argc, char* argv[])
{
    test_parser("ceshiyxnn");
    printf("\n");
    test_parser("yuhycjnanyjcjl");
    printf("\n");


    test_dict("lg");
    test_dict("lgh");
    test_dict("lghl");
    test_dict("liangghl");
    test_dict("lghlmcl");

    return 0;
}

