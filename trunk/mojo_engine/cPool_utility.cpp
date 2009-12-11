/***********************************************************************************************************************
/*
/*    cPool_utility.cpp / mojo_engine
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
using namespace mojo;
#include "cMonitor.h"
#include "cVersion.h"
#include "cMachlist.h"
#include <Mswsock.h>
#include "net_stuff.h"


//==============================================================================================
// PROTOTYPES
//==============================================================================================

//==============================================================================================
// DATA
//==============================================================================================

extern cVersion g_Version;
extern cPool g_Pool;

//==============================================================================================
// CODE
//==============================================================================================

//----------------------------------------------------------------------------------------------
// GET CONNECTION STATUS
// return values match mojo::nConnectionStatus::eConnectionStatus enum.
//----------------------------------------------------------------------------------------------
int cPool :: get_connection_status ( DWORD dwIP )
{
	if ( 0 == dwIP ) // ERROR OR LOCAL IP?
		return 0;

	int iRetVal = 0;

	if ( get_accept_socket_info_by_ip ( dwIP ) )
		iRetVal += 1;

	if ( get_connect_socket_info_by_ip ( dwIP ) )
		iRetVal += 2;

	return iRetVal;
}


//----------------------------------------------------------------------------------------------
// GET SOCKET INFO BY IP
//----------------------------------------------------------------------------------------------
sSocketInfo * cPool :: get_socket_info_by_ip ( DWORD dwIP )
{

	sSocketInfo * pRetVal = NULL;

	SocketInfoList.lock();

	for ( sSocketInfo * p = this->SocketInfoList.first(); p; p = SocketInfoList.next ( p ) )
	{
		if ( dwIP == p->dwRemoteIP )
		{
			pRetVal = p;
			break;
		}
	}

	SocketInfoList.unlock();

	return pRetVal;
}


//----------------------------------------------------------------------------------------------
// GET CONNECT SOCKET INFO BY ADDRESS
//----------------------------------------------------------------------------------------------
sSocketInfo * cPool :: get_socket_info_by_address ( const wchar_t * pAddr )
{
	sSocketInfo * pRetVal = NULL;

	SocketInfoList.lock();

	for ( sSocketInfo * p = this->SocketInfoList.first(); p; p = SocketInfoList.next ( p ) )
	{
		if ( 0 == wcscmp ( pAddr, p->awRemoteIP ) )
		{
			pRetVal = p;
			break;
		}

		else if ( 0 == _wcsicmp ( pAddr, p->sRemoteName.cstr() ) )
		{
			pRetVal = p;
			break;
		}
	}

	SocketInfoList.unlock();

	return pRetVal;
}


//----------------------------------------------------------------------------------------------
// SET NAGLE
//----------------------------------------------------------------------------------------------
void cPool :: set_nagle ( SOCKET socket, bool bOn )
{
	BOOL bOptVal = bOn ? FALSE : TRUE;

	if ( SOCKET_ERROR == setsockopt ( socket,  IPPROTO_TCP, TCP_NODELAY, (char*)&bOptVal, sizeof(bOptVal) ) )
		LOG_SYSTEM_ERROR_TE ( L"setsockopt for TCP_NODELAY", WSAGetLastError());
}


//----------------------------------------------------------------------------------------------
//  IS NAGLE ON
//----------------------------------------------------------------------------------------------
bool cPool :: is_nagle_on ( SOCKET socket )
{
	int iOptVal = 0;
	int iOptLen = sizeof(iOptVal);

	if ( SOCKET_ERROR == getsockopt ( socket,  IPPROTO_TCP, TCP_NODELAY, (char*)&iOptVal, &iOptLen ) )
		LOG_SYSTEM_ERROR_TE ( L"getsockopt for TCP_NODELAY", WSAGetLastError());

	return iOptVal ? false : true;
}


//----------------------------------------------------------------------------------------------
// GET SOCKET INFO BY SOCKET
//----------------------------------------------------------------------------------------------
sSocketInfo * cPool :: get_socket_info_by_socket ( SOCKET s )
{
	sSocketInfo * pRetVal = NULL;

	SocketInfoList.lock();

	for ( sSocketInfo * p = this->SocketInfoList.first(); p; p = SocketInfoList.next ( p ) )
	{
		if ( p->Socket == s )
		{
			pRetVal = p;
			break;
		}
	}

	SocketInfoList.unlock();

	return pRetVal;
}


//----------------------------------------------------------------------------------------------
// GET CONNECT SOCKET INFO BY IP
//----------------------------------------------------------------------------------------------
sSocketInfo * cPool :: get_connect_socket_info_by_ip ( DWORD dwIP )
{
	sSocketInfo * pRetVal = NULL;

	SocketInfoList.lock();

	for ( sSocketInfo * p = this->SocketInfoList.first(); p; p = SocketInfoList.next ( p ) )
	{
		if ( dwIP == p->dwRemoteIP )
			if ( sSocketInfo::connect == p->eType )
			{
				pRetVal = p;
				break;
			}
	}

	SocketInfoList.unlock();

	return pRetVal;
}


//----------------------------------------------------------------------------------------------
// GET ACCEPT SOCKET INFO BY IP
//----------------------------------------------------------------------------------------------
sSocketInfo * cPool :: get_accept_socket_info_by_ip ( DWORD dwIP )
{
	sSocketInfo * pRetVal = NULL;

	SocketInfoList.lock();

	for ( sSocketInfo * p = this->SocketInfoList.first(); p; p = SocketInfoList.next ( p ) )
	{
		if ( dwIP == p->dwRemoteIP )
			if ( sSocketInfo::accept == p->eType )
			{
				pRetVal = p;
				break;
			}
	}

	SocketInfoList.unlock();

	return pRetVal;
}


//----------------------------------------------------------------------------------------------
//  IS CONNECTED
//----------------------------------------------------------------------------------------------
bool cPool :: versions_match ( DWORD dwIP )
{
	sSocketInfo * p = get_accept_socket_info_by_ip (  dwIP );

	if ( ! p )
		return false;

	if ( p->RemoteVersion == g_Version )
		return true;

	else
		return false;
}


//----------------------------------------------------------------------------------------------
//  IS CONNECTED
//----------------------------------------------------------------------------------------------
int cPool :: is_connected ( DWORD dwIP )
{
	int iRetVal = 0;

	if ( get_accept_socket_info_by_ip ( dwIP ) )
		iRetVal ++;

	if ( get_connect_socket_info_by_ip ( dwIP ) )
		iRetVal ++;

	return iRetVal;
}


//---------------------------------------------------------------------------------------------
// SOCKET INFO :: PRINT
//---------------------------------------------------------------------------------------------
const wchar_t * sSocketInfo::get_text ( mojo::cStrW * pRet )
{
	wchar_t t [2000];
	wchar_t awRemoteIP [16];
	wchar_t awLocalIP [16];

	ip_dword_to_aw ( awRemoteIP, dwRemoteIP );
	ip_dword_to_aw ( awLocalIP, dwLocalIP );

	mojo::cStrW sRemoteVersion;
	RemoteVersion.get_text ( & sRemoteVersion );

	wsprintf ( t,	L"\tRemoteName:\t%s\r"
					L"\tSocket:\t\t%d\r"
					L"\tRemoteIP:\t%s\r"
					L"\tLocalIP:\t\t%s\r"
					L"\tRemoteVersion:\t%s\r",
					sRemoteName.cstr(),
					Socket,
					awRemoteIP,
					awLocalIP,
					sRemoteVersion.cstr() );
	*pRet += t;
	return pRet->cstr();

}


//---------------------------------------------------------------------------------------------
// SIG AS TEXT
//---------------------------------------------------------------------------------------------
#if 0
const wchar_t * cPool::sig_as_text ( cStrW * pRet, const wchar_t * pSig )
{
	// cStrW sName;
	const wchar_t * pName;
	::cVersion Version;

	if ( ! parse_sig ( NULL, &pName, &Version, pSig ) )
		return NULL;

	cStrW sVersion;
	Version.get_text ( &sVersion );
	(*pRet) = s_awAppTitle;
	(*pRet) += L" ";
	(*pRet) += sVersion;
	(*pRet) += L" ";
	(*pRet) += pName;

	return pRet->cstr();	
}
#endif

//---------------------------------------------------------------------------------------------
// SIGNATURE CONSTRUCTOR
//---------------------------------------------------------------------------------------------
#if 0
sSig :: sSig ()
{
	assert(g_Pool.local_name_len() );
	wcscpy_s  ( awAppTitle, sizeof(awAppTitle)/sizeof(wchar_t), s_awAppTitle );
	Version = g_Version;
	iDomainNameLen = g_Pool.local_name_len() + 1;
}
#endif

//---------------------------------------------------------------------------------------------
// MAKE SIG WITH EXTRA
//---------------------------------------------------------------------------------------------
#if 0
wchar_t * cPool :: make_sig_with_extra ( cStrW * pRet, const wchar_t * pExtra )
{
	// Qty in wchar_t's
	int iExtraQty = wcslen ( pExtra );
	int iSigQty = sig_len();

	int iTotalQty = iExtraQty + iSigQty + 2; // add 2 for two terminating NULLs

	wchar_t * pBuf = new wchar_t [ iTotalQty ];

	memcpy ( pBuf, sig(), 2 * iSigQty );

	pBuf [ iSigQty ] = 0;
	memcpy ( pBuf + iSigQty + 1, pExtra, 2 * iExtraQty );
	pBuf [ iTotalQty - 1 ] = 0;

	pRet->append ( pBuf, iTotalQty );

	delete[] pBuf;

	return pRet->cstr();

}
#endif	


//---------------------------------------------------------------------------------------------
// MAKE CONNECT SIG
//---------------------------------------------------------------------------------------------
#if 0
const wchar_t * cPool :: make_sig ( cStrW * pRet )
{
	sSig sig;
	pRet->erase();
	pRet->append ( (wchar_t*) &sig, sizeof(sig) / sizeof(wchar_t) );
	(*pRet) += cPool::local_name();

	return pRet->cstr();
}
#endif


//---------------------------------------------------------------------------------------------
// PARSE SIG
//---------------------------------------------------------------------------------------------
#if 0
bool cPool :: parse_sig ( const wchar_t ** ppRetExtra, const wchar_t ** ppRetName, ::cVersion * pRetVersion, const wchar_t * pMsg )
{
	sSig * pSig = ( sSig *) pMsg;

	if ( 0 != wcscmp ( s_awAppTitle, pSig->awAppTitle ) )
		return false;

	*pRetVersion	= pSig->Version;
	*ppRetName		= pMsg + sizeof(sSig)/sizeof(wchar_t);

	if ( ppRetExtra )
		(*ppRetExtra) = pMsg + sizeof(sSig)/sizeof(wchar_t) + pSig->iDomainNameLen;

	return true;
}
#endif

//---------------------------------------------------------------------------------------------
// PARSE CONNECT SIG
//---------------------------------------------------------------------------------------------
#if 0
bool cPool :: parse_sig ( cStrW *pRetExtra, cStrW * pRetName, ::cVersion * pRetVersion, const wchar_t * pMsg )
{
	sSig * pSig = ( sSig *) pMsg;

	if ( 0 != wcscmp ( s_awAppTitle, pSig->awAppTitle ) )
		return false;

#if 0
	if ( ! ( g_Version == pSig->Version ) )
		return false;
#endif

	(*pRetVersion)	= pSig->Version;
	(*pRetName)		= pMsg + sizeof(sSig)/sizeof(wchar_t);

	if ( pRetExtra )
		(*pRetExtra) = pMsg + sizeof(sSig)/sizeof(wchar_t) + pSig->iDomainNameLen;

	return true;
}
#endif


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
