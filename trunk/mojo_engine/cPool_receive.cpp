/*************************************************************************************************
/*
/*   cPool_receive.cpp / mojo_engine
/*   
/*   started June 8, 2009
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
#include "cSettings.h"
#include "cException.h"
#include "tCircBuf.h"
#include "cMessenger.h"

//=============================================================================================
// PROTOTYPES
//=============================================================================================

//==============================================================================================
// DATA
//==============================================================================================
extern cMessenger g_Messenger;

//=============================================================================================
// CODE
//=============================================================================================

//----------------------------------------------------------------------------------------------
// RECEIVE CALLBACK
//----------------------------------------------------------------------------------------------
void cPool::receive_cb ( cLapPlus * pLap )
{
	if ( 1 <= pLap->dwQtyBytesXfered )
	{
		g_Messenger.receive ( pLap->pSocketInfo, pLap->sBuf.buffer(), pLap->dwQtyBytesXfered );
	}

	pLap->pPool->receive ( pLap );
}


//----------------------------------------------------------------------------------------------
//  SEND
//----------------------------------------------------------------------------------------------
void cPool :: receive ( cLapPlus * pLap )
{
	assert ( pLap );
	assert ( 8092 == pLap->sBuf.bufsize() ); 

	WSABUF wsabuf;
	wsabuf.buf = pLap->sBuf.buffer();
	wsabuf.len = pLap->sBuf.bufsize();

	DWORD dwFlags = 0;

	assert ( pLap->pSocketInfo );

	DWORD retval = WSARecv ( pLap->sock, &wsabuf, 1, &(pLap->dwQtyBytesXfered), &dwFlags, (OVERLAPPED*)pLap, NULL );

	UNREFERENCED_PARAMETER ( retval );
}


//----------------------------------------------------------------------------------------------
//  POST INITIAL RECEIVE
//----------------------------------------------------------------------------------------------
void cPool::post_initial_receive ( sSocketInfo * pSI )
{
	assert ( pSI );

	cLapPlus * pLap = new cLapPlus;
	pLap->pfCallback = receive_cb;
	pLap->sock = pSI->Socket;
	pLap->pPool = this;
	pLap->pSocketInfo = pSI;

	receive ( pLap );
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