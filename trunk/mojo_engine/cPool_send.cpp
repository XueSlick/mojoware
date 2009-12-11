/*************************************************************************************************
/*
/*   cPool_send.cpp
/*   
/*   started June 2, 2009
/*   
/*************************************************************************************************/

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
#include "cSettings.h"
#include "cException.h"


//=============================================================================================
// PROTOTYPES
//=============================================================================================

namespace j
{
	void  ip_dword_to_aw ( wchar_t aw[16], DWORD dw );
}


//==============================================================================================
// DATA
//==============================================================================================

//=============================================================================================
// CODE
//=============================================================================================

//----------------------------------------------------------------------------------------------
// SEND CALLBACK
//----------------------------------------------------------------------------------------------
void cPool::send_cb ( cLapPlus * pLapPlus )
{
	delete pLapPlus;
}


//----------------------------------------------------------------------------------------------
//  SEND
//----------------------------------------------------------------------------------------------
bool cPool :: send ( DWORD dwIP, const char * pMsg, unsigned uLen )
{
	sSocketInfo * pSI = get_connect_socket_info_by_ip ( dwIP ); // things are laggier with accept sockets

	if ( ! pSI )
	{
#if 0
		wchar_t awIP[16];
		ip_dword_to_aw ( awIP, dwIP );
		wchar_t t [1000];
		wsprintf ( t, L"Address not found: %s.", awIP );
		LOG ( t );
#endif
		return false;
	}

	cLapPlus * pLap = new cLapPlus;
	pLap->pSocketInfo = pSI;
	pLap->pPool = this;
	pLap->sock = pSI->Socket;
	pLap->pfCallback = send_cb;

	pLap->sBuf.erase();
	pLap->sBuf.append ( pMsg, uLen );

	//-------------------------------------
	//  WSA BUFFER
	//-------------------------------------
	const int iWSABUFQTY = 100;
	static WSABUF aWsaBuf[iWSABUFQTY];
	static int iWsaBufIndex = 0;

	int iIndexCopy = iWsaBufIndex;

	WSABUF * pWsaBuf = & aWsaBuf[iIndexCopy];
	iWsaBufIndex = ( iWsaBufIndex + 1 ) % iWSABUFQTY;
	pWsaBuf->buf = (char*) pLap->sBuf.cstr();
	pWsaBuf->len = pLap->sBuf.len();

	//-------------------------------------
	//  DO IT
	//-------------------------------------
	if ( SOCKET_ERROR == WSASend ( pLap->sock, pWsaBuf, 1, &pLap->dwQtyBytesXfered, 0, (OVERLAPPED*)pLap, NULL ) )
	{
		int iError = WSAGetLastError();

		if ( iError != WSA_IO_PENDING ) // this isn't really an error
			LOG_SYSTEM_ERROR_TE ( L"WSASend", iError );
	}

	return true;
}


//----------------------------------------------------------------------------------------------
//  SEND
//----------------------------------------------------------------------------------------------
#if 0
void cPool :: send ( const wchar_t * pAddr, DWORD dwPrefix, const char * pMsg, int iCount )
{
	sSocketInfo * pSI = get_socket_info_by_address ( pAddr );

	if ( ! pSI )
		throw ( cException ( L"Address not found.", pAddr, 0 ) );

	cLapPlus * pLap = new cLapPlus;
	pLap->pSocketInfo = pSI;
	pLap->pPool = this;
	pLap->sock = pSI->Socket;
	pLap->pfCallback = send_cb;
	// pLap->op = cLapPlus::

	char acPrefix[4];
	*((DWORD*)(acPrefix)) = dwPrefix;
	pLap->sBuf.erase();
	pLap->sBuf.append ( acPrefix, 4 );
	pLap->sBuf.append ( pMsg, iCount );

	//-------------------------------------
	//  WSA BUFFER
	//-------------------------------------
	const int iWSABUFQTY = 100;
	static WSABUF aWsaBuf[iWSABUFQTY];
	static int iWsaBufIndex = 0;

	int iIndexCopy = iWsaBufIndex;

	WSABUF * pWsaBuf = & aWsaBuf[iIndexCopy];
	iWsaBufIndex = ( iWsaBufIndex + 1 ) % iWSABUFQTY;
	pWsaBuf->buf = pLap->sBuf.cstr();
	pWsaBuf->len = pLap->sBuf.len() + 1;

	//-------------------------------------
	//  DO IT
	//-------------------------------------
	if ( SOCKET_ERROR == WSASend ( pLap->sock, pWsaBuf, 1, &pLap->dwQtyBytesXfered, 0, (OVERLAPPED*)pLap, NULL ) )
	{
		int iError = WSAGetLastError();

		if ( iError != WSA_IO_PENDING ) // this isn't really an error
			LOG_SYSTEM_ERROR_TE ( L"WSASend", iError );
	}
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