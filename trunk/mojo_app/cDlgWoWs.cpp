/***********************************************************************************************************************
/*
/*    cDlgWoWs.cpp / mojo_app
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"



//======================================================================================================================
// DATA
//======================================================================================================================

const int iButtonHeight = 27;
const int iMargin = 9;

//======================================================================================================================
// PROTOTYPES
//======================================================================================================================

//======================================================================================================================
// CODE
//======================================================================================================================



//----------------------------------------------------------------------------------------------------------------------
// WM INIT
//----------------------------------------------------------------------------------------------------------------------
void cDlgWoWs::wm_init ()
{
	const int iMargin = 9;
	const int iLeftMargin = 0;

	ListView.hwnd = GetDlgItem ( hwnd, ID_LIST_VIEW );
	register_child ( &ListView,

							  nAnchor::left,		0,		iLeftMargin,
							  nAnchor::top,			0,		iMargin,
							  nAnchor::right,		0,		-iMargin,
							  nAnchor::bottom,		0,      -iMargin );
}


//----------------------------------------------------------------------------------------------------------------------
// DEFAULT PROC
//----------------------------------------------------------------------------------------------------------------------
INT_PTR CALLBACK cDlgWoWs::dialog_proc (HWND hwnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	cWin * pWin = user_data_to_pWin ( hwnd );
	cDlgWoWs * pThis = static_cast<cDlgWoWs*>(pWin);

	switch ( uMessage )
	{


	case WM_COMMAND:
		{
			if ( BN_CLICKED == HIWORD ( wParam ) )
			{
				switch 	( LOWORD ( wParam ) )
				{
				case ID_TOGGLE_MOUSEOVER:
					PostMessage ( g_hwnd, WM_COMMAND, ID_TOGGLE_MOUSEOVER, ID_TOGGLE_MOUSEOVER );
					break;

				case ID_TOGGLE_HOTKEYS:
					pThis->cWin::balloon ( GetDlgItem ( hwnd, ID_TOGGLE_HOTKEYS ), L"Sorry.", L"Hotkeys aren't implemented yet." );
					break;

				case ID_TOGGLE_BROADCAST:
					PostMessage ( g_hwnd, WM_COMMAND, ID_TOGGLE_BROADCAST, ID_TOGGLE_BROADCAST );
					break;

				case ID_VIEW_MONITOR:
					PostMessage ( g_hwnd, WM_COMMAND, ID_VIEW_MONITOR, 0 );
					break;

				case ID_VIEW_WOWS:
					PostMessage ( g_hwnd, WM_COMMAND, ID_VIEW_WOWS, 0 );
					break;
				}
			}
		}
		break;


	case WM_INITDIALOG:
		{
			set_user_data ( hwnd, lParam );
			cWin * pWin = lParam_to_pWin ( hwnd, lParam );
			pThis = static_cast<cDlgWoWs*>(pWin);
			pThis->hwnd = hwnd;
			pThis->wm_init ();
		}
		break;

#if 0
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
#endif

#if 0
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