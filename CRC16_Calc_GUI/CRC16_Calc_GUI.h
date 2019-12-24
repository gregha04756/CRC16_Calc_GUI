
// CRC16_Calc_GUI.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CCRC16_Calc_GUIApp:
// See CRC16_Calc_GUI.cpp for the implementation of this class
//

class CCRC16_Calc_GUIApp : public CWinApp
{
public:
	CCRC16_Calc_GUIApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CCRC16_Calc_GUIApp theApp;