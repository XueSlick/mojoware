/***********************************************************************************************************************
/*
/*    cDlgSettingsConnection.cpp / mojo_app
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"

//======================================================================================================================
//  DATA
//======================================================================================================================

sDlgDatum cDlgSettingsConnection :: aDlgData [] = 
{
	sDlgDatum ( ID_CONNECT_YES,							L"bConnect"						),
	sDlgDatum ( ID_AUTO_FIND_YES,						L"bConnectAutomatically"		),
	sDlgDatum ( 0,										NULL							),
};


//======================================================================================================================
//  PROTOTYPES
//======================================================================================================================

//======================================================================================================================
//  CODE
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
//  REGISTER CHILDREN
//----------------------------------------------------------------------------------------------------------------------
void cDlgSettingsConnection :: register_children ()
{
	const int iBigMargin	= 32;
	const int iMargin 		= 10;
	const int iButtonHeight = 25;
	const int iButtonWidth	= 90;

	AutoFindLabel.hwnd 	= GetDlgItem ( hwnd, ID_AUTO_FIND_LABEL );
	ListLabel.hwnd		= GetDlgItem ( hwnd, ID_LIST_LABEL );
	ConnectLabel.hwnd	= GetDlgItem ( hwnd, ID_CONNECT_LABEL );

	OK.hwnd = GetDlgItem ( hwnd, ID_OK );
	register_child ( &OK,
								nAnchor::right,		0,		- ( iMargin + iBigMargin + 2 * iButtonWidth ),
								nAnchor::bottom,	0,		- ( iMargin + iButtonHeight ),
								nAnchor::right,		0,	    - ( iMargin + iBigMargin + iButtonWidth ),
								nAnchor::bottom,	0,		- iMargin );


	Cancel.hwnd = GetDlgItem ( hwnd, ID_CANCEL );
	register_child ( &Cancel,
								nAnchor::right,		0,		- ( iBigMargin + iButtonWidth ),
								nAnchor::bottom,	0,		- ( iMargin + iButtonHeight ),
								nAnchor::right,		0,	    - iBigMargin,
								nAnchor::bottom,	0,		- iMargin );

	reposition_children ();
}



//----------------------------------------------------------------------------------------------------------------------
//  SET TEXT
//----------------------------------------------------------------------------------------------------------------------
void cDlgSettingsConnection :: 	set_text ()
{
	set_item_text ( 0, 							L"DlgSettingsConnection.Title", g_awAppTitle );

	set_item_text ( ID_CONNECT_LABEL, 			L"DlgSettingsConnection.Connect.Label" );
	set_item_text ( ID_CONNECT_CAPTION, 		L"DlgSettingsConnection.Connect.Caption" );
	set_item_text ( ID_CONNECT_YES, 			L"DlgSettingsConnection.Connect.Yes" );
	set_item_text ( ID_CONNECT_NO, 				L"DlgSettingsConnection.Connect.No" );

	set_item_text ( ID_AUTO_FIND_LABEL, 		L"DlgSettingsConnection.AutoFind.Label" );
	set_item_text ( ID_AUTO_FIND_CAPTION, 		L"DlgSettingsConnection.AutoFind.Caption" );
	set_item_text ( ID_AUTO_FIND_YES, 			L"DlgSettingsConnection.AutoFind.Yes" );
	set_item_text ( ID_AUTO_FIND_NO, 			L"DlgSettingsConnection.AutoFind.No" );

	set_item_text ( ID_LIST_LABEL,	 			L"DlgSettingsConnection.List.Label" );
	set_item_text ( ID_LIST_CAPTION, 			L"DlgSettingsConnection.List.Caption" );

	set_item_text ( ID_LINK, 					L"DlgSettingsConnection.Link", L"http://mojoware.org" );
	set_item_text ( ID_MORE_OPTIONS, 			L"DlgSettingsConnection.MoreOptions" );
	set_item_text ( ID_RESTORE_DEFAULTS, 		L"DlgSettingsConnection.RestoreDefaults" );
	set_item_text ( ID_OK, 						L"DlgSettingsConnection.OK"  );
	set_item_text ( ID_CANCEL, 					L"DlgSettingsConnection.Cancel" );

	SetWindowText ( GetDlgItem ( hwnd, ID_LIST ), L"(Disabled in this build.)" );
}


//----------------------------------------------------------------------------------------------------------------------
//  SET STATE
//----------------------------------------------------------------------------------------------------------------------
void cDlgSettingsConnection :: set_state ()
{

	RECT r;
	GetWindowRect ( hwnd, &r );

	if ( IsDlgButtonChecked ( hwnd, ID_CONNECT_YES ) )
		CheckDlgButton ( hwnd, ID_CONNECT_NO, FALSE );
	else
		CheckDlgButton ( hwnd, ID_CONNECT_NO, TRUE );

	if ( IsDlgButtonChecked ( hwnd, ID_AUTO_FIND_YES ) )
		CheckDlgButton ( hwnd, ID_AUTO_FIND_NO, FALSE );
	else
		CheckDlgButton ( hwnd, ID_AUTO_FIND_NO, TRUE );
	
	if ( ! IsDlgButtonChecked ( hwnd, ID_CONNECT_YES ) )
	{

		// NO CONNECTIONS, SO HIDE MOST OF THE WINDOW
		ShowWindow ( GetDlgItem ( hwnd, ID_CONNECT_LABEL ),				SW_HIDE );
		ShowWindow ( GetDlgItem ( hwnd, ID_CONNECT_RULE ),				SW_HIDE );
		ShowWindow ( GetDlgItem ( hwnd, ID_AUTO_FIND_LABEL ), 			SW_HIDE );
		ShowWindow ( GetDlgItem ( hwnd, ID_AUTO_FIND_RULE ), 			SW_HIDE );
		ShowWindow ( GetDlgItem ( hwnd, ID_AUTO_FIND_CAPTION ), 		SW_HIDE );

		ShowWindow ( GetDlgItem ( hwnd, ID_AUTO_FIND_YES ), 			SW_HIDE );

		ShowWindow ( GetDlgItem ( hwnd, ID_RESTORE_DEFAULTS ),					SW_HIDE );
		ShowWindow ( GetDlgItem ( hwnd, ID_MORE_OPTIONS ),					SW_HIDE );

		MoveWindow ( hwnd, r.left, r.top, r.right-r.left, 200, TRUE );	
	}

	else
	{
		// CONNECTIONS, SO DISPLAY EVERYTHING
		ShowWindow ( GetDlgItem ( hwnd, ID_CONNECT_LABEL ),				SW_SHOW );
		ShowWindow ( GetDlgItem ( hwnd, ID_CONNECT_RULE ),				SW_SHOW );
		ShowWindow ( GetDlgItem ( hwnd, ID_AUTO_FIND_LABEL ), 			SW_SHOW );
		ShowWindow ( GetDlgItem ( hwnd, ID_AUTO_FIND_RULE ), 			SW_SHOW );
		ShowWindow ( GetDlgItem ( hwnd, ID_AUTO_FIND_CAPTION ), 		SW_SHOW );
		ShowWindow ( GetDlgItem ( hwnd, ID_AUTO_FIND_YES ), 			SW_SHOW);
		ShowWindow ( GetDlgItem ( hwnd, ID_RESTORE_DEFAULTS ),					SW_SHOW);
		ShowWindow ( GetDlgItem ( hwnd, ID_MORE_OPTIONS ),					SW_SHOW );

		MoveWindow ( hwnd, r.left, r.top, r.right-r.left, rFullWin.bottom - rFullWin.top, TRUE );
	}
}


//----------------------------------------------------------------------------------------------------------------------
//  WM INIT
//----------------------------------------------------------------------------------------------------------------------
void cDlgSettingsConnection :: wm_init ()
{

	GetWindowRect ( hwnd, &rFullWin );

	set_text();
	cDlgVars::wm_init ( this->hwnd, &aDlgData[0] );

	register_children ();

	//--------------------------------------
	//  MAKE SURE GROUPED RADIO BUTTONS 
	//  GOT SET
	//--------------------------------------


	set_state();

	AutoFindLabel.wm_init();
	ListLabel.wm_init();
	ConnectLabel.wm_init();
}


//----------------------------------------------------------------------------------------------------------------------
//  DIALOG PROC
//----------------------------------------------------------------------------------------------------------------------
INT_PTR CALLBACK cDlgSettingsConnection :: dialog_proc ( HWND hwnd, UINT uMessage, WPARAM wParam, LPARAM lParam )
{
	cWin * pWin = user_data_to_pWin ( hwnd );
	cDlgSettingsConnection * pThis = static_cast<cDlgSettingsConnection*>(pWin);

	switch ( uMessage )
	{
#if 0
	case WM_CTLCOLORBTN:
	case WM_CTLCOLOREDIT:
	case WM_CTLCOLORDLG:
	case WM_CTLCOLORSTATIC:
		{
			HDC hdc = (HDC) wParam;
			HWND hCtrl = (HWND) lParam;

			if ( hCtrl == GetDlgItem ( hwnd, ID_CONNECT_HEAD ) ||
				 hCtrl == GetDlgItem ( hwnd, ID_AUTO_FIND_HEAD ) )
				SetTextColor	( hdc, RGB ( 0x44, 0x44, 0xdd ) );

			SetBkMode		( hdc, OPAQUE );
			SetBkColor		( hdc, RGB ( 0xFF, 0xFF, 0xFF ) );
			return 			reinterpret_cast<INT_PTR> ( GetStockObject ( WHITE_BRUSH ) );
		}
		break;
#endif

	case WM_INITDIALOG:
		{
			set_user_data ( hwnd, lParam );
			cWin * pWin = lParam_to_pWin ( hwnd, lParam );
			pThis = static_cast<cDlgSettingsConnection*>(pWin);
			pThis->hwnd = hwnd;
			pThis->wm_init ();
		}
		break;

	case WM_COMMAND:
		{
			switch ( LOWORD ( wParam ) ) // ID
			{
			case ID_MORE_OPTIONS:
				assert(pThis);
				if ( pThis )
				{
					cDlgSettingsConnectionAdvanced d;
					d.make_dlg();
				}
				break;

			case ID_CANCEL:
				break;

			case ID_RESTORE_DEFAULTS:
				if ( pThis )
				{
					cSettings Defaults;
					Defaults.init();
					pThis->settings_to_dlg ( hwnd, &Defaults );
					pThis->set_state();
				}
				break;

			case ID_CONNECT_YES:
			case ID_CONNECT_NO:
				if ( pThis )
					pThis->set_state();
				break;

			case ID_OK:
				assert ( pThis );
				if ( pThis )
				{
					pThis->dlg_to_settings ( &g_Settings, hwnd );
					g_Settings.save_to_file ();
					mojo::set ( L"bConnect", g_Settings.bConnect );
					mojo::set ( L"bConnectAutomatically", g_Settings.bConnectAutomatically );
				}
				break;

			default:
				break;
			} // end ID switch

			break;
		} // end WM_COMMAND

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
		break; // end WM_COMMAND switch
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