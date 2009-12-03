/*************************************************************************************************
/*
/*   cDlgCursorBlind.h
/*   
/*   started June 9, 2009
/*   
/*************************************************************************************************/

#pragma once

#include "cDlg.h"

//------------------------------------------------------------------------------------------------
//  CLASS cDlg
//------------------------------------------------------------------------------------------------
class cDlgCursorBlind : public cDlg
{
public:

	void draw_text ( HWND );
	virtual int idd () { return IDD_CURSOR_BLIND; }
	virtual DialogProc * dialog_proc () { return dialog_proc; }
	static DialogProc dialog_proc;	
};
