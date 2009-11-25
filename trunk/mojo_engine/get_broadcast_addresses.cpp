/***********************************************************************************************************************
/*
/*    get_broadcast_addresses.cpp / mojo_engine
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

//-------------------------------------------------------------------------------------------------------
//  MEMORY DEBUG
//-------------------------------------------------------------------------------------------------------

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

//-------------------------------------------------------------------------------------------------------
//  INCLUDES
//-------------------------------------------------------------------------------------------------------

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include "tStr.h"
#include "cLog.h"
#include "tArray2.h"

#if 0
	#include "cMonitor.h"
#endif

// extern cMonitor g_Monitor;
extern mojo::cLog g_Log;

using namespace mojo;

//======================================================================================================================
// PROTOTYPES
//======================================================================================================================

bool get_ip_address_table ( PMIB_IPADDRTABLE pIPAT );
DWORD ip_pw_to_dword ( const wchar_t *pAW );
void  ip_dword_to_aw ( wchar_t aw[16], DWORD dw );


//======================================================================================================================
// CODE
//======================================================================================================================

//---------------------------------------------------------------------------------------------
// GET IP ADDRESS TABLE
// called as subroutine by other functions in this file
//---------------------------------------------------------------------------------------------
bool get_ip_address_table ( PMIB_IPADDRTABLE * ppIPAT  )
{
	*ppIPAT = 0;
	DWORD dwSize = 0;
    for ( int i = 0; i < 10; i++ )
	{
        DWORD dwResult = GetIpAddrTable ( *ppIPAT, &dwSize, FALSE );

		if ( ERROR_INSUFFICIENT_BUFFER == dwResult )
		{
			if ( *ppIPAT )
				delete *ppIPAT;

			*ppIPAT = ( PMIB_IPADDRTABLE ) new BYTE [dwSize];

			if ( NULL == *ppIPAT )
			{
				LOG_SYSTEM_ERROR_T ( L"Memory allocation failed." );
				return false;
			}

			else
				continue;
        }

		else if ( NO_ERROR == dwResult )
			break;

		else
		{
			*ppIPAT = 0;
            LOG_SYSTEM_ERROR_TE ( L"GetIpAddrTable", dwResult );
			return false;
        }
    }

	return true;
}


//---------------------------------------------------------------------------------------------
// GET BEST IP
//---------------------------------------------------------------------------------------------
bool get_best_ip ( DWORD * pdwFromIP, DWORD dwDestIP )
{
	bool bRetVal = false;
	DWORD dwIndex;

	DWORD dwResult = GetBestInterface ( dwDestIP, &dwIndex );

	if ( dwResult != NO_ERROR )
	{
		LOG_SYSTEM_ERROR_TE ( L"GetBestInterface", dwIndex );
		return false;
	}

    PMIB_IPADDRTABLE pIPAT = 0; 

	if ( ! get_ip_address_table ( &pIPAT ) )
		return false;

	for ( DWORD i = 0; i < pIPAT->dwNumEntries; i++ )
	{
		MIB_IPADDRROW & r = pIPAT->table[i];

		if ( r.dwIndex == dwIndex )
		{
			*pdwFromIP = r.dwAddr;
			bRetVal = true;
			break;
		}
	}

	delete[] pIPAT;
	return bRetVal;
}


//---------------------------------------------------------------------------------------------
// GET BROADCAST ADDRESSES
//---------------------------------------------------------------------------------------------
bool get_ip_addresses ( mojo::cArrayU * pRet )
{
	PMIB_IPADDRTABLE pIPAT = 0; 

	//--------------------------------
	// GET TABLE
	//--------------------------------
	DWORD dwSize = 0;
    for ( int i = 0; i < 10; i++ )
	{
        DWORD dwResult = GetIpAddrTable ( pIPAT, &dwSize, FALSE );

		if ( ERROR_INSUFFICIENT_BUFFER == dwResult )
		{
			if ( pIPAT )
				delete pIPAT;

			pIPAT = ( PMIB_IPADDRTABLE ) new BYTE [dwSize];

			if ( NULL == pIPAT )
			{
				LOG_SYSTEM_ERROR_T ( L"Memory allocation failed." );
				return false;
			}

			else
				continue;
        }

		else if ( NO_ERROR == dwResult )
			break;

		else
		{
            LOG_SYSTEM_ERROR_TE ( L"GetIpAddrTable", dwResult );

			if ( pIPAT )
				delete[] pIPAT;

			return false;
        }
    }

	for ( DWORD i = 0; i < pIPAT->dwNumEntries; i++ )
	{
		MIB_IPADDRROW & r = pIPAT->table[i];

		// loopback addr 127.0.0.1?
		if ( 0x0100007f == r.dwAddr )
			continue;

#if 0
		wchar_t awAddr[16];

		wchar_t awMask[16];
		wchar_t awBaddr[16];
		ip_dword_to_aw ( awAddr, r.dwAddr );
		ip_dword_to_aw ( awMask, r.dwMask );
		ip_dword_to_aw ( awBaddr, r.dwBCastAddr );


		DWORD dwBaddr = ( r.dwAddr & r.dwMask ) | ~ r.dwMask;

		ip_dword_to_aw ( awBaddr, dwBaddr );
#endif


		if ( pRet )
			pRet->append ( r.dwAddr );
	}

	//---------------------------------
	//  DISPLAY RESULTS
	//---------------------------------
#if 0
	g_Monitor.print ( L"IP addresses found:", 0, black, bold );
	for ( DWORD j = 0; j < pRet->qty(); j++ )
	{
		wchar_t t[16];
		ip_dword_to_aw ( t, (*pRet)[j] );
		g_Monitor.print ( t, 1, purple, bold );
	}
	g_Monitor.print ( L"" );
#endif

	if ( pIPAT )
		delete[] pIPAT;

	return true;
}




//---------------------------------------------------------------------------------------------
// GET BROADCAST ADDRESSES
//---------------------------------------------------------------------------------------------
bool get_broadcast_addresses ( mojo::tArray2<DWORD> * pRet )
{

    PMIB_IPADDRTABLE pIPAT = 0; 

	//--------------------------------
	// GET TABLE
	//--------------------------------
	DWORD dwSize = 0;
    for ( int i = 0; i < 10; i++ )
	{
        DWORD dwResult = GetIpAddrTable ( pIPAT, &dwSize, FALSE );

		if ( ERROR_INSUFFICIENT_BUFFER == dwResult )
		{
			if ( pIPAT )
				delete pIPAT;

			pIPAT = ( PMIB_IPADDRTABLE ) new BYTE [dwSize];

			if ( NULL == pIPAT )
			{
				LOG_SYSTEM_ERROR_T ( L"Memory allocation failed." );
				return false;
			}

			else
				continue;
        }

		else if ( NO_ERROR == dwResult )
			break;

		else
		{
            LOG_SYSTEM_ERROR_TE ( L"GetIpAddrTable", dwResult );
			return false;
        }
    }

	for ( DWORD i = 0; i < pIPAT->dwNumEntries; i++ )
	{
		MIB_IPADDRROW & r = pIPAT->table[i];

		// loopback addr 127.0.0.1?
		if ( 0x0100007f == r.dwAddr )
			continue;

#if 0
		wchar_t awAddr[16];
		wchar_t awMask[16];
		wchar_t awBaddr[16];
		ip_dword_to_aw ( awAddr, r.dwAddr );
		ip_dword_to_aw ( awMask, r.dwMask );
		ip_dword_to_aw ( awBaddr, r.dwBCastAddr );
#endif


		DWORD dwBaddr = ( r.dwAddr & r.dwMask ) | ~ r.dwMask;

		// ip_dword_to_aw ( awBaddr, dwBaddr );

		pRet->append ( dwBaddr );
	}

	//---------------------------------
	//  DISPLAY RESULTS
	//---------------------------------

#if 0 // MONITOR STUFF
	g_Monitor.print ( L"Broadcast addresses found:", 0, black, bold );
	for ( DWORD j = 0; j < pRet->qty(); j++ )
	{
		wchar_t t[16];
		ip_dword_to_aw ( t, (*pRet)[j] );
		g_Monitor.print ( t, 1, purple, bold );
	}
	g_Monitor.print ( L"" );
#endif

	return true;
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