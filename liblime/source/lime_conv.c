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
#include <windows.h>

static const char* GetLastErrorMessage()
{
    static char message[8196];
    char* p = message;
    DWORD err;

    err = GetLastError();
    p += sprintf(message, "0x%08x: ", (int)err);
    FormatMessage(
            FORMAT_MESSAGE_FROM_SYSTEM,
            NULL,
            err,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            p,
            message + sizeof(message) - p,
            NULL);

    return message;
}

void lime_utf8_to_ucs2(const char* s, wchar_t* ws, int sizeof_ws)
{
    if (!MultiByteToWideChar(CP_UTF8, 0, s, -1, ws, 100)) {
        printf("MultiByteToWideChar error: %s\n", GetLastErrorMessage());
    }
}

void lime_ucs2_to_gbk(const wchar_t* ws, char* s, int sizeof_s)
{
    if (!WideCharToMultiByte(CP_ACP, 0, ws, -1, s, sizeof_s, NULL, NULL)) {
        printf("WideCharToMultiByte error: %s\n", GetLastErrorMessage());
    }
}

