// wbus.h : main header file for the wbus DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols



// CwbusApp
// See wbus.cpp for the implementation of this class
//

class CwbusApp : public CWinApp
{
public:
	CwbusApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()

};
