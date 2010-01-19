/***********************************************************************************************************************
/*
/*    cDlgFigPredefinedHotkeys.cpp / mojo_app
/*   
/*    Copyright 2010 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"


//======================================================================================================================
// DATA
//======================================================================================================================

sDlgDatum cDlgFigPredefinedHotkeys :: aData [] = 
{
	sDlgDatum ( ID_TOGGLE_MOJO_TRIGGER,                   L"ToggleMojo"                         ),
	sDlgDatum ( ID_TOGGLE_WINDOW_BROADCAST_TRIGGER,       L"ToggleWindowBroadcast"              ),
	sDlgDatum ( ID_TOGGLE_COMPUTER_BROADCAST_TRIGGER,     L"ToggleComputerBroadcast"            ),
	sDlgDatum ( ID_TOGGLE_MOUSEOVER_TRIGGER,              L"ToggleMouseover"                    ),
	sDlgDatum ( ID_TOGGLE_HOTKEYS_TRIGGER,                L"ToggleHotkeys"                      ),
	sDlgDatum ( ID_BRING_MOUSEOVER_CURSOR_HOME_TRIGGER,   L"BringMouseoverCursorHome"           ),
	sDlgDatum ( ID_HIDE_SHOW_MOJO_TRIGGER,                L"HideShowMojo"                       ),
	sDlgDatum ( 0,                                        NULL                                  ),
};


//======================================================================================================================
// PROTOTYPES
//======================================================================================================================

//======================================================================================================================
// CODE
//======================================================================================================================



//----------------------------------------------------------------------------------------------------------------------
//  SET TEXT
//----------------------------------------------------------------------------------------------------------------------
void cDlgFigPredefinedHotkeys :: set_text ()
{
	set_item_text ( ID_LINK, 						L"DlgFigPredefinedHotkeys.Link", L"http://mojoware.org/help/predefined_hotkeys.html" );
}

//----------------------------------------------------------------------------------------------------------------------
// WM INIT
//----------------------------------------------------------------------------------------------------------------------
void cDlgFigPredefinedHotkeys :: wm_init ()
{

	pFigOriginal = reinterpret_cast<cFigPredefinedHotkeys*>(pUserData);
	pUserData = 0;

	VarsFig.wm_init ( hwnd, aData, reinterpret_cast<cFig *>( pFigOriginal ) );
	set_text();

	InvalidateRect ( hwnd, NULL, TRUE );
	UpdateWindow ( hwnd );
}

//----------------------------------------------------------------------------------------------------------------------
// ON BUTTON
//----------------------------------------------------------------------------------------------------------------------
void cDlgFigPredefinedHotkeys :: on_button ( int iButtonID )
{
	cDlgSetTrigger d;
	mojo::cTrigger t;
	int iTriggerID = 0;

	switch ( iButtonID )
	{
	case ID_TOGGLE_MOJO_BUTTON:                  iTriggerID = ID_TOGGLE_MOJO_TRIGGER;                 break;
	case ID_TOGGLE_WINDOW_BROADCAST_BUTTON:      iTriggerID = ID_TOGGLE_WINDOW_BROADCAST_TRIGGER;     break;
	case ID_TOGGLE_COMPUTER_BROADCAST_BUTTON:    iTriggerID = ID_TOGGLE_COMPUTER_BROADCAST_TRIGGER;   break;
	case ID_TOGGLE_MOUSEOVER_BUTTON:             iTriggerID = ID_TOGGLE_MOUSEOVER_TRIGGER;            break;
	case ID_TOGGLE_HOTKEYS_BUTTON:               iTriggerID = ID_TOGGLE_HOTKEYS_TRIGGER;              break;
	case ID_BRING_MOUSEOVER_CURSOR_HOME_BUTTON:  iTriggerID = ID_BRING_MOUSEOVER_CURSOR_HOME_TRIGGER; break;
	case ID_HIDE_SHOW_MOJO_BUTTON:               iTriggerID = ID_HIDE_SHOW_MOJO_TRIGGER;              break;
	default: assert(0);
	}

	if ( IDOK == d.make_dlg(&t) )
	{
		cStrW s;
		t.print ( &s );
		SetWindowText ( GetDlgItem ( hwnd, iTriggerID ), s.cstr() );
	}
}


//----------------------------------------------------------------------------------------------------------------------
// DEFAULT PROC
//----------------------------------------------------------------------------------------------------------------------
INT_PTR CALLBACK cDlgFigPredefinedHotkeys::dialog_proc (HWND hwnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	cWin * pWin = user_data_to_pWin ( hwnd );
	cDlgFigPredefinedHotkeys * pThis = static_cast<cDlgFigPredefinedHotkeys*>(pWin);

	switch ( uMessage )
	{
	case WM_INITDIALOG:
		{
			set_user_data ( hwnd, lParam );
			cWin * pWin = lParam_to_pWin ( hwnd, lParam );
			pThis = static_cast<cDlgFigPredefinedHotkeys*>(pWin);
			pThis->hwnd = hwnd;
			pThis->wm_init();
		}
		break;


	case WM_CTLCOLORSTATIC:
		{
			wchar_t awName [2000];
			GetClassName ( (HWND)lParam, awName, sizeof(awName)/sizeof(wchar_t ) );
			if ( 0 == wcscmp ( L"Edit", awName ) )
			{
				LONG_PTR Style = GetWindowLongPtr ( (HWND)lParam, GWL_STYLE );
				if ( 0 == ( WS_DISABLED & Style ) )
					return (INT_PTR) GetStockObject ( WHITE_BRUSH );
			}
		}
		break;

	case WM_COMMAND:
		switch ( LOWORD ( wParam ) )
		{
		case ID_TOGGLE_MOJO_BUTTON:
		case ID_TOGGLE_WINDOW_BROADCAST_BUTTON:
		case ID_TOGGLE_COMPUTER_BROADCAST_BUTTON:
		case ID_TOGGLE_MOUSEOVER_BUTTON:
		case ID_TOGGLE_HOTKEYS_BUTTON:
		case ID_BRING_MOUSEOVER_CURSOR_HOME_BUTTON:
		case ID_HIDE_SHOW_MOJO_BUTTON:
			pThis->on_button ( LOWORD(wParam) );
			break;

		case ID_CANCEL:
		case IDCANCEL:
			break;

		case ID_OK:
		case IDOK:
			pThis->VarsFig.dlg_to_vars();
			g_FigMgr.set_fig ( pThis->VarsFig.pFigOriginal->dwSerialNumber, (cFigWoW*)pThis->VarsFig.pFigCopy );
			g_FigMgr.save_to_file();
			pThis->pFigOriginal->set_engine();
			break;
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