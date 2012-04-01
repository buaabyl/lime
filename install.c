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
 *  2012.00.00     created by buaabyl@gmail.com
 *
 *
 *
 *
 * ## About Input Method Manager
 * https://msdn.microsoft.com/en-us/library/windows/desktop/dd317702(v=vs.85).aspx
 * 
 * ## Using Input Method Manager
 * https://msdn.microsoft.com/en-us/library/windows/desktop/dd374108(v=vs.85).aspx
 *
 * ## Input Method Manager Reference
 * https://msdn.microsoft.com/en-us/library/windows/desktop/dd318653(v=vs.85).aspx
 *
 * ## About Text Services Framework
 * https://msdn.microsoft.com/en-us/library/windows/desktop/ms538047(v=vs.85).aspx
 *
 * ## Using Text Services Framework
 * https://msdn.microsoft.com/en-us/library/windows/desktop/ms629225(v=vs.85).aspx
 *
 * ## Text Services Framework Reference
 * https://msdn.microsoft.com/en-us/library/windows/desktop/ms629038(v=vs.85).aspx
 *
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <windows.h>
#include <shlwapi.h>

#define IME_PATH    L"wbus.ime"
#define IME_NAME    L"Windows Input Bus"

static const wchar_t* GetLastErrorMessage()
{
    static wchar_t message[8196];
    wchar_t* p = message;
    DWORD err;

    err = GetLastError();
    p += wsprintf(message, L"0x%08x: ", (int)err);
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

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd)
{
    wchar_t ime_path[MAX_PATH];
    HKL keylayout;

    OutputDebugString(L"ime install tools\n");

    if (!GetSystemDirectory(ime_path, MAX_PATH)) {
        wprintf(L"Error: GetSystemDirectory: %s\n", GetLastErrorMessage());
        return -1;
    }
    wprintf(L"Info: SystemDirectory: \"%s\"\n", ime_path);

    PathAppend(ime_path, IME_PATH);
    if (!PathFileExists(ime_path)) {
        wprintf(L"Error: \"%s\" missing\n", ime_path);
        return -1;
    }

    wprintf(L"Info: install \"%s\"\n", ime_path);
    keylayout = ImmInstallIME(ime_path, IME_NAME);
    if (keylayout == 0) {
        wprintf(L"Error: ImmInstallIME return %d\n", keylayout);
        if (GetLastError() != ERROR_SUCCESS) {
            wprintf(L"%s\n", GetLastErrorMessage());
        }
        return -1;
    }

    wprintf(L"OK\n");

    return 0;
}

