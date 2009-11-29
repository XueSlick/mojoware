/*************************************************************************************************
/*
/*   cPool_worker_thread.cpp / mojo_engine
/*   
/*   started May 26, 2009
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

	for ( ; ; )
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
