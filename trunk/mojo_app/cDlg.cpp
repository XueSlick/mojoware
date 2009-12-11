/***********************************************************************************************************************
/*
/*    cDlg.cpp / mojo_app
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"
#include <windowsx.h>
#include "resource.h"


//----------------------------------------------------------------------------------------------------------------------
// MAKE DIALOG
//----------------------------------------------------------------------------------------------------------------------
HWND cDlg :: make_dlg ()
{
	cWin * pWin = static_cast<cWin *>(this);

	hwnd = CreateDialogParam ( g_hInstance, MAKEINTRESOURCE ( idd() ), g_hwnd, dialog_proc(), (LPARAM) pWin );
	return hwnd;
}


//----------------------------------------------------------------------------------------------------------------------
// MAKE WIN
//----------------------------------------------------------------------------------------------------------------------
INT_PTR cDlgModal :: make_dlg ( void * pUserDataArg )
{
	this->pUserData = pUserDataArg;
	cWin * pWin = static_cast<cWin *>(this);
	INT_PTR Result = DialogBoxParam ( g_hInstance, MAKEINTRESOURCE (idd()), g_hwnd, dialog_proc(), (LPARAM) pWin );
	return Result;
}


//----------------------------------------------------------------------------------------------------------------------
// DRAW TEXT
// GDI VERSION
// pR can be NULL
// eType is subhead, text, caption, or note
//----------------------------------------------------------------------------------------------------------------------
int cDlg::draw_text ( HDC hdc, RECT *pR, _eType eType, const wchar_t * pTxt, bool bDraw, DWORD dwStyles )
{
	assert ( pR );
	assert ( g_hMenuFont );
	assert ( g_hCaptionFont );

	HFONT hFont = 0;

	COLORREF color = RGB ( 0, 0, 0 );

	switch ( eType )
	{
	case head:
		hFont = g_hHeadFont;
		color = RGB ( 0x0, 0, 0xee );
		break;

	case subhead:
		hFont = g_hSubheadFont;
		break;

	case dialog_box_head:
		hFont = g_hDialogBoxHeadFont;
		color = RGB ( 50, 64, 150 );
		break;

	case caption:
		hFont = g_hCaptionFont;
		color = RGB ( 255, 255, 255 );
		break;

	case text:
		hFont = g_hMenuFont;
		break;

	case note:
		color = RGB ( 92, 92, 92 );
		hFont = g_hMenuFont;
		break;

	default:
		assert(0);
	}

	assert ( hFont );

	SelectObject	( hdc, hFont );
	SetTextColor    ( hdc, color );
	SetBkMode		( hdc, TRANSPARENT );

	int iRetVal = 0;

	if ( 0 == dwStyles )
		dwStyles = DT_EXPANDTABS | DT_LEFT | DT_TOP | DT_WORDBREAK;

	if ( ! bDraw )
		iRetVal = DrawText ( hdc, pTxt, (int) wcslen ( pTxt ), pR, dwStyles | DT_CALCRECT );

	else
		iRetVal = DrawText ( hdc, pTxt, (int) wcslen ( pTxt ), pR,  dwStyles );

	return iRetVal;
}


//----------------------------------------------------------------------------------------------------------------------
// DRAW TEXT
// GDI VERSION
// pR can be NULL
// eType is subhead, text, or note
//----------------------------------------------------------------------------------------------------------------------
int cDlg::draw_text ( HWND hwnd, RECT *pR, _eType eType, const wchar_t * pTxt, bool bDraw, DWORD dwStyles )
{
	HDC hdc = GetDC ( hwnd );
	RECT rect;

	if ( ! pR )
	{
		GetClientRect ( hwnd, &rect );
		pR = &rect;
	}

	int iResult = draw_text ( hdc, pR, eType, pTxt, bDraw, dwStyles );
	ReleaseDC ( hwnd, hdc );

	return iResult;
}




//----------------------------------------------------------------------------------------------------------------------
// LPARAM TO * WIN
//----------------------------------------------------------------------------------------------------------------------
cWin * cDlg::lParam_to_pWin ( HWND hDlg, LPARAM lParam )
{
	UNREFERENCED_PARAMETER ( lParam );

	return reinterpret_cast<cWin*>(static_cast<LONG_PTR> ( GetWindowLongPtr ( hDlg, GWLP_USERDATA ) ) );
}


//----------------------------------------------------------------------------------------------------------------------
// LPARAM TO USER DATA 
//----------------------------------------------------------------------------------------------------------------------
void cDlg::set_user_data ( HWND hDlg, LPARAM lParam )
{
	SetWindowLongPtr ( hDlg, GWLP_USERDATA, static_cast<LONG_PTR>(lParam) );
}


//----------------------------------------------------------------------------------------------------------------------
// DEFAULT DIALOG PROC
//----------------------------------------------------------------------------------------------------------------------
INT_PTR CALLBACK cDlg::dialog_proc ( HWND hDlg, UINT uMessage, WPARAM wParam, LPARAM lParam )
{
	cWin * pWin = user_data_to_pWin ( hDlg );
	cDlg * pThis = static_cast<cDlg*>(pWin);

	switch ( uMessage )
	{
			
	case WM_SIZE:
		if ( pThis )
			pThis->cWin::wm_size ( LOWORD(lParam), HIWORD(lParam) );
		break;

	case WM_INITDIALOG:
		{
			set_user_data ( hDlg, lParam );
			cWin * pWin = lParam_to_pWin ( hDlg, lParam );
			pThis = static_cast<cDlg*>(pWin);

			HICON hIcon = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE ( IDI_mojo ), IMAGE_ICON, 16, 16, 0);
			SendMessage ( pThis->hwnd, WM_SETICON, ICON_BIG, (LPARAM) hIcon ) ;
		}
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		{
			int iID    = LOWORD(wParam);
			int iEvent = HIWORD(wParam);
			iEvent;

			if ( iID == ID_OK || iID == ID_CANCEL || iID == ID_CANCEL || iID == ID_ACCEPT )
			{
				DestroyWindow ( hDlg );
				pThis->hwnd = 0;  ///////////// DO THIS HERE IN BASE CLASS, NOT IN DERIVED CLASSES
				return (INT_PTR)TRUE;
			}
		}
		break;
	}

	return (INT_PTR)FALSE;
}


//----------------------------------------------------------------------------------------------------------------------
// PROC -- MODAL
//----------------------------------------------------------------------------------------------------------------------
INT_PTR CALLBACK cDlgModal::dialog_proc (HWND hDlg, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	cWin * pWin = user_data_to_pWin ( hDlg );
	cDlgModal * pThis = static_cast<cDlgModal*>(pWin);

	switch ( uMessage )
	{

	case WM_INITDIALOG:
		{
			set_user_data ( hDlg, lParam );
			cWin * pWin = lParam_to_pWin ( hDlg, lParam );
			pThis = static_cast<cDlgModal*>(pWin);
			HICON hIcon = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE ( IDI_mojo ), IMAGE_ICON, 16, 16, 0);
			SendMessage ( pThis->hwnd, WM_SETICON, ICON_BIG, (LPARAM) hIcon ) ;
		}
		return (INT_PTR)TRUE;

	case WM_SIZE:
		if ( pThis )
			pThis->cWin::wm_size ( LOWORD(lParam), HIWORD(lParam) );
		break;

	case WM_MOVE:
		if ( pThis )
			pThis->cWin::wm_move ( LOWORD(lParam), HIWORD(lParam) );
		break;

	case WM_COMMAND:
		{
			switch ( LOWORD ( wParam ) )  // iID
			{
			case ID_OK:
			case ID_CANCEL:
			case IDOK:
			case IDCANCEL:
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}
		}
		break;
	}
	return (INT_PTR)FALSE;
}

////////////// FOLLOWING CODE HAS BEEN MOVED TO cDlgVars:

//----------------------------------------------------------------------------------------------------------------------
// UPDATE VARS
//----------------------------------------------------------------------------------------------------------------------
#if 0
void cDlg :: update_vars ()
{
	for ( sDlgDatum * d = DlgData.pHead; d; d = d->pNext )
	{
		d->update_var ();
	}
}
#endif

//----------------------------------------------------------------------------------------------------------------------
// UPDATE DLG
//----------------------------------------------------------------------------------------------------------------------
#if 0
void cDlg :: update_dlg ()
{
	for ( sDlgDatum * d = DlgData.pHead; d; d = d->pNext )
	{
		d->update_ctrl ();
	}
}
#endif


#if 0
//----------------------------------------------------------------------------------------------
// UPDATE VAR -- INT
//----------------------------------------------------------------------------------------------
void sDlgDatumInt::update_var ( HWND hParent, void * pVar, int iID )
{
	HWND hCtrl = GetDlgItem ( hParent, iID );
	wchar_t b[100];
	Edit_GetText ( hCtrl, b, sizeof(b)-1 );
	*(int*)pVar = wcstoul ( b, NULL, 10 );
}


//----------------------------------------------------------------------------------------------
// UPDATE CTRL -- INT
//----------------------------------------------------------------------------------------------
void sDlgDatumInt::update_ctrl ( HWND hParent, int ID, void * pVar )
{
	HWND hCtrl = GetDlgItem ( hParent, iID );
	wchar_t b[200];
	int iVal = *(int*)pVar;
	_itow_s ( iVal, b, sizeof(b)/sizeof(wchar_t), 10 );
	Edit_SetText ( hCtrl, b );
}



//----------------------------------------------------------------------------------------------
// UPDATE VAR -- UINT
//----------------------------------------------------------------------------------------------
void sDlgDatumUnsigned::update_var ()
{
	HWND hCtrl = GetDlgItem ( hParent, iID );
	wchar_t b[100];
	Edit_GetText ( hCtrl, b, sizeof(b)-1 );
	*(unsigned*)pVar = wcstoul ( b, NULL, 10 );
}


//----------------------------------------------------------------------------------------------
// UPDATE DLG -- UINT
//----------------------------------------------------------------------------------------------
void sDlgDatumUnsigned::update_ctrl ()
{
	HWND hCtrl = GetDlgItem ( hParent, iID );
	wchar_t b[200];
	unsigned uVal = *(unsigned*)pVar;
	_ultow_s ( uVal, b, sizeof(b)/sizeof(wchar_t), 10 );
	Edit_SetText ( hCtrl, b );
}


//----------------------------------------------------------------------------------------------
// UPDATE VAR -- bool
//----------------------------------------------------------------------------------------------
void sDlgDatumBool::update_var ()
{
	if ( IsDlgButtonChecked ( hParent, iID ) )
		*pVar = true;

	else
		*pVar = false;
}


//----------------------------------------------------------------------------------------------
// UPDATE DLG -- bool
//----------------------------------------------------------------------------------------------
void sDlgDatumBool::update_ctrl ()
{
	// HWND hCtrl = GetDlgItem ( hParent, iID );

	if ( *pVar )
		CheckDlgButton ( hParent, iID, TRUE );

	else
		CheckDlgButton ( hParent, iID, FALSE );
}

#endif


/***********************************************************************************************************************
/*
/*    This file is part of Mojo.  For more information, see http://mojoware.org.
/*
/*    You may redistribute and/or modify Mojo under the terms of the GNU General Public License, version 3, as
/*    published by the Free Software Foundation.  You should have received a copy of the license with mojo.  If you
/*    did not, go to http://www.gnu.org.
/* 
/*    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT
/*    NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
/*    IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
/*    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
/*    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
/*    STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
/*    EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
/*
/***********************************************************************************************************************/