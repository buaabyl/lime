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
#ifndef LIBSGSCEL_H_EAA3EE18_E6C6_566F_629A_23D43DC92F75_INCLUDED_
#define LIBSGSCEL_H_EAA3EE18_E6C6_566F_629A_23D43DC92F75_INCLUDED_

typedef struct sgscel_word_t {
    struct sgscel_word_t*   next;
    char*                   pinyin;
    char*                   string;
    double                  frequency;
}sgscel_word_t;

typedef struct {
    char* file;
    char* name;
    char* category;
    char* description;
    char* example;
    sgscel_word_t*    words;
}sgscel_db_t;


sgscel_db_t* sgscel_load(const char* dbname);
void sgscel_destroy(sgscel_db_t* db);

#endif

