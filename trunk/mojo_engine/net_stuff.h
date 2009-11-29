/***********************************************************************************************************************
/*
/*    net_stuff.h / mojo_engine
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#pragma once


#include <Winnetwk.h>
#include "tListNI.h"
#include "tList.h"



//======================================================================================================================
// PROTOTYPES
//======================================================================================================================

//======================================================================================================================
// DATA
//======================================================================================================================
// used by get_net_resources
struct sNetResource
{
	sNetResource () : pNext(0), pPrev(0) {}
	sNetResource (NETRESOURCE & r);

	DWORD dwScope;
	DWORD dwType;
	DWORD dwDisplayType;
	DWORD dwUsage;
	mojo::cStrW sLocalName;
	mojo::cStrW sRemoteName;
	mojo::cStrW sComment;
	mojo::cStrW sProvider;
	sNetResource * pNext;
	sNetResource * pPrev;
};

const wchar_t * get_full_dns_name ( mojo::cStrW * pRet );
int get_name_info ( mojo::cStrW * pName, wchar_t * pwDottedDec );
int get_addr_info ( DWORD * puIP, mojo::cStrW * psIP, wchar_t * pNameOrDottedDec );
int get_ip_from_name ( DWORD * puIP, wchar_t * pName ); // uses getaddrinfo
int net_server_enum ( tListNI<mojo::cStrW> * pList );
bool get_net_resources ( mojo::tList<sNetResource> * pListRet, int * piQtyRet ); // this one is the user interface
BOOL get_net_resources ( mojo::tList<sNetResource> * pListRet, int * piQtyRet,  void * /* LPNETRESOURCE */ lpnr);
DWORD netbios_name_to_ip ( mojo::cStrW * pNeatNameRet, DWORD * pRet, char acRet[16], wchar_t awRet[16], const wchar_t * pName );

char * ip_dword_to_pc ( DWORD dw );  
DWORD  ip_pc_to_dword ( char * pc ); 
void   ip_dword_to_ac ( char ac[16], DWORD dw ); 
void   ip_dword_to_aw ( wchar_t aw[16], DWORD dw );
DWORD  ip_pw_to_dword ( const wchar_t *pAW );

bool   get_netbios_name ( mojo::cStrW * pRet );  // of this computer
bool   get_this_pc ( DWORD * puIP, mojo::cStrW * pName );

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