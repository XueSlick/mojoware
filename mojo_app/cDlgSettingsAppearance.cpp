/***********************************************************************************************************************
/*
/*    cDlgSettingsAppearance.cpp / mojo_app
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"

//======================================================================================================================
//  DATA
//======================================================================================================================

sDlgDatum cDlgSettingsAppearance :: aDlgData [] = 
{
	sDlgDatum ( ID_TOON_ICON_WIDTH,              		   L"uToonIconWidth"      ),
	sDlgDatum ( ID_TOON_ICON_HEIGHT,                       L"uToonIconHeight"     ),
	sDlgDatum ( ID_WOW_ICON_WIDTH,              		   L"uWoWIconWidth"       ),
	sDlgDatum ( ID_WOW_ICON_HEIGHT,                        L"uWoWIconHeight"      ),
	sDlgDatum ( 0,                                         NULL                   ),
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
void cDlgSettingsAppearance :: register_children ()
{
	const int iBigMargin	= 32;
	const int iMargin 		= 10;
	const int iButtonHeight = 25;
	const int iButtonWidth	= 90;

	IconLabel.hwnd          = GetDlgItem ( hwnd, ID_ICON_LABEL );

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
void cDlgSettingsAppearance :: set_text ()
{
	set_item_text ( 0, 							   L"DlgSettingsAppearance.Title", g_awAppTitle );
	set_item_text ( ID_ICON_LABEL,                 L"DlgSettingsAppearance.Icon.Label", g_awAppTitle );
	set_item_text ( ID_TOON_ICON_HEIGHT_LABEL,     L"DlgSettingsAppearance.ToonIconHeightLabel" );
	set_item_text ( ID_TOON_ICON_WIDTH_LABEL,      L"DlgSettingsAppearance.ToonIconWidthLabel" );
	set_item_text ( ID_WOW_ICON_HEIGHT_LABEL,      L"DlgSettingsAppearance.WoWIconHeightLabel" );
	set_item_text ( ID_WOW_ICON_WIDTH_LABEL,       L"DlgSettingsAppearance.WoWIconWidthLabel" );
	set_item_text ( ID_LINK,                       L"DlgSettingsAppearance.Link", L"http://mojoware.org" );
}


//----------------------------------------------------------------------------------------------------------------------
//  SET STATE
//----------------------------------------------------------------------------------------------------------------------
void cDlgSettingsAppearance :: set_state ()
{

}


//----------------------------------------------------------------------------------------------------------------------
//  WM INIT
//----------------------------------------------------------------------------------------------------------------------
void cDlgSettingsAppearance :: wm_init ()
{
	set_text();
	cDlgVars::wm_init ( this->hwnd, &aDlgData[0] );

	register_children ();

	const int iMarginX = 24;
	IconLabel.hwnd = GetDlgItem( hwnd, ID_ICON_LABEL );
	register_child ( &IconLabel,
							  nAnchor::left,		0,		iMarginX,
							  nAnchor::top,		    0,		iMarginX,
							  nAnchor::right,		0,		-iMarginX,
							  nAnchor::top,         0,      iMarginX + 25 );
	IconLabel.init();
	this->reposition_children ();
}


//----------------------------------------------------------------------------------------------------------------------
//  DIALOG PROC
//----------------------------------------------------------------------------------------------------------------------
INT_PTR CALLBACK cDlgSettingsAppearance :: dialog_proc ( HWND hwnd, UINT uMessage, WPARAM wParam, LPARAM lParam )
{
	cWin * pWin = user_data_to_pWin ( hwnd );
	cDlgSettingsAppearance * pThis = static_cast<cDlgSettingsAppearance*>(pWin);

	switch ( uMessage )
	{
	case WM_INITDIALOG:
		{
			set_user_data ( hwnd, lParam );
			cWin * pWin = lParam_to_pWin ( hwnd, lParam );
			pThis = static_cast<cDlgSettingsAppearance*>(pWin);
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