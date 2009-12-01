/***********************************************************************************************************************
/*
/*    net_stuff.cpp / mojo_engine
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

#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <assert.h>
#include <lm.h>
#include <ws2tcpip.h>
#include <winnetwk.h>
#include "stdafx.h"
#include "tStr.h"
#include "tList.h"
#include "net_stuff.h"
using namespace mojo;


//======================================================================================================================
// DATA
//======================================================================================================================

//======================================================================================================================
// PROTOTYPES
//======================================================================================================================

//======================================================================================================================
// CODE
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
// GET FULL DNS NAME
//----------------------------------------------------------------------------------------------------------------------
const wchar_t * get_full_dns_name ( mojo::cStrW * pRet )
{
	pRet->erase();

	COMPUTER_NAME_FORMAT cnf = ComputerNameDnsFullyQualified;

	wchar_t acBuf [8192];
	DWORD dwSize = sizeof ( acBuf ) / sizeof ( wchar_t );

    if ( 0 != GetComputerNameEx ( cnf, acBuf, &dwSize ) )
		*pRet = acBuf;

	//-----------------------------------
	//  TEMP
	//-----------------------------------
	{
		LOG_V ( L"In get_full_dns_name(); GetComputerNameExe returned  %s", acBuf );
		LOG_V ( L"Still in get_full_dns_name(); pRet == %s", acBuf );
	}

	return pRet->cstr();
}


//----------------------------------------------------------------------------------------------------------------------
// GET NAME INFO
// converts dotted dec to name
//----------------------------------------------------------------------------------------------------------------------
int get_name_info ( cStrW * pName, wchar_t * pwDottedDec )
{
    WSADATA wsaData;
    int iResult;

    DWORD dwRetval;

    struct sockaddr_in sa;
    wchar_t awName [NI_MAXHOST];
    wchar_t awServiceInfo [NI_MAXSERV];

    iResult = WSAStartup ( MAKEWORD ( 2, 2 ), &wsaData );

    if (iResult != 0) 
	{
        return 1;
    }

	cStrW w = pwDottedDec;

	// cStrW_to_cStrN ( &n, &w );  // replaced nov 9 2009 with constructor
	cStrN n ( pwDottedDec );

    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = inet_addr ( n.cstr() );
    sa.sin_port = htons(0);

    dwRetval = GetNameInfo ( ( struct sockaddr * ) &sa,
                           sizeof ( struct sockaddr ),
                           awName, NI_MAXHOST, 
						   awServiceInfo, NI_MAXSERV, 
						   NI_NAMEREQD );

    if (dwRetval != 0) 
	{
		DWORD dwError = WSAGetLastError();
        LOG_SYSTEM_ERROR_TE ( L"GetNameInfo", dwError ); 
		return dwError;
    } 
	
	* pName = awName;

    WSACleanup();

	return 0;  
}


//----------------------------------------------------------------------------------------------------------------------
// GET ADDR INFO
// converts name to IP address
//----------------------------------------------------------------------------------------------------------------------
int get_addr_info ( DWORD * puIP, cStrW * psIP, wchar_t * pNameOrDottedDec )
{
	*puIP = 0;

	WSADATA wsaData;

    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if ( 0 != iResult )
	{
        LOG_SYSTEM_ERROR_TE ( L"WSAStartup", iResult);
        return 0;
    }

	addrinfoW Hints;
	addrinfoW * pResults;

    ZeroMemory( &Hints, sizeof(Hints) );
    Hints.ai_family = AF_INET; // AF_INET == 2 == IPv4
    Hints.ai_socktype = 0;
    Hints.ai_protocol = 0;

	iResult = GetAddrInfoW ( pNameOrDottedDec, NULL, NULL, /* &Hints, */ &pResults );

	if ( 0 != iResult )
	{
		LOG_SYSTEM_ERROR_TE ( L"GetAddrInfoW", iResult );
		FreeAddrInfoW ( pResults );
        WSACleanup();
		return iResult;
	}

    for ( addrinfoW * p = pResults; p != NULL; p = p->ai_next ) 
	{
		if ( AF_INET == p->ai_family )
		{
			sockaddr_in * pSocketAddrIn = (sockaddr_in *) (p->ai_addr );
			*puIP = * (DWORD*) &pSocketAddrIn->sin_addr;
			wchar_t awIP[16];
			ip_dword_to_aw( awIP, *puIP );
			*psIP = awIP;
			break;
		}
	}

	FreeAddrInfoW ( pResults );
	WSACleanup();

	return iResult;
}



