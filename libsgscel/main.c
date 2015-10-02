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
#include "libsgscel.h"

static void _puts(uint8_t* str)
{
    for (;*str != '\0';str++) {
        if (*str == '\r') {
            continue;
        } else if (*str == '\n') {
            printf("# ");
        } else {
            putchar(*str);
        }
    }
    putchar('\n');
}


int main(int argc, const char* argv[])
{
    sgscel_db_t* db;
    const sgscel_word_t* word;
    double max_frequency = 0;
    int nr_words = 0;

    if (argc != 2) {
        printf("Usage: %s test.scel\n", argv[0]);
        return -1;
    } 

    db = sgscel_load(argv[1]);
    if (db != NULL) {
        printf("# vim: set fileencoding=utf-8\n");
        printf("# -*- coding:utf-8 -*-\n");
        printf("#\n");
        printf("# @name         : %s\n", db->name);
        printf("# @category     : %s\n", db->category);
        printf("# @description  : %s\n", db->description);
        printf("# @example      : "); _puts(db->example);

        for (word = db->words;word != NULL;word = word->next) {
            nr_words++;
            if (word->frequency > max_frequency) {
                max_frequency = word->frequency;
            }
        }
        max_frequency = max_frequency + 1;
        printf("# @count        : %d\n", nr_words);
        printf("# @frequency    : %.0f\n", max_frequency);
        printf("\n");

        for (word = db->words;word != NULL;word = word->next) {
            printf("%s %s %.6f\n", word->pinyin, word->string, word->frequency/max_frequency);
        }

        sgscel_destroy(db);
    }

    return 0;
}


