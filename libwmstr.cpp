#include "stdafx.h"
#include <Windows.h> 

const TCHAR* get_windows_message_str(DWORD message)
{
    if (message == WM_SHOWWINDOW) {
        return _T("WM_SHOWWINDOW");
    }
    if (message == WM_KEYUP) {
        return _T("WM_KEYUP");
    }
    if (message == WM_WTSSESSION_CHANGE) {
        return _T("WM_WTSSESSION_CHANGE");
    }
    if (message == WM_APPCOMMAND) {
        return _T("WM_APPCOMMAND");
    }
    if (message == WM_INPUTLANGCHANGEREQUEST) {
        return _T("WM_INPUTLANGCHANGEREQUEST");
    }
    if (message == WM_NULL) {
        return _T("WM_NULL");
    }
    if (message == WM_CREATE) {
        return _T("WM_CREATE");
    }
    if (message == WM_DESTROY) {
        return _T("WM_DESTROY");
    }
    if (message == WM_MOVE) {
        return _T("WM_MOVE");
    }
    if (message == WM_SIZE) {
        return _T("WM_SIZE");
    }
    if (message == WM_ACTIVATE) {
        return _T("WM_ACTIVATE");
    }
    if (message == WM_SETFOCUS) {
        return _T("WM_SETFOCUS");
    }
    if (message == WM_KILLFOCUS) {
        return _T("WM_KILLFOCUS");
    }
    if (message == WM_ENABLE) {
        return _T("WM_ENABLE");
    }
    if (message == WM_SETREDRAW) {
        return _T("WM_SETREDRAW");
    }
    if (message == WM_SETTEXT) {
        return _T("WM_SETTEXT");
    }
    if (message == WM_GETTEXT) {
        return _T("WM_GETTEXT");
    }
    if (message == WM_GETTEXTLENGTH) {
        return _T("WM_GETTEXTLENGTH");
    }
    if (message == WM_PAINT) {
        return _T("WM_PAINT");
    }
    if (message == WM_CLOSE) {
        return _T("WM_CLOSE");
    }
    if (message == WM_QUERYENDSESSION) {
        return _T("WM_QUERYENDSESSION");
    }
    if (message == WM_QUERYOPEN) {
        return _T("WM_QUERYOPEN");
    }
    if (message == WM_ENDSESSION) {
        return _T("WM_ENDSESSION");
    }
    if (message == WM_QUIT) {
        return _T("WM_QUIT");
    }
    if (message == WM_ERASEBKGND) {
        return _T("WM_ERASEBKGND");
    }
    if (message == WM_SYSCOLORCHANGE) {
        return _T("WM_SYSCOLORCHANGE");
    }
    if (message == WM_WININICHANGE) {
        return _T("WM_WININICHANGE");
    }
    if (message == WM_SETTINGCHANGE) {
        return _T("WM_SETTINGCHANGE");
    }
    if (message == WM_DEVMODECHANGE) {
        return _T("WM_DEVMODECHANGE");
    }
    if (message == WM_ACTIVATEAPP) {
        return _T("WM_ACTIVATEAPP");
    }
    if (message == WM_FONTCHANGE) {
        return _T("WM_FONTCHANGE");
    }
    if (message == WM_TIMECHANGE) {
        return _T("WM_TIMECHANGE");
    }
    if (message == WM_CANCELMODE) {
        return _T("WM_CANCELMODE");
    }
    if (message == WM_SETCURSOR) {
        return _T("WM_SETCURSOR");
    }
    if (message == WM_MOUSEACTIVATE) {
        return _T("WM_MOUSEACTIVATE");
    }
    if (message == WM_CHILDACTIVATE) {
        return _T("WM_CHILDACTIVATE");
    }
    if (message == WM_QUEUESYNC) {
        return _T("WM_QUEUESYNC");
    }
    if (message == WM_GETMINMAXINFO) {
        return _T("WM_GETMINMAXINFO");
    }
    if (message == WM_PAINTICON) {
        return _T("WM_PAINTICON");
    }
    if (message == WM_ICONERASEBKGND) {
        return _T("WM_ICONERASEBKGND");
    }
    if (message == WM_NEXTDLGCTL) {
        return _T("WM_NEXTDLGCTL");
    }
    if (message == WM_SPOOLERSTATUS) {
        return _T("WM_SPOOLERSTATUS");
    }
    if (message == WM_DRAWITEM) {
        return _T("WM_DRAWITEM");
    }
    if (message == WM_MEASUREITEM) {
        return _T("WM_MEASUREITEM");
    }
    if (message == WM_DELETEITEM) {
        return _T("WM_DELETEITEM");
    }
    if (message == WM_VKEYTOITEM) {
        return _T("WM_VKEYTOITEM");
    }
    if (message == WM_CHARTOITEM) {
        return _T("WM_CHARTOITEM");
    }
    if (message == WM_SETFONT) {
        return _T("WM_SETFONT");
    }
    if (message == WM_GETFONT) {
        return _T("WM_GETFONT");
    }
    if (message == WM_SETHOTKEY) {
        return _T("WM_SETHOTKEY");
    }
    if (message == WM_GETHOTKEY) {
        return _T("WM_GETHOTKEY");
    }
    if (message == WM_QUERYDRAGICON) {
        return _T("WM_QUERYDRAGICON");
    }
    if (message == WM_COMPAREITEM) {
        return _T("WM_COMPAREITEM");
    }
    if (message == WM_GETOBJECT) {
        return _T("WM_GETOBJECT");
    }
    if (message == WM_COMPACTING) {
        return _T("WM_COMPACTING");
    }
    if (message == WM_COMMNOTIFY) {
        return _T("WM_COMMNOTIFY");
    }
    if (message == WM_WINDOWPOSCHANGING) {
        return _T("WM_WINDOWPOSCHANGING");
    }
    if (message == WM_WINDOWPOSCHANGED) {
        return _T("WM_WINDOWPOSCHANGED");
    }
    if (message == WM_POWER) {
        return _T("WM_POWER");
    }
    if (message == WM_COPYDATA) {
        return _T("WM_COPYDATA");
    }
    if (message == WM_CANCELJOURNAL) {
        return _T("WM_CANCELJOURNAL");
    }
    if (message == WM_NOTIFY) {
        return _T("WM_NOTIFY");
    }
    if (message == WM_INPUTLANGCHANGE) {
        return _T("WM_INPUTLANGCHANGE");
    }
    if (message == WM_TCARD) {
        return _T("WM_TCARD");
    }
    if (message == WM_HELP) {
        return _T("WM_HELP");
    }
    if (message == WM_USERCHANGED) {
        return _T("WM_USERCHANGED");
    }
    if (message == WM_NOTIFYFORMAT) {
        return _T("WM_NOTIFYFORMAT");
    }
    if (message == WM_CONTEXTMENU) {
        return _T("WM_CONTEXTMENU");
    }
    if (message == WM_STYLECHANGING) {
        return _T("WM_STYLECHANGING");
    }
    if (message == WM_STYLECHANGED) {
        return _T("WM_STYLECHANGED");
    }
    if (message == WM_DISPLAYCHANGE) {
        return _T("WM_DISPLAYCHANGE");
    }
    if (message == WM_GETICON) {
        return _T("WM_GETICON");
    }
    if (message == WM_SETICON) {
        return _T("WM_SETICON");
    }
    if (message == WM_NCCREATE) {
        return _T("WM_NCCREATE");
    }
    if (message == WM_NCDESTROY) {
        return _T("WM_NCDESTROY");
    }
    if (message == WM_NCCALCSIZE) {
        return _T("WM_NCCALCSIZE");
    }
    if (message == WM_NCHITTEST) {
        return _T("WM_NCHITTEST");
    }
    if (message == WM_NCPAINT) {
        return _T("WM_NCPAINT");
    }
    if (message == WM_NCACTIVATE) {
        return _T("WM_NCACTIVATE");
    }
    if (message == WM_GETDLGCODE) {
        return _T("WM_GETDLGCODE");
    }
    if (message == WM_SYNCPAINT) {
        return _T("WM_SYNCPAINT");
    }
    if (message == WM_NCMOUSEMOVE) {
        return _T("WM_NCMOUSEMOVE");
    }
    if (message == WM_NCLBUTTONDOWN) {
        return _T("WM_NCLBUTTONDOWN");
    }
    if (message == WM_NCLBUTTONUP) {
        return _T("WM_NCLBUTTONUP");
    }
    if (message == WM_NCLBUTTONDBLCLK) {
        return _T("WM_NCLBUTTONDBLCLK");
    }
    if (message == WM_NCRBUTTONDOWN) {
        return _T("WM_NCRBUTTONDOWN");
    }
    if (message == WM_NCRBUTTONUP) {
        return _T("WM_NCRBUTTONUP");
    }
    if (message == WM_NCRBUTTONDBLCLK) {
        return _T("WM_NCRBUTTONDBLCLK");
    }
    if (message == WM_NCMBUTTONDOWN) {
        return _T("WM_NCMBUTTONDOWN");
    }
    if (message == WM_NCMBUTTONUP) {
        return _T("WM_NCMBUTTONUP");
    }
    if (message == WM_NCMBUTTONDBLCLK) {
        return _T("WM_NCMBUTTONDBLCLK");
    }
    if (message == WM_NCXBUTTONDOWN) {
        return _T("WM_NCXBUTTONDOWN");
    }
    if (message == WM_NCXBUTTONUP) {
        return _T("WM_NCXBUTTONUP");
    }
    if (message == WM_NCXBUTTONDBLCLK) {
        return _T("WM_NCXBUTTONDBLCLK");
    }
    if (message == WM_INPUT_DEVICE_CHANGE) {
        return _T("WM_INPUT_DEVICE_CHANGE");
    }
    if (message == WM_INPUT) {
        return _T("WM_INPUT");
    }
    if (message == WM_KEYFIRST) {
        return _T("WM_KEYFIRST");
    }
    if (message == WM_KEYDOWN) {
        return _T("WM_KEYDOWN");
    }
    if (message == WM_CHAR) {
        return _T("WM_CHAR");
    }
    if (message == WM_DEADCHAR) {
        return _T("WM_DEADCHAR");
    }
    if (message == WM_SYSKEYDOWN) {
        return _T("WM_SYSKEYDOWN");
    }
    if (message == WM_SYSKEYUP) {
        return _T("WM_SYSKEYUP");
    }
    if (message == WM_SYSCHAR) {
        return _T("WM_SYSCHAR");
    }
    if (message == WM_SYSDEADCHAR) {
        return _T("WM_SYSDEADCHAR");
    }
    if (message == WM_UNICHAR) {
        return _T("WM_UNICHAR");
    }
    if (message == WM_KEYLAST) {
        return _T("WM_KEYLAST");
    }
    if (message == WM_IME_STARTCOMPOSITION) {
        return _T("WM_IME_STARTCOMPOSITION");
    }
    if (message == WM_IME_ENDCOMPOSITION) {
        return _T("WM_IME_ENDCOMPOSITION");
    }
    if (message == WM_IME_COMPOSITION) {
        return _T("WM_IME_COMPOSITION");
    }
    if (message == WM_IME_KEYLAST) {
        return _T("WM_IME_KEYLAST");
    }
    if (message == WM_INITDIALOG) {
        return _T("WM_INITDIALOG");
    }
    if (message == WM_COMMAND) {
        return _T("WM_COMMAND");
    }
    if (message == WM_SYSCOMMAND) {
        return _T("WM_SYSCOMMAND");
    }
    if (message == WM_TIMER) {
        return _T("WM_TIMER");
    }
    if (message == WM_HSCROLL) {
        return _T("WM_HSCROLL");
    }
    if (message == WM_VSCROLL) {
        return _T("WM_VSCROLL");
    }
    if (message == WM_INITMENU) {
        return _T("WM_INITMENU");
    }
    if (message == WM_INITMENUPOPUP) {
        return _T("WM_INITMENUPOPUP");
    }
    if (message == WM_GESTURE) {
        return _T("WM_GESTURE");
    }
    if (message == WM_GESTURENOTIFY) {
        return _T("WM_GESTURENOTIFY");
    }
    if (message == WM_MENUSELECT) {
        return _T("WM_MENUSELECT");
    }
    if (message == WM_MENUCHAR) {
        return _T("WM_MENUCHAR");
    }
    if (message == WM_ENTERIDLE) {
        return _T("WM_ENTERIDLE");
    }
    if (message == WM_MENURBUTTONUP) {
        return _T("WM_MENURBUTTONUP");
    }
    if (message == WM_MENUDRAG) {
        return _T("WM_MENUDRAG");
    }
    if (message == WM_MENUGETOBJECT) {
        return _T("WM_MENUGETOBJECT");
    }
    if (message == WM_UNINITMENUPOPUP) {
        return _T("WM_UNINITMENUPOPUP");
    }
    if (message == WM_MENUCOMMAND) {
        return _T("WM_MENUCOMMAND");
    }
    if (message == WM_CHANGEUISTATE) {
        return _T("WM_CHANGEUISTATE");
    }
    if (message == WM_UPDATEUISTATE) {
        return _T("WM_UPDATEUISTATE");
    }
    if (message == WM_QUERYUISTATE) {
        return _T("WM_QUERYUISTATE");
    }
    if (message == WM_CTLCOLORMSGBOX) {
        return _T("WM_CTLCOLORMSGBOX");
    }
    if (message == WM_CTLCOLOREDIT) {
        return _T("WM_CTLCOLOREDIT");
    }
    if (message == WM_CTLCOLORLISTBOX) {
        return _T("WM_CTLCOLORLISTBOX");
    }
    if (message == WM_CTLCOLORBTN) {
        return _T("WM_CTLCOLORBTN");
    }
    if (message == WM_CTLCOLORDLG) {
        return _T("WM_CTLCOLORDLG");
    }
    if (message == WM_CTLCOLORSCROLLBAR) {
        return _T("WM_CTLCOLORSCROLLBAR");
    }
    if (message == WM_CTLCOLORSTATIC) {
        return _T("WM_CTLCOLORSTATIC");
    }
    if (message == WM_MOUSEFIRST) {
        return _T("WM_MOUSEFIRST");
    }
    if (message == WM_MOUSEMOVE) {
        return _T("WM_MOUSEMOVE");
    }
    if (message == WM_LBUTTONDOWN) {
        return _T("WM_LBUTTONDOWN");
    }
    if (message == WM_LBUTTONUP) {
        return _T("WM_LBUTTONUP");
    }
    if (message == WM_LBUTTONDBLCLK) {
        return _T("WM_LBUTTONDBLCLK");
    }
    if (message == WM_RBUTTONDOWN) {
        return _T("WM_RBUTTONDOWN");
    }
    if (message == WM_RBUTTONUP) {
        return _T("WM_RBUTTONUP");
    }
    if (message == WM_RBUTTONDBLCLK) {
        return _T("WM_RBUTTONDBLCLK");
    }
    if (message == WM_MBUTTONDOWN) {
        return _T("WM_MBUTTONDOWN");
    }
    if (message == WM_MBUTTONUP) {
        return _T("WM_MBUTTONUP");
    }
    if (message == WM_MBUTTONDBLCLK) {
        return _T("WM_MBUTTONDBLCLK");
    }
    if (message == WM_MOUSEWHEEL) {
        return _T("WM_MOUSEWHEEL");
    }
    if (message == WM_XBUTTONDOWN) {
        return _T("WM_XBUTTONDOWN");
    }
    if (message == WM_XBUTTONUP) {
        return _T("WM_XBUTTONUP");
    }
    if (message == WM_XBUTTONDBLCLK) {
        return _T("WM_XBUTTONDBLCLK");
    }
    if (message == WM_MOUSEHWHEEL) {
        return _T("WM_MOUSEHWHEEL");
    }
    if (message == WM_MOUSELAST) {
        return _T("WM_MOUSELAST");
    }
    if (message == WM_PARENTNOTIFY) {
        return _T("WM_PARENTNOTIFY");
    }
    if (message == WM_ENTERMENULOOP) {
        return _T("WM_ENTERMENULOOP");
    }
    if (message == WM_EXITMENULOOP) {
        return _T("WM_EXITMENULOOP");
    }
    if (message == WM_NEXTMENU) {
        return _T("WM_NEXTMENU");
    }
    if (message == WM_SIZING) {
        return _T("WM_SIZING");
    }
    if (message == WM_CAPTURECHANGED) {
        return _T("WM_CAPTURECHANGED");
    }
    if (message == WM_MOVING) {
        return _T("WM_MOVING");
    }
    if (message == WM_POWERBROADCAST) {
        return _T("WM_POWERBROADCAST");
    }
    if (message == WM_DEVICECHANGE) {
        return _T("WM_DEVICECHANGE");
    }
    if (message == WM_MDICREATE) {
        return _T("WM_MDICREATE");
    }
    if (message == WM_MDIDESTROY) {
        return _T("WM_MDIDESTROY");
    }
    if (message == WM_MDIACTIVATE) {
        return _T("WM_MDIACTIVATE");
    }
    if (message == WM_MDIRESTORE) {
        return _T("WM_MDIRESTORE");
    }
    if (message == WM_MDINEXT) {
        return _T("WM_MDINEXT");
    }
    if (message == WM_MDIMAXIMIZE) {
        return _T("WM_MDIMAXIMIZE");
    }
    if (message == WM_MDITILE) {
        return _T("WM_MDITILE");
    }
    if (message == WM_MDICASCADE) {
        return _T("WM_MDICASCADE");
    }
    if (message == WM_MDIICONARRANGE) {
        return _T("WM_MDIICONARRANGE");
    }
    if (message == WM_MDIGETACTIVE) {
        return _T("WM_MDIGETACTIVE");
    }
    if (message == WM_MDISETMENU) {
        return _T("WM_MDISETMENU");
    }
    if (message == WM_ENTERSIZEMOVE) {
        return _T("WM_ENTERSIZEMOVE");
    }
    if (message == WM_EXITSIZEMOVE) {
        return _T("WM_EXITSIZEMOVE");
    }
    if (message == WM_DROPFILES) {
        return _T("WM_DROPFILES");
    }
    if (message == WM_MDIREFRESHMENU) {
        return _T("WM_MDIREFRESHMENU");
    }
    if (message == WM_TOUCH) {
        return _T("WM_TOUCH");
    }
    if (message == WM_IME_SETCONTEXT) {
        return _T("WM_IME_SETCONTEXT");
    }
    if (message == WM_IME_NOTIFY) {
        return _T("WM_IME_NOTIFY");
    }
    if (message == WM_IME_CONTROL) {
        return _T("WM_IME_CONTROL");
    }
    if (message == WM_IME_COMPOSITIONFULL) {
        return _T("WM_IME_COMPOSITIONFULL");
    }
    if (message == WM_IME_SELECT) {
        return _T("WM_IME_SELECT");
    }
    if (message == WM_IME_CHAR) {
        return _T("WM_IME_CHAR");
    }
    if (message == WM_IME_REQUEST) {
        return _T("WM_IME_REQUEST");
    }
    if (message == WM_IME_KEYDOWN) {
        return _T("WM_IME_KEYDOWN");
    }
    if (message == WM_IME_KEYUP) {
        return _T("WM_IME_KEYUP");
    }
    if (message == WM_MOUSEHOVER) {
        return _T("WM_MOUSEHOVER");
    }
    if (message == WM_MOUSELEAVE) {
        return _T("WM_MOUSELEAVE");
    }
    if (message == WM_NCMOUSEHOVER) {
        return _T("WM_NCMOUSEHOVER");
    }
    if (message == WM_NCMOUSELEAVE) {
        return _T("WM_NCMOUSELEAVE");
    }
    if (message == WM_TABLET_FIRST) {
        return _T("WM_TABLET_FIRST");
    }
    if (message == WM_TABLET_LAST) {
        return _T("WM_TABLET_LAST");
    }
    if (message == WM_CUT) {
        return _T("WM_CUT");
    }
    if (message == WM_COPY) {
        return _T("WM_COPY");
    }
    if (message == WM_PASTE) {
        return _T("WM_PASTE");
    }
    if (message == WM_CLEAR) {
        return _T("WM_CLEAR");
    }
    if (message == WM_UNDO) {
        return _T("WM_UNDO");
    }
    if (message == WM_RENDERFORMAT) {
        return _T("WM_RENDERFORMAT");
    }
    if (message == WM_RENDERALLFORMATS) {
        return _T("WM_RENDERALLFORMATS");
    }
    if (message == WM_DESTROYCLIPBOARD) {
        return _T("WM_DESTROYCLIPBOARD");
    }
    if (message == WM_DRAWCLIPBOARD) {
        return _T("WM_DRAWCLIPBOARD");
    }
    if (message == WM_PAINTCLIPBOARD) {
        return _T("WM_PAINTCLIPBOARD");
    }
    if (message == WM_VSCROLLCLIPBOARD) {
        return _T("WM_VSCROLLCLIPBOARD");
    }
    if (message == WM_SIZECLIPBOARD) {
        return _T("WM_SIZECLIPBOARD");
    }
    if (message == WM_ASKCBFORMATNAME) {
        return _T("WM_ASKCBFORMATNAME");
    }
    if (message == WM_CHANGECBCHAIN) {
        return _T("WM_CHANGECBCHAIN");
    }
    if (message == WM_HSCROLLCLIPBOARD) {
        return _T("WM_HSCROLLCLIPBOARD");
    }
    if (message == WM_QUERYNEWPALETTE) {
        return _T("WM_QUERYNEWPALETTE");
    }
    if (message == WM_PALETTEISCHANGING) {
        return _T("WM_PALETTEISCHANGING");
    }
    if (message == WM_PALETTECHANGED) {
        return _T("WM_PALETTECHANGED");
    }
    if (message == WM_HOTKEY) {
        return _T("WM_HOTKEY");
    }
    if (message == WM_PRINT) {
        return _T("WM_PRINT");
    }
    if (message == WM_PRINTCLIENT) {
        return _T("WM_PRINTCLIENT");
    }
    if (message == WM_THEMECHANGED) {
        return _T("WM_THEMECHANGED");
    }
    if (message == WM_CLIPBOARDUPDATE) {
        return _T("WM_CLIPBOARDUPDATE");
    }
    if (message == WM_DWMCOMPOSITIONCHANGED) {
        return _T("WM_DWMCOMPOSITIONCHANGED");
    }
    if (message == WM_DWMNCRENDERINGCHANGED) {
        return _T("WM_DWMNCRENDERINGCHANGED");
    }
    if (message == WM_DWMCOLORIZATIONCOLORCHANGED) {
        return _T("WM_DWMCOLORIZATIONCOLORCHANGED");
    }
    if (message == WM_DWMWINDOWMAXIMIZEDCHANGE) {
        return _T("WM_DWMWINDOWMAXIMIZEDCHANGE");
    }
    if (message == WM_DWMSENDICONICTHUMBNAIL) {
        return _T("WM_DWMSENDICONICTHUMBNAIL");
    }
    if (message == WM_DWMSENDICONICLIVEPREVIEWBITMAP) {
        return _T("WM_DWMSENDICONICLIVEPREVIEWBITMAP");
    }
    if (message == WM_GETTITLEBARINFOEX) {
        return _T("WM_GETTITLEBARINFOEX");
    }
    if (message == WM_HANDHELDFIRST) {
        return _T("WM_HANDHELDFIRST");
    }
    if (message == WM_HANDHELDLAST) {
        return _T("WM_HANDHELDLAST");
    }
    if (message == WM_AFXFIRST) {
        return _T("WM_AFXFIRST");
    }
    if (message == WM_AFXLAST) {
        return _T("WM_AFXLAST");
    }
    if (message == WM_PENWINFIRST) {
        return _T("WM_PENWINFIRST");
    }
    if (message == WM_PENWINLAST) {
        return _T("WM_PENWINLAST");
    }
    if (message == WM_APP) {
        return _T("WM_APP");
    }
    if (message == WM_USER) {
        return _T("WM_USER");
    }

    return _T("unknown");
}

