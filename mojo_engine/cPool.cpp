/********************************************************************************************************
/*
/*    cPool.cpp / mojo_engine
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/*******************************************************************************************************/

//-------------------------------------------------------------------------------------------------------
//  MEMORY DEBUG
//-------------------------------------------------------------------------------------------------------

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

//-------------------------------------------------------------------------------------------------------
//  INCLUDES
//-------------------------------------------------------------------------------------------------------

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <process.h>
#include "cPool.h"
#include "cLapPlus.h"
#include "tStr.h"
#include "cLog.h"
#include "tList2.h"

using namespace mojo;

#include "cSettings.h"
#include "cMessenger.h"


//======================================================================================================================
// PROTOTYPES
//======================================================================================================================

//======================================================================================================================
// DATA
//======================================================================================================================

const wchar_t * get_full_dns_name ( cStrW * pRet );
extern cSettings g_Settings;


//======================================================================================================================
// CODE
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
//  ON SOCKET CLOSE NOTIFICATION
//----------------------------------------------------------------------------------------------------------------------

#pragma warning ( push )
#pragma warning ( disable : 4706 ) // assignment within conditional expression

#if 1 // july 7 2009
void cPool :: on_socket_close_notification ( WPARAM wParam, LPARAM lParam )
{
	UNREFERENCED_PARAMETER ( lParam );

	SOCKET Socket = static_cast<SOCKET>(wParam);
	DWORD dwIP = 0;
	sSocketInfo * pSI = 0;
	
	#ifdef _DEBUG
		DWORD  dwEvent = WSAGETSELECTEVENT ( lParam );
		assert ( FD_CLOSE == dwEvent );
	#endif

	//------------------------------------
	// CLOSE THE SOCKET THAT NOTIFIED US
	//------------------------------------

	if ( pSI = get_socket_info_by_socket ( Socket ) )
	{
		dwIP = pSI->dwRemoteIP;
		this->SocketInfoList.rem_del ( pSI );
		closesocket ( Socket );
	}

#if 0
	//------------------------------------
	// CLOSE OTHER SOCKETS THAT CONNECT
	// TO SAME IP
	//------------------------------------

	if ( dwIP )
	{
		while ( pSI = get_socket_info_by_ip ( dwIP ) )
		{
			SOCKET s = pSI->Socket;
			this->SocketInfoList.rem_del ( pSI );
			closesocket ( s );
		}
	}
#endif

	cMessenger::tell_app_that_connections_changed(); 
}

#else // prior to july 7 2009

void cPool :: on_socket_close_notification ( WPARAM wParam, LPARAM lParam )
{
	SOCKET Socket = static_cast<SOCKET>(wParam);
	DWORD  dwError = WSAGETSELECTERROR ( lParam );
	DWORD  dwEvent = WSAGETSELECTEVENT ( lParam );

	assert ( FD_CLOSE == dwEvent );

	sSocketInfo * pSI = get_socket_info_by_socket ( Socket );

	assert ( pSI );

	this->SocketInfoList.rem_del ( pSI );

	closesocket ( Socket );

	update_monitor_connections();
}
#endif

#pragma warning ( pop )


