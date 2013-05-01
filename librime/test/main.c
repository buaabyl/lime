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

int test(const char* s)
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

int main(int argc, char* argv[])
{
    test("ceshiyxnn");
    printf("\n");
    test("yuhycjnanyjcjl");
    printf("\n");

    return 0;
}

