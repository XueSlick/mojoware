/***********************************************************************************************************************
/*
/*    cFinder_target.cpp / mojo_engine
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"

#if 0 //////////////////////////////////

#include "cFinder.h"
#include <process.h>
#include <Psapi.h>
#include "set_privilege.h" // not used in current build

using namespace mojo;

//======================================================================================================================
//  DEFINE
//======================================================================================================================

//-----------------------------------------------------------------------
//  If SE_DEBUG is defined, Mojo obtains SeDebug privileges in order
//  to get a handle to WoW's process with which Mojo can find out
//  the pathname from which WoW was launched.  This may be useful
//  in the future in order to automatically discriminate between 
//  multiple instances of WoW on the same machine.  However I'm worried
//  that Warden may notice that something is being done to WoW's
//  process that requires SeDebug privileges (even though the action
//  is utterly harmless) so I've excluded the code (disabled it) from
//  the public builds by commenting out the following #define.
//
//  #define SE_DEBUG
//
//------------------------------------------------------------------------


//======================================================================================================================
//  DATA
//======================================================================================================================

//======================================================================================================================
//  CODE
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
// PROCESS TO MODULE
// Pass in process ID, get exe pathname
// With some process (e.g. WoW) equires SeDebugPrivilge
//----------------------------------------------------------------------------------------------------------------------
bool process_to_module ( wchar_t * pwRet, int iRetSize, DWORD dwProcessID )
{
	HANDLE hProcess = OpenProcess ( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
									FALSE, 
									dwProcessID );

	if ( NULL == hProcess )
	{
		DWORD dwError = GetLastError();
		dwError;
	}

	else
	{
		if ( 0 == GetModuleFileNameEx ( hProcess, NULL, pwRet, iRetSize-1 ) )
		{
			CloseHandle ( hProcess );
			return false;
		}
		// GetProcessImageFileName ( hProcess, pwRet, iRetSize-1 ); 
		// QueryFullProcessImageName( hProcess, 0, pwRet, iRetSize-1 ); // Specific to Windows 7
	}

	CloseHandle ( hProcess );
	return true;
}


//----------------------------------------------------------------------------------------------------------------------
//  FIND WOW CALL BACK
//----------------------------------------------------------------------------------------------------------------------
BOOL CALLBACK find_wow_cb ( HWND hwnd, LPARAM lParam )
{
	cArrayTarget * pTarget = reinterpret_cast<cArrayTarget*>(lParam);

	const wchar_t awWowClassName [] = L"GxWindowClassD3d";
	const int iSIZE = sizeof (awWowClassName) / sizeof ( wchar_t );
	wchar_t awFoundClassName [ iSIZE + 1];

#ifdef SE_DEBUG
	wchar_t awPath [ MAX_PATH + 1 ];
#endif

	if ( GetClassName ( hwnd, awFoundClassName, iSIZE ) )
	{
		if ( 0 == wcscmp ( awFoundClassName, awWowClassName ) )
		{
#ifdef SE_DEBUG
			DWORD dwProcessID;
			GetWindowThreadProcessId ( hwnd, &dwProcessID );

			if ( process_to_module ( awPath, MAX_PATH, dwProcessID ) )
			{
				cTarget t;
				t.hwnd = hwnd;
				t.sPath = awPath;
				pTarget->append ( t );
			}
#else

			cTarget t;
			t.hwnd = hwnd;
			pTarget->append ( t );

#endif

		}
	}

	return TRUE;
}


//----------------------------------------------------------------------------------------------------------------------
//  FIND WOW
//  look for copies of World of Warcraft
//----------------------------------------------------------------------------------------------------------------------
bool cFinder::find_wow ()
{


#ifdef SE_DEBUG // NOT USED IN CURRENT BUILD
	HANDLE hToken;

	if ( get_thread_token ( &hToken ) )
		set_privilege ( hToken, SE_DEBUG_NAME, TRUE );
#endif

	cArrayTarget aTarget;

	EnumWindows ( find_wow_cb, (LPARAM) &aTarget );

#ifdef SE_DEBUG // NOT USED IN CURRENT BUILD

	set_privilege ( hToken, SE_DEBUG_NAME, FALSE );

	if ( hToken )
		CloseHandle ( hToken );

#endif

	g_KeyBroadcaster.receive_from_finder ( & aTarget );

	return true;
}

#endif ////////////////////////////////////////////////