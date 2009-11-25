/***********************************************************************************************************************
/*
/*    cLapPlus.h / mojo_engine
/*
/*    This data structure wraps (and extends) the Winsock OVERLAPPED struct.  It's used by cPool.
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#pragma once

//======================================================================================================================
// TYPEDEF
//======================================================================================================================

typedef void fCALLBACK ( class cLapPlus * );

//======================================================================================================================
// CLASS
//======================================================================================================================

//---------------------------------------------------------------------------------------------
// LAP PLUS
//---------------------------------------------------------------------------------------------
class cLapPlus
{
public:

	cLapPlus () : sBuf( cPool :: iACCEPT_BUFFER_SIZE ) { init(); };

	void init () {   
				  sBuf.erase();
#ifdef _DEBUG
				  sBuf 				= "cLapPlus::sBuf";
#endif
				  dwQtyBytesXfered 	= 0;
				  sock				= INVALID_SOCKET;
				  pfCallback		= NULL;
				  pPool				= NULL;
				  pSocketInfo		= NULL;
				  dwError			= 0;
				  memset ( &lap, 0, sizeof(lap) ); }

	WSAOVERLAPPED			lap;
	mojo::cStrN				sBuf;
	DWORD					dwQtyBytesXfered;
	// nPool::eOp				op;
	SOCKET					sock;
	fCALLBACK   *			pfCallback;
	cPool		*			pPool;
	sSocketInfo *			pSocketInfo;
	DWORD 					dwError;

	cLapPlus * pPrev;
	cLapPlus * pNext;
};


/***********************************************************************************************************************
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
/***********************************************************************************************************************/