//----------------------------------------------------------------------------------------------------------------------
// GET IP FROM NAME
// uses getaddrinfo() like Microsoft recommends
// returns GetAddrInfoW result which equals 0 on success
//----------------------------------------------------------------------------------------------------------------------
int get_ip_from_name ( DWORD * puIP, wchar_t * pName )
{
	*puIP = 0;

	WSADATA wsaData;

    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if ( 0 != iResult )
	{
        LOG_SYSTEM_ERROR_TE ( L"WSAStartup", iResult);
        return 0;
    }

	addrinfoW Hints;
	addrinfoW * pResults;

    ZeroMemory( &Hints, sizeof(Hints) );
    Hints.ai_family = AF_INET; // AF_INET == 2 == IPv4
    Hints.ai_socktype = 0;
    Hints.ai_protocol = 0;

	iResult = GetAddrInfoW ( pName, NULL, NULL, /* &Hints, */ &pResults );

	if ( 0 != iResult )
	{
		LOG_SYSTEM_ERROR_TE ( L"GetAddrInfoW", iResult );
		FreeAddrInfoW ( pResults );
        WSACleanup();
		return iResult;
	}

	// DWORD dwRetVal = 0;

    for ( addrinfoW * p = pResults; p != NULL; p = p->ai_next ) 
	{
		if ( AF_INET == p->ai_family )
		{
			sockaddr_in * pSocketAddrIn = (sockaddr_in *) (p->ai_addr );
			*puIP = * (DWORD*) &pSocketAddrIn->sin_addr;
			// char * pcT3 = inet_ntoa ( pSocketAddrIn->sin_addr );
			// char * pc = ip_dword_to_pc ( dwRetVal );
			break;
		}
	}

	FreeAddrInfoW ( pResults );
	WSACleanup();

	return iResult;
}


//----------------------------------------------------------------------------------------------------------------------
// RETURNS NAMES OF PC'S ON NETWORK
// uses NetServerEnum
// returns qty names
//----------------------------------------------------------------------------------------------------------------------
int net_server_enum ( tListNI<cStrW> * pList ) 
{
	BYTE * pResults;
	DWORD dwEntriesRead, dwTotalEntries;
	DWORD dwResumeHandle;

	NET_API_STATUS s = NetServerEnum ( NULL, 101, &pResults, MAX_PREFERRED_LENGTH, &dwEntriesRead, 
		                               &dwTotalEntries, SV_TYPE_WORKSTATION | SV_TYPE_ALL ,
									   NULL, &dwResumeHandle );

	if ( s != NERR_Success )
	{
		LOG_SYSTEM_ERROR_TE ( L"NetServerEnum", s );
		NetApiBufferFree ( pResults ); 
		return 0;
	}

	for ( DWORD i = 0; i < dwEntriesRead; i++ )
	{
		SERVER_INFO_101 * p = ((SERVER_INFO_101*) pResults) + i;
		cStrW * pNewS = new cStrW;
		*pNewS = p->sv101_name;
		pList->insert ( pNewS );
	}

	NetApiBufferFree ( pResults );

	return dwEntriesRead;
}



//----------------------------------------------------------------------------------------------------------------------
// GET THIS PC
//----------------------------------------------------------------------------------------------------------------------
bool get_this_pc ( DWORD * puIP, cStrW * pName )
{

	/*  HERE IS MICROSOFT CODE TO DO THIS:
	hostent* localHost;
	char* localIP;

	localHost = gethostbyname(""); // deprecated in favor of getaddrinfo
	localIP = inet_ntoa (*(struct in_addr *)*localHost->h_addr_list);
	*/

	cStrW sNetBiosName;

	if ( ! get_netbios_name ( &sNetBiosName ) )
		return false;

	cStrW sNeatName;
	char acIP[16];
	wchar_t awIP[16];

	if ( 0 != netbios_name_to_ip ( pName, puIP, acIP, awIP, sNetBiosName.cstr() ) )
		return false;

	return true;
}


