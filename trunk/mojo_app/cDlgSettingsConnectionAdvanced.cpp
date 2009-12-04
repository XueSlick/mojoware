/***********************************************************************************************************************
/*
/*    cDlgSettingsConnectionAdvanced.cpp / mojo_app
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"
#include <windowsx.h>

//======================================================================================================================
//  DATA
//======================================================================================================================

sDlgDatum cDlgSettingsConnectionAdvanced :: aDlgData [] = 
{
	sDlgDatum ( ID_SPECIFY_LOCAL_IP_YES,				L"bUseSpecifiedLocalIP"		),
	sDlgDatum ( ID_PORT,								L"uPort"					),
	sDlgDatum ( ID_USE_NAGLE,							L"bUseNagle"				),
	sDlgDatum ( 0,										NULL						),
};


//======================================================================================================================
//  PROTOTYPES
//======================================================================================================================

//======================================================================================================================
//  CODE
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
//  ON OKAY
//----------------------------------------------------------------------------------------------------------------------
bool cDlgSettingsConnectionAdvanced :: 	on_okay ()
{
	dlg_to_settings ( &g_Settings, hwnd );
	g_Settings.save_to_file ();

	//-------------------------------------------
	//  IF APPROPRIATE, SAVE SPECIFIED LOCAL IP
	//-------------------------------------------

	if ( g_Settings.bUseSpecifiedLocalIP )
	{
		HWND hCtrl = GetDlgItem ( hwnd, ID_LOCAL_IP );

		int iIndex = ComboBox_GetCurSel ( hCtrl );

		if ( CB_ERR == iIndex ) // ERROR
		{
			cScribPack sp ( L"DlgSettingsConnectionAdvanced.SpecifyLocalIP.Error" );
			this->balloon ( hCtrl, L"Missing info.", sp.head() );
			return false;
		}

		else
		{
			int iLen = ComboBox_GetLBTextLen ( hCtrl, iIndex );

			cStrW s ( iLen + 1 );

			ComboBox_GetLBText ( hCtrl, iIndex, s.buffer() );

			g_Settings.uSpecifiedLocalIP = mojo::ip_pw_to_dword ( s.cstr() );
		}
	}

	mojo::set ( L"bUseSpecifiedLocalIP", 	g_Settings.bUseSpecifiedLocalIP );
	mojo::set ( L"uSpecifiedLocalIP",		g_Settings.uSpecifiedLocalIP );
	mojo::set ( L"bUseNagle",				g_Settings.bUseNagle );
	mojo::set ( L"uPort",					g_Settings.uPort );

	return true;
}
	

//----------------------------------------------------------------------------------------------------------------------
//  SET TEXT
//----------------------------------------------------------------------------------------------------------------------
void cDlgSettingsConnectionAdvanced :: 	set_text ()
{
	// temp

	SetWindowText ( GetDlgItem ( hwnd, ID_PASSPHRASE ), L"(Disabled in this build.)" );

	set_item_text ( 0, 								L"DlgSettingsConnectionAdvanced.Title", g_awAppTitle );

	set_item_text ( ID_SPECIFY_LOCAL_IP_LABEL,		L"DlgSettingsConnectionAdvanced.SpecifyLocalIP.Label" );
	set_item_text ( ID_SPECIFY_LOCAL_IP_CAPTION, 	L"DlgSettingsConnectionAdvanced.SpecifyLocalIP.Caption" );
	set_item_text ( ID_SPECIFY_LOCAL_IP_YES, 		L"DlgSettingsConnectionAdvanced.SpecifyLocalIP.Yes" );
	set_item_text ( ID_SPECIFY_LOCAL_IP_NO, 		L"DlgSettingsConnectionAdvanced.SpecifyLocalIP.No" );

	set_item_text ( ID_NETWORK_OPTIONS_LABEL,		L"DlgSettingsConnectionAdvanced.NetworkOptions.Label" );
	set_item_text ( ID_USE_NAGLE,					L"DlgSettingsConnectionAdvanced.UseNagle" );

	set_item_text ( ID_PORT_LABEL,					L"DlgSettingsConnectionAdvanced.Port.Label" );
	set_item_text ( ID_PORT_CAPTION,				L"DlgSettingsConnectionAdvanced.Port.Caption" );

	set_item_text ( ID_PASSPHRASE_LABEL,			L"DlgSettingsConnectionAdvanced.Passphrase.Label" );
	set_item_text ( ID_PASSPHRASE_CAPTION,			L"DlgSettingsConnectionAdvanced.Passphrase.Caption" );

	set_item_text ( ID_LINK, 						L"DlgSettingsConnectionAdvanced.Link", L"http://mojoware.org" );
	set_item_text ( ID_OK, 							L"DlgSettingsConnectionAdvanced.OK"  );
	set_item_text ( ID_CANCEL, 						L"DlgSettingsConnectionAdvanced.Cancel" );
	set_item_text ( ID_RESTORE_DEFAULTS, 			L"DlgSettingsConnectionAdvanced.RestoreDefaults" );
}


//----------------------------------------------------------------------------------------------------------------------
//  SET STATE
//----------------------------------------------------------------------------------------------------------------------
void cDlgSettingsConnectionAdvanced :: set_state ()
{
	if ( IsDlgButtonChecked ( hwnd, ID_SPECIFY_LOCAL_IP_YES ) )
	{
		CheckDlgButton ( hwnd, ID_SPECIFY_LOCAL_IP_NO, FALSE );
		EnableWindow ( GetDlgItem ( hwnd, ID_LOCAL_IP ), TRUE );
	}

	else
	{
		CheckDlgButton ( hwnd, ID_SPECIFY_LOCAL_IP_NO, TRUE );
		EnableWindow ( GetDlgItem ( hwnd, ID_LOCAL_IP ), FALSE );
	}
}


//----------------------------------------------------------------------------------------------------------------------
//  WM INIT
//----------------------------------------------------------------------------------------------------------------------
void cDlgSettingsConnectionAdvanced :: wm_init_sub ()
{
	mojo::cArrayU aIP;

 	if ( ! get_ip_addresses ( &aIP ) )
		LOG ( L"get_ip_addresses() failed." );

	else
	{
		HWND hCtrl = GetDlgItem ( hwnd, ID_LOCAL_IP );

		for ( unsigned i = 0; i < aIP.qty(); i++ )
		{
			DWORD dwIP = aIP[i]; // COMMENTING THIS OUT DOESN"T FIX BUG
			cStrW s;
			mojo::ip_dword_to_cStrW ( &s, dwIP );
			ComboBox_AddString ( hCtrl, s.cstr() );

			//-----------------------------
			//  SET CURRENT SELECTION
			//-----------------------------

			if ( g_Settings.bUseSpecifiedLocalIP )
				if ( dwIP == g_Settings.uSpecifiedLocalIP )
					ComboBox_SetCurSel ( hCtrl, i );
		}
	}
}


//----------------------------------------------------------------------------------------------------------------------
//  WM INIT
//----------------------------------------------------------------------------------------------------------------------
void cDlgSettingsConnectionAdvanced :: wm_init ()
{

	wm_init_sub ();

	set_text();
	cDlgVars::wm_init ( this->hwnd, &aDlgData[0] );
	set_state();

	SpecifyLocalIpLabel.hwnd = GetDlgItem ( hwnd, ID_SPECIFY_LOCAL_IP_LABEL );
	SpecifyLocalIpLabel.wm_init();

	NetworkOptionsLabel.hwnd = GetDlgItem ( hwnd, ID_NETWORK_OPTIONS_LABEL );
	NetworkOptionsLabel.wm_init();

	PortLabel.hwnd = GetDlgItem ( hwnd, ID_PORT_LABEL );
	PortLabel.wm_init();

	PassphraseLabel.hwnd = GetDlgItem ( hwnd, ID_PASSPHRASE_LABEL );
	PassphraseLabel.wm_init();




}


//----------------------------------------------------------------------------------------------------------------------
//  DIALOG PROC
//----------------------------------------------------------------------------------------------------------------------
INT_PTR CALLBACK cDlgSettingsConnectionAdvanced :: dialog_proc ( HWND hwnd, UINT uMessage, WPARAM wParam, LPARAM lParam )
{
	cWin * pWin = user_data_to_pWin ( hwnd );
	cDlgSettingsConnectionAdvanced * pThis = static_cast<cDlgSettingsConnectionAdvanced*>(pWin);

	switch ( uMessage )
	{
	case WM_INITDIALOG:
		{
			set_user_data ( hwnd, lParam );
			cWin * pWin = lParam_to_pWin ( hwnd, lParam );
			pThis = static_cast<cDlgSettingsConnectionAdvanced*>(pWin);
			pThis->hwnd = hwnd;
			pThis->wm_init ();
		}
		break;

	case WM_COMMAND:
		{
			switch ( LOWORD ( wParam ) ) // ID
			{
			case ID_SPECIFY_LOCAL_IP_YES:
			case ID_SPECIFY_LOCAL_IP_NO:
				if ( pThis )
					pThis->set_state();
				break;

			case ID_OK:
				assert ( pThis );
				if ( pThis )
					if ( ! pThis->on_okay() )
						return true;
	
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