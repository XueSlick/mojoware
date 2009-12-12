/***********************************************************************************************************************
/*
/*    cDlgGetTrigger.cpp / mojo_app
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"
// #include "wingdi.h"

//======================================================================================================================
// DATA
//======================================================================================================================

const int cDlgGetTrigger :: s_iQtyInRow = 4; // of combo box fields for keys
const int cDlgGetTrigger :: s_iComboDimY = 25;
const int cDlgGetTrigger :: iMarginX = 24;
const int cDlgGetTrigger :: iMarginY = 12;
const int cDlgGetTrigger :: iGutter = 5;


//======================================================================================================================
// CODE
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
//  SET TEXT
//----------------------------------------------------------------------------------------------------------------------
void cDlgGetTrigger :: set_text ()
{
	set_item_text ( ID_LINK, 						L"DlgGetTrigger.Link", L"http://mojoware.org/help/triggers.html" );
}


//----------------------------------------------------------------------------------------------------------------------
//  SET STATE
//----------------------------------------------------------------------------------------------------------------------
void cDlgGetTrigger :: set_state ( int iID )
{
	switch ( iID )
	{
	case ID_CAPSLOCK_ON:
		if ( IsDlgButtonChecked ( hwnd, ID_CAPSLOCK_ON ) )
			CheckDlgButton ( hwnd, ID_CAPSLOCK_OFF, FALSE );
		break;

	case ID_CAPSLOCK_OFF:
		if ( IsDlgButtonChecked ( hwnd, ID_CAPSLOCK_OFF ) )
			CheckDlgButton ( hwnd, ID_CAPSLOCK_ON, FALSE );
		break;

	case ID_NUMLOCK_ON:
		if ( IsDlgButtonChecked ( hwnd, ID_NUMLOCK_ON ) )
			CheckDlgButton ( hwnd, ID_NUMLOCK_OFF, FALSE );
		break;

	case ID_NUMLOCK_OFF:
		if ( IsDlgButtonChecked ( hwnd, ID_NUMLOCK_OFF ) )
			CheckDlgButton ( hwnd, ID_NUMLOCK_ON, FALSE );
		break;

	case ID_SCROLLLOCK_ON:
		if ( IsDlgButtonChecked ( hwnd, ID_SCROLLLOCK_ON ) )
			CheckDlgButton ( hwnd, ID_SCROLLLOCK_OFF, FALSE );
		break;

	case ID_SCROLLLOCK_OFF:
		if ( IsDlgButtonChecked ( hwnd, ID_SCROLLLOCK_OFF ) )
			CheckDlgButton ( hwnd, ID_SCROLLLOCK_ON, FALSE );
		break;
	}
}


//----------------------------------------------------------------------------------------------------------------------
// CHANGE SIZE
//----------------------------------------------------------------------------------------------------------------------
void cDlgGetTrigger :: change_size ()
{
	int iNewQtyOfRows = 1 + ( ( ((int)aCombo.qty()) - 1 ) / s_iQtyInRow );

	int iExtraDimY = ( iNewQtyOfRows - 1 ) * ( iGutter + s_iComboDimY );
	RECT r;
	GetWindowRect ( hwnd, &r );
	int iNewBottom = r.top + iInitialWindowHeight + iExtraDimY;

	if ( iNewBottom != r.bottom )
		MoveWindow ( hwnd, r.left, r.top, r.right - r.left, iNewBottom - r.top, TRUE );
}


//----------------------------------------------------------------------------------------------------------------------
// ON CLEAR
//----------------------------------------------------------------------------------------------------------------------
void cDlgGetTrigger :: on_clear ()
{
	CheckDlgButton ( hwnd, ID_CAPSLOCK_ON,    FALSE );
	CheckDlgButton ( hwnd, ID_CAPSLOCK_OFF,   FALSE );
	CheckDlgButton ( hwnd, ID_NUMLOCK_ON,     FALSE );
	CheckDlgButton ( hwnd, ID_NUMLOCK_OFF,    FALSE );
	CheckDlgButton ( hwnd, ID_SCROLLLOCK_ON,  FALSE );
	CheckDlgButton ( hwnd, ID_SCROLLLOCK_OFF, FALSE );

	for ( unsigned i = 0; i < aCombo.qty(); i++ )
		DestroyWindow ( aCombo[i].hwnd );

	aCombo.erase();

	add_combo();
}



//----------------------------------------------------------------------------------------------------------------------
// WM KEY EVENT
//----------------------------------------------------------------------------------------------------------------------
void cDlgGetTrigger :: wm_key_event ( WORD wExVK )
{
	// WORD wExVK;

	// while ( KeyBuf.get ( &wExVK ) )
	{
		int iComboQty = aCombo.qty();

		HWND hFocus = GetFocus();

		for ( int i = 0; i < iComboQty; i++ )
		{
			if ( aCombo[i].hEdit == hFocus )
			{
				SetWindowText ( aCombo[i].hEdit, cKeyboard::ex_vk_to_pretty_name ( wExVK ) );
				on_combo_changed ( aCombo[i].hwnd );
			}
		}
	}
}


//----------------------------------------------------------------------------------------------
// WM INIT
//----------------------------------------------------------------------------------------------
void cDlgGetTrigger :: wm_init ()
{
	CheckDlgButton ( hwnd, ID_PRESSED, TRUE );

	set_text ();

	RECT r;
	GetWindowRect ( hwnd, &r );
	iInitialWindowHeight = r.bottom - r.top;

	int iCtrlDimY = 23;
	int iMarginY  = 12;
	int iGutterY  = 6;

	iInitialComboPosY = 81;

	HWND hHead = GetDlgItem ( hwnd, ID_HEAD );
	SetWindowFont ( hHead, g_hDialogBoxHeadFont, TRUE );

	mojo::register_for_key_events ( /* &KeyBuf, */ hwnd );
	mojo::start_swallowing_key_events ( hwnd );

	add_combo();

	const int iDimX        = 90;

	int iLockDimX = 160;
	int iLockPosX = iMarginX * 2;
	int iLockBottom = iMarginY + iCtrlDimY * 3;
	int iPressedBottom = iLockBottom + 3 * iCtrlDimY + iMarginY;

	//---------------------------------
	//  PRESSED STUFF
	//---------------------------------

	PressedLabel.hwnd = GetDlgItem ( hwnd, ID_PRESSED_LABEL );
	PressedLabel.init();
	register_child ( &PressedLabel,
							  nAnchor::left,		0,		iMarginX,
							  nAnchor::bottom,		0,		- ( iPressedBottom + 3 * iCtrlDimY ) ,
							  nAnchor::right,		0,		-iMarginX,
							  nAnchor::bottom,		0,      - ( iPressedBottom + 2 * iCtrlDimY )  );

	Pressed.hwnd = GetDlgItem ( hwnd, ID_PRESSED );
	register_child ( &Pressed,
							  nAnchor::left,		0,		iLockPosX ,
							  nAnchor::bottom,		0,		- ( iPressedBottom + 2 * iCtrlDimY ),
							  nAnchor::left,		0,		iLockPosX + iLockDimX,
							  nAnchor::bottom,		0,      - ( iPressedBottom + iCtrlDimY ) );

	Released.hwnd = GetDlgItem ( hwnd, ID_RELEASED );
	register_child ( &Released,
							  nAnchor::left,		0,		iLockPosX ,
							  nAnchor::bottom,		0,		- ( iPressedBottom + iCtrlDimY ),
							  nAnchor::left,		0,		iLockPosX + iLockDimX,
							  nAnchor::bottom,		0,      - iPressedBottom );

	//---------------------------------
	//  LOCK STUFF
	//---------------------------------
	



	LockLabel.hwnd = GetDlgItem ( hwnd, ID_LOCK_LABEL );
	LockLabel.init();
	register_child ( &LockLabel,
							  nAnchor::left,		0,		iMarginX,
							  nAnchor::bottom,		0,		- ( iLockBottom + 3 * iCtrlDimY ) ,
							  nAnchor::right,		0,		-iMarginX,
							  nAnchor::bottom,		0,      - ( iLockBottom + 2 * iCtrlDimY  )  );

	CapsLockOn.hwnd = GetDlgItem ( hwnd, ID_CAPSLOCK_ON );
	register_child ( &CapsLockOn,
							  nAnchor::left,		0,		iLockPosX,
							  nAnchor::bottom,		0,		- ( iLockBottom + 2 * iCtrlDimY  ) ,
							  nAnchor::left,		0,		iMarginX + iLockDimX,
							  nAnchor::bottom,		0,      - ( iLockBottom + 1 * iCtrlDimY  )  );

	NumLockOn.hwnd = GetDlgItem ( hwnd, ID_NUMLOCK_ON );
	register_child ( &NumLockOn,
							  nAnchor::left,		0,		iLockPosX + iLockDimX,
							  nAnchor::bottom,		0,		- ( iLockBottom + 2 * iCtrlDimY  ) ,
							  nAnchor::left,		0,		iMarginX + 2 * iLockDimX,
							  nAnchor::bottom,		0,      - ( iLockBottom + 1 * iCtrlDimY  )  );

	ScrollLockOn.hwnd = GetDlgItem ( hwnd, ID_SCROLLLOCK_ON );
	register_child ( &ScrollLockOn,
							  nAnchor::left,		0,		iLockPosX + 2* iLockDimX,
							  nAnchor::bottom,		0,		- ( iLockBottom + 2 * iCtrlDimY  ) ,
							  nAnchor::left,		0,		iMarginX + 3 * iLockDimX,
							  nAnchor::bottom,		0,      - ( iLockBottom + 1 * iCtrlDimY  )  );

	CapsLockOff.hwnd = GetDlgItem ( hwnd, ID_CAPSLOCK_OFF );
	register_child ( &CapsLockOff,
							  nAnchor::left,		0,		iLockPosX,
							  nAnchor::bottom,		0,		- ( iLockBottom + iCtrlDimY ),
							  nAnchor::left,		0,		iMarginX + iLockDimX,
							  nAnchor::bottom,		0,      - iLockBottom );

	NumLockOff.hwnd = GetDlgItem ( hwnd, ID_NUMLOCK_OFF );
	register_child ( &NumLockOff,
							  nAnchor::left,		0,		iLockPosX + iLockDimX,
							  nAnchor::bottom,		0,		- ( iLockBottom + iCtrlDimY ),
							  nAnchor::left,		0,		iMarginX + 2 * iLockDimX,
							  nAnchor::bottom,		0,      - iLockBottom );

	ScrollLockOff.hwnd = GetDlgItem ( hwnd, ID_SCROLLLOCK_OFF );
	register_child ( &ScrollLockOff,
							  nAnchor::left,		0,		iLockPosX + 2* iLockDimX,
							  nAnchor::bottom,		0,		- ( iLockBottom + iCtrlDimY ),
							  nAnchor::left,		0,		iMarginX + 3 * iLockDimX,
							  nAnchor::bottom,		0,      - iLockBottom );


	//---------------------------------
	//  BOTTOM BUTTONS
	//---------------------------------

	Clear.hwnd = GetDlgItem ( hwnd, ID_CLEAR );
	register_child ( &Clear,
							  nAnchor::left,		0,		iMarginX,
							  nAnchor::bottom,		0,		- ( 1 * iMarginY + iCtrlDimY ),
							  nAnchor::left,		0,		iMarginX + iDimX,
							  nAnchor::bottom,		0,      - ( 1 * iMarginY ) );

	OK.hwnd = GetDlgItem ( hwnd, IDOK );
	register_child ( &OK,
							  nAnchor::right,		0,		- ( iMarginX * 2 + iDimX * 2 ), 
							  nAnchor::bottom,		0,		- ( 1 * iMarginY + iCtrlDimY ),
							  nAnchor::right,		0,		- ( iMarginX * 2 + iDimX  ),
							  nAnchor::bottom,		0,      - ( 1 * iMarginY ) );

	Cancel.hwnd = GetDlgItem ( hwnd, IDCANCEL );
	register_child ( &Cancel,
							  nAnchor::right,		0,		- ( iMarginX * 1 + iDimX * 1 ), 
							  nAnchor::bottom,		0,		- ( 1 * iMarginY + iCtrlDimY ),
							  nAnchor::right,		0,		- ( iMarginX * 1 ),
							  nAnchor::bottom,		0,      - ( 1 * iMarginY ) );

	Link.hwnd = GetDlgItem ( hwnd, ID_LINK );
	register_child ( &Link,
							  nAnchor::left,		0,		iMarginX,
							  nAnchor::bottom,		0,		- ( 3 * iGutterY + 2 * iCtrlDimY ),
							  nAnchor::left,		0,		iMarginX + 200,
							  nAnchor::bottom,		0,      - ( 3 * iGutterY + iCtrlDimY) );

	this->reposition_children ();
}


