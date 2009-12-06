/***********************************************************************************************************************
/*
/*    cDlgSettingsPerformance.cpp / mojo_app
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"

//======================================================================================================================
//  DATA
//======================================================================================================================

sDlgDatum cDlgSettingsPerformance :: aDlgData [] = 
{
	sDlgDatum ( ID_RAISE_PROCESS_PRIORITY,              L"bRaiseProcessPriority"        ),
	sDlgDatum ( ID_MAXIMIZE_TIMER_RESOLUTION,           L"bMaximizeTimerResolution"     ),
	sDlgDatum ( 0,                                      NULL                            ),
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
void cDlgSettingsPerformance :: register_children ()
{
	const int iBigMargin	= 32;
	const int iMargin 		= 10;
	const int iButtonHeight = 25;
	const int iButtonWidth	= 90;

	MojoLabel.hwnd          = GetDlgItem ( hwnd, ID_MOJO_LABEL );
	SystemLabel.hwnd        = GetDlgItem ( hwnd, ID_SYSTEM_LABEL );

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
void cDlgSettingsPerformance :: set_text ()
{
	set_item_text ( 0, 							   L"DlgSettingsPerformance.Title", g_awAppTitle );

	set_item_text ( ID_MOJO_LABEL, 			       L"DlgSettingsPerformance.Mojo.Label", g_awAppTitle );
	set_item_text ( ID_SYSTEM_LABEL, 			   L"DlgSettingsPerformance.System.Label" );
	set_item_text ( ID_RAISE_PROCESS_PRIORITY,     L"DlgSettingsPerformance.RaiseProcessPriority" );
	set_item_text ( ID_MAXIMIZE_TIMER_RESOLUTION,  L"DlgSettingsPerformance.MaximizeTimerResolution" );
	set_item_text ( ID_LINK,                       L"DlgSettingsPerformance.Link", L"http://mojoware.org" );
}


//----------------------------------------------------------------------------------------------------------------------
//  SET STATE
//----------------------------------------------------------------------------------------------------------------------
void cDlgSettingsPerformance :: set_state ()
{

}


//----------------------------------------------------------------------------------------------------------------------
//  WM INIT
//----------------------------------------------------------------------------------------------------------------------
void cDlgSettingsPerformance :: wm_init ()
{



	set_text();
	cDlgVars::wm_init ( this->hwnd, &aDlgData[0] );

	register_children ();

	//--------------------------------------
	//  MAKE SURE GROUPED RADIO BUTTONS 
	//  GOT SET
	//--------------------------------------

	MojoLabel.wm_init();
	SystemLabel.wm_init();
}


//----------------------------------------------------------------------------------------------------------------------
//  DIALOG PROC
//----------------------------------------------------------------------------------------------------------------------
INT_PTR CALLBACK cDlgSettingsPerformance :: dialog_proc ( HWND hwnd, UINT uMessage, WPARAM wParam, LPARAM lParam )
{
	cWin * pWin = user_data_to_pWin ( hwnd );
	cDlgSettingsPerformance * pThis = static_cast<cDlgSettingsPerformance*>(pWin);

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
			pThis = static_cast<cDlgSettingsPerformance*>(pWin);
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
					mojo::set ( L"bRaiseProcessPriority",    g_Settings.bRaiseProcessPriority );
					mojo::set ( L"bMaximizeTimerResolution", g_Settings.bMaximizeTimerResolution );
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