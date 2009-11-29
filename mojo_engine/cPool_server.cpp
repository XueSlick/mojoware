/*************************************************************************************************
/*
/*   cPool_server.cpp / mojo_engine
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
#include "net_stuff.h"
#include "cMessenger.h"
#include "cMachlist.h"
#include <Mswsock.h>
#include "cMemo.h"


using namespace mojo;

#define TRACE_CONNECT


//==============================================================================================
// DATA
//==============================================================================================
static LPFN_ACCEPTEX s_pfAcceptEx = NULL;
extern cMachlist 	g_Machlist;
extern cMessenger 	g_Messenger;

//==============================================================================================
// PROTOTYPES
//==============================================================================================

namespace mojo
{
void put_ad_lib_memo ( mojo::cMemo::_eSeverity e, const wchar_t * pHeadFormatString, const wchar_t * pBodyFormatString=NULL, ... );
}

//==============================================================================================
// CODE
//==============================================================================================

//----------------------------------------------------------------------------------------------
// GET ACCEPTEX ADDRESSES
// Pass in accept ex info, get remote and local IP addresses
//----------------------------------------------------------------------------------------------
bool cPool::get_acceptex_addresses ( DWORD * pRetLocalIP, DWORD * pRetRemoteIP, char * pcBuf, int iBufSize, SOCKET Socket )
{
	LPFN_GETACCEPTEXSOCKADDRS pfGetAcceptExSockAddrs = NULL;
	GUID guid = WSAID_GETACCEPTEXSOCKADDRS;
	SOCKADDR * pL = NULL;
	SOCKADDR * pR = NULL;
	int iSizeR, iSizeL;
	// int LocalSockaddrLen = 0;
	// int RemoteSockaddrLen = 0;
	DWORD dwQty;

	*pRetLocalIP = *pRetRemoteIP = 0;

	int iResult = WSAIoctl (					
		Socket,                                  
		SIO_GET_EXTENSION_FUNCTION_POINTER,         
		&guid,         
		sizeof(guid),         
		&pfGetAcceptExSockAddrs,         
		sizeof(pfGetAcceptExSockAddrs),         
		&dwQty,         
		NULL,         
		NULL  ); 

	if ( SOCKET_ERROR == iResult )
	{
		DWORD dwError = WSAGetLastError ();
		LOG_SYSTEM_ERROR_TE ( L"WSAIoctl getting GetAcceptExSockAddrs", dwError );
		return false;
	}

	pfGetAcceptExSockAddrs ( 
			pcBuf,
			iBufSize - ((sizeof(SOCKADDR_IN) + 16) * 2),
			sizeof(SOCKADDR_IN) + 16,
			sizeof(SOCKADDR_IN) + 16,
			&pL,
			&iSizeL,
			&pR,
			&iSizeR  );

	// GET LOCAL INFO

	sockaddr_in * pSAL = (sockaddr_in*)pL;   // SAL = socket address local
	*pRetLocalIP = pSAL->sin_addr.S_un.S_addr;
	// char * pLocal = inet_ntoa ( *(in_addr*)pRetLocalIP );

	sockaddr_in * pSAR = (sockaddr_in*)pR;   // SAR = socket address remote
	*pRetRemoteIP = pSAR->sin_addr.S_un.S_addr;
	// char * pRemote = inet_ntoa ( *(in_addr*)pRetRemoteIP );

	return true;
}


//----------------------------------------------------------------------------------------------
// ACCEPT EX CALLBACK
//----------------------------------------------------------------------------------------------

// #pragma warning ( push )
// #pragma warning ( disable : 4239 ) // nonstandard extension used : 'argument' : conversion from 'sMon' to 'sMon &'

void cPool::acceptex_cb ( cLapPlus * pLapPlus )
{
	cPool * pThis = pLapPlus->pPool;

	wchar_t * pMsg = (wchar_t *) pLapPlus->sBuf.cstr();

	//------------------------------------------------
	// Accept socket is in default state for connected
	// socket.  Make it inherit properties of the
	// listen socket.
	//------------------------------------------------
	int iResult  = setsockopt (
							 pLapPlus->sock, 
							 SOL_SOCKET,
							 SO_UPDATE_ACCEPT_CONTEXT,
							 (char*)&(pLapPlus->pPool->sListenSocket),
							 sizeof(pLapPlus->pPool->sListenSocket)
							 );
	if ( SOCKET_ERROR == iResult )
		LOG_SYSTEM_ERROR_TE ( L"setsockopt", WSAGetLastError() );

	//------------------------------------------------
	// Request notification on close.
	//------------------------------------------------

	// TODO 

	//	iResult = WSAAsyncSelect( pLapPlus->sock, g_hwnd, uWM_SOCKET_HAS_CLOSED, FD_CLOSE );
	iResult = WSAAsyncSelect( pLapPlus->sock, g_Messenger.hwnd, cMessenger::uWM_SOCKET_HAS_CLOSED, FD_CLOSE );

	if ( 0 != iResult )
		LOG_SYSTEM_ERROR_TE ( L"WSAAsyncSelect", WSAGetLastError() );

	//------------------------------------------------
	// Get IP addresses.
	//------------------------------------------------
	DWORD dwRemoteIP, dwLocalIP;
	bool bResult = get_acceptex_addresses ( &dwLocalIP, &dwRemoteIP, (char*) pLapPlus->sBuf.cstr(), pLapPlus->sBuf.bufsize(), pLapPlus->sock );
	UNREFERENCED_PARAMETER ( bResult );

	// char * pLocal = inet_ntoa ( *(in_addr*)&dwLocalIP );
	// char * pRemote = inet_ntoa ( *(in_addr*)&dwRemoteIP );

	//------------------------------------------------
	// Get remote HotkeyNet info
	//------------------------------------------------
	cVersion * pRemoteVersion;
	// cStrW sRemoteName; 
	const wchar_t * pRemoteName;
	const wchar_t * pDisplayList;
	const wchar_t * pRemoteAppTitle;

	if ( ! cSignature::parse ( &pDisplayList, &pRemoteName, &pRemoteAppTitle, &pRemoteVersion, (const char*) pMsg ) )

	// if ( ! pLapPlus->pPool->parse_sig ( NULL, &pRemoteName, &RemoteVersion, pMsg ) )
	{
		LOG_SYSTEM_ERROR_TE ( L"Bad connect string received by acceptex_cb.", 0 );
		LOG_SYSTEM_ERROR_TE ( pMsg, 0 );
		return;
	}

	//------------------------------------------------
	// Create socket info and add it to list.
	//------------------------------------------------
	sSocketInfo * pSI	= new sSocketInfo;
	pSI->dwLocalIP		= dwLocalIP;
	pSI->dwRemoteIP		= dwRemoteIP;
	ip_dword_to_aw	( pSI->awRemoteIP, dwRemoteIP );
	pSI->Socket			= pLapPlus->sock;
	pSI->sRemoteName	= pRemoteName;
	pSI->RemoteVersion  = *pRemoteVersion;
	pSI->eType			= sSocketInfo::accept;
	pSI->pMach			= g_Machlist.get_by_ip_or_add ( dwRemoteIP, pDisplayList );
	pSI->dwMachHandle   = pSI->pMach->dwSerialNumber;
	pLapPlus->pPool->SocketInfoList.append ( pSI );

	mojo::put_ad_lib_memo ( mojo::cMemo::success, L"Connection open (type A)",	L"Remote name: %s\n"
																		  		L"Remote IP: %s",
																		  		pSI->sRemoteName.cstr(),
																		  		pSI->awRemoteIP );

	//------------------------------------------------
	// SET MACH DATA
	//------------------------------------------------
	pSI->pMach->lock();
	pSI->pMach->sName = pRemoteName;
	pSI->pMach->sDottedDec = pSI->awRemoteIP;
	pSI->pMach->unlock();

	//------------------------------------------------
	// SET LOCAL MACHINE'S IP ADDRESS
	//------------------------------------------------
	if ( cMach * pLocal = g_Machlist.local_machine () )
	{
		if ( pLocal->dwIP != dwLocalIP )
		{
			pLocal->dwIP = dwLocalIP;
			wchar_t t[16];
			ip_dword_to_aw ( t, dwLocalIP );
			pLocal->sDottedDec = t;
		}
	}

	// bool b1 = is_nagle_on ( pLapPlus->sock );

	//------------------------------------------------
	//  Update display
	//------------------------------------------------
	cMessenger::tell_app_that_connections_changed ();

	// delete pLapPlus;

	pThis->post_initial_receive ( pSI );

	pThis->post_accept_ex ( pLapPlus );
}

// #pragma warning ( pop )

//----------------------------------------------------------------------------------------------
// GET ACCEPT EX POINTER
//----------------------------------------------------------------------------------------------
bool cPool::get_accept_ex_pointer ( SOCKET s )
{
	GUID guidAcceptEx = WSAID_ACCEPTEX;
	DWORD dwQtyBytesReturned;

	int iResult = WSAIoctl (
				s, 
				SIO_GET_EXTENSION_FUNCTION_POINTER, 
				&guidAcceptEx, 
				sizeof(guidAcceptEx),
				&s_pfAcceptEx, 
				sizeof ( s_pfAcceptEx), 
				&dwQtyBytesReturned,
				NULL, 
				NULL );

	if ( 0 != iResult )
	{
		LOG_SYSTEM_ERROR_TE ( L"WSAIoctl while getting AcceptEx", iResult );
		return false;
	}

	else
		return true;
}


//----------------------------------------------------------------------------------------------
//  POST ACCEPT EX
//----------------------------------------------------------------------------------------------
bool cPool :: post_accept_ex ( cLapPlus * pLapPlus ) 
{
	//-----------------------------------------
	// CREATE ACCEPT SOCKET
	//-----------------------------------------
	SOCKET sAcceptSocket = create_socket ();
	if ( INVALID_SOCKET == sAcceptSocket )
		return false;

	//-----------------------------------------
	// CREATE OR RECYCLE OVERLAPPED
	//-----------------------------------------
	if ( NULL == pLapPlus )
		pLapPlus = new cLapPlus;

	else
		pLapPlus->init();

	// pLapPlus->op			= nPool::accept;
	pLapPlus->sock			= sAcceptSocket;
	pLapPlus->pfCallback	= acceptex_cb;
	pLapPlus->pPool			= this;

	//-----------------------------------------
	// CALL ACCEPT EX
	//-----------------------------------------
	assert ( pLapPlus->sBuf.bufsize() == this->iACCEPT_BUFFER_SIZE );

	// char * pBuffer = pLapPlus->sBuf.buffer();
	// int iBufSize = pLapPlus->sBuf.bufsize();

	BOOL iResult =	s_pfAcceptEx (	
							sListenSocket, 
							sAcceptSocket,
							pLapPlus->sBuf.buffer(), 
							pLapPlus->sBuf.bufsize() - ((sizeof(sockaddr_in) + 16) * 2),
							sizeof(sockaddr_in) + 16, 
							sizeof(sockaddr_in) + 16, 
							&pLapPlus->dwQtyBytesXfered, 
							reinterpret_cast<WSAOVERLAPPED*>(pLapPlus) );

	if ( FALSE == iResult )
	{
		DWORD dwError = WSAGetLastError();

		if ( ERROR_IO_PENDING == dwError ) // 997
			; // not an error

		else if ( WSAECONNRESET == dwError )
			; // not an error; connection was terminated by remote

		else
		{
			LOG_SYSTEM_ERROR_TE ( L"AcceptEx", dwError );
			return false;
		}
	}

	//-----------------------------------------
	// ASSOCIATE ACCEPT SOCKET WITH IO PORT
	//-----------------------------------------
	HANDLE hResult = CreateIoCompletionPort ( (HANDLE) sAcceptSocket, hIoPort, (u_long)0, 0);
	if ( hResult != hIoPort )
	{
		LOG_SYSTEM_ERROR_TE ( L"CreateIoCompletionPort for accept socket", GetLastError() );
		return false;
	}

	return true;
}