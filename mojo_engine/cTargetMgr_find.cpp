/***********************************************************************************************************************
/*
/*    cTargetMgr_find.cpp / mojo_engine
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"
#include <process.h>
#include <Psapi.h>
#include "set_privilege.h" // not used in current build

using namespace mojo;


//======================================================================================================================
//  DEFINE
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
//  If SE_DEBUG is defined, Mojo obtains SeDebug privileges in order to get a handle to WoW's process with which Mojo 
//  can find out the pathname from which a found WoW was launched.  This may be useful in the future in order to 
//  automatically discriminate between multiple instances of WoW on the same machine.  However I'm worried that 
//  Warden may notice that something is being done to WoW's process that requires SeDebug privileges (even though the 
//  action is utterly harmless) so I've excluded the code (disabled it) from the public builds by commenting out the 
//  following #define.
//
//  #define SE_DEBUG
//
//----------------------------------------------------------------------------------------------------------------------


//======================================================================================================================
//  DATA
//======================================================================================================================

//======================================================================================================================
//  CODE
//======================================================================================================================


//----------------------------------------------------------------------------------------------------------------------
//  HWND IS IN ARRAY
//----------------------------------------------------------------------------------------------------------------------
bool cTargetMgr :: hwnd_is_in_array ( cArrayTarget * pRay, HWND hwnd )
{
	for ( unsigned i = 0; i < pRay->qty(); i++ )
	{
		if ( hwnd == (*pRay)[i].hwnd )
			return true;
	}

	return false;
}


//----------------------------------------------------------------------------------------------------------------------
//  TARGET IS IN ARRAY
//----------------------------------------------------------------------------------------------------------------------
bool cTargetMgr :: target_is_in_array ( cArrayTarget * pRay, DWORD hMach, HWND hwnd, DWORD dwProcessID )
{
	for ( unsigned i = 0; i < pRay->qty(); i++ )
	{
		if ( hwnd == (*pRay)[i].hwnd && hMach == (*pRay)[i].hMach && dwProcessID ==(*pRay)[i].dwProcessID )
			return true;
	}

	return false;
}

//----------------------------------------------------------------------------------------------------------------------
//  TARGET IS IN ARRAY
//----------------------------------------------------------------------------------------------------------------------
cTarget * cTargetMgr :: find_target_in_array ( cArrayTarget * pRay, cTarget * a )
{
	for ( unsigned i = 0; i < pRay->qty(); i++ )
	{
		if ( a->hMach == 1 && a->bLaunchByMojo == true )
		{
			if ( (*pRay)[i].dwID == a->dwID )
				return &(*pRay)[i];
		}
		
		else
		{
			if ( a->hwnd == (*pRay)[i].hwnd && a->hMach == (*pRay)[i].hMach && a->dwProcessID ==(*pRay)[i].dwProcessID )
				return &(*pRay)[i];
		}
	}

	return false;
}


//----------------------------------------------------------------------------------------------------------------------
//  FIND HWND IN LIST
//----------------------------------------------------------------------------------------------------------------------
cTarget * cTargetMgr :: find_hwnd_in_list ( HWND hwnd )
{
	for ( cTarget * p = List.pHead; p; p = p->pNext )
	{
		if ( hwnd == p->hwnd )
			return p;
	}

	return NULL;
}

//----------------------------------------------------------------------------------------------------------------------
//  FIND TARGET IN LIST
//----------------------------------------------------------------------------------------------------------------------
cTarget * cTargetMgr :: find_target_in_list ( cTarget * a )
{
	for ( cTarget * p = List.pHead; p; p = p->pNext )
	{
		if ( a->bLaunchByMojo == true )
		{
			if ( p->hMach == a->hMach && p->dwID == a->dwID )
				return p;
		}

		else
		{
			if ( a->hwnd == p->hwnd && a->hMach == p->hMach && a->dwProcessID == p->dwProcessID )
				return p;
		}
	}

	return NULL;
}


//----------------------------------------------------------------------------------------------------------------------
//  FIND TARGET IN LIST
//----------------------------------------------------------------------------------------------------------------------
cTarget * cTargetMgr :: find_target_in_list ( DWORD hMach, HWND hwnd, DWORD dwProcessID )
{
	for ( cTarget * p = List.pHead; p; p = p->pNext )
	{
		if ( hwnd == p->hwnd && hMach == p->hMach && dwProcessID == p->dwProcessID )
			return p;
	}

	return NULL;
}


//----------------------------------------------------------------------------------------------------------------------
//  RECEIVE LOCAL FINDS
//----------------------------------------------------------------------------------------------------------------------
void cTargetMgr :: receive_local_finds ( cArrayTarget * a )
{
	bool bChanged = false;

	List.lock();

	//-------------------------------------------
	//  FOR EACH ITEM IN ARRAY
	//-------------------------------------------

	for ( unsigned i = 0; i < a->qty(); i++ )
	{
		(*a)[i].hMach = 1; // LOCAL
		(*a)[i].bIsRunning = true;

		//-------------------------------------------
		//  ADD IT TO LIST IF IT'S NOT IN LIST
		//-------------------------------------------

//		cTarget * pTarget = find_target_in_list ( (*a)[i].hMach, (*a)[i].hwnd, (*a)[i].dwProcessID );

		cTarget * pFound = find_target_in_list ( &(*a)[i] );

		if ( pFound )
		{
			(*a)[i].sName         = pFound->sName; 
			pFound->bIsRunning     = true;
		}

		else
		{
			// (*a)[i].sName         = L"(found running)";
			(*a)[i].bLaunchByMojo = false;
		
			cTarget * pNew = new cTarget ( (*a)[i] );
			pNew->sName = (*a)[i].sName;
			pNew->hwnd = (*a)[i].hwnd;
			pNew->hMach = 1; // LOCAL
			pNew->bLaunchByMojo = false;
			pNew->bIsRunning = true;
			List.append ( pNew );
			bChanged = true;
		}
	}

	//-------------------------------------------
	//  FOR EACH LOCAL TARGET IN LIST
	//-------------------------------------------

	cTarget * p = List.pHead;
	while ( p )
	{
		cTarget * pNext = p->pNext;

		if ( 1 == p->hMach ) // LOCAL TARGETS ONLY
		{
			//-------------------------------------------
			//  REMOVE IT FROM LIST IF IT'S NOT IN ARRAY
			//  AND IF IT'S NOT LAUNCH-BY-MOJO
			//-------------------------------------------

			cTarget * pFound = find_target_in_array ( a, p );

			if ( pFound ) // in array
			{
				if ( ! p->bIsRunning )
				{
					p->bIsRunning = true;
					bChanged = true;
				}
			}

			else // not in array
			{
				if ( p->bIsRunning )
				{
					p->bIsRunning = false;
					bChanged = true;
				}

				if ( ! p->bLaunchByMojo )
				{
					List.rem_del ( p );
					bChanged = true;
				}
			}
		}

		p = pNext;
	}

	List.unlock();

	if ( bChanged )
		cMessenger::tell_app_that_targets_changed();
}


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
			DWORD dwProcessID;
			GetWindowThreadProcessId ( hwnd, &dwProcessID );

#ifdef SE_DEBUG

			if ( process_to_module ( awPath, MAX_PATH, dwProcessID ) )
			{
				cTarget t;
				t.hwnd = hwnd;
				t.sPath = awPath;
				pTarget->append ( t );
			}
#else
			cTarget t;
			t.dwProcessID = dwProcessID;
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
bool cTargetMgr :: find_wow ()
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

	this->receive_local_finds( & aTarget );
	cArrayTarget aComplete;
	mojo::get_local_targets ( &aComplete );
	this->broadcast_targets ( & aComplete );

	return true;
}


/***********************************************************************************************************************
/*
/*    This file is part of Mojo.  For more information, see http://mojoware.org.
/*
/*    You may redistribute and/or modify Mojo under the terms of the GNU General Public License, version 3, as
/*    published by the Free Software Foundation.  You should have received a copy of the license with Mojo.  If you
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
