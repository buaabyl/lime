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
 *  2017.04.16     created by {user}
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <windows.h>

int (*add)(int a, int b);

int wmain(int argc, wchar_t* argv[])
{
    HANDLE h;

    if (argc != 2) {
        printf("Error: load *.dll\n");
        return -1;
    }

    h = LoadLibrary(argv[1]);

    //bug in windows PE file, all PE file header is ANSI,
    //so there are no unicode version of GetProcAddress!
    add = (int(*)(int,int))GetProcAddress(h, "add");
    if (add) {
        printf("%d + %d = %d\n", 1, 2, add(1,2));
    }


    CloseHandle(h);

    return 0;
}