//----------------------------------------------------------------------------------------------------------------------
// DEFAULT PROC
//----------------------------------------------------------------------------------------------------------------------
INT_PTR CALLBACK cDlgGetTrigger::dialog_proc (HWND hwnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	cWin * pWin = user_data_to_pWin ( hwnd );
	cDlgGetTrigger * pThis = static_cast<cDlgGetTrigger*>(pWin);

	switch ( uMessage )
	{
	case WM_SIZE:
		if ( pThis )
		{
			pThis->cWin::wm_size ( LOWORD(lParam), HIWORD(lParam) );
			InvalidateRect ( hwnd, NULL, FALSE );
			UpdateWindow ( hwnd );
			return ( INT_PTR ) TRUE;
		}
		break;

	case WM_CTLCOLORSTATIC:

		if ( lParam == (long) GetDlgItem ( hwnd, ID_HEAD ) )
		{
			HDC hdc = (HDC)wParam;
			SetTextColor ( hdc, RGB ( 90, 104, 170 ) );		
			SetBkMode(hdc, TRANSPARENT );
			SetDCBrushColor ( hdc, GetSysColor ( COLOR_BTNFACE ) );
			return (LONG) GetStockObject ( DC_BRUSH );
		}
		break;

	case mojo::uWM_KEY_EVENT_OCCURRED:
		pThis->wm_key_event((WORD)wParam); // wExVK );
		break;

#if 0
	case WM_PAINT:
		break;
#endif

	case WM_INITDIALOG:
		{
			set_user_data ( hwnd, lParam );
			cWin * pWin = lParam_to_pWin ( hwnd, lParam );
			pThis = static_cast<cDlgGetTrigger*>(pWin);
			pThis->hwnd = hwnd;
			pThis->wm_init ();
		}
		break;

	case WM_COMMAND:
		{
			if ( CBN_SELCHANGE == HIWORD ( wParam ) )
				pThis->on_combo_changed ( ( HWND ) lParam );

			int iID = LOWORD ( wParam );

			switch ( iID )
			{
			case ID_CLEAR:
				pThis->on_clear ();
				break;

			case IDCANCEL:
				mojo::unregister_for_key_events ( hwnd );
				break;

			case IDOK:

				if ( ! pThis->init_trigger () )
					return true; // stay in the dialog

				else
				{
					mojo::unregister_for_key_events ( hwnd );
					*((mojo::cTrigger*)pThis->pUserData ) = pThis->Trigger;

					//------------------------------------
					//   TEMP FOR TESTING
					//------------------------------------

					cStrW s, t;
					pThis->Trigger.print ( &t );

					s.f ( L"For testing purposes, Mojo has stored your trigger "
						  L"in an internal format and then converted it from that "
						  L"format into something humans can read.  Here's the result "
						  L"of that process:\n\n"

						  L"      %s\n\n"

						  L"If that's not what you entered, please let Freddie know.\n\n"

						  L"It doesn't matter if Mojo changes the order as long as the "
						  L"last key stays at the end.  The last key is special since it's "
						  L"the real trigger.  The other keys are preconditions.\n\n"

						  L"Thanks a lot.",

						  t.cstr() );

					MessageBox ( hwnd, s.cstr(), g_awAppTitle, MB_OK );

				}
				break;

			case ID_NUMLOCK_ON:
			case ID_NUMLOCK_OFF:
			case ID_CAPSLOCK_ON:
			case ID_CAPSLOCK_OFF:
			case ID_SCROLLLOCK_ON:
			case ID_SCROLLLOCK_OFF:
				pThis->set_state ( iID );
				pThis->draw_trigger();
			}
		}

		break;

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