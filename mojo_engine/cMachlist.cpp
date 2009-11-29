/***********************************************************************************************************************
/*
/*    cMachlist.cpp / mojo_engine
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"
// #include "cMachlist.h"
// #include "cPool.h"

using namespace mojo;

//======================================================================================================================
// CODE
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
//  HANDLE TO IP
//----------------------------------------------------------------------------------------------------------------------
DWORD cMachlist :: handle_to_ip ( DWORD dwHandle )
{
	if ( 0 == dwHandle )
		return 0;

	DWORD dwRet = 0;

	lock();

	for ( cMach * p = first(); p; p = next(p) )
	{
		if ( dwHandle == p->dwSerialNumber )
		{
			dwRet = p->dwIP;
			break;
		}
	}

	unlock();

	return dwRet;
}


//----------------------------------------------------------------------------------------------------------------------
// GET BY IP OR ADD
//----------------------------------------------------------------------------------------------------------------------
cMach * cMachlist::get_by_ip_or_add ( DWORD dwIP, const wchar_t * pDisplayList )
{
	cMach * pRetVal = NULL;
	bool bAdded = false;

	lock();

	for ( cMach * p = first(); p; p = next(p) )
	{
		if ( p->dwIP == dwIP )
		{
			pRetVal = p;
			break;
		}
	}

	if ( ! pRetVal )
	{
		pRetVal = new cMach;
		pRetVal->dwIP = dwIP;
		wchar_t awIP[16];
		ip_dword_to_aw ( awIP, dwIP );
		pRetVal->sDottedDec = awIP;
		get_draw_position_from_file ( pRetVal );

		if ( pDisplayList )
			pRetVal->DisplayList.set_from_text ( pDisplayList );

		append ( pRetVal );
		pRetVal->dwSerialNumber = ++this->dwLastSerialNumberAssigned;
		bAdded = true;
	}

	unlock();

	if ( bAdded )
	{
		cMessenger::tell_app_that_connections_changed();

#if 0 // MONITOR STUFF
		PostMessage ( g_hwnd, uWM_MONITOR_CONNECTIONS, 0, 0 );

		if ( pDisplayList )
			PostMessage	( g_hwnd, uWM_SET_MOUSEOVER_DISPLAY_LIST,	0, 0 );
#endif
	}
	
	return pRetVal;
}



//---------------------------------------------------------------------------------------------
// FILE PATHNAME
//---------------------------------------------------------------------------------------------
const wchar_t * cMachlist :: file_pathname ( cStrW * pRet )
{
	*pRet = g_sAppDataDirectory;
	*pRet += g_sAppTitle.cstr();
	*pRet += L".draw_positions.txt";
	return pRet->cstr();
}


//---------------------------------------------------------------------------------------------
// GET DRAW POSITION FROM FILE
// matches by IP only
//---------------------------------------------------------------------------------------------
bool cMachlist :: get_draw_position_from_file ( cMach * pMach )
{
	assert ( pMach );

	if ( ( ! pMach->bThisPC ) && ( 0 == pMach->sDottedDec.len() ) )
		return false;

	//-------------------------------------
	// OPEN FILE
	//-------------------------------------
	cStrW sPathname;
	file_pathname(&sPathname);
	cFileIn f ( sPathname.cstr() );
	FILE * h = f.h;

	if ( ! h )
		return false;

	cStrW sLine;

	wchar_t * pRgx = L"(.*?), (.*?), (\\d*?), (\\d*?), (.*?)";

	cStrW aRet[5];

	while ( f.get_line ( &sLine ) )
	{
		if ( ! match ( aRet, sizeof(aRet)/sizeof(cStrW), pRgx, sLine.cstr() ) )
		{
			sLine.erase();
			continue;
		}

		// wchar_t * pName = pMach->bThisPC ? L"local" : pMach->sName.cstr();
		wchar_t * pIP	= pMach->bThisPC ? L"local" : pMach->sDottedDec.cstr();

		// MATCH BY IP ONLY
		if ( 0 == wcscmp ( aRet[1].cstr(), pIP ) )
		{
			pMach->DrawPos.x = _wtoi ( aRet[2].cstr() );
			pMach->DrawPos.y = _wtoi ( aRet[3].cstr() );

			if ( 0 == wcscmp ( aRet[4].cstr(), L"valid" ) )
				pMach->bValidDrawPos = true;

			else
				pMach->bValidDrawPos = false;

			return true;
		}

		sLine.erase();
	}

	return false;
}


//---------------------------------------------------------------------------------------------
//  SAVE DRAW POSITIONS TO FILE
//---------------------------------------------------------------------------------------------
void cMachlist :: save_draw_positions_to_file ()
{
	if ( ! qty() )
		return;

	//-------------------------------------
	// OPEN FILE
	//-------------------------------------
	cStrW sPathname;
	file_pathname(&sPathname);
	cFileOut f ( sPathname.cstr() );
	FILE * h = f.h;

	if ( ! h )
		return;

	//-------------------------------------
	// WRITE INFO TO FILE
	//-------------------------------------
	for ( cMach * p = first(); p; p = next(p) )
	{
		if ( p->bThisPC )
			fwprintf ( h, L"local, local, %d, %d, %s\n", p->DrawPos.x, p->DrawPos.y, p->bValidDrawPos ? L"valid" : L"invalid" );

		else
			fwprintf ( h, L"%s, %s, %d, %d, %s\n", p->sName.cstr(), p->sDottedDec.cstr(), p->DrawPos.x, p->DrawPos.y, p->bValidDrawPos ? L"valid" : L"invalid" );
	}
}


//---------------------------------------------------------------------------------------------
//  LOCAL MACHINE
//---------------------------------------------------------------------------------------------
cMach * cMachlist :: local_machine ()
{
	for ( cMach * p = first(); p; p = next(p) )
	{
		if ( p->bThisPC )
			return p;
	}

	return NULL;
}


//---------------------------------------------------------------------------------------------
//  INIT AND INSERT LOCAL MACHINE
//  called synchronously by main UI thread on startup
//---------------------------------------------------------------------------------------------
void cMachlist :: init_and_insert_local_machine ()
{
	cMach * p = new cMach;

	p->bThisPC = true;
	::get_full_dns_name ( &p->sName );
	p->DisplayList.set_from_local_hardware ();
	append ( p );
	p->dwSerialNumber = ++this->dwLastSerialNumberAssigned;
	get_draw_position_from_file ( p );
}


//---------------------------------------------------------------------------------------------
//  RECEIVE NEW MACH
//  can be called by outside thread
//---------------------------------------------------------------------------------------------
#if 0
void cMachlist :: receive_new_mach ( cMach * pNew )
{
	assert(pNew);
	receipts.append ( pNew ); 
	SendMessage ( g_hwnd, uWM_MACHLIST_RECEIPTS, 0, 0 );
}
#endif

//---------------------------------------------------------------------------------------------
//  ADD RECEIPTS
//  can only be called by UI thread
//---------------------------------------------------------------------------------------------
#if 0
#pragma warning ( push )
#pragma warning ( disable : 4706 ) // assignment within conditional expression


void cMachlist::add_receipts ()
{
	cMach * pNew;

	while ( pNew = receipts.pop() )
	{
		assert ( pNew );
		assert ( pNew->dwIP );

		cMach * pOld = get_by_ip ( pNew->dwIP );

		if ( NULL == pOld )
		{
			append ( pNew );
			pNew->dwSerialNumber = ++this->dwLastSerialNumberAssigned;
			continue;
		}

		else
		{
			if ( 0 == pOld->sName.len() )
				pOld->sName = pNew->sName;

			else
			{
				if ( 0 != wcscmp ( pOld->sName.cstr(), pNew->sName.cstr() ) )
				{
					LOG_SYSTEM_ERROR_TE ( L"Names don't match.", 0 );
				}
			}

			delete pNew;
		}
	}


	cMessenger::tell_app_that_connections_changed();
#if 0 // MONITOR STUFF
	SendMessage ( g_hwnd, uWM_MONITOR_CONNECTIONS, 0, 0 );
#endif

}
#pragma warning ( pop )
#endif

//---------------------------------------------------------------------------------------------
// GET BY NAME
//---------------------------------------------------------------------------------------------
cMach * cMachlist::get_by_name ( const wchar_t * pName )
{
	for ( cMach *p = first(); p; p = next( p ) )
	{
		if ( 0 == wcscmp ( pName, p->sName.cstr() ) )
			return p;
	}

	return NULL;
}

//---------------------------------------------------------------------------------------------
// GET BY IP AND NAME
//---------------------------------------------------------------------------------------------
#if 0
cMach * cMachlist::get_by_ip_and_name ( DWORD dwArgIP, const wchar_t * pArgName )
{
	for ( cMach *p = first(); p; p = next( p ) )
	{
		if ( ( p->dwIP == dwArgIP ) && ( 0 == wcscmp ( pArgName, p->sName.cstr() ) ) )
			return p;
	}

	return NULL;
}
#endif

//---------------------------------------------------------------------------------------------
// GET BY IP
//---------------------------------------------------------------------------------------------
cMach * cMachlist::get_by_ip ( DWORD dwIP )
{
	for ( cMach *p = first(); p; p = next( p ) )
	{
		if ( dwIP == p->dwIP )
			return p;
	}

	return NULL;
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