//----------------------------------------------------------------------------------------------------------------------
//  CREATE LISTEN SOCKET
//----------------------------------------------------------------------------------------------------------------------
bool cPool :: create_listen_socket ( void ) 
{
	//-------------------------------------
	// LINGER STUFF SEEMS NOT TO BE USED.
	//-------------------------------------
	int nRet = 0;
	LINGER lingerStruct;
	struct addrinfoW hints = {0};
	struct addrinfoW *addrlocal = NULL;

	lingerStruct.l_onoff = 1;
	lingerStruct.l_linger = 0;

	//-------------------------------------
	// RESOLVE INTERFACE
	//-------------------------------------
    hints.ai_flags  = AI_PASSIVE;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_IP;

	wchar_t awPort[100];
	_ultow_s ( g_Settings.uPort, awPort, sizeof(awPort)/sizeof(wchar_t), 10 );

	if ( GetAddrInfo(NULL, awPort, &hints, &addrlocal) != 0 ) {
		LOG_SYSTEM_ERROR_TE ( L"getaddrinfo", WSAGetLastError() );
        return(false);
	}

	if ( addrlocal == NULL ) {
		LOG_SYSTEM_ERROR_TE (L"getaddrinfo() failed to resolve/convert the interface", 0);
        return(FALSE);
	}

	//-------------------------------------
	// CREATE LISTEN SOCKET
	//-------------------------------------
	sListenSocket = create_socket ();

	if ( sListenSocket == INVALID_SOCKET) {
		FreeAddrInfo(addrlocal);
		return(FALSE);
	}

	//----------------------------------------
	// ASSOCIATE LISTENING SOCKET WITH IO PORT
	// (Thi is from AcceptEx page on MSDN, not
	// from the SDK iocpserverex sample.
	//----------------------------------------
	HANDLE hResult = CreateIoCompletionPort ( ( HANDLE ) sListenSocket, hIoPort, (u_long)0, 0);
	if ( hResult != hIoPort )
	{
		LOG_SYSTEM_ERROR_TE ( L"CreateIoCompletionPort (called for sListen)", GetLastError() );
	}

	//-------------------------------------
	// BIND LISTEN SOCKET
	//-------------------------------------
    nRet = bind ( sListenSocket, addrlocal->ai_addr, (int) addrlocal->ai_addrlen);
	if ( nRet == SOCKET_ERROR) {
		LOG_SYSTEM_ERROR_TE ( L"bind", WSAGetLastError());
		FreeAddrInfo(addrlocal);
		return(FALSE);
	}

	//-------------------------------------
	//  TURN NAGEL OFF
	//-------------------------------------
	if ( g_Settings.bDisableNagle )
		set_nagle ( sListenSocket, false );

	//-------------------------------------
	// CALL LISTEN
	// Argument "100" from MSDN page on
	// AcceptEx not from iocpservex sample.
	//-------------------------------------
	nRet = listen ( sListenSocket, 100 );
	if ( nRet == SOCKET_ERROR ) {
		LOG_SYSTEM_ERROR_TE ( L"listen", WSAGetLastError());
		FreeAddrInfo(addrlocal);
		return(FALSE);
	}

	get_accept_ex_pointer ( sListenSocket );
    
	FreeAddrInfo(addrlocal);

	return true;
}


//----------------------------------------------------------------------------------------------------------------------
//  CONSTRUCTOR
//----------------------------------------------------------------------------------------------------------------------
cPool :: cPool () : pSig ( 0 )
{
	this->pLapPlusList = new tList2<cLapPlus>;
}


//----------------------------------------------------------------------------------------------------------------------
//  START
//----------------------------------------------------------------------------------------------------------------------
void cPool :: start ()
{
	//-----------------------------------------------
	// INIT SIGNATURE
	//-----------------------------------------------

	pSig = new cSignature;

	//-----------------------------------------------
	// init Winsock
	//-----------------------------------------------
	WSADATA wsad;
    if ( 0 != WSAStartup ( MAKEWORD ( 2, 2 ), &wsad ) )
	{
        LOG_SYSTEM_ERROR_TE ( L"WSAStartup", WSAGetLastError() );
		return;
	}

	//-----------------------------------------------
	// initialize statics
	//-----------------------------------------------
	// if ( 0 == sLocalName.len() )
		// get_full_dns_name ( &sLocalName );
	// if ( 0 == sConnectSig.len() )
		// make_sig ( &sConnectSig );

	// pLocalNameBuf = sLocalName.buffer();

	//-----------------------------------------------
	// create io completion port
	//-----------------------------------------------
	hIoPort = CreateIoCompletionPort ( INVALID_HANDLE_VALUE, NULL, 0, 0 );
	if ( 0 == hIoPort )
		LOG_SYSTEM_ERROR_TE ( L"CreateIoCompletionPort", WSAGetLastError() );

	//-----------------------------------------------
	// calc number of worker threads
	//-----------------------------------------------
    SYSTEM_INFO si;
	GetSystemInfo(&si);
	DWORD dwQtyCPU = si.dwNumberOfProcessors;

	//-----------------------------------------------
	// create worker threads
	//-----------------------------------------------
	for( DWORD i = 0; i < dwQtyCPU; i++ ) 
	{
		HANDLE  hThread;
		unsigned uThreadID;

		hThread = reinterpret_cast<HANDLE> ( _beginthreadex ( NULL, 0, worker_thread, hIoPort, 0, &uThreadID ) );
 
		if ( hThread == NULL ) 
			LOG_SYSTEM_ERROR_TE ( L"_beginthreadex", GetLastError() );
	}

	//-----------------------------------------------
	// create listen socket
	//-----------------------------------------------
	if ( ! create_listen_socket () )
		NULL; // bad

	//-----------------------------------------------
	// create accept socket
	//-----------------------------------------------
	if ( ! post_accept_ex () )
		NULL ; // bad
}


