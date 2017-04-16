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
#ifndef IME_INTERFACE_H_F4BE29AD_026A_E209_1B5E_9CDEAA87386F_INCLUDED_
#define IME_INTERFACE_H_F4BE29AD_026A_E209_1B5E_9CDEAA87386F_INCLUDED_

////////////////////////////////////////////////////////////////////////////////
// this make sure all IME interface export as `c` function, will be C++ function without `extern "C"`!
// don't remove it.
extern "C" {

BOOL WINAPI ImeInquire(LPIMEINFO lpImeInfo, LPTSTR lpszUIClass, DWORD dwSystemInfoFlags);
DWORD WINAPI ImeConversionList(
        HIMC hIMC,
        LPCTSTR lpSrc,
        LPCANDIDATELIST lpDst,
        DWORD dwBufLen,
        UINT uFlag);
BOOL WINAPI ImeConfigure(HKL hKL, HWND hWnd, DWORD dwMode, LPVOID lpData);
BOOL WINAPI ImeDestroy(UINT uReserved);
BOOL WINAPI ImeSetActiveContext(HIMC hIMC, BOOL fFlag);
BOOL WINAPI ImeProcessKey(HIMC hIMC, UINT uVirKey, LPARAM lParam, CONST LPBYTE lpbKeyState);
BOOL WINAPI NotifyIME(HIMC hIMC, DWORD dwAction, DWORD dwIndex, DWORD dwValue);
BOOL WINAPI ImeSelect(HIMC hIMC, BOOL selected);
BOOL WINAPI ImeSetCompositionString(
        HIMC hIMC,
        DWORD dwIndex,
        LPVOID lpComp,
        DWORD dwCompLen,
        LPVOID lpRead,
        DWORD dwReadLen);
UINT WINAPI ImeToAsciiEx(
        UINT virtual_key,
        UINT scan_code,
        CONST LPBYTE key_state,
        LPTRANSMSGLIST message_list,
        UINT fuState,
        HIMC hIMC);
BOOL WINAPI ImeRegisterWord(LPCTSTR lpszReading, DWORD dwStyle, LPCTSTR lpszString);
BOOL WINAPI ImeUnregisterWord(LPCTSTR lpszReading, DWORD dwStyle, LPCTSTR lpszString);
UINT WINAPI ImeGetRegisterWordStyle(UINT nItem, LPSTYLEBUF lpStyleBuf);
UINT WINAPI ImeEnumRegisterWord(
        REGISTERWORDENUMPROC lpfnEnumProc,
        LPCTSTR lpszReading,
        DWORD dwStyle,
        LPCTSTR lpszString,
        LPVOID lpData);
DWORD WINAPI ImeGetImeMenuItems(
        HIMC hIMC,
        DWORD dwFlags,
        DWORD dwType,
        LPIMEMENUITEMINFO lpImeParentMenu,
        LPIMEMENUITEMINFO lpImeMenu,
        DWORD dwSize);
LRESULT WINAPI ImeEscape(HIMC hIMC, UINT uEscape, LPVOID lpData);

}



#endif

