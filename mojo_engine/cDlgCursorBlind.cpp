/*************************************************************************************************
/*
/*   cDlgCursorBlind.cpp
/*   
/*   started October 19, 2008
/*   
/*************************************************************************************************/

#include "stdafx.h"
#include "cDlgCursorBlind.h"


//===============================================================================================
// DATA
//===============================================================================================


//===============================================================================================
// PROTOTYPES
//===============================================================================================


//===============================================================================================
// CODE
//===============================================================================================


//----------------------------------------------------------------------------------------------
// DIALOG PROC
//----------------------------------------------------------------------------------------------
INT_PTR CALLBACK cDlgCursorBlind::dialog_proc (HWND hwnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	cWin * pWin = user_data_to_pWin ( hwnd );
	cDlgCursorBlind * pThis = static_cast<cDlgCursorBlind*>(pWin);

	switch ( uMessage )
	{

	case WM_ERASEBKGND:
		return 1;
	}

	return cDlg::dialog_proc ( hwnd, uMessage, wParam, lParam );
}
