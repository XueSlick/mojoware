/********************************************************************************************************
/*
/*    initialize_engine.cpp / mojo_engine
/*
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/*******************************************************************************************************/

#include "stdafx.h"

using namespace mojo;


const wchar_t * g_apDefaultScribs [];


//--------------------------------------------------------------------------------------------------------
//  SHUT DOWN ENGINE
//--------------------------------------------------------------------------------------------------------
void mojo :: shut_down_engine ()
{
	g_Settings.save_to_file ();
}


//--------------------------------------------------------------------------------------------------------
//  LOAD ENGINE SETTINGS
//  Normal start-up order is (1) call this; (2) if you want, use set() functions to override the engine's
//  saved settings; (3) call initialize_engine().
//--------------------------------------------------------------------------------------------------------
MOJO_ENGINE_API bool mojo :: load_engine_settings ( const wchar_t * pAppDataDirectory )
{
	cStrW sPath = pAppDataDirectory;
	sPath += g_pwEngineTitle;
	sPath += L".settings.txt";
	g_Settings.set_pathname ( sPath.cstr() );
	return g_Settings.load_from_file ();
}


//--------------------------------------------------------------------------------------------------------
//  INITIALIZE ENGINE
//--------------------------------------------------------------------------------------------------------
bool mojo :: initialize_engine ( HINSTANCE hAppInstance, 
								  HWND hwndApp,
								  const wchar_t * pAppTitle,
								  const wchar_t * apScribs [],
								  const wchar_t * pScribPathname [],
	                              const wchar_t  * pAppDataDirectory,
								  const cVersion * pVersionRequiredByApp )
{
	assert ( hwndApp );
	assert ( pAppDataDirectory );
	assert ( pVersionRequiredByApp );
	assert ( pAppTitle );

	g_hAppInstance      = hAppInstance;
	g_sAppTitle         = pAppTitle;
	g_sAppDataDirectory = pAppDataDirectory;

	assert ( g_sAppTitle.len() );

	//----------------------------------------------
	// RUN ONCE (do this first)
	//----------------------------------------------

	static bool bInitialized = false;

	if ( bInitialized )
	{
		LOG ( L"Error: an attempt was made to reinitialize the engine." );
		return false;
	}

	bInitialized = true;

	//----------------------------------------------
	// CHECK WINDOW HANDLE (do this second)
	//----------------------------------------------

	if ( NULL == hwndApp )
	{
		LOG ( L"Bad argument:  initialize_engine was called with null window handle." );
		throw ( cException ( L"BadArgument",  L"initialize_engine." ) );
	}

	else
		g_hwndApp = hwndApp;

	//----------------------------------------------
	// LOAD DEFAULT (BUILT-IN) SCRIBS (do this third)
	//----------------------------------------------

	LOG ( L"About to load default scribs." );
	int iQty = g_ScribMgr.load_scribs ( g_apDefaultScribs, false );
	LOG_V ( L"Just loaded engine's compiled scribs; qty = %d", iQty );
	assert (iQty);

	//----------------------------------------------
	// LOAD USER-SUPPLIED SCRIBS
	//----------------------------------------------

	if ( apScribs )
	{
		int iQty = mojo::load_scribs ( apScribs, true );
		UNREFERENCED_PARAMETER(iQty);
	}

	else if ( pScribPathname )
	{
		int iQty = g_ScribMgr.load_scribs ( apScribs, true );

		if ( iQty )
			mojo::put_memo ( cMemo::success, L"LoadScribFile", iQty );

		else
			mojo::put_memo ( cMemo::error, L"CannotLoadScribFile", pScribPathname );
	}

	//----------------------------------------------
	// INITIALIZE INPUT EVENT SCRIBS
	//----------------------------------------------
		// g_ScribMgr.log();
		LOG ( L"About to call cInputEvent::load_scribs()" );
		cInputEvent::load_scribs();

	//----------------------------------------------
	// START MESSENGER THREAD
	//----------------------------------------------

	LOG ( L"About to start messenger thread." );

	g_Messenger.start_thread ();

	//----------------------------------------------
	// CHECK VERSION
	//----------------------------------------------

	LOG ( L"About to check versions." );

	unsigned uHookMaj, uHookMin, uHookPip, uHookBld;
	mojo_hooks::get_version ( &uHookMaj, &uHookMin, &uHookPip, &uHookBld );
	cVersion HooksDllVersion ( uHookMaj, uHookMin, uHookPip, uHookBld );

	if ( g_Version 			!= *pVersionRequiredByApp 	||
		 g_Version 			!= HooksDllVersion  		||
		 HooksDllVersion 	!= *pVersionRequiredByApp 		)
	{
		cStrW sExe, sEngine, sHooks;
		g_Version.get_text ( &sEngine);
		pVersionRequiredByApp->get_text ( &sExe );
		HooksDllVersion.get_text ( &sHooks );
		cException e ( L"FileVersionMismatch", sExe.cstr(), L"mojo_engine.dll", sEngine.cstr(), L"mojo_hooks.dll", sHooks.cstr() );
		LOG ( &e );
		throw ( e );
	}

	//----------------------------------------------
	// SET PATHNAME FOR SETTINGS FILE
	//----------------------------------------------

	cStrW sPath = pAppDataDirectory;
	sPath += g_pwEngineTitle;
	sPath += L".settings.txt";
	g_Settings.set_pathname ( sPath.cstr() );

	//----------------------------------------------
	// START HOOK THREAD AND INSTALL HOOKS
	//----------------------------------------------

	LOG ( L"About to start hooks thread." );

	g_dwHookThreadID = mojo_hooks::start_thread ( g_hwndApp, cMessenger::keyboard_hook_service_routine, cMessenger::mouse_hook_service_routine );

	//-------------------------------------
	// INSERT cMACH FOR LOCAL PC
	//-------------------------------------

	// do this before g_Finder and g_Pool start
	// because they need to send the local
	// machine's DisplayList

	g_Machlist.init_and_insert_local_machine ();

	//-------------------------------------
	// START SOCKET (TCP) COMMUNICATIONS
	//-------------------------------------

	LOG ( L"About to start g_Pool." );

	g_Pool.start(); 

	//----------------------------------------------
	// START Discovery THREADS
	//----------------------------------------------

	LOG ( L"About to start g_Finder." );

	g_Finder.start();

	return true;
}


/********************************************************************************************************
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
/********************************************************************************************************/