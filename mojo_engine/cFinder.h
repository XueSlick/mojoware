/***********************************************************************************************************************
/*
/*    cFinder.h / mojo_engine
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/*    This class runs in its own low priority thread (two threads, actually) and every few seconds, broadcasts
/*    a UDP message to all adapters on the local network asking if another copy of the program is listening.  The
/*    reason there are two threads is that one broadcasts and the other listens.   When the listening thread hears
/*    the broadcast from another PC, it checks whether a TCP connection already exists between the two machines.
/*    If not, this class asks class cPool to create a TCP connection between the two machines.
/*
/**********************************************************************************************************************/

#pragma once

#include "cSignature.h"

//======================================================================================================================
// CLASS
//======================================================================================================================

class cFinder
{
public:

	cFinder ();

	bool start_threads ();
	static bool					get_local_ip	( DWORD * pdwLocalIP );
	static bool					is_local_ip		( DWORD dwIP );

private:

	bool						make_socket		( SOCKET * pRet, DWORD dwLocalIP );
	bool						send			( int iPort, const wchar_t * pTxt );
	bool						receive			( int iPort );
	static unsigned _stdcall	server_thread	( void *p );
	static unsigned _stdcall	client_thread	( void *p );


	mojo::cStrW sLocalName;
	unsigned uServerThreadID;
	unsigned uClientThreadID;
	unsigned uPort;

	DWORD m_dwLocalIP;

	cSignature Sig;
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