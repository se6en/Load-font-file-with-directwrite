
// D2DLoadFontFile.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CD2DLoadFontFileApp:
// See D2DLoadFontFile.cpp for the implementation of this class
//

class CD2DLoadFontFileApp : public CWinApp
{
public:
	CD2DLoadFontFileApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CD2DLoadFontFileApp theApp;