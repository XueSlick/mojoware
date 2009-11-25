/***********************************************************************************************************************
/*
/*    cDlgMessageBox.h / mojo_app
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"
#include "cDlgMessageBox.h"
#include "window_stuff.h"


//===============================================================================================
// DATA
//===============================================================================================

//===============================================================================================
// PROTOTYPES
//===============================================================================================

//===============================================================================================
// CODE
//===============================================================================================

//-----------------------------------------------------------------------------------------------
// ON INIT
//-----------------------------------------------------------------------------------------------
INT_PTR cDlgMessageBox::wm_init ( HWND hwnd )
{
	// window title
	SetWindowText ( hwnd, g_awAppTitle );

	// get required text height
	int iTextHeight = draw_text ( hwnd, false );

	// adjust size of text ctrl which is a static rectangle
	HWND hCtrl = GetDlgItem ( hwnd, ID_TEXT );
	RECT rCtrl;
	GetClientRect ( hCtrl, &rCtrl );
	int iOldCtrlHeight = rCtrl.bottom - rCtrl.top;
	cPtI CtrlUL (0,0);
	client_coord_to_parent_client_coord ( hCtrl, &CtrlUL );
	MoveWindow ( hCtrl, CtrlUL.x, CtrlUL.y, rCtrl.right - rCtrl.left, iTextHeight, TRUE );
	int iHeightDelta = iTextHeight - iOldCtrlHeight;

	// adjust size of dialog box
	RECT rDlg;
	GetWindowRect ( hwnd, &rDlg );
	MoveWindow ( hwnd, rDlg.left, rDlg.top, rDlg.right - rDlg.left, ( rDlg.bottom - rDlg.top ) + iHeightDelta, TRUE );

	// adjust position of button
	HWND hButton = GetDlgItem ( hwnd, ID_OK );
	RECT rB;
	GetClientRect ( hButton, &rB );
	cPtI ButtonUL (0,0);
	client_coord_to_parent_client_coord ( hButton, &ButtonUL );
	MoveWindow ( hButton, ButtonUL.x, ButtonUL.y + iHeightDelta, rB.right - rB.left, rB.bottom - rB.top, TRUE );

	return (INT_PTR) TRUE;
}


//-----------------------------------------------------------------------------------------------
// DRAW TEXT
// if bDraw is false, this calculates required height and returns it instead of drawing
//-----------------------------------------------------------------------------------------------
#if 1
int cDlgMessageBox::draw_text ( HWND hwnd, bool bDraw )
{
	wchar_t * pTxt = reinterpret_cast<wchar_t *>(pUserData);

	if ( ! pTxt )
		return 0;

	RECT rDlg, rCtrl;
	HWND hCtrl = GetDlgItem ( hwnd, ID_TEXT );

	GetClientRect ( hwnd, &rDlg );
	GetClientRect ( hCtrl, &rCtrl );
	HDC hdc = GetDC ( hCtrl );
	SetBkMode ( hdc, TRANSPARENT );
	HFONT hFont = g_hMenuFont;
	SelectObject ( hdc, hFont );

	int iRetVal = 0;

	if ( ! bDraw )
		iRetVal = DrawText ( hdc, pTxt, (int) wcslen ( pTxt ), &rCtrl, DT_EXPANDTABS | DT_LEFT | DT_TOP | DT_WORDBREAK | DT_CALCRECT );

	else
		iRetVal = DrawText ( hdc, pTxt, (int) wcslen ( pTxt ), &rCtrl, DT_EXPANDTABS | DT_LEFT | DT_TOP | DT_WORDBREAK );

	ReleaseDC ( hCtrl, hdc );

	return iRetVal;
}
#endif


//----------------------------------------------------------------------------------------------
// DEFAULT PROC
//----------------------------------------------------------------------------------------------
INT_PTR CALLBACK cDlgMessageBox::dialog_proc (HWND hwnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	cWin * pWin = user_data_to_pWin ( hwnd );
	cDlgMessageBox * pThis = static_cast<cDlgMessageBox*>(pWin);

	switch ( uMessage )
	{
	case WM_PAINT:
		pThis->draw_text ( hwnd );
		break;

	case WM_INITDIALOG:
		{
			set_user_data ( hwnd, lParam );
			cWin * pWin = lParam_to_pWin ( hwnd, lParam );
			pThis = static_cast<cDlgMessageBox*>(pWin);
			pThis->hwnd = hwnd;
			return pThis->wm_init ( hwnd );
		}
		break;

	case WM_CTLCOLORDLG:
			return (INT_PTR) GetStockObject ( WHITE_BRUSH );
			break;

	case WM_COMMAND:
		{
			int iID = LOWORD(wParam);

			if ( iID == ID_OK )
			{

			}
		}
		break;

	default:
		break;
	}

	return cDlgModal::dialog_proc ( hwnd, uMessage, wParam, lParam );
}


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