/***********************************************************************************************************************
/*
/*    cPool_worker_thread.cpp / mojo_engine
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
#include <assert.h>
#include "cPool.h"
#include "cLapPlus.h"
#include <Mswsock.h>


//==============================================================================================
// DATA
//==============================================================================================

//==============================================================================================
// PROTOTYPES
//==============================================================================================

//==============================================================================================
// CODE
//==============================================================================================

//----------------------------------------------------------------------------------------------
//  SERVER (WORKER) THREAD
//----------------------------------------------------------------------------------------------
unsigned _stdcall cPool :: worker_thread ( void * pArg )
{
	HANDLE hIoPort = reinterpret_cast<HANDLE>(pArg);

	DWORD dwQtyBytesXfered;
	ULONG_PTR Key;
	OVERLAPPED * pLap;

	for (;;)
	{
		BOOL iResult = GetQueuedCompletionStatus (
							hIoPort,
							&dwQtyBytesXfered,
							&Key,
							&pLap,
							INFINITE 
							);

		UNREFERENCED_PARAMETER ( iResult );

		cLapPlus * pLapPlus = reinterpret_cast<cLapPlus*>(pLap);
		pLapPlus->dwQtyBytesXfered = dwQtyBytesXfered;
		// pLapPlus->pSocketInfo = reinterpret_cast<sSocketInfo *> ( Key );

		if ( ! pLap )
			assert(0);
		
		if ( ! pLapPlus->pfCallback )
			assert(0);

		if ( 0 == iResult )
			pLapPlus->dwError = GetLastError();

		else
			pLapPlus->dwError = 0;

		pLapPlus->pfCallback ( pLapPlus );
	}

	// return 0;
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