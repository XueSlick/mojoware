/*************************************************************************************************
/*
/*   cPool_client.cpp / mojo_engine
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
#include "cMonitor.h"
#include "cVersion.h"
#include "cMachlist.h"
#include <Mswsock.h>
#include "cFinder.h"
#include "net_stuff.h"
#include "cMessenger.h"
#include "cMemo.h"
#include "cLog.h"


using namespace mojo;

#include "cSettings.h"

//======================================================================================================================
// DEFINES
//======================================================================================================================
#define TRACE_CONNECT

//======================================================================================================================
// DATA
//======================================================================================================================

extern cLog g_Log;

//======================================================================================================================
// PROTOTYPES
//======================================================================================================================

#if 0
namespace j
{
	DWORD ip_pw_to_dword ( const wchar_t *pAW );
	void  ip_dword_to_aw ( wchar_t aw[16], DWORD dw );
}
#endif


void ip_dword_to_aw ( wchar_t aw[16], DWORD dw );
namespace mojo
{
void put_ad_lib_memo		( mojo::cMemo::_eSeverity e, const wchar_t * pHeadFormatString, const wchar_t * pBodyFormatString=NULL, ... );
}

//=============================================================================================
// DATA
//=============================================================================================

extern cVersion    g_Version;
extern cSettings   g_Settings;
extern cMachlist   g_Machlist;
extern cMessenger  g_Messenger;



//=============================================================================================
// CODE
//=============================================================================================

//----------------------------------------------------------------------------------------------
// CONNECT EX CALLBACK
//----------------------------------------------------------------------------------------------

#pragma warning ( push )
#pragma warning ( disable : 4239 ) //  nonstandard extension used : 'argument' : conversion from 'sMon' to 'sMon &'

void cPool::connectex_cb ( cLapPlus * pLapPlus )
{
	//------------------------------------------------
	//  DID ERROR OCCUR?
	//------------------------------------------------

	if ( pLapPlus->dwError )
	{
		LOG_SYSTEM_ERROR_TE ( L"ConnectEx", pLapPlus->dwError );
		delete pLapPlus;
		return;
	}

	// wchar_t * pMsg = (wchar_t *) pLapPlus->sBuf.cstr();

	// cMonitor::print ( L"We're in connectex_cb." );

	//------------------------------------------------
	// Socket is in default state for connected
	// socket, so change that. 
	//------------------------------------------------
	int iResult = setsockopt (
						pLapPlus->sock, 
						SOL_SOCKET, 
						SO_UPDATE_CONNECT_CONTEXT, 
						NULL, 
						0 );
	if ( SOCKET_ERROR == iResult )
		LOG_SYSTEM_ERROR_TE ( L"setsockopt", WSAGetLastError() );

	//------------------------------------------------
	// Request notification on close.
	//------------------------------------------------

	// TODO 
	// g_hwndApp in next line should be DLL's hwnd not the app's
	// iResult = WSAAsyncSelect( pLapPlus->sock, g_hwnd, uWM_SOCKET_HAS_CLOSED, FD_CLOSE );
	iResult = WSAAsyncSelect( pLapPlus->sock, g_Messenger.hwnd, cMessenger::uWM_SOCKET_HAS_CLOSED, FD_CLOSE );
	if ( 0 != iResult )
		LOG_SYSTEM_ERROR_TE ( L"WSAAsyncSelect", WSAGetLastError() );

	//------------------------------------------------
	// Get IP addresses.
	//------------------------------------------------
	sockaddr_in saLocal, saRemote;

	int iLocalSize = sizeof(saLocal);
	int iRemoteSize = sizeof(saRemote);
	if ( 0 != getsockname ( pLapPlus->sock, (SOCKADDR*)&saLocal, &iLocalSize ) )
	{
		LOG_SYSTEM_ERROR_TE ( L"getsockname", WSAGetLastError() );
	}

	if ( 0 != getpeername ( pLapPlus->sock, (SOCKADDR*)&saRemote, &iRemoteSize ) )
	{
		LOG_SYSTEM_ERROR_TE ( L"getpeername", WSAGetLastError() );
	}

	DWORD dwLocalIP		= *(DWORD*)&saLocal .sin_addr;
	DWORD dwRemoteIP	= *(DWORD*)&saRemote.sin_addr;
	
	// char * pLocal  = inet_ntoa  ( *(in_addr*)&dwLocalIP  );
	// char * pRemote = inet_ntoa ( *(in_addr*)&dwRemoteIP );

	//------------------------------------------------
	// Get remote HotkeyNet info
	//------------------------------------------------


	//------------------------------------------------
	// Create socket info and add it to list.
	//------------------------------------------------
	sSocketInfo * pSI	= new sSocketInfo;
	pSI->dwLocalIP		= dwLocalIP;
	pSI->dwRemoteIP		= dwRemoteIP;
	ip_dword_to_aw	( pSI->awRemoteIP, dwRemoteIP );
	pSI->Socket			= pLapPlus->sock;
	pSI->eType			= sSocketInfo::connect;
	pSI->pMach			= g_Machlist.get_by_ip_or_add ( dwRemoteIP );
	pSI->dwMachHandle   = pSI->pMach->dwSerialNumber;
	pLapPlus->pPool->SocketInfoList.append ( pSI );

	is_nagle_on ( pLapPlus->sock );

	mojo::put_ad_lib_memo ( mojo::cMemo::success, L"Connection open (type C)",	L"Remote IP: %s",
																		  		pSI->awRemoteIP );

	//------------------------------------------------
	//  Update display
	//------------------------------------------------

	cMessenger::tell_app_that_machlist_changed ();

	pLapPlus->pPool->post_initial_receive ( pSI );

	delete pLapPlus;
}

#pragma warning ( pop )


//----------------------------------------------------------------------------------------------
//  CONNECT TO
//----------------------------------------------------------------------------------------------
bool cPool :: connect_to ( DWORD dwIP )
{
	wchar_t awIP [16];
	ip_dword_to_aw ( awIP, dwIP );
	return connect_to ( awIP );
}


//----------------------------------------------------------------------------------------------
//  CONNECT TO
//  assumes pAddr is IP address
//----------------------------------------------------------------------------------------------

#pragma warning ( push )
#pragma warning ( disable : 4239 ) // nonstandard extension used : 'argument' : conversion from 'sMon' to 'sMon &'

bool cPool :: connect_to ( const wchar_t * pAddr )
{

	if ( ! g_Settings.bConnect )
		return false;

#ifdef TRACE_CONNECT
	// g_Monitor.rcv ( sMon ( L"Requesting connection.", 0, black, bold ), sMon ( pAddr, 1, purple, bold ) );
#endif

	bool bRetVal = false;
	DWORD dwLocalIP = 0;

	//--------------------------------------------
	// GET_LOCAL_ADDRESS
	//--------------------------------------------
	if ( ! cFinder::get_local_ip ( &dwLocalIP ) )
		return false;

	if ( ! cFinder::is_local_ip ( dwLocalIP ) )
	{
		wchar_t awIP[16], t[1000];
		ip_dword_to_aw ( awIP, dwLocalIP );
		wsprintf ( t, L"%s is not assigned to this computer.", awIP );
#if 0
		g_Monitor.rcv ( sMon ( L"Invalid IP address.", 0, red, bold ),
						sMon ( t, 1, red, regular ),
						sMon ( L"Change IP address on Connection Settings Panel.", 1, red ) );
#endif

		LOG ( L"is_local_ip failed in cConnector::start_threads." );
		LOG ( t );
	}

	//--------------------------------------------
	// INITIALIZE LOCAL ADDRESS
	//--------------------------------------------
	sockaddr_in saiLocalAddr;
	saiLocalAddr.sin_family = AF_INET;
	saiLocalAddr.sin_addr.s_addr = dwLocalIP; // htonl (INADDR_ANY);
	saiLocalAddr.sin_port = htons( 0 );

	//--------------------------------------------
	// INITIALIZE REMOTE ADDRESS
	//--------------------------------------------
    sockaddr_in saiRemoteAddr;
    saiRemoteAddr.sin_family = AF_INET;
	saiRemoteAddr.sin_addr.s_addr = ip_pw_to_dword ( pAddr );
    saiRemoteAddr.sin_port = htons( static_cast<u_short>(g_Settings.uPort) );

	//--------------------------------------------
	// MAKE SOCKET
	//--------------------------------------------
	SOCKET Socket = WSASocket( AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED );
	if ( Socket == INVALID_SOCKET ) 
	{
		LOG_SYSTEM_ERROR_TE ( L"WSASocket", WSAGetLastError() );
		return false;
    }

	//--------------------------------------------
	// ASSOCIATE SOCKET WITH IO PORT
	//--------------------------------------------
	HANDLE hResult = CreateIoCompletionPort ( (HANDLE) Socket, hIoPort, (u_long)0, 0);
	if ( hResult != hIoPort )
	{
		LOG_SYSTEM_ERROR_TE ( L"CreateIoCompletionPort for accept socket", GetLastError() );
		closesocket ( Socket );
		return false;
	}

	//--------------------------------------------
	// BIND
	// I wrote a comment in HKN1 that this is
	// required for ConnectEx, but I can't find
	// such a requirement in MSDN docs.
	//--------------------------------------------
	int iBindResult = bind ( Socket, (SOCKADDR*)&saiLocalAddr, sizeof ( sockaddr_in) );
	if ( SOCKET_ERROR == iBindResult )
	{
		LOG_SYSTEM_ERROR_TE ( L"bind", WSAGetLastError() );
		closesocket ( Socket );
		return false;
	}

	//--------------------------------------------
	// TURN NAGLE OFF
	//--------------------------------------------
	if ( g_Settings.bDisableNagle )
		if ( g_Settings.bDisableNagle )set_nagle ( Socket, false );

	//--------------------------------------------
	// GET CONNECT EX POINTER
	//--------------------------------------------
	LPFN_CONNECTEX pfConnectEx = reinterpret_cast<LPFN_CONNECTEX> ( get_connect_ex_pointer ( Socket ) );
	if ( ! pfConnectEx )
	{
		closesocket ( Socket );
		LOG ( L"Failed to get pointer to ConnectEx.  (This may not be implemented in Wine.)" );
		put_ad_lib_memo ( cMemo::error, L"System function call failed.", L"Unable to get pointer to ConnectEx.\n" L"This may not be implemented in Wine." );
		return false;
	}

	//--------------------------------------------
	// MAKE LAP PLUS
	//--------------------------------------------
	cLapPlus * pLapPlus		= new cLapPlus;
	pLapPlus->pfCallback	= connectex_cb;
	pLapPlus->sock			= Socket;
	// pLapPlus->op			= nPool::connect;
	pLapPlus->pPool			= this;

	//--------------------------------------------
	// DISPLAY STATUS INFO
	//--------------------------------------------
#if 0
	{
		wchar_t t [1000];
		wsprintf ( t, L"Client is about to call ConnectEx on %s", pAddr );
		cMonitor::print ( t );
		cMonitor::print ( L"");
	}
#endif

	//-----------------------------------------
	// call ConnectEx
	//-----------------------------------------
	DWORD dwQtyBytesSent;
		
	BOOL iResult = pfConnectEx (
						Socket,
						(SOCKADDR*)&saiRemoteAddr,
						sizeof(saiRemoteAddr),
						(void*)pSig->buf(), // (void *)cPool::sig(),
						pSig->len(), // (cPool::sig_len()+1) * 2,
						&dwQtyBytesSent,
						reinterpret_cast<WSAOVERLAPPED*>(pLapPlus) );

	//-----------------------------------------
	// Check return value; either try next
	// address or break because we succeeded.
	//-----------------------------------------
	if ( iResult != SOCKET_ERROR )
		bRetVal = true;

	else 
	{
		DWORD dwError = WSAGetLastError();

		if ( ERROR_IO_PENDING == dwError )
			bRetVal = true;
			
		else
		{
			LOG_SYSTEM_ERROR_TE ( L"ConnectEx", dwError );
			closesocket ( Socket);
			bRetVal = false;
		}
	}

	//-----------------------------------------
	// Display action on monitor.
	//-----------------------------------------
#if 0
	cStrW sTextSig;
	sig_as_text ( &sTextSig, sig() );

	if ( bRetVal )
		g_Monitor.rcv ( sMon ( L"We just called ConnectEx successfully", 0, black ), sMon ( pAddr, 1, blue ), sMon ( sTextSig.cstr(), 1, blue ) );

	else
		g_Monitor.rcv ( sMon ( L"We just called ConnectEx unsuccessfully", 0, black ), sMon ( pAddr, 1, red ), sMon ( sTextSig.cstr(), 1, red ) );
#endif

	return bRetVal;
}

#pragma warning ( pop )


//----------------------------------------------------------------------------------------------
// GET CONNECT EX POINTER
// returns LPFN_CONNECTEX but it's cast to void* so LPFN_CONNECTEX doesn't have to 
// be defined in cPool.h
//----------------------------------------------------------------------------------------------
void * cPool::get_connect_ex_pointer ( SOCKET s )
{
	GUID guidConnectEx = WSAID_CONNECTEX;
	DWORD dwQtyBytesReturned;
	LPFN_CONNECTEX pfRetVal;

	int iResult = WSAIoctl (
				s, 
				SIO_GET_EXTENSION_FUNCTION_POINTER, 
				&guidConnectEx, 
				sizeof(guidConnectEx),
				&pfRetVal, 
				sizeof ( pfRetVal), 
				&dwQtyBytesReturned,
				NULL, 
				NULL );

	if ( 0 != iResult )
	{
		LOG_SYSTEM_ERROR_TE ( L"WSAIoctl while getting ConnectEx", iResult );
		return NULL;
	}

	else
		return pfRetVal;
}


//----------------------------------------------------------------------------------------------
//  CONNECT TO
//----------------------------------------------------------------------------------------------
#if 0
bool cPool :: connect_to ( const wchar_t * pAddr )
{
	bool bRetVal = false;

	addrinfoW * paiResult	= NULL;
	addrinfoW  aiHints;

	ZeroMemory ( &aiHints, sizeof(aiHints) );

	aiHints.ai_family	= AF_UNSPEC;
	aiHints.ai_socktype = SOCK_STREAM;
	aiHints.ai_protocol = IPPROTO_TCP;

	//--------------------------------------------
	// get address(es)
	//--------------------------------------------
	int iResult = GetAddrInfo ( pAddr, awPort, & aiHints, & paiResult);

	if ( iResult != 0 ) 
	{
		LOG_SYSTEM_ERROR_TE ( L"GetAddrInfo", iResult );
		return false;
	}

	SOCKET sSocket = INVALID_SOCKET;

	//--------------------------------------------
	// loop through addresses till one connects
	//--------------------------------------------
    for ( addrinfoW * pai = paiResult; pai != NULL ; pai = pai->ai_next)
	{
		//--------------------------------------------
		// make socket
		//--------------------------------------------
		sSocket = WSASocket ( pai->ai_family, pai->ai_socktype, pai->ai_protocol, NULL, 0, WSA_FLAG_OVERLAPPED );
		if ( sSocket == INVALID_SOCKET) 
		{
			LOG_SYSTEM_ERROR_TE ( L"WSASocket", WSAGetLastError() );
			FreeAddrInfoW ( paiResult );
			return false;
		}

		//--------------------------------------------
		// ASSOCIATE SOCKET WITH IO PORT
		//--------------------------------------------
		HANDLE hResult = CreateIoCompletionPort ( (HANDLE) sSocket, hIoPort, (u_long)0, 0);
		if ( hResult != hIoPort )
		{
			LOG_SYSTEM_ERROR_TE ( L"CreateIoCompletionPort for accept socket", GetLastError() );
			return false;
		}

		//--------------------------------------------
		// BIND SOCKET
		//--------------------------------------------
#if 0
		int iBindResult = bind ( sSocket, pai->ai_addr, pai->ai_addrlen);
		if ( SOCKET_ERROR == iBindResult )
		{
			LOG_SYSTEM_ERROR_TE ( L"bind", WSAGetLastError() );
			continue;
		}
#endif

		//--------------------------------------------
		// DISPLAY STATUS INFO
		//--------------------------------------------
		{
			wchar_t t [1000];
			sockaddr_in * pSAI = reinterpret_cast<sockaddr_in*>(pai->ai_addr);
			char * pDotDec = inet_ntoa ( pSAI->sin_addr );
			wsprintf ( t, L"Client is about to call ConnectEx on %S", pDotDec );
			cMonitor::print ( t );
			cMonitor::print ( L"");
		}

		//--------------------------------------------
		// GET CONNECT EX POINTER
		//--------------------------------------------
		LPFN_CONNECTEX pfConnectEx = reinterpret_cast<LPFN_CONNECTEX> ( get_connect_ex_pointer ( sSocket ) );
		if ( ! pfConnectEx )
			return false;

		//-----------------------------------------
		// Create overlapped
		//-----------------------------------------
		cLapPlus * pNewLap		= new cLapPlus;
		pNewLap->op				= nPool::connect;
		pNewLap->sock			= sSocket;
		pNewLap->pfCallback		= connectex_cb;
		pNewLap->pPool			= this;

		//-----------------------------------------
		// call ConnectEx
		//-----------------------------------------
		int iLen = pai->ai_addrlen;
		DWORD dwQtyBytesSent;
		
		BOOL iResult = pfConnectEx (
						sSocket,
						pai->ai_addr,
						(int)pai->ai_addrlen,
						(void *)cPool::sig(),
						cPool::sig_len() * 2,
						&dwQtyBytesSent,
						reinterpret_cast<WSAOVERLAPPED*>(pNewLap) );


		// iResult = connect ( sSocket, pai->ai_addr, (int)pai->ai_addrlen );

		//-----------------------------------------
		// Check return value; either try next
		// address or break because we succeeded.
		//-----------------------------------------
		if ( iResult != SOCKET_ERROR )
		{
			bRetVal = true;
			// break;
		}

		else 
		{
			DWORD dwError = WSAGetLastError();

			if ( ERROR_IO_PENDING == dwError )
			{
				bRetVal = true;
				// break; // this is actually success
			}
			
			else
			{
				LOG_SYSTEM_ERROR_TE ( L"ConnectEx", dwError );
				closesocket ( sSocket);
				continue;
			}		
		}
	}

	//-----------------------------------------
	// Clean up
	// address or break because we succeeded.
	//-----------------------------------------
	FreeAddrInfoW ( paiResult );


	//-----------------------------------------
	// Display action on monitor.
	//-----------------------------------------
	cStrW sTextSig;
	sig_as_text ( &sTextSig, sig() );

	if ( bRetVal )
		g_Monitor.rcv ( sMon ( L"We just called ConnectEx successfully", 0, black ), sMon ( pAddr, 1, blue ), sMon ( sTextSig.cstr(), 1, blue ) );

	else
		g_Monitor.rcv ( sMon ( L"We just called ConnectEx unsuccessfully", 0, black ), sMon ( pAddr, 1, red ), sMon ( sTextSig.cstr(), 1, red ) );

	return bRetVal;
}
#endif