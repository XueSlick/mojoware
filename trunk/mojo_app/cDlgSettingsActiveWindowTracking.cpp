/***********************************************************************************************************************
/*
/*    cDlgSettingsActiveWindowTracking.cpp / mojo_app
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"

//======================================================================================================================
//  DATA
//======================================================================================================================

sDlgDatum cDlgSettingsActiveWindowTracking :: aDlgData [] = 
{
	sDlgDatum ( ID_ACTIVE_WINDOW_TRACKING_DELAY,        L"uActiveWindowTrackingDelay"        ),
	sDlgDatum ( ID_ACTIVE_WINDOW_TRACKING_DELAY_SET,    L"bActiveWindowTrackingDelaySet"     ),
	sDlgDatum ( 0,                                      NULL                                 ),
};


//======================================================================================================================
//  PROTOTYPES
//======================================================================================================================

//======================================================================================================================
//  CODE
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
//  SETTINGS TO DIALOG
//----------------------------------------------------------------------------------------------------------------------
void cDlgSettingsActiveWindowTracking :: settings_to_dlg ( HWND h, cSettings * p )
{
	cDlgVars::settings_to_dlg ( h, p );

	Button_SetCheck ( GetDlgItem ( h, ID_ACTIVE_WINDOW_TRACKING_SYSTEM ), FALSE );
	Button_SetCheck ( GetDlgItem ( h, ID_ACTIVE_WINDOW_TRACKING_OFF ),    FALSE );
	Button_SetCheck ( GetDlgItem ( h, ID_ACTIVE_WINDOW_TRACKING_ON ),     FALSE );

	if ( 0 == p->uActiveWindowTracking )
		Button_SetCheck ( GetDlgItem ( h, ID_ACTIVE_WINDOW_TRACKING_SYSTEM ), TRUE );

	if ( 1 == p->uActiveWindowTracking )
		Button_SetCheck ( GetDlgItem ( h, ID_ACTIVE_WINDOW_TRACKING_OFF ), TRUE );

	if ( 2 == p->uActiveWindowTracking )
		Button_SetCheck ( GetDlgItem ( h, ID_ACTIVE_WINDOW_TRACKING_ON ), TRUE );


	Button_SetCheck ( GetDlgItem ( h, ID_ACTIVE_WINDOW_TRACKING_Z_SYSTEM ), FALSE );
	Button_SetCheck ( GetDlgItem ( h, ID_ACTIVE_WINDOW_TRACKING_Z_OFF ),    FALSE );
	Button_SetCheck ( GetDlgItem ( h, ID_ACTIVE_WINDOW_TRACKING_Z_ON ),     FALSE );

	if ( 0 == p->uActiveWindowTrackingZ )
		Button_SetCheck ( GetDlgItem ( h, ID_ACTIVE_WINDOW_TRACKING_Z_SYSTEM ), TRUE );

	if ( 1 == p->uActiveWindowTrackingZ )
		Button_SetCheck ( GetDlgItem ( h, ID_ACTIVE_WINDOW_TRACKING_Z_OFF ), TRUE );

	if ( 2 == p->uActiveWindowTrackingZ )
		Button_SetCheck ( GetDlgItem ( h, ID_ACTIVE_WINDOW_TRACKING_Z_ON ), TRUE );
}


//----------------------------------------------------------------------------------------------------------------------
//  DIALOG TO SETTINGS
//----------------------------------------------------------------------------------------------------------------------
void cDlgSettingsActiveWindowTracking :: dlg_to_settings ( cSettings * p, HWND h )
{
	cDlgVars::dlg_to_settings ( p, h );

	if ( Button_GetCheck ( GetDlgItem ( hwnd, ID_ACTIVE_WINDOW_TRACKING_SYSTEM ) ) )
		p->uActiveWindowTracking = 0;

	else if ( Button_GetCheck ( GetDlgItem ( hwnd, ID_ACTIVE_WINDOW_TRACKING_OFF ) ) )
		p->uActiveWindowTracking = 1;

	else if ( Button_GetCheck ( GetDlgItem ( hwnd, ID_ACTIVE_WINDOW_TRACKING_ON ) ) )
		p->uActiveWindowTracking = 2;


	if ( Button_GetCheck ( GetDlgItem ( hwnd, ID_ACTIVE_WINDOW_TRACKING_Z_SYSTEM ) ) )
		p->uActiveWindowTrackingZ = 0;

	else if ( Button_GetCheck ( GetDlgItem ( hwnd, ID_ACTIVE_WINDOW_TRACKING_Z_OFF ) ) )
		p->uActiveWindowTrackingZ = 1;

	else if ( Button_GetCheck ( GetDlgItem ( hwnd, ID_ACTIVE_WINDOW_TRACKING_Z_ON ) ) )
		p->uActiveWindowTrackingZ = 2;
}


//----------------------------------------------------------------------------------------------------------------------
//  REGISTER CHILDREN
//----------------------------------------------------------------------------------------------------------------------
void cDlgSettingsActiveWindowTracking :: register_children ()
{
	const int iBigMargin	= 32;
	const int iMargin 		= 10;
	const int iButtonHeight = 25;
	const int iButtonWidth	= 90;


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
void cDlgSettingsActiveWindowTracking :: set_text ()
{
	SetWindowText ( GetDlgItem ( hwnd, ID_HEAD ),  L"Set active window tracking" );
	set_item_text ( ID_LINK,                       L"DlgSettingsActiveWindowTracking.Link", L"http://mojoware.org/help/active_window_tracking.html" );
}


//----------------------------------------------------------------------------------------------------------------------
//  SET STATE
//----------------------------------------------------------------------------------------------------------------------
void cDlgSettingsActiveWindowTracking :: set_state ()
{
	if ( Button_GetCheck ( GetDlgItem ( hwnd, ID_ACTIVE_WINDOW_TRACKING_DELAY_SET ) ) )
		EnableWindow ( GetDlgItem ( hwnd, ID_ACTIVE_WINDOW_TRACKING_DELAY ), TRUE );

	else
		EnableWindow ( GetDlgItem ( hwnd, ID_ACTIVE_WINDOW_TRACKING_DELAY ), FALSE );
}


//----------------------------------------------------------------------------------------------------------------------
//  WM INIT
//----------------------------------------------------------------------------------------------------------------------
void cDlgSettingsActiveWindowTracking :: wm_init ()
{
	SetWindowText ( hwnd, L"Mojo: Set active window tracking" );
	HWND hHead = GetDlgItem ( hwnd, ID_HEAD );
	SetWindowFont ( hHead, g_hDialogBoxHeadFont, TRUE );
	set_text();
	cDlgVars::wm_init ( this->hwnd, &aDlgData[0] );
	register_children ();
}


//----------------------------------------------------------------------------------------------------------------------
//  DIALOG PROC
//----------------------------------------------------------------------------------------------------------------------
INT_PTR CALLBACK cDlgSettingsActiveWindowTracking :: dialog_proc ( HWND hwnd, UINT uMessage, WPARAM wParam, LPARAM lParam )
{
	cWin * pWin = user_data_to_pWin ( hwnd );
	cDlgSettingsActiveWindowTracking * pThis = static_cast<cDlgSettingsActiveWindowTracking*>(pWin);

	switch ( uMessage )
	{
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

	case WM_INITDIALOG:
		{
			set_user_data ( hwnd, lParam );
			cWin * pWin = lParam_to_pWin ( hwnd, lParam );
			pThis = static_cast<cDlgSettingsActiveWindowTracking*>(pWin);
			pThis->hwnd = hwnd;
			pThis->wm_init ();
		}
		break;

	case WM_COMMAND:
		{
			switch ( LOWORD ( wParam ) ) // ID
			{
			case ID_CANCEL:
				break;

			case ID_ACTIVE_WINDOW_TRACKING_DELAY_SET:
				pThis->set_state();
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

			case ID_OK:
				assert ( pThis );
				if ( pThis )
				{
					pThis->dlg_to_settings ( &g_Settings, hwnd );
					g_Settings.save_to_file ();
					if ( 1 == g_Settings.uActiveWindowTracking )
						mojo::set_active_window_tracking ( false );
					else if ( 2 == g_Settings.uActiveWindowTracking )
						mojo::set_active_window_tracking ( true );
					if ( 1 == g_Settings.uActiveWindowTrackingZ )
						mojo::set_active_window_tracking_z_order ( false );
					else if ( 2 == g_Settings.uActiveWindowTrackingZ )
						mojo::set_active_window_tracking_z_order ( true );
					if ( g_Settings.bActiveWindowTrackingDelaySet )
						mojo::set_active_window_tracking_delay ( g_Settings.uActiveWindowTrackingDelay );
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