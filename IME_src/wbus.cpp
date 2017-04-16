// wbus.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "wbus.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

// CwbusApp

BEGIN_MESSAGE_MAP(CwbusApp, CWinApp)
END_MESSAGE_MAP()


// CwbusApp construction

CwbusApp::CwbusApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CwbusApp object

CwbusApp theApp;

const TCHAR* get_windows_message_str(DWORD message);

LRESULT WINAPI UIWinProc(HWND ui_window, UINT message, WPARAM wParam, LPARAM lParam)
{
    CString msg;

    msg.Format(L"Proc: %p, message %d (%s), %08x, %08x\n", ui_window, message, get_windows_message_str(message), wParam, lParam);
    OutputDebugString(msg);

    return DefWindowProc(ui_window, message, wParam, lParam);
}


// CwbusApp initialization

BOOL CwbusApp::InitInstance()
{
	CWinApp::InitInstance();
    OutputDebugString(L"InitInstance BEGIN\n");

    //@title UniversalInject
    //@url https://github.com/dwendt/UniversalInject/blob/master/UInjectIME/main.cpp
    WNDCLASSEX	wc;
    wc.cbSize			= sizeof(WNDCLASSEX);
    wc.style			= CS_VREDRAW | CS_HREDRAW | CS_IME;
    wc.lpfnWndProc	    = UIWinProc;
    wc.cbClsExtra		= 0;
    wc.cbWndExtra		= sizeof(LONG_PTR) * 2;
    wc.hInstance		= this->m_hInstance;
    wc.hCursor		    = NULL;
    wc.hIcon			= NULL;
    wc.lpszMenuName	    = NULL;
    wc.lpszClassName	= imeui_class_name;
    wc.hbrBackground	= NULL;
    wc.hIconSm		    = NULL;

    if (!RegisterClassEx(&wc)) {
        return FALSE;
    }


    OutputDebugString(L"InitInstance DONE\n");
	return TRUE;
}
