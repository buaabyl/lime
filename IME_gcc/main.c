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
#include <inttypes.h>

#include <time.h>

#include <windows.h>
#include <shlwapi.h>
#include <imm.h>
#include "immdev.h"

#define imeui_class_name L"wbus"

//@title GetSystemMetrics
//@url https://msdn.microsoft.com/en-us/library/windows/desktop/ms724385(v=vs.85).aspx
typedef enum {
    SYS_NORMAL_BOOT = 0,
    SYS_FAIL_SAFE_BOOT = 1,
    SYS_FAIL_SAFE_NETBOOT = 2
}sys_cleanboot_t;

static sys_cleanboot_t _sys_boottype = SYS_NORMAL_BOOT;


//@ref https://github.com/chewing/windows-chewing/tree/master/ChewingIME
//
//@title Input Method Manager Reference
//@url https://msdn.microsoft.com/en-us/library/windows/desktop/dd318653(v=vs.85).aspx
//
//@title IMEINFO structre
//@url https://msdn.microsoft.com/en-us/library/aa741226(v=vs.85).aspx
//
//@title IMEINFO property enum
//@url https://msdn.microsoft.com/en-us/library/windows/desktop/dd318567(v=vs.85).aspx
//
//  IMEINFO.fdwProperty 
//
//  - IME_PROP_AT_CARET
//      If set, conversion window is at the caret position. 
//      If clear, the window is near the caret position.
//  - IME_PROP_SPECIAL_UI
//      If set, the IME has a nonstandard user interface. The application should not draw in the IME window.
//  - IME_PROP_CANDLIST_START_FROM_1
//      If set, strings in the candidate list are numbered starting at 1.
//      If clear, strings start at 0.
//  - IME_PROP_UNICODE
//      If set, the IME is viewed as a Unicode IME. The operating system and 
//      the IME communicate through the Unicode IME interface. 
//      If clear, the IME uses the ANSI interface to communicate with the operating system.
//  - IME_PROP_COMPLETE_ON_UNSELECT
//      If set, the IME completes the composition string when the IME is deactivated.
//      If clear, the IME cancels the composition string when the IME is deactivated,
//      for example, from a keyboard layout change.
//  - IME_PROP_ACCEPT_WIDE_VKEY
//      If set, the IME processes the injected Unicode that came from 
//      the SendInput function by using VK_PACKET.
//      If clear, the IME might not process the injected Unicode,
//      and might send the injected Unicode to the application directly.
//
//@title IME Conversion Mode Values
//@url https://msdn.microsoft.com/en-us/library/windows/desktop/dd318165(v=vs.85).aspx
//
BOOL WINAPI ImeInquire(LPIMEINFO lpImeInfo, LPWSTR lpszUIClass, DWORD dwSystemInfoFlags)
{
    OutputDebugString(L"ImeInquire\n");

    _sys_boottype = (sys_cleanboot_t)GetSystemMetrics(SM_CLEANBOOT);
    if (_sys_boottype != SYS_NORMAL_BOOT) {
        OutputDebugString(L"ImeInquire: not SYS_NORMAL_BOOT\n");
        return FALSE;
    }

	if (dwSystemInfoFlags & IME_SYSINFO_WINLOGON) {
        OutputDebugString(L"ImeInquire: IME_SYSINFO_WINLOGON\n");
		return FALSE;
    }

    OutputDebugString(L"ImeInquire: set fdwProperty\n");
    memset(lpImeInfo, 0, sizeof(IMEINFO));
    lpImeInfo->fdwProperty = 0;
    //lpImeInfo->fdwProperty |= IME_PROP_AT_CARET;
    lpImeInfo->fdwProperty |= IME_PROP_CANDLIST_START_FROM_1;
    lpImeInfo->fdwProperty |= IME_PROP_UNICODE;
    //lpImeInfo->fdwProperty |= IME_PROP_COMPLETE_ON_UNSELECT;
    //lpImeInfo->fdwProperty |= IME_PROP_ACCEPT_WIDE_VKEY;
    //lpImeInfo->fdwProperty |= IME_PROP_NEED_ALTKEY;
    //lpImeInfo->fdwProperty |= IME_PROP_END_UNLOAD;      // unload all method when exit?
    lpImeInfo->fdwProperty |= IME_PROP_KBD_CHAR_FIRST;  // let keyboard convert char first?
    lpImeInfo->fdwProperty |= IME_PROP_SPECIAL_UI;

    lpImeInfo->fdwSentenceCaps  = IME_SMODE_NONE;
    lpImeInfo->fdwUICaps        = UI_CAP_SOFTKBD | UI_CAP_2700;
    lpImeInfo->fdwSCSCaps       = SCS_CAP_COMPSTR;
    lpImeInfo->fdwSelectCaps    = 0;

    OutputDebugString(L"ImeInquire: set fdwConversionCaps\n");
    //Howto change mode:
    //```c
    //  HIMC hIMC;   
    //  DWORD dwConv;
    //  DWORD dwSent;   
    //  
    //  hIMC = ImmGetContext(m_hWnd);   
    //  ImmGetConversionStatus(hIMC, &dwConv, &dwSent);   
    //   
    //  if((dwConv & IME_CMODE_LANGUAGE) != IME_CMODE_NATIVE) {
    //    in chinese mode
    //  } else {
    //    native english mode
    //  }
    //  
    //  ImmSetConversionStatus(hIMC, dwConv, dwSent);
    //  ImmReleaseContext(m_hWnd, hIMC);
    //```
    //
    lpImeInfo->fdwConversionCaps = 0;
    lpImeInfo->fdwConversionCaps |= IME_CMODE_ALPHANUMERIC;
    //lpImeInfo->fdwConversionCaps |= IME_CMODE_ALPHANUMERIC;
    //lpImeInfo->fdwConversionCaps |= IME_CMODE_FULLSHAPE;    // full shape for all chars
    //lpImeInfo->fdwConversionCaps |= IME_CMODE_SYMBOL;       // chinese symbol

    lpImeInfo->dwPrivateDataSize = 0;

    OutputDebugString(L"ImeInquire: set lpszUIClass\n");
    wcscpy(lpszUIClass, imeui_class_name);

    OutputDebugString(L"ImeInquire: DONE\n");
    return TRUE;
}