//----------------------------------------------------------------------------------------------------------------------
// GET NETBIOS NAME
//----------------------------------------------------------------------------------------------------------------------
bool get_netbios_name ( cStrW * pRet )
{
	wchar_t b [ MAX_COMPUTERNAME_LENGTH + 1 ];
	DWORD dwSize = sizeof(b)/sizeof(wchar_t);

	if ( 0 == GetComputerName ( b, &dwSize ) )
		return false;

	*pRet = b;

	return true;
}



//----------------------------------------------------------------------------------------------------------------------
// sNETRESOURCE CONSTRUCTOR
//----------------------------------------------------------------------------------------------------------------------
sNetResource::sNetResource ( NETRESOURCE &r )
{
	pNext			= 0;
	pPrev			= 0;
	dwScope			= r.dwScope;
	dwType			= r.dwType;
	dwDisplayType	= r.dwDisplayType;
	dwUsage			= r.dwUsage;
	sLocalName		= r.lpLocalName; 
	sRemoteName		= r.lpRemoteName;
	sComment		= r.lpComment;
	sProvider		= r.lpProvider;
}


//----------------------------------------------------------------------------------------------------------------------
// IP DWORD TO PC
//----------------------------------------------------------------------------------------------------------------------
char * ip_dword_to_pc ( DWORD dw )
{
	return inet_ntoa ( *(in_addr*) &dw );
}


//----------------------------------------------------------------------------------------------------------------------
// IP PC TO DWORD
//----------------------------------------------------------------------------------------------------------------------
DWORD  ip_pc_to_dword ( char * pc )
{ 
	return inet_addr ( pc );
}

//----------------------------------------------------------------------------------------------------------------------
// IP DWORD TO AC
//----------------------------------------------------------------------------------------------------------------------
void ip_dword_to_ac ( char ac [16], DWORD dw )
{
	strncpy_s ( ac, 16, inet_ntoa ( *(in_addr*) &dw ), 16 ); 
}

//----------------------------------------------------------------------------------------------------------------------
// IP DWORD TO AC
//----------------------------------------------------------------------------------------------------------------------
void ip_dword_to_aw ( wchar_t aw[16], DWORD dw )
{
	cStrN n = inet_ntoa ( *(in_addr*) &dw );
	cStrW w ( n );
	wcscpy_s( aw, 16, w.cstr() );
}

//----------------------------------------------------------------------------------------------------------------------
// IP DWORD TO AC
//----------------------------------------------------------------------------------------------------------------------
DWORD ip_pw_to_dword ( const wchar_t *pAW )
{
	cStrW w = pAW;
	cStrN n ( w );
	// cStrW_to_cStrN ( &n, &w ); // nov 9 2009 replaced with constructor
	return inet_addr ( n.cstr() );
}

//----------------------------------------------------------------------------------------------------------------------
// GET NETWORK COMPUTERS
// This is the user interface to the function
//----------------------------------------------------------------------------------------------------------------------
bool get_net_resources ( tList<sNetResource> * pListRet, int * piQtyRet )
{

    LPNETRESOURCE lpnr = NULL;
	bool bRetVal;

	if ( TRUE == get_net_resources ( pListRet, piQtyRet, lpnr ) )
		bRetVal = true;

	else
		bRetVal = false;

	return bRetVal;
}