//----------------------------------------------------------------------------------------------------------------------
//  DESTRUCTOR
//----------------------------------------------------------------------------------------------------------------------
cPool :: ~cPool ()
{
	if ( pSig )
		delete pSig;

	SocketInfoList.lock();

	for ( sSocketInfo * p = this->SocketInfoList.first(); p; p = SocketInfoList.next ( p ) )
	{
		shutdown	( p->Socket, SD_SEND );
		closesocket ( p->Socket );
	}

	SocketInfoList.unlock();

	if ( pLapPlusList )
		delete pLapPlusList;

	WSACleanup();
}


//----------------------------------------------------------------------------------------------------------------------
//  CREATE SOCKET
//  From SDK IOCP sample
//----------------------------------------------------------------------------------------------------------------------
SOCKET cPool :: create_socket ()
{
	int nRet = 0;
	int nZero = 0;
	SOCKET sSocket = INVALID_SOCKET;

	sSocket = WSASocket ( AF_INET, SOCK_STREAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED );

	if ( sSocket == INVALID_SOCKET ) 
	{
		LOG_SYSTEM_ERROR_TE ( L"WSASocket", WSAGetLastError());
		return(sSocket);
	}

    //
	// Disable send buffering on the socket.  Setting SO_SNDBUF
	// to 0 causes winsock to stop buffering sends and perform
	// sends directly from our buffers, thereby save one memory copy.
	//
    // However, this does prevent the socket from ever filling the
    // send pipeline. This can lead to packets being sent that are
    // not full (i.e. the overhead of the IP and TCP headers is 
    // great compared to the amount of data being carried).
    //
    // Disabling the send buffer has less serious repercussions 
    // than disabling the receive buffer.
	//
	nZero = 0;
	nRet = setsockopt( sSocket, SOL_SOCKET, SO_SNDBUF, (char *)&nZero, sizeof(nZero));

	if ( nRet == SOCKET_ERROR ) 
	{
		LOG_SYSTEM_ERROR_TE ( L"setsockopt(SNDBUF)", WSAGetLastError());
		return(sSocket);
	}

    //
    // Don't disable receive buffering. This will cause poor network
    // performance since if no receive is posted and no receive buffers,
    // the TCP stack will set the window size to zero and the peer will
    // no longer be allowed to send data.
    //

    // 
    // Do not set a linger value...especially don't set it to an abortive
    // close. If you set abortive close and there happens to be a bit of
    // data remaining to be transfered (or data that has not been 
    // acknowledged by the peer), the connection will be forcefully reset
    // and will lead to a loss of data (i.e. the peer won't get the last
    // bit of data). This is BAD. If you are worried about malicious
    // clients connecting and then not sending or receiving, the server
    // should maintain a timer on each connection. If after some point,
    // the server deems a connection is "stale" it can then set linger
    // to be abortive and close the connection.
    //

    /*
	LINGER lingerStruct;

	lingerStruct.l_onoff = 1;
	lingerStruct.l_linger = 0;
	nRet = setsockopt(sdSocket, SOL_SOCKET, SO_LINGER,
					  (char *)&lingerStruct, sizeof(lingerStruct));
	if( nRet == SOCKET_ERROR ) {
		myprintf("setsockopt(SO_LINGER) failed: %d\n", WSAGetLastError());
		return(sdSocket);
	}
    */

	return ( sSocket );
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
/**********************************************************************************************************************/