DWORD WINAPI ImeConversionList(
        HIMC hIMC,
        LPCTSTR lpSrc,
        LPCANDIDATELIST lpDst,
        DWORD dwBufLen,
        UINT uFlag)
{
    OutputDebugString(L"ImeConversionList\n");
    return FALSE;
}

BOOL WINAPI ImeConfigure(HKL hKL, HWND hWnd, DWORD dwMode, LPVOID lpData)
{
    OutputDebugString(L"ImeConfigure\n");
    return FALSE;
}

BOOL WINAPI ImeDestroy(UINT uReserved)
{
    OutputDebugString(L"ImeDestroy\n");
    return TRUE;
}

BOOL WINAPI ImeSetActiveContext(HIMC hIMC, BOOL fFlag)
{
    OutputDebugString(L"ImeSetActiveContext\n");
    return TRUE;
}

BOOL WINAPI ImeProcessKey(HIMC hIMC, UINT uVirKey, LPARAM lParam, CONST LPBYTE lpbKeyState)
{
    wchar_t msg[1024];
    wsprintf(msg, L"ImeProcessKey: uVirKey = %d, lParam = 0x%08x\n", uVirKey, lParam);
    OutputDebugString(msg);
    return TRUE;
}

BOOL WINAPI NotifyIME(HIMC hIMC, DWORD dwAction, DWORD dwIndex, DWORD dwValue)
{
    wchar_t msg[1024];
    wsprintf(msg, L"NotifyIME: dwAction = %d, dwIndex = 0x%08x, dwValue = 0x%08x\n",
            dwAction, dwIndex, dwValue);
    OutputDebugString(msg);
    return TRUE;
}

BOOL WINAPI ImeSelect(HIMC hIMC, BOOL selected)
{
    wchar_t msg[1024];
    wsprintf(msg, L"ImeSelect: selected = %d\n", selected);
    OutputDebugString(msg);
    return TRUE;
}

BOOL WINAPI ImeSetCompositionString(HIMC hIMC, DWORD dwIndex, LPVOID lpComp, DWORD dwCompLen, LPVOID lpRead, DWORD dwReadLen)
{
    OutputDebugString(L"ImeSetCompositionString\n");
    return FALSE;
}

