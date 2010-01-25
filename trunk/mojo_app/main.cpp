/***********************************************************************************************************************
/*
/*    main.cpp / mojo_app
/*
/*	  begun 3 november 2009
/*
/*    This file contains the application's entry point and initialization code.
/*
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"

using namespace mojo;


//======================================================================================================================
// DATA
//======================================================================================================================

static const wchar_t			s_awWindowClass [] = L"MojoWindowClass";



//======================================================================================================================
// PROTOTYPES
//======================================================================================================================

bool 							create_window ( HINSTANCE hInstance );
ATOM							register_class ( HINSTANCE hInstance );
BOOL							init_app ( HINSTANCE, int );
bool							previous_instance_is_running ();
void							clean_up_for_exit ();
void							create_shortcut();
void							set_menu_item_text ( HWND hwnd, unsigned uID, const wchar_t * pTxt );
void							load_script_sub ();
void							set_tray_icon ();
void							remove_tray_icon ();
bool							desktop_shortcut_exists ();
LONG WINAPI						unhandled_exception_filter ( EXCEPTION_POINTERS * pExceptionInfo );
void							set_mouseover_deactivates_screen_savers();
void 							make_fonts ();


//======================================================================================================================
// CODE
//======================================================================================================================



//----------------------------------------------------------------------------------------------------------------------
//  TEST
//----------------------------------------------------------------------------------------------------------------------
void test ( )
{



#if 0
	cFogMgr fm;

	cFogTree * pPDH = (cFogTree*) fm.Root.pRight;

	cFog * pClone = pPDH->clone(); pClone;

	cFog * p = fm.get_by_handle ( 3 );

	p;

	cFog * pNew = p->clone();

	pNew;

	cFogStr * pFS = (cFogStr*) p;

	const wchar_t * pw = pFS->cstr(); pw;

	cStrW X = *pFS;

	int x =3;
	x++;
#endif

}


//----------------------------------------------------------------------------------------------------------------------
// MAIN
// Long functions are usually a bad idea but this one is only a list of actions, so there's no problem.
// In fact, breaking it up would only make it harder to understand the order of start-up events.
//----------------------------------------------------------------------------------------------------------------------
int APIENTRY _tWinMain ( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR pCmdLine, int iCmdShow )
{
	UNREFERENCED_PARAMETER ( pCmdLine );
	UNREFERENCED_PARAMETER ( iCmdShow );
	UNREFERENCED_PARAMETER ( hPrevInstance );

	g_hInstance = hInstance;

	//-------------------------------------
	// MEMORY LEAK DETECTION
	//-------------------------------------

	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	//-------------------------------------
	// PREVENT MULTIPLE INSTANCES
	//-------------------------------------

	if ( previous_instance_is_running () )
		return 0;

	//-------------------------------------
	// COMMON CONTROLS
	//-------------------------------------

	init_common_controls();

	//-------------------------------------
	// GET NONCLIENTMETRICS
	//-------------------------------------

	memset( & g_NonClientMetrics, 0, sizeof(g_NonClientMetrics) );
	g_NonClientMetrics.cbSize = sizeof ( g_NonClientMetrics );
	SystemParametersInfo ( SPI_GETNONCLIENTMETRICS, sizeof(g_NonClientMetrics), &g_NonClientMetrics, 0 );

	//-------------------------------------
	// MAKE FONTS
	//-------------------------------------

	make_fonts ();

	//----------------------------------------------
	// LOAD OUR SETTINGS
	//----------------------------------------------
	{
		cStrW sPath;
		mojo::get_our_local_app_data_directory ( &sPath, g_awAppTitle );
		sPath += g_awAppTitle;
		sPath += L".settings.txt";
		g_Settings.set_pathname ( sPath.cstr() );
		g_Settings.load_from_file ();
	}

	//-------------------------------------
	// CREATE WINDOW
	//-------------------------------------

	if ( ! create_window ( hInstance ) )
		return FALSE;

	//-------------------------------------
	// TELL ENGINE TO LOAD ITS SETTINGS
	//-------------------------------------

	cStrW sDataDir;
	mojo::get_our_local_app_data_directory ( &sDataDir, g_awAppTitle );
	if ( ! mojo :: load_engine_settings ( sDataDir.cstr() ) )
		LOG ( L"mojo::load_engine_settings failed." );

	//-------------------------------------
	// OVERRIDE ENGINE SETTINGS WITH OURS
	//-------------------------------------

	mojo::set ( L"bConnect", 				g_Settings.bConnect );
	mojo::set ( L"bConnectAutomatically", 	g_Settings.bConnectAutomatically );
	mojo::set ( L"bUseSpecifiedLocalIP", 	g_Settings.bUseSpecifiedLocalIP );
	mojo::set ( L"uSpecifiedLocalIP",		g_Settings.uSpecifiedLocalIP );
	mojo::set ( L"bUseNagle",				g_Settings.bUseNagle );
	mojo::set ( L"uPort",					g_Settings.uPort );
	mojo::set ( L"bWindowBroadcastIsOn",          g_Settings.bWindowBroadcastIsOn );
	mojo::set ( L"bRaiseProcessPriority",   g_Settings.bRaiseProcessPriority );

	if ( 1 == g_Settings.uActiveWindowTracking )
		mojo::set_active_window_tracking ( false );
	else if ( 2 == g_Settings.uActiveWindowTracking )
		mojo::set_active_window_tracking ( true );

	if ( 1 == g_Settings.uActiveWindowTrackingZ )
		mojo::set_active_window_tracking_z_order ( false );
	else if ( 2 == g_Settings.uActiveWindowTrackingZ )
		mojo::set_active_window_tracking_z_order ( true );

	if ( ! g_Settings.bActiveWindowTrackingDelaySystem )
		mojo::set_active_window_tracking_delay ( g_Settings.uActiveWindowTrackingDelay );

	//-------------------------------------
	// INIT ENGINE (after window exists)
	//-------------------------------------

	try  
	{
		mojo::initialize_engine ( hInstance, g_hwnd, g_awAppTitle, NULL, NULL, sDataDir.cstr(), & g_Version );
	}

	catch ( cException &e ) 
	{
		LOG ( &e );
		message_box ( &e );
		return 0;
	}



	//-------------------------------------
	// LOAD APP'S SCRIBS
	//-------------------------------------

	mojo::load_scribs ( g_apDefaultScribs, false );

	//-------------------------------------
	//  RE-INITIALIZE SCRIBS IN WINDOW
	//-------------------------------------

	g_WinMain.set_text_recursive();

	//-------------------------------------
	//  SET PREDEFINED HOTKEYS
	//-------------------------------------
	{
		cFigPredefinedHotkeys * p = g_FigMgr.get_predefined_hotkeys();

		assert(p);

		if ( p )
			p->set_engine();
	}

	//-------------------------------------
	//  SET ENGINE'S LAUNCH TARGETS
	//-------------------------------------

	g_FigMgr.set_engine_launch_targets();

	//-------------------------------------
	//  LICENSE AGREEMENT
	//-------------------------------------

	if ( ! g_Settings.bLicenseAccepted )
	{
		cDlgLicense dl;
		if ( ID_ACCEPT != dl.make_dlg() )
			return 0;
	}

	//-------------------------------------
	//  TIMER RESOLUTION
	//-------------------------------------
#if 0
	if ( g_Settings.bMaximumTimerResolution )
		maximize_timer_resolution_on_start_up ();
#endif

	//-------------------------------------
	//  MOUSEOVER DEACTIVATES SCREEN SAVERS
	//-------------------------------------
#if 0
	set_mouseover_deactivates_screen_savers();
#endif

	//-------------------------------------
	//  TRAY ICON
	//-------------------------------------
#if 0
	if ( g_Settings.bShowTrayIcon || g_Settings.bStartMinimizedToTray )
		set_tray_icon ();
#endif

	//-------------------------------------
	//  ACCELERATOR TABLE
	//-------------------------------------
	HACCEL hAccelTable;
	hAccelTable = LoadAccelerators ( hInstance, MAKEINTRESOURCE ( IDC_mojo ) );

	//-------------------------------------
	//  SHOW WINDOW
	//-------------------------------------

	g_WinMain.move_to_center();
	ShowWindow   ( g_hwnd, iCmdShow);
	UpdateWindow ( g_hwnd );

	//-------------------------------------
	//  PRINT WELCOME
	//-------------------------------------

	cStrW sVersion;
	mojo::put_memo ( cMemo::success, L"Welcome", g_awAppTitle, g_Version.get_text ( &sVersion ), g_awWebsite );

	//-------------------------------------
	//  TEMPORARY HOTKEY FOR DEBUGGING
	//-------------------------------------

	// RegisterHotKey ( g_hwnd, VK_F1, MOD_ALT | MOD_CONTROL | MOD_SHIFT, VK_F1 );

	//-------------------------------------
	//  MESSAGE LOOP
	//-------------------------------------

	MSG msg;
	while ( GetMessage ( &msg, NULL, 0, 0 ) )
	{
		if ( ! TranslateAccelerator ( msg.hwnd, hAccelTable, &msg ) )
		{
			TranslateMessage ( &msg );
			DispatchMessage  ( &msg );
		}
	}

	//-------------------------------------
	// CLEAN UP
	//-------------------------------------

	clean_up_for_exit ();

	//-------------------------------------
	// EXIT
	//-------------------------------------
	return (int) msg.wParam;
}


//----------------------------------------------------------------------------------------------------------------------
// CLEAN UP FOR EXIT
//----------------------------------------------------------------------------------------------------------------------
void clean_up_for_exit ()
{
	mojo::shut_down_engine ();
	g_Settings.save_to_file ();

#if 0
	if ( g_pLoader )
		delete g_pLoader;

	restore_timer_resolution ();
	remove_tray_icon ();
#endif
}


//----------------------------------------------------------------------------------------------------------------------
// REGISTER CLASS
//----------------------------------------------------------------------------------------------------------------------
ATOM register_class ( HINSTANCE hInstance )
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= cWinMain::window_proc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_mojo));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(ID_MAIN_MENU);
	wcex.lpszClassName	= s_awWindowClass; // szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_mojo));

	return RegisterClassEx(&wcex);
}


//----------------------------------------------------------------------------------------------------------------------
// CREATE WINDOW
//----------------------------------------------------------------------------------------------------------------------
bool create_window ( HINSTANCE hInstance )
{
	cStrW t = g_awAppTitle;
	t += L" ";
	cStrW v;
	g_Version.get_text ( &v );
	t += v;

	register_class ( hInstance );

	g_hwnd = CreateWindow ( s_awWindowClass, t.cstr(), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, static_cast<cWin*>(&g_WinMain ) );

	return ( g_hwnd ? true : false );
}


//----------------------------------------------------------------------------------------------------------------------
// MAKE FONTS
//----------------------------------------------------------------------------------------------------------------------
void make_fonts ()
{
	g_hMenuFont = CreateFontIndirect ( &g_NonClientMetrics.lfMenuFont );

	//-------------------------------
	// SUBHEAD FONT
	//-------------------------------
	{
		LOGFONT lf = g_NonClientMetrics.lfCaptionFont;
		lf.lfWeight = 400;
		lf.lfHeight = (long) ( 1.3 * lf.lfHeight );
		g_hSubheadFont = CreateFontIndirect ( &lf );
	}

	//-------------------------------
	// DIALOG BOX HEAD FONT
	//-------------------------------
	{
		LOGFONT lf = g_NonClientMetrics.lfCaptionFont;
		lf.lfWeight = 700;
		lf.lfHeight = (long) ( 1.3 * lf.lfHeight );
		g_hDialogBoxHeadFont = CreateFontIndirect ( &lf );
	}

	//-------------------------------
	// HEAD FONT
	//-------------------------------
	{
		LOGFONT lfHead = g_NonClientMetrics.lfCaptionFont;
		lfHead.lfHeight = (long) ( 1.8 * lfHead.lfHeight );
		wcscpy_s ( lfHead.lfFaceName, sizeof ( lfHead.lfFaceName)/sizeof(wchar_t), L"Verdana" );
		g_hHeadFont = CreateFontIndirect ( &lfHead ) ;	
	}

	//-------------------------------
	// CAPTION FONT
	//-------------------------------

	if ( os_version_or_higher ( 6, 0 ) )
	{
		LOGFONT f = g_NonClientMetrics.lfCaptionFont;
		f.lfWeight = 700;
		g_hCaptionFont = CreateFontIndirect ( &f ) ;	
	}

	else
	{
		g_hCaptionFont = CreateFontIndirect ( &g_NonClientMetrics.lfCaptionFont );
	}
}


/***********************************************************************************************************************
/*
/*    This file is part of Mojo.  For more information, see http://mojoware.org.
/*
/*    You may redistribute and/or modify Mojo under the terms of the GNU General Public License, 
/*    version 3, as published by the Free Software Foundation.  You should have received a copy of the 
/*    license with mojo.  If you did not, go to http://www.gnu.org.
/*     
/*    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
/*    INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A 
/*    PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
/*    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
/*    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
/*    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
/*    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
/*    EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
/*
/***********************************************************************************************************************/

