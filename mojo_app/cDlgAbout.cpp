/***********************************************************************************************************************
/*
/*    cDlgAbout.cpp / mojo_app
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"
#include "cDlgAbout.h"
#include "window_stuff.h"


//======================================================================================================================
// DATA
//======================================================================================================================

//======================================================================================================================
// PROTOTYPES
//======================================================================================================================

//======================================================================================================================
// CODE
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
// WM INITDIALOG
//----------------------------------------------------------------------------------------------------------------------
INT_PTR cDlgAbout::wm_init ( HWND hwnd )
{
	//---------------------------------
	// LINK TEXT
	//---------------------------------

	cScribPack t1 ( L"DlgAbout.Link", g_awWebsite, g_awAppTitle  );
	SetWindowText ( GetDlgItem ( hwnd, ID_LINK ), t1.head.cstr() );

	//---------------------------------
	// WINDOW TITLE
	//---------------------------------

	cStrW m = L"About ";
	m += g_awAppTitle;
	SetWindowText ( hwnd, m.cstr() );

	//---------------------------------
	// GET REQUIRED TEXT HEIGHT
	//---------------------------------

	int iTextHeight = draw_text ( hwnd, false );

	//---------------------------------
	// ADJUST SIZE OF TEXT CONTROL
	//---------------------------------

	HWND hCtrl = GetDlgItem ( hwnd, ID_TEXT );
	RECT rCtrl;
	GetClientRect ( hCtrl, &rCtrl );
	int iOldCtrlHeight = rCtrl.bottom - rCtrl.top;
	cPtI CtrlUL (0,0);
	client_coord_to_parent_client_coord ( hCtrl, &CtrlUL );
	MoveWindow ( hCtrl, CtrlUL.x, CtrlUL.y, rCtrl.right - rCtrl.left, iTextHeight, TRUE );
	int iHeightDelta = iTextHeight - iOldCtrlHeight;

	//---------------------------------
	// ADJUST SIZE OF DIALOG BOX
	//---------------------------------
	RECT rWin;
	GetWindowRect ( hwnd, &rWin );
	rWin.bottom += iHeightDelta;
	MoveWindow ( hwnd, rWin.left, rWin.top, rWin.right - rWin.left, rWin.bottom - rWin.top , TRUE );
	
	//---------------------------------
	// BUTTON
	//---------------------------------

	HWND hButton = GetDlgItem ( hwnd, ID_OK );
	RECT rB;
	GetClientRect ( hButton, &rB );
	cPtI ButtonUL (0,0);
	client_coord_to_parent_client_coord ( hButton, &ButtonUL );
	
	MoveWindow ( hButton, ButtonUL.x, ButtonUL.y + iHeightDelta, rB.right - rB.left, rB.bottom - rB.top, TRUE );

	//----------------------------------------
	//  SYSLINK
	//----------------------------------------

	Link.hwnd = GetDlgItem ( hwnd, ID_LINK );

	register_child ( &Link,

							  nAnchor::left,		0,		CtrlUL.x,
							  nAnchor::bottom,	0,		-30,
							  nAnchor::left,		0,		CtrlUL.x + 150,
							  nAnchor::bottom,	0,		0     );

	//----------------------------------------
	//  SET SIZE TO MAKE cWIN LIBRARY 
	//  REPOSITION CHILD WINDOWS
	//----------------------------------------
	{
		RECT r;
		GetWindowRect ( hwnd, &r);
		MoveWindow ( hwnd, r.left, r.top, 1+r.right - r.left,1+ r.bottom - r.top, TRUE );
		this->move_to_center();
	}

	return (INT_PTR) TRUE;
}


//----------------------------------------------------------------------------------------------------------------------
// DRAW TEXT
// if bDraw is false, this calculates required height and returns it instead of drawing
//----------------------------------------------------------------------------------------------------------------------
int cDlgAbout::draw_text ( HWND hwnd, bool bDraw )
{
	int iRetVal = 0;

	//-------------------------------------
	// GET SCRIBS
	//-------------------------------------

	cStrW sVersion;
	g_Version.get_text ( &sVersion );

	cScribPack sp ( L"DlgAbout",
					g_awAppTitle,
					sVersion.cstr(),
					__WDATE__,
					L"http://mojoware.org" );

	//-------------------------------------
	// DRAW HEAD
	//-------------------------------------

	HWND hCtrl = GetDlgItem ( hwnd, ID_HEAD );
	cDlg::draw_text ( hCtrl, NULL, cDlg::head, sp.head.cstr(), bDraw );


	//-------------------------------------
	// DRAW BODY
	//-------------------------------------

	hCtrl = GetDlgItem ( hwnd, ID_TEXT );
	iRetVal = cDlg::draw_text ( hCtrl, NULL, cDlg::text, sp.body.cstr(), bDraw );

	return iRetVal;
}


//----------------------------------------------------------------------------------------------------------------------
// DEFAULT PROC
//----------------------------------------------------------------------------------------------------------------------
INT_PTR CALLBACK cDlgAbout::dialog_proc (HWND hwnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	cWin * pWin = user_data_to_pWin ( hwnd );
	cDlgAbout * pThis = static_cast<cDlgAbout*>(pWin);

	switch ( uMessage )
	{
	case WM_PAINT:
		pThis->draw_text ( hwnd );
		break;

	case WM_INITDIALOG:
		{
			set_user_data ( hwnd, lParam );
			cWin * pWin = lParam_to_pWin ( hwnd, lParam );
			pThis = static_cast<cDlgAbout*>(pWin);
			pThis->hwnd = hwnd;
			return pThis->wm_init ( hwnd );
		}
		break;

	case WM_CTLCOLORDLG:
	case WM_CTLCOLORSTATIC:
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

#if 1
	case WM_NOTIFY:
		{
			switch( wParam )
			{
			case ID_LINK:

				switch ( ((LPNMHDR)lParam)->code )
				{
					case NM_CLICK:
					case NM_RETURN:
					{
						wchar_t * pURL = PNMLINK(lParam)->item.szUrl;
						ShellExecute ( NULL, L"open", pURL, NULL, NULL, SW_SHOW );
					}
					break;
				}
				break;
			}
        }
		break;
#endif

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