//@title IActiveIME::ToAsciiEx method
//@url https://msdn.microsoft.com/en-us/library/aa752001(v=vs.85).aspx
UINT WINAPI ImeToAsciiEx(
        UINT virtual_key,
        UINT scan_code,
        CONST LPBYTE key_state,
        LPTRANSMSGLIST message_list,
        UINT fuState,
        HIMC hIMC)
{
    OutputDebugString(L"ImeToAsciiEx\n");
    return 0;
}

BOOL WINAPI ImeRegisterWord(LPCTSTR lpszReading, DWORD dwStyle, LPCTSTR lpszString)
{
    OutputDebugString(L"ImeRegisterWord\n");
    return FALSE;
}


BOOL WINAPI ImeUnregisterWord(LPCTSTR lpszReading, DWORD dwStyle, LPCTSTR lpszString)
{
    OutputDebugString(L"ImeUnregisterWord\n");
	return TRUE;
}

UINT WINAPI ImeGetRegisterWordStyle(UINT nItem, LPSTYLEBUF lpStyleBuf)
{
    OutputDebugString(L"ImeGetRegisterWordStyle\n");
	return FALSE;
}

UINT WINAPI ImeEnumRegisterWord(
        REGISTERWORDENUMPROC lpfnEnumProc,
        LPCTSTR lpszReading,
        DWORD dwStyle,
        LPCTSTR lpszString,
        LPVOID lpData)
{
    OutputDebugString(L"ImeEnumRegisterWord\n");
    return FALSE;
}

DWORD WINAPI ImeGetImeMenuItems(
        HIMC hIMC,
        DWORD dwFlags,
        DWORD dwType,
        LPIMEMENUITEMINFO lpImeParentMenu,
        LPIMEMENUITEMINFO lpImeMenu,
        DWORD dwSize)
{
    OutputDebugString(L"ImeGetImeMenuItems\n");
    return FALSE;
}

LRESULT WINAPI ImeEscape(HIMC hIMC, UINT uEscape, LPVOID lpData)
{
    OutputDebugString(L"ImeEscape\n");
    return FALSE;
}

const TCHAR* get_windows_message_str(DWORD message);

LRESULT WINAPI UIWinProc(HWND ui_window, UINT message, WPARAM wParam, LPARAM lParam)
{
    wchar_t msg[1024];

    wsprintf(msg, L"Proc: %p, message %d (%s), %08x, %08x\n", ui_window, message, get_windows_message_str(message), wParam, lParam);
    OutputDebugString(msg);

    return DefWindowProc(ui_window, message, wParam, lParam);
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    wchar_t msg[1024];

    switch (fdwReason) {
        case DLL_PROCESS_ATTACH:
            OutputDebugString(L"process.attach BEGIN\n");

            //@title UniversalInject
            //@url https://github.com/dwendt/UniversalInject/blob/master/UInjectIME/main.cpp

            WNDCLASSEX	wc;
            wc.cbSize			= sizeof(WNDCLASSEX);
            wc.style			= CS_VREDRAW | CS_HREDRAW | CS_IME;
            wc.lpfnWndProc	    = UIWinProc;
            wc.cbClsExtra		= 0;
            wc.cbWndExtra		= sizeof(LONG_PTR) * 2;
            wc.hInstance		= hinstDLL;
            wc.hCursor		    = NULL;
            wc.hIcon			= NULL;
            wc.lpszMenuName	    = NULL;
            wc.lpszClassName	= imeui_class_name;
            wc.hbrBackground	= NULL;
            wc.hIconSm		    = NULL;

            if (!RegisterClassEx(&wc)) {
                OutputDebugString(L"process.attach RegisterClassEx Error\n");
                wsprintf(msg, L"GetLastError() = %d\n", GetLastError());
                return FALSE;
            }

            OutputDebugString(L"process.attach DONE\n");
            break;

        case DLL_THREAD_ATTACH:
            printf("thread.attach\n");
            break;

        case DLL_THREAD_DETACH:
            printf("thread.detach\n");
            break;

        case DLL_PROCESS_DETACH:
            printf("process.detach\n");
            break;

        default:
            printf("other\n");
            break;
    }

    return TRUE;
}


