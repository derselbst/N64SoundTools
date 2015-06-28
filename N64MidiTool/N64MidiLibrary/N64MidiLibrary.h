// N64MidiLibrary.h : main header file for the N64MidiLibrary DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

// CN64MidiLibraryApp
// See N64MidiLibrary.cpp for the implementation of this class
//

class CN64MidiLibraryApp : public CWinApp
{
public:
	CN64MidiLibraryApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