//----------------------------------------------------------------------------------------------------------------------
// GET NETWORK COMPUTERS
// MY ADAPTATION OF MSDN SAMPLE
// RETURNS NAMES IN LIST DELIMITED BY SEMICOLONS
//----------------------------------------------------------------------------------------------------------------------
BOOL get_net_resources ( tList<sNetResource> * pListRet, int * piQtyRet, void * lpnrArg )
{
	LPNETRESOURCE lpnr = ( LPNETRESOURCE ) lpnrArg;

    DWORD dwResult, dwResultEnum;
    HANDLE hEnum;
    DWORD cbBuffer = 3 * 16384;     // Microsoft says 16K is a good size so let's do extra
    DWORD cEntries = static_cast<DWORD>(-1);			// enumerate all possible entries
    LPNETRESOURCE lpnrLocal;		// pointer to enumerated structures

    dwResult = WNetOpenEnum ( RESOURCE_GLOBALNET, // all network resources
                              RESOURCETYPE_ANY,   // all resources
                              0,  // enumerate all resources
                              lpnr,       // NULL first time the function is called
                              &hEnum);    // handle to the resource

    if (dwResult != NO_ERROR)
	{
        return FALSE;
    }

    lpnrLocal = (LPNETRESOURCE) GlobalAlloc(GPTR, cbBuffer);

    if (lpnrLocal == NULL) 
	{
        return FALSE;
    }

    do 
	{
        ZeroMemory(lpnrLocal, cbBuffer);

        dwResultEnum = WNetEnumResource(hEnum,  
                                        &cEntries,      
                                        lpnrLocal,  
                                        &cbBuffer);  

        if (dwResultEnum == NO_ERROR) 
		{
            for (DWORD i = 0; i < cEntries; i++) 
			{
				// STORE THE INFO IN DATA STRUCTURE THAT GETS RETURNED TO USER

				if ( lpnrLocal[i].dwDisplayType == RESOURCEDISPLAYTYPE_SERVER )
				{
					sNetResource * n = new sNetResource ( lpnrLocal[i] );
					pListRet->append ( n );
					(*piQtyRet)++;
				}

                // IF THE OS FUNCTION GIVES US A "CONTAINER," 
				// CALL THIS FUNCTION RECURSIVELY
				// TO GET CONTAINER CONTENTS:

                if (RESOURCEUSAGE_CONTAINER == (lpnrLocal[i].dwUsage & RESOURCEUSAGE_CONTAINER))
				{ 

					{
						if ( ! get_net_resources ( pListRet, piQtyRet, /* psRet, */ &lpnrLocal[i]) )
						NULL;
					}
				}
			}
        }

        else if (dwResultEnum != ERROR_NO_MORE_ITEMS) 
		{
            break;
        }

    } while ( dwResultEnum != ERROR_NO_MORE_ITEMS );

    GlobalFree ( (HGLOBAL) lpnrLocal );

    dwResult = WNetCloseEnum ( hEnum );

    if (dwResult !=  NO_ERROR )
	{
        return FALSE;
    }

    return TRUE;
}


//----------------------------------------------------------------------------------------------------------------------
// NETBIOS NAME TO IP
// Call WSAStartUp before calling this
// returns zero on success
// WinSock error on failure
//----------------------------------------------------------------------------------------------------------------------
DWORD netbios_name_to_ip ( cStrW * pNeatNameRet, DWORD * pRetDW, char acRet[16], wchar_t awRet[16], const wchar_t * pName )
{
	WSADATA wsaData;

	if ( DWORD dwResult = WSAStartup ( MAKEWORD(2, 2), &wsaData ) )
	   return dwResult;

	cStrW w = pName;
	w.trim_left ( (wchar_t) '\\' );
	*pNeatNameRet = w;
	cStrN  n ( w );
	// mojo::cStrW_to_cStrN ( &n, &w ); // nov 10 2009

	DWORD dwRetVal;

	hostent * pHE = gethostbyname ( n.cstr() );

	if ( pHE ) // ( pHE = gethostbyname ( n.cstr() ) )
	{
		dwRetVal = 0;
		*pRetDW = *(u_long *) pHE->h_addr_list[0];
		ip_dword_to_ac ( acRet, *pRetDW );
		ip_dword_to_aw ( awRet, *pRetDW );
	}

	else
	{
		dwRetVal = WSAGetLastError();
		strcpy_s ( acRet, sizeof(acRet)/sizeof(char),	  "UNKNOWN" );
		wcscpy_s ( awRet, sizeof(awRet)/sizeof(wchar_t), L"UNKNOWN" );
		*pRetDW = 0;
	}

	return dwRetVal;
}


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