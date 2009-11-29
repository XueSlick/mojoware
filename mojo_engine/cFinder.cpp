/***********************************************************************************************************************
/*
/*    cFinder.cpp / mojo_engine
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"
#include "cFinder.h"
#include <process.h>
#include "cPool.h"
#include <Psapi.h> // for GetModuleFileNameEx
#include "se_debug_privilege.h"
using namespace mojo;

//======================================================================================================================
// DATA
//======================================================================================================================

extern cPool g_Pool;


//======================================================================================================================
// PROTOTYPES
//======================================================================================================================

bool get_broadcast_addresses	( tArray<DWORD> * );
bool get_ip_addresses			( cArrayU * );
bool get_best_ip				( DWORD * pdwFromIP, DWORD dwDestIP );


//======================================================================================================================
// CODE
//======================================================================================================================


//----------------------------------------------------------------------------------------------------------------------
//  IS_LOCAL_IP
//  tests whether the IP is assigned to this machine.
//----------------------------------------------------------------------------------------------------------------------
bool cFinder::is_local_ip ( DWORD dwArg )
{
	cArrayU aIP;
	::get_ip_addresses ( &aIP );
	
	for ( DWORD i = 0; i < aIP.qty(); i++ )
	{
		if ( aIP[i] == dwArg )
			return true;
	}

	return false;
}


//----------------------------------------------------------------------------------------------------------------------
//  INIT_LOCAL_IP
//----------------------------------------------------------------------------------------------------------------------
bool cFinder::get_local_ip ( DWORD * pdwLocalIP )
{
	if ( g_Settings.bUseSpecifiedLocalIP )
		*pdwLocalIP = g_Settings.uSpecifiedLocalIP;
		
	else if ( ! ( get_best_ip ( pdwLocalIP, INADDR_ANY ) ) )
	{
		LOG ( L"Unable to get best IP in cFinder::init_local_ip." );
			return false;
	}

	return true;
}


//----------------------------------------------------------------------------------------------------------------------
//  MAKE SEND SOCKET
//----------------------------------------------------------------------------------------------------------------------
bool cFinder :: make_send_socket ( SOCKET * pRet, DWORD dwLocalIP )
{
	SOCKADDR_IN saClient	= {0};
	BOOL fBroadcast			= TRUE;

	SOCKET s = socket ( AF_INET, SOCK_DGRAM, IPPROTO_UDP );

	//----------------------------------
	// SET SOCK OPT
	//----------------------------------
    int iError = setsockopt (	s,
								SOL_SOCKET,
								SO_BROADCAST,
								(CHAR *) &fBroadcast,
								sizeof ( BOOL ) );

	if ( SOCKET_ERROR == iError )
	{
		LOG_SYSTEM_ERROR_TE ( L"setsockopt", WSAGetLastError () );
		return false;
	}

	//----------------------------------
	// LOCAL ADDRESS
	//----------------------------------
    // bind to a local socket and an interface.
    saClient.sin_family = AF_INET;
    saClient.sin_addr.s_addr = dwLocalIP; 
    saClient.sin_port = htons ( 0 );

	//----------------------------------
	// BIND
	//----------------------------------
    if ( SOCKET_ERROR == bind ( s, (SOCKADDR *) &saClient, sizeof (SOCKADDR_IN) ) )
	{
		LOG_SYSTEM_ERROR_TE ( L"bind", WSAGetLastError () );
		return false;
	}

	*pRet = s;
	return true;
}


//----------------------------------------------------------------------------------------------------------------------
//  MAKE REMOTE SOCKET ADDRESS
//----------------------------------------------------------------------------------------------------------------------
void cFinder :: make_remote_socket_address ( SOCKADDR_IN * pRet )
{
	//----------------------------------
	// REMOTE ADDRESS
	//----------------------------------
    pRet->sin_family			= AF_INET;
    pRet->sin_addr.s_addr	= htonl ( INADDR_BROADCAST );
	pRet->sin_port = htons ( static_cast<u_short>( g_Settings.uPort ) );
}


//----------------------------------------------------------------------------------------------------------------------
//  SEND
//----------------------------------------------------------------------------------------------------------------------
bool cFinder :: send ()
{
	//----------------------------------
	// REMOTE ADDRESS
	//----------------------------------

	SOCKADDR_IN saServer = {0};
	make_remote_socket_address ( &saServer );

	for (;;)
	{
		find_wow ();
		send_inner_loop( &saServer );
		Sleep ( g_Settings.uDiscoveryyBroadcastInterval );

	}
}


//----------------------------------------------------------------------------------------------------------------------
//  SEND INNER LOOP
//----------------------------------------------------------------------------------------------------------------------
bool cFinder :: send_inner_loop ( SOCKADDR_IN * psaServer )
{
	//----------------------------------
	// INNER LOOP -- SEND TO
	//----------------------------------
	// for ( ;; )
	{
		SOCKET s;

		if ( ! get_local_ip ( &m_dwLocalIP ) )
			return false;

		if (  ! is_local_ip ( m_dwLocalIP ) )
		{
			wchar_t awIP[16];
			ip_dword_to_aw ( awIP, m_dwLocalIP );

			mojo::put_ad_lib_memo ( mojo::cMemo::error, L"Auto Find", L"Specified IP address %s does not exist on this computer.\nChange IP address in connection settings.", awIP );
			LOG_V ( L"Bad local ip: %s", awIP );
		}

		if ( ! make_send_socket ( &s, m_dwLocalIP ) )
		{
			LOG ( L"Unable to make socket in cFinder::send." );
		}

		int iError = sendto (	s,
								( const char * ) pSig->buf(), 
								pSig->len(),
								0,
								(SOCKADDR *) psaServer,
								(int)sizeof ( SOCKADDR_IN )
							);

		if ( SOCKET_ERROR == iError )
		{
			LOG_SYSTEM_ERROR_TE ( L"sendto", WSAGetLastError () );
		}

		//------------------------------------------
		//  CHECK FOR ERROR
		//------------------------------------------

		SOCKADDR_IN saTemp = {0};
		int iRetSize = sizeof(saTemp);
		if ( SOCKET_ERROR == getsockname ( s, (SOCKADDR*) &saTemp, &iRetSize ) )
			LOG_SYSTEM_ERROR_TE ( L"getsockname", WSAGetLastError() );

		//------------------------------------------
		//  DISPLAY WHAT WE JUST DID
		//------------------------------------------

#if 0
		DWORD dwLocalIP = *(DWORD*) &saTemp.sin_addr;
		wchar_t awIP[16];
		ip_dword_to_aw ( awIP, dwLocalIP );
		mojo::put_ad_lib_memo ( mojo::cMemo::success, L"cFinder", L"Send from %s", awIP );
#endif

		closesocket ( s );
	}

	return true;
}


//----------------------------------------------------------------------------------------------------------------------
//  SERVER THREAD
//----------------------------------------------------------------------------------------------------------------------
unsigned _stdcall cFinder::server_thread ( void * pArg )
{
	cFinder * pThis = reinterpret_cast<cFinder *>(pArg);

	pThis->receive ();

	return 0;
}
	

//----------------------------------------------------------------------------------------------------------------------
//  CLIENT THREAD
//----------------------------------------------------------------------------------------------------------------------
unsigned _stdcall cFinder::client_thread ( void * pArg )
{
	// Memory leak is in here

	cFinder * pThis = reinterpret_cast<cFinder*>(pArg);

	pThis->send ();

	return 0;
}
	

//----------------------------------------------------------------------------------------------------------------------
//   START THREADS
//----------------------------------------------------------------------------------------------------------------------
bool cFinder::start ()
{
	pSig = new cSignature;

	get_full_dns_name 	( &sLocalName );
	get_local_ip 		( &m_dwLocalIP );

#if 0
	if ( ! is_local_ip ( m_dwLocalIP ) )
	{
		wchar_t awIP[16], t[1000];
		ip_dword_to_aw ( awIP, m_dwLocalIP );
		wsprintf ( t, L"%s is not assigned to this computer.", awIP );

		g_Monitor.rcv ( sMon ( L"Invalid IP address.", 0, red, bold ),
						sMon ( t, 1, red, regular ),
						sMon ( L"Change IP address on Connection Settings Panel.", 1, red ) );

		LOG ( L"is_local_ip failed in cFinder::start_threads." );
		LOG ( t );
	}
#endif

	WSADATA wsad;

    if ( 0 != WSAStartup ( MAKEWORD ( 2, 2 ), &wsad ) )
    {
        LOG_SYSTEM_ERROR_T ( L"WSAStartup" );
        return false;
    }

	HANDLE hServer = (HANDLE) _beginthreadex (  NULL, 0, &server_thread, reinterpret_cast<void*>(this), 0, &uServerThreadID );
	HANDLE hClient = (HANDLE) _beginthreadex (  NULL, 0, &client_thread, reinterpret_cast<void*>(this), 0, &uClientThreadID );

	if ( g_Settings.bRunDiscoveryyThreadsAtLowPriority )
	{
		if ( 0 == SetThreadPriority ( hServer, THREAD_PRIORITY_BELOW_NORMAL ) )
			LOG_SYSTEM_ERROR_T ( L"SetThreadPriority" );

		if ( 0 == SetThreadPriority ( hClient, THREAD_PRIORITY_BELOW_NORMAL ) )
			LOG_SYSTEM_ERROR_T ( L"SetThreadPriority" );
	}

	return true;
}


//----------------------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR
//----------------------------------------------------------------------------------------------------------------------
cFinder::cFinder () :  m_dwLocalIP (0), pSig (0) {}


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