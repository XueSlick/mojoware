/***********************************************************************************************************************
/*
/*    cFinder_receive.cpp / mojo_engine
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"
#include "cFinder.h"
#include <process.h>
#include "cPool.h"

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
//  RECEIVE
//----------------------------------------------------------------------------------------------------------------------
bool cFinder :: receive ()
{
	SOCKET ReceiveSocket;
	make_receive_socket ( &ReceiveSocket );

	for ( ;; )
	{
		if ( ! g_Settings.bConnect )
		{
			Sleep ( 5000 );
			continue;
		}

    	char acBuf [ 2000 ];
    	SOCKADDR_IN		sinRemote;
		int iRemoteSize = sizeof ( sinRemote );

		//--------------------------------------------------
		//  RECEIVE DATAGRAM (THREAD WAITS HERE)
		//--------------------------------------------------
		int iResult = recvfrom ( ReceiveSocket, acBuf, sizeof(acBuf)-1, 0, (SOCKADDR *) &sinRemote, &iRemoteSize );

        if ( iResult == SOCKET_ERROR )
		{
            LOG_SYSTEM_ERROR_TE ( L"recvfrom", WSAGetLastError() );
			continue;
		}

		//--------------------------------------------------
		// HAS "ONE PC" BEEN SELECTED SINCE WE STARTED
		// LISTENING?
		//--------------------------------------------------

		if ( ! g_Settings.bConnect ) // added dec 8 2009 without testing
		{
			continue;
		}

		//--------------------------------------------------
		// WAS SOMETHING VALID RECEIVED?
		//--------------------------------------------------
        else if ( 0 < iResult )
        {
			const wchar_t * pRemoteName = 0, * pDisplayList = 0, * pRemoteAppTitle;
			cVersion * pRemoteVersion;
			acBuf [ iResult ] = '\0';
			DWORD dwRemoteIP = *(DWORD*) &sinRemote.sin_addr;

			//--------------------------------------------------
			//  PARSE AND CHECK SIGNATURE
			//--------------------------------------------------

			if ( ! cSignature::parse ( &pDisplayList, &pRemoteName, &pRemoteAppTitle, &pRemoteVersion, acBuf ) )
			{
				LOG ( L"Bad signature (non-matching engine name) received by cFinder." );
				continue;
			}

			//--------------------------------------------------
			//  CHECK FOR CORRECT VERSION
			//--------------------------------------------------

			else if ( *pRemoteVersion != g_Version )
			{
				cStrW sIP;
				mojo::ip_dword_to_cStrW ( &sIP, dwRemoteIP );
				mojo::put_ad_lib_memo ( cMemo::error, L"Cannot connect", L"Cannot connect to %s because it's running a different version of %s.", sIP.cstr(), g_sAppTitle.cstr() );
				continue;
			} 

			//--------------------------------------------------
			//  IS IT US BASED ON IP?
			//--------------------------------------------------

			else if ( dwRemoteIP == m_dwLocalIP )
				continue;

			//--------------------------------------------------
			//  IS IT US BASED ON NAME?
			//--------------------------------------------------

			if ( 0 == wcscmp ( pRemoteName, sLocalName.cstr() ) )
				continue;

			//--------------------------------------------------
			// IS IT A KNOWN REMOTE?
			//--------------------------------------------------

			else if ( cMach * p = g_Machlist.get_by_ip ( dwRemoteIP ) )
			{
				//-----------------------------------
				// SET NAME IF IT WASN'T KNOWN
				//-----------------------------------

				if ( 0 == p->sName.len() )
				{
					p->lock();
					p->sName = pRemoteName;
					p->unlock();
				}

				//-----------------------------------
				//  IF NECESSARY, SEND
				//  TCP CONNECTION REQUEST
				//-----------------------------------

				if ( ! g_Pool.get_connect_socket_info_by_ip ( dwRemoteIP ) )
				{
					g_Pool.connect_to ( dwRemoteIP );
				}
			}

			//--------------------------------------------------
			//  FINALLY, WE KNOW THAT IT'S AN UNKNOWN PC
			//--------------------------------------------------

			else
			{
				g_Pool.connect_to ( dwRemoteIP );

				//--------------------------------------------
				// ADD THE NEW REMOTE MACHINE TO MACH LIST
				//--------------------------------------------
				{
					cMach * pNewMach = g_Machlist.get_by_ip_or_add ( dwRemoteIP, pDisplayList );
					pNewMach->lock();
					pNewMach->dwIP				= dwRemoteIP;
					pNewMach->sName				= pRemoteName;
					pNewMach->unlock();
				}
			}
        }
	}
}


//----------------------------------------------------------------------------------------------------------------------
//  MAKE RECEIVE SOCKET 
//----------------------------------------------------------------------------------------------------------------------
bool cFinder :: make_receive_socket ( SOCKET * pRet )
{
    SOCKET			ReceiveSocket;
	SOCKADDR_IN		sinLocal;


    ReceiveSocket = socket ( AF_INET, SOCK_DGRAM, 0 );

    if ( ReceiveSocket == INVALID_SOCKET )
    {
        LOG_SYSTEM_ERROR_TE ( L"socket", WSAGetLastError() );
		*pRet = INVALID_SOCKET;
        return false;
    }

    sinLocal.sin_family = AF_INET;
    sinLocal.sin_port = htons((short) g_Settings.uPort );
	sinLocal.sin_addr.s_addr = htonl(INADDR_ANY);

    if ( bind ( ReceiveSocket, ( SOCKADDR * ) &sinLocal, sizeof ( sinLocal ) ) == SOCKET_ERROR )
    {
		DWORD dwError = WSAGetLastError();

		if ( WSAEADDRINUSE == dwError ) // 10048 means port in use
		{
			mojo::put_ad_lib_memo ( mojo::cMemo::error, 
				                     L"The program is trying to use a port that is already in use.",
									 L"Select a different port on Connections Settings, More Options." );
		}

        LOG_SYSTEM_ERROR_TE ( L"bind", dwError );

		*pRet = INVALID_SOCKET;
        return false;
    }
    
	*pRet = ReceiveSocket;
	return true;
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