/***********************************************************************************************************************
/*
/*    cWinMain_wm_command.cpp / mojo_app
/*
/*    This file contains the handler for WM_COMMAND messages that are received by the program's main 
/*    window.
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"


void test ();
void create_click_once_shortcut ();

//======================================================================================================================
//  CODE
//======================================================================================================================


//----------------------------------------------------------------------------------------------------------------------
// WM_COMMAND
//----------------------------------------------------------------------------------------------------------------------
void cWinMain :: wm_command ( WPARAM wParam, LPARAM lParam )
{
	UNREFERENCED_PARAMETER ( lParam );
	UNREFERENCED_PARAMETER ( wParam );

	WORD wID    = LOWORD(wParam);
	WORD wEvent = HIWORD(wParam);

	UNREFERENCED_PARAMETER ( wEvent );

	switch ( wID )
	{
	case ID_ABOUT:
		{
			cDlgAbout d;
			d.make_dlg();
		}
		break;

	case ID_CLEAR:
		PostMessage ( DlgMonitor.hwnd, WM_COMMAND, ID_CLEAR, ID_CLEAR );
		break;

	case ID_CONNECTION_SETTINGS:
		{
			cDlgSettingsConnection d;
			d.make_dlg ();
		}
		break;

	case ID_CREATE_DESKTOP_SHORTCUT:
		create_click_once_shortcut ();
		break;

	case ID_EXIT:
		DestroyWindow(hwnd);
		break;

	case ID_MOUSEOVER_SETTINGS:
		{
			cDlgMouseOver d;
			d.make_dlg ();
		}
		break;

	case ID_PERFORMANCE_SETTINGS:
		{
			cDlgSettingsPerformance d;
			d.make_dlg ();
		}
		break;

	case ID_SHOW_APP_LOG:
		show_app_data_file ( L"mojo.log.txt" );
		break;

	case ID_SHOW_APP_SETTINGS:
		show_app_data_file ( L"mojo.settings.txt" );
		break;

	case ID_SHOW_ENGINE_LOG:
		show_app_data_file ( L"mojo_engine.log.txt" );
		break;

	case ID_SHOW_ENGINE_SETTINGS:
		show_app_data_file ( L"mojo_engine.settings.txt" );
		break;

	case ID_SHOW_INSTALLATION_DIRECTORY:
		{
			cStrW s1, s2;
			s1 = L"Location of Mojo's executable files:\n\n";
			get_module_directory ( & s2 );
			s1 += s2;
			message_box ( s1.cstr() );
		}
		break;

	case ID_SHOW_LICENSE_DIALOG:
		{
			cDlgLicense d;
			d.make_dlg();
		}
		break;

	case ID_TOGGLE_BROADCAST:
		toggle_broadcast ();
		break;

	case ID_TOGGLE_HOTKEYS:
		toggle_hotkeys ();
		message_box ( L"Hotkeys aren't implemented yet." );
		break;

	case ID_TOGGLE_MOUSEOVER:
		toggle_mouseover ();
		break;

	case ID_TEST:
		::test ();
		break;

	case ID_VIEW_COMPUTERS:
		g_Settings.uView = computers;
		set_view();
		break;

	case ID_VIEW_MONITOR:
		g_Settings.uView = monitor;
		set_view();
		break;

	case ID_VIEW_WOWS:
		g_Settings.uView = wows;
		set_view();
		break;

#if 0
		case ID_RESTORE_DEFAULT_SETTINGS:
			g_Settings.restore_defaults();
			break;

		case ID_SETTINGS_MOUSEOVER:
			{
				cDlgMouseOver d;
				d.make_dlg();
			}
			break;

		case ID_HIDE_HOTKEYNET:
			set_tray_icon ();
			ShowWindow ( g_hwnd, SW_HIDE );
			break;

		case ID_TOGGLE_SHOW_STATE:
			ShowWindow ( g_hwnd, IsWindowVisible(g_hwnd) ? SW_HIDE : SW_RESTORE );
			break;

		case ID_TOGGLE_HOTKEYS:
			toggle_hotkeys();
			break;

		case ID_DEBUG:
			debug();
			break;

		case ID_ASK_QUESTION:
			show_forum();
			break;

		case ID_REPORT_BUG:
			report_bug ();
			break;

		case ID_INSTRUCTIONS:
			show_instructions ();
			break;

		case ID_SETTINGS_CONNECTION:
			{
				cDlgSettingsConnection d;
				d.make_dlg();
			}
			break;

		case ID_SETTINGS_SECURITY:
			{
				cDlgSettingsSecurity d;
				d.make_dlg();
			}
			break;

		case ID_SETTINGS_START_UP:
			{
				cDlgSettingsStartUp d;
				d.make_dlg();
			}
			break;

		case ID_SETTINGS_DISPLAY:
			{
				cDlgSettingsDisplay d;
				d.make_dlg();
			}
			break;

		case ID_INSTALLATION_OPTIONS:
			installation_options ();
			break;

		case ID_CREATE_SHORTCUT:
			create_clickonce_shortcut ();
			break;

		case ID_CREATE_SCRIPT:
			if ( create_script () )
				load_script_sub();
			break;

		case ID_OPEN_DEBUG_LOG:
			open_text_file ( g_Log.filename() );
			break;

		case ID_LOAD_SCRIPT:
			g_Monitor.clear();
			load_script();
			break;

		case ID_RELOAD_SCRIPT:
			g_Monitor.clear();
			load_script_sub ();
			break;

		case ID_EDIT_SCRIPT:
			edit_script();
			break;

		case IDM_ABOUT:
			{
				cDlgMessageBox d;
				d.make_dlg();
			}
			break;
#endif
	}
}


//----------------------------------------------------------------------------------------------------------------------
// TOGGLE HOTKEYS
//----------------------------------------------------------------------------------------------------------------------
#if 0
void toggle_hotkeys ()
{
	g_Settings.bHotkeysAreOn = g_Settings.bHotkeysAreOn ? false : true;
	set_menu_item_text ( g_hwnd, ID_TOGGLE_HOTKEYS, g_Settings.bHotkeysAreOn ? L"Turn hotkeys off" : L"Turn hotkeys on" );
	InvalidateRect ( g_hwnd, NULL, TRUE );
	UpdateWindow ( g_hwnd );
	set_tray_icon ();
}
#endif


//----------------------------------------------------------------------------------------------------------------------
// SET MENU ITEM TEXT
//----------------------------------------------------------------------------------------------------------------------
#if 0
void set_menu_item_text ( HWND hwnd, unsigned uID, const wchar_t * pTxt )
{
	HMENU hMenu = GetMenu ( hwnd );
	MENUITEMINFO mii;
	memset ( &mii, 0, sizeof ( mii ) );
	mii.cbSize = sizeof(mii);
	mii.fMask = MIIM_STRING;
	mii.dwTypeData = (LPWSTR) pTxt;
	SetMenuItemInfo ( hMenu, uID, FALSE, &mii );
}
#endif


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

