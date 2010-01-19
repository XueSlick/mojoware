/***********************************************************************************************************************
/*
/*    cWinMain.cpp / mojo_app
/* 
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"

using namespace nAnchor;

//======================================================================================================================
// PROTOTYPES
//======================================================================================================================

INT_PTR CALLBACK about_dlg_proc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

void on_size ( int x, int y );
void write_to_rich_edit ( HWND, const wchar_t * );
void create_shortcut();
void installation_options ();
bool create_script();
void open_text_file ( const wchar_t * pName );
void show_instructions ();
void show_forum ();
void report_bug ();
void wm_tray_icon ( WPARAM wParam, LPARAM lParam );
void set_mouseover_display_list ();


//======================================================================================================================
// DATA
//======================================================================================================================

extern unsigned uWM_ARE_YOU_ME; // defined in single_instance

//======================================================================================================================
// CODE
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
// SET VIEW
//----------------------------------------------------------------------------------------------------------------------
void cWinMain :: set_view ()
{

	ShowWindow ( DlgComputers.hwnd, SW_HIDE );
	ShowWindow ( DlgWoWs.hwnd,      SW_HIDE );
	ShowWindow ( DlgMonitor.hwnd,   SW_HIDE );
	ShowWindow ( DlgToons.hwnd,     SW_HIDE );

	switch ( ( _eView ) ( g_Settings.uView ) )
	{
	case computers:
		ShowWindow ( DlgComputers.hwnd, SW_SHOW );
		break;

	case toons:
		ShowWindow ( DlgToons.hwnd, SW_SHOW );
		break;

	case wows:
		ShowWindow ( DlgWoWs.hwnd, SW_SHOW );
		break;

	case monitor:
		ShowWindow ( DlgMonitor.hwnd, SW_SHOW );
		break;
	}
}


//----------------------------------------------------------------------------------------------------------------------
// HIDE OR SHOW CURSOR
//----------------------------------------------------------------------------------------------------------------------
#if 1
void cWinMain::hide_or_show_cursor ( WPARAM wParam )
{
	static bool bOn = true;

	//---------------------------------
	//  SHOW CURSOR
	//---------------------------------
	if ( 1 == wParam && ! bOn )
	{
		ShowWindow ( DlgCursorBlind.hwnd, SW_HIDE );
		bOn = true;
		ShowCursor ( TRUE );
	}

	//---------------------------------
	//  HIDE CURSOR
	//---------------------------------
	else if ( 0 == wParam && bOn )
	{
		cPtI ptCenter;
		ptCenter.x = GetSystemMetrics ( SM_CXSCREEN ) / 2;
		ptCenter.y = GetSystemMetrics ( SM_CYSCREEN ) / 2;
		SetWindowPos ( DlgCursorBlind.hwnd, HWND_TOPMOST, ptCenter.x, ptCenter.y, 1, 1, SWP_NOREDRAW );
		ShowWindow ( DlgCursorBlind.hwnd, SW_NORMAL );
		SetCursorPos ( ptCenter.x, ptCenter.y );
		bOn = false;
		ShowCursor ( FALSE );
	}
}
#endif


//----------------------------------------------------------------------------------------------------------------------
// INSTALLATION OPTIONS
//----------------------------------------------------------------------------------------------------------------------
#if 0
void installation_options ()
{
	cDlgInstallationOptions d;
	d.make_dlg ();
}
#endif

//----------------------------------------------------------------------------------------------------------------------
// CREATE SCRIPT
//----------------------------------------------------------------------------------------------------------------------
#if 0
bool create_script ()
{
	cStrW sDestPath;
	get_my_documents_path ( &sDestPath );
	sDestPath += L'\\';
	sDestPath += g_awAppTitle;
	BOOL iResult = CreateDirectoryW ( sDestPath.cstr(), NULL );
	sDestPath += L"\\script.txt";

	cStrW sSourcePath;
	get_module_path ( &sSourcePath );
	sSourcePath += L"script.txt";

	if ( file_exists ( sDestPath.cstr() ) )
	{
		cStrW m;

		m += L"A file with the name of the sample script already exists.\n\n";

		m += L"Do you want to erase it and create a new script?";

		int iResult = MessageBox ( g_hwnd, m.cstr(), g_awAppTitle, MB_YESNO );

		if ( IDNO == iResult )
			return false;
	}

	cFileOutN f ( sDestPath.cstr() );

	fprintf ( f.handle(),
		
		"%s",
		
		"// Sample script for HotkeyNet 2\n"
		"\n"
		"function f1()\n"
		"{\n"
		"    print ( \"Welcome to HotkeyNet 2!\" );\n"
		"}\n"
		"\n"
		"function AutoExec()\n"
		"{\n"
		"    f1();\n"
		"}\n"
		);

	g_Settings.sScriptName = sDestPath;

	return true;
}
#endif

//----------------------------------------------------------------------------------------------------------------------
// OPEN TEXT FILE
//----------------------------------------------------------------------------------------------------------------------
#if 0
void open_text_file ( const wchar_t * pName )
{
	HINSTANCE result = ShellExecute ( NULL, L"open", pName, NULL, NULL, SW_SHOW );

	if ( 32 < (int) result )
		return;

	switch ( (int) result )
	{
	case ERROR_FILE_NOT_FOUND:
		cMonitor::print ( L"Script not found.", 1, red, regular );
		break;

	case ERROR_PATH_NOT_FOUND:
		cMonitor::print ( L"Path not found.", 1, red, regular );

	case SE_ERR_ACCESSDENIED:
		cMonitor::print ( L"The operating system denied access to the script.", 1, red, regular );

	case SE_ERR_ASSOCINCOMPLETE:
		cMonitor::print ( L"File name association is incomplete or invalid.", 1, red, regular );

	default:
		cMonitor::print ( L"Operating system function ShellExecute cannot open script.", 1, red, regular );
	}
}
#endif

//----------------------------------------------------------------------------------------------------------------------
// EDIT SCRIPT
//----------------------------------------------------------------------------------------------------------------------
#if 0
void cWinMain :: edit_script ()
{
	open_text_file ( g_Settings.sScriptName.cstr() );
}
#endif

//----------------------------------------------------------------------------------------------------------------------
// LOAD SCRIPT
// gets settings name from "open file" dialog box
//----------------------------------------------------------------------------------------------------------------------
#if 0
void cWinMain :: load_script ()
{
	cStrW sNew;

	bool bResult = get_open_file_name ( &sNew, g_hwnd, NULL, g_Settings.sScriptName.cstr(), L"HKN2: Load Script" );

	if ( ! bResult )
		return;

	if ( ! sNew.len() )
		return;

	g_Settings.sScriptName = sNew;

	load_script_sub();
}
#endif


//----------------------------------------------------------------------------------------------------------------------
// LOAD SCRIPT SUB
// loads from settings name
//----------------------------------------------------------------------------------------------------------------------
#if 0
void cWinMain :: load_script_sub()
{
	if ( 0 == g_Settings.sScriptName.len() )
		return;

	// cMonitor::clear();

	if ( g_pLoader )
		delete g_pLoader;

	g_pLoader = new cLoader;

	cMonitor::print ( L"Loading script.", 0, black, regular);
	cMonitor::print ( g_Settings.sScriptName.cstr(), 1, blue, regular );
	cMonitor::print ( L"" );

	if ( ! g_pLoader->load ( g_Settings.sScriptName.cstr() ) )
	{
		delete g_pLoader;
		g_pLoader = NULL;
		cMonitor::print ( L"Unable to load script.", 1, red, regular );
		cMonitor::print ( L"" );
	}

	HWND hCtrl = GetDlgItem ( this->DlgMain.hwnd, ID_SCRIPT_NAME );
	Edit_SetText ( hCtrl, g_Settings.sScriptName.cstr() );
}
#endif


//----------------------------------------------------------------------------------------------------------------------
// DO DEBUG
//----------------------------------------------------------------------------------------------------------------------

struct sBag
{
	sBag * pNext;
	sBag * pPrev;
};

struct sStuff
{
	sStuff * pNext;
	sStuff * pPrev;
	tList<sBag> List;
};


//----------------------------------------------------------------------------------------------------------------------
// WM CREATE
//----------------------------------------------------------------------------------------------------------------------
void cWinMain::wm_create ( HWND hwndArg )
{
	g_hwnd = this->hwnd = hwndArg;

	LoadLibrary ( L"riched20.dll" );



	const int iViewStripWidth = 98;

	//----------------------------------------
	//  CORNER LOGO
	//----------------------------------------

	DlgCornerLogo.make_dlg();
	register_child ( &DlgCornerLogo,
							  nAnchor::left,       0, 0,
							  nAnchor::top,        0, 0,
							  nAnchor::left,       0, iViewStripWidth,
							  nAnchor::top,        0, DlgModeStrip.get_height() );

	SetParent ( DlgCornerLogo.hwnd, hwndArg );

	//----------------------------------------
	//  MODE STRIP
	//----------------------------------------

	DlgModeStrip.make_dlg();
	register_child ( &DlgModeStrip,
							  nAnchor::left,       0, iViewStripWidth,
							  nAnchor::top,        0, 0,
							  nAnchor::right,      0, 0,
							  nAnchor::top,        0, DlgModeStrip.get_height() );

	SetParent ( DlgModeStrip.hwnd, hwndArg );

	ShowWindow ( DlgModeStrip.hwnd, SW_SHOW );

	//----------------------------------------
	//  VIEW STRIP
	//----------------------------------------

	DlgViewStrip.make_dlg();
	register_child ( &DlgViewStrip,
							  nAnchor::left,       0, 0,
							  nAnchor::top,        0, DlgModeStrip.get_height(),
							  nAnchor::left,       0, iViewStripWidth,
							  nAnchor::bottom,     0, 0 );

	SetParent ( DlgModeStrip.hwnd, hwndArg );

	ShowWindow ( DlgModeStrip.hwnd, SW_SHOW );

	//----------------------------------------
	//  MONITOR DIALOG
	//----------------------------------------

	DlgMonitor.make_dlg ();

	register_child ( &DlgMonitor,
							  nAnchor::left,       0, iViewStripWidth,
							  nAnchor::top,        0, DlgModeStrip.get_height(),
							  nAnchor::right,      0, 0,
							  nAnchor::bottom,     0, 0 );

	SetParent ( DlgMonitor.hwnd, hwndArg );

	//----------------------------------------
	//  WOWS DIALOG
	//----------------------------------------

	DlgWoWs.make_dlg ();

	register_child ( &DlgWoWs,
							  nAnchor::left,       0, iViewStripWidth,
							  nAnchor::top,        0, DlgModeStrip.get_height(),
							  nAnchor::right,      0, 0,
							  nAnchor::bottom,     0, 0 );

	SetParent ( DlgWoWs.hwnd, hwndArg );

	//----------------------------------------
	//  COMPUTERS DIALOG
	//----------------------------------------

	DlgComputers.make_dlg ();

	register_child ( &DlgComputers,
							  nAnchor::left,       0, iViewStripWidth,
							  nAnchor::top,        0, DlgModeStrip.get_height(),
							  nAnchor::right,      0, 0,
							  nAnchor::bottom,     0, 0 );

	SetParent ( DlgComputers.hwnd, hwndArg );

	//----------------------------------------
	//  TOONS DIALOG
	//----------------------------------------

	DlgToons.make_dlg ();

	register_child ( &DlgToons,
							  nAnchor::left,       0, iViewStripWidth,
							  nAnchor::top,        0, DlgModeStrip.get_height(),
							  nAnchor::right,      0, 0,
							  nAnchor::bottom,     0, 0 );

	SetParent ( DlgToons.hwnd, hwndArg );

	//----------------------------------------
	//  CURSOR BLIND (HIDES CURSOR WHILE
	//  IT'S MOUSED OVER TO A REMOTE
	//----------------------------------------

	DlgCursorBlind.make_dlg();
	ShowWindow ( DlgCursorBlind.hwnd, SW_HIDE );
	SetParent ( DlgCursorBlind.hwnd, NULL ); // NO PARENT TO FACILITATE POSITIONING IN CENTER OF SCREEN

	set_size ( 800, 600 );

	set_view ();

	// set_menu_item_text ( g_hwnd, ID_TOGGLE_BROADCAST, g_Settings.bWindowBroadcastIsOn ? L"Turn broadcast off" : L"Turn broadcast on" );

	g_Settings.bWindowBroadcastIsOn = g_Settings.bWindowBroadcastIsOn ? false : true;
	g_Settings.bHotkeysAreOn  = g_Settings.bHotkeysAreOn  ? false : true;
	g_Settings.bMouseoverIsOn = g_Settings.bMouseoverIsOn ? false : true;
	toggle_broadcast ();
	toggle_hotkeys   ();
	toggle_mouseover ();
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

