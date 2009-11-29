/********************************************************************************************************
/*
/*    cPool.h / mojo_engine
/*
/*	  This class implements TCP/IP communications (the program's basic communication method) based on
/*    Winsock2 and an IoCompletionPort.
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/*******************************************************************************************************/

#pragma once

#include "cVersion.h"
#include "cSignature.h"
#include "tList2.h"
#include "cMach.h"

//=======================================================================================================
// DATA
//=======================================================================================================

//=======================================================================================================
// CLASSES
//=======================================================================================================

//-------------------------------------------------------------------------------------------------------
// SOCKET INFO
//-------------------------------------------------------------------------------------------------------
struct sSocketInfo
{
	const wchar_t * get_text ( mojo::cStrW * pRet );

	enum { connect, accept } eType;

	SOCKET              Socket;
	DWORD               dwLocalIP;
	DWORD               dwRemoteIP;
	mojo::cStrW         sRemoteName; // fully q dns name
	mojo::cVersion      RemoteVersion;
	DWORD 				dwMachHandle;
	class mojo::cMach * pMach;
	sSocketInfo *       pNext;
	sSocketInfo *       pPrev;
	wchar_t             awRemoteIP [16];
};


//-------------------------------------------------------------------------------------------------------
// CLASS POOL
//-------------------------------------------------------------------------------------------------------
class cPool
{
public:

	cPool  ();
	~cPool ();

	void start (); 

	bool send ( DWORD dwIP, const char * pMsg, unsigned uLen );
	void receive ( class cLapPlus * pLap );
	void post_initial_receive ( sSocketInfo * pSI );

	bool connect_to ( const wchar_t * pAddr );
	bool connect_to ( DWORD dwIP );

	int get_connection_status ( DWORD dwMachHandle );

	sSocketInfo * get_accept_socket_info_by_ip  ( DWORD dwIP );
	sSocketInfo * get_connect_socket_info_by_ip ( DWORD dwIP );
	sSocketInfo * get_socket_info_by_ip ( DWORD dwIP );
	sSocketInfo * get_socket_info_by_socket		( SOCKET s );
	sSocketInfo * get_socket_info_by_address	( const wchar_t * pAddr );

	static void set_nagle ( SOCKET socket, bool bOn );
	static bool is_nagle_on ( SOCKET socket );

	void on_socket_close_notification ( WPARAM wParam, LPARAM lParam );

	int is_connected   ( DWORD dwIP ); // returns qty of connections
	bool versions_match ( DWORD dwIP );

	static const int iACCEPT_BUFFER_SIZE = 8092;

	// const wchar_t *			sig ()						{ return sConnectSig.cstr(); }
	// int						sig_len ()					{ return sConnectSig.len() ; }
	const wchar_t *			local_name  ()				{ return sLocalName.cstr() ; }
	int						local_name_len ()			{ return sLocalName.len(); }
	// const wchar_t *			sig_as_text					( cStrW * pRet, const wchar_t * pSig );
	// bool					parse_sig					( cStrW * pExtra, cStrW * pRetName, ::cVersion * pRetVersion, const wchar_t * pMsg );
	// bool					parse_sig					( const wchar_t ** ppRetExtra, const wchar_t ** ppRetName, ::cVersion * pRetVersion, const wchar_t * pMsg );
	// wchar_t *				make_sig_with_extra			( cStrW * pRet, const wchar_t * pExtra );

private:

	cSignature				* pSig;

	// const wchar_t *			make_sig					( cStrW * pRet );

	static void				receive_cb					( class cLapPlus *);
	static void				send_cb						( cLapPlus * );
	static void				acceptex_cb					( class cLapPlus *);
	static void				connectex_cb				( cLapPlus * );
	static bool				get_acceptex_addresses		( DWORD * pRetLocalIP, DWORD * pRetRemoteIP, char * pcBuf, int iBufSize, SOCKET Socket );
	bool					get_accept_ex_pointer		( SOCKET s );
	void *					get_connect_ex_pointer		( SOCKET s );

	static	unsigned _stdcall worker_thread				( void * pArg );

	mojo::cStrW				sConnectSig;
	mojo::cStrW				sLocalName;

	HANDLE					hIoPort;
	bool					create_listen_socket		(); 
	bool					post_accept_ex				( cLapPlus * pLapPlus = NULL ); 
	SOCKET					create_socket				(); 
	SOCKET					sListenSocket;

	tList2<sSocketInfo>		SocketInfoList;
	tList2<cLapPlus>	*	pLapPlusList;
};


/********************************************************************************************************
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
/********************************************************************************************************/