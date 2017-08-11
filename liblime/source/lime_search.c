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
#include "sqlite3.h"
#include "lime_pinyin.h"
#include "lime_syllable.h"
#include "lime_search.h"
#include "lime_conv.h"

////////////////////////////////////////////////////////////////////////////////
struct lime_search_handle_t{
    sqlite3* db[MAX_DICTS_COUNT];
};

static const char _SQL_SELECT_PHRASE[] = "SELECT phrase, freq FROM py_phrase WHERE";
static char* _generate_select_sql(const lime_syllable_t* sequences);
static lime_candidate_t* _search(sqlite3* db, const lime_syllable_t* sequences, int nr_words);

////////////////////////////////////////////////////////////////////////////////
lime_search_handle_t* lime_search_open(const char* directory)
{
    char filename[4096];
    lime_search_handle_t* h = NULL;
    int rc;
    int nr_opened_dbs = 0;
    int i;

    h = (lime_search_handle_t*)malloc(sizeof(lime_search_handle_t));
    memset(h, 0, sizeof(lime_search_handle_t));

    for (i = 0;i < MAX_DICTS_COUNT;i++) {
        sprintf(filename, "%s/py_phrase_%02d.db", directory, i);

        rc = sqlite3_open(filename, &h->db[i]);
        if (rc != SQLITE_OK) {
            h->db[i] = NULL;
            continue;
        }

        nr_opened_dbs++;
    }

    if (nr_opened_dbs == 0) {
        free(h);
        h = NULL;
    }

    return h;
}

void lime_search_close(lime_search_handle_t* h)
{
    int i;

    if (h == NULL) {
        return;
    }

    for (i = 0;i < MAX_DICTS_COUNT;i++) {
        if (h->db[i] != NULL) {
            sqlite3_close(h->db[i]);
        }
    }

    free(h);
}

lime_candidate_t* lime_search_candidates(lime_search_handle_t* dicts, const lime_syllable_t* sequences)
{
    int nr_words;
    const lime_syllable_t* sequence;
    lime_candidate_t* candidates = NULL;
    lime_candidate_t* results[MAX_DICTS_COUNT];
    lime_candidate_t* p;
    int i;

    nr_words = 0;
    for (sequence = sequences;sequence != NULL;sequence = sequence->next) {
        nr_words++;
    }
    if (nr_words == 0) {
        return NULL;
    }
    if (nr_words > MAX_DICTS_COUNT) {
        nr_words = MAX_DICTS_COUNT;
    }

    //searching all valid dict
    memset(results, 0, sizeof(results));
    for (;nr_words <= MAX_DICTS_COUNT;nr_words++) {
        if (dicts->db[nr_words-1] == NULL) {
            continue;
        }

        results[nr_words-1] = _search(dicts->db[nr_words-1], sequences, nr_words);
        if ((nr_words < MAX_UNIQ_SEARCH_THRESHOLD) && (results[nr_words-1] != NULL)) {
            break;
        }
    }

    //merge all result
    candidates = NULL;
    p = NULL;
    for (i = 0;i < MAX_DICTS_COUNT;i++) {
        if (results[i] == NULL) {
            continue;
        }
        if (candidates == NULL) {
            candidates = results[i];
            p = candidates;
        } else {
            p->next = results[i];
        }

        if (p != NULL) {
            while (p->next != NULL) {
                p = p->next;
            }
        }
    }

    return candidates;
}

void lime_candidates_destroy(lime_candidate_t* candidates)
{
    lime_candidate_t* p;

    while (candidates != NULL) {
        p = candidates;
        candidates = candidates->next;
        free(p);
    }
}

void lime_candidates_dump(const lime_candidate_t* candidates)
{
    wchar_t ucs[16];
    char  mbcs[32];

    for (;candidates != NULL;candidates = candidates->next) {
        lime_utf8_to_ucs2(candidates->str, ucs, sizeof(ucs));
        lime_ucs2_to_gbk(ucs, mbcs, sizeof(mbcs));
        printf("%4d: %s\n", candidates->freq, mbcs);
    }
}

char* _generate_select_sql(const lime_syllable_t* sequences)
{
    char tmp[16];
    char* sql = NULL;
    int first_where_condition;
    int i;

    sql = (char*)malloc(sizeof(_SQL_SELECT_PHRASE) + sizeof("AND s?? = vvv AND y?? = vvv ") * MAX_DICTS_COUNT);
    strcpy(sql, _SQL_SELECT_PHRASE);

    i = 0;
    first_where_condition = 1;
    while (sequences != NULL) {
        if (sequences->consonant_id != -1) {
            if (first_where_condition) {
                first_where_condition = 0;
                sprintf(tmp, " s%d = %d", i, sequences->consonant_id);
            } else {
                sprintf(tmp, " AND s%d = %d", i, sequences->consonant_id);
            }
            strcat(sql, tmp);
        }
        if (sequences->vowel_id != -1) {
            if (first_where_condition) {
                first_where_condition = 0;
                sprintf(tmp, " y%d = %d", i, sequences->vowel_id);
            } else {
                sprintf(tmp, " AND y%d = %d", i, sequences->vowel_id);
            }
            strcat(sql, tmp);
        }

        sequences = sequences->next;
        i++;
    }

    strcat(sql, " LIMIT 5");

    return sql;
}

lime_candidate_t* _search(sqlite3* db, const lime_syllable_t* sequences, int nr_words)
{
    lime_candidate_t* candidates = NULL;
    lime_candidate_t* p;
    char* sql;
    const char* phrase;
    int freq;
    int rc;
    sqlite3_stmt* stmt;
    int nr_candidates = 0;

    sql = _generate_select_sql(sequences);
    //printf("py_phrase_%02d: \"%s\"\n", nr_words-1, sql);

    rc = sqlite3_prepare(db, sql, strlen(sql), &stmt, NULL);
    if (rc != SQLITE_OK) {
        printf("Error: %s\n", sqlite3_errmsg(db));
    }

    while (1) {
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_DONE) {
            break;
        }
        if (rc != SQLITE_ROW) {
            printf("Error: %s\n", sqlite3_errmsg(db));
            break;
        }

        if (candidates == NULL) {
            candidates = (lime_candidate_t*)malloc(sizeof(lime_candidate_t));
            p = candidates;
        } else {
            p->next = (lime_candidate_t*)malloc(sizeof(lime_candidate_t));
            p = p->next;
        }
        memset(p, 0, sizeof(lime_candidate_t));

        phrase = sqlite3_column_text(stmt, 0);
        freq   = sqlite3_column_int(stmt, 1);
        strcpy(p->str, phrase);
        p->freq = freq;
        nr_candidates++;

    }
    sqlite3_finalize(stmt);

    free(sql);
    sql = NULL;

    //printf("py_phrase_%02d: %d candidates\n", nr_words-1, nr_candidates);

    return candidates;
}


