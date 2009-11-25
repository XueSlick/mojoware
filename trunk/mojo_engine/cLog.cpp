/********************************************************************************************************
/*
/*   cLog.cpp / mojo_engine
/*
/*    Copyright 2009 Robert Sacks
/*
/*    This file is part of Mojo.  You may redistribute and/or modify Mojo under the terms of the GNU 
/*    General Public License, version 3, as published by the Free Software Foundation.  You should have
/*    received a copy of the license with mojo.  If you did not, go to http://www.gnu.org.
/*
/*    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR 
/*    IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND 
/*    FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
/*    CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
/*    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
/*    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER 
/*    IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT 
/*    OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
/*
/********************************************************************************************************/

//-------------------------------------------------------------------------------------------------------
//  MEMORY DEBUG
//-------------------------------------------------------------------------------------------------------

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

//-------------------------------------------------------------------------------------------------------
//  INCLUDES
//-------------------------------------------------------------------------------------------------------

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#endif

#include <Windows.h>
#include "tStr.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include <stdio.h>
#include <tchar.h>
#include <time.h>
#include "cLog.h"
#include "files.h"
#include "share.h"
#include "cVersion.h"

using namespace mojo;

//=======================================================================================================
//  DATA
//=======================================================================================================

extern		cLog g_Log;


//=======================================================================================================
//  PROTOTYPES
//=======================================================================================================

wchar_t * 	get_full_dns_name ( mojo::cStrW * pRet );
bool 		replace_format_specification_fields ( wchar_t * pRet, int iSize,  const wchar_t * pIn, va_list pVA );

namespace mojo {


bool 		get_system_version ( cStrW * );
void		format_system_error ( cStrW * sSE, unsigned int dwSystemError );


//=======================================================================================================
// CODE
//=======================================================================================================

#pragma warning ( push )
#pragma warning ( disable : 4996 ) // deprecated io functions, e.g. _wopen replaced by _wsopen_s

//-------------------------------------------------------------------------------------------------------
// GET TEXT
//-------------------------------------------------------------------------------------------------------
void cLog :: get_text ( cStrW * pRet )
{
	pRet->erase();

	int fd = _wopen ( Filename.cstr(),  _O_RDONLY ); // | _O_WTEXT  );

	// _wsopen_s ( &hFile, Filename.cstr(), _O_RDONLY, _SH_DENYNO , _S_IREAD | _S_IWRITE  );

	if ( -1 == fd )
		return;

	int iSize = _lseek ( fd, 0, SEEK_END );

	if ( ! iSize )
		return;

	_lseek ( fd, 0, SEEK_SET );

	char * b = new char [  iSize + sizeof(wchar_t) ];

	int iQtyRead = _read ( fd, b, iSize  );

	 * (wchar_t*) ( &b[iQtyRead] ) = 0;

	 wchar_t * w = (wchar_t*) b;

	 if ( w[0] == 0xFEFF )
		*pRet = w + 1;

	 else
		 *pRet = w;

	delete[] b;

	_close ( fd );
}



//-------------------------------------------------------------------------------------------------------
// GET PRETTY TIME
//-------------------------------------------------------------------------------------------------------
void get_pretty_time ( cStrW * p )
{
	SYSTEMTIME Time;

	GetLocalTime ( &Time );

	wchar_t b [500];

	static wchar_t * apcMonths [] = 
	{
		L"", L"January", L"February", L"March", L"April", L"May", L"June", 
			L"July", L"August", L"September", L"October", L"November", L"December"
	};

	static wchar_t * apcDays [] = { /* "", */ L"Sunday", L"Monday", L"Tuesday", L"Wednesday", L"Thursday", L"Friday", L"Saturday" };

	swprintf ( b, sizeof(b)/sizeof(wchar_t)-1, L"%02d:%02d:%02d on %s, %s %d, %d",
		Time.wHour,
		Time.wMinute,
		Time.wSecond,
		apcDays[Time.wDayOfWeek],
		apcMonths[Time.wMonth],
		Time.wDay,
		Time.wYear  );

	*p = b;
}


//-------------------------------------------------------------------------------------------------------
//  PRINT HEADER
//-------------------------------------------------------------------------------------------------------
void cLog::print_header ( wchar_t * p )
{

	log ( L"-------------------------------------------------------------------------------------" );
	log ( p );
	log ( L"-------------------------------------------------------------------------------------" );
	log ( L"" );

	cStrW s, t;

	cStrW sVersion;
	Version.get_text ( &sVersion );
	s = sAppTitle;
	s += L" ";
	s += sVersion;
	log ( s.cstr() );

	get_pretty_time ( &t );
	s = L"File opened at ";
	s += t;
	log ( s.cstr() );

	cStrW sName;
	::get_full_dns_name ( &sName );
	s = L"DNS name: ";
	s += sName;
	log ( s.cstr() );

	cStrW sOsVer;
	if ( get_system_version ( & sOsVer )  )
	{
		s = L"OS version: ";
		s += sOsVer;
		log ( s.cstr() );
	}

	log ( L"" );
	log ( L"-------------------------------------------------------------------------------------" );
	log ( L"" );



#if 0

	wchar_t acBuf[4096];
	gethostname ( acBuf, sizeof(acBuf) );
	s = "Hostname: ";
	s += acBuf;
	log ( s.cstr() );

#endif


}




//-------------------------------------------------------------------------------------------------------
// GET COMPUTER NAME
//-------------------------------------------------------------------------------------------------------
void get_computer_name ( cStrW * pRet )
{
    TCHAR buffer[256] = TEXT("");
    TCHAR szDescription[8][32] = {TEXT("NetBIOS"), 
        TEXT("DNS hostname"), 
        TEXT("DNS domain"), 
        TEXT("DNS fully-qualified"), 
        TEXT("Physical NetBIOS"), 
        TEXT("Physical DNS hostname"), 
        TEXT("Physical DNS domain"), 
        TEXT("Physical DNS fully-qualified")};
    int cnf = 0;
    DWORD dwSize = sizeof(buffer);

	wchar_t buf [4096];
	
    for (cnf = 0; cnf < ComputerNameMax; cnf++)
    {
        if (!GetComputerNameEx(
            (COMPUTER_NAME_FORMAT)cnf, buffer, &dwSize))
        {
            swprintf ( buf, sizeof(buf)/sizeof(wchar_t)-1, TEXT("GetComputerNameEx failed (%d)\n"), GetLastError () );
            return;
        }

        else swprintf ( buf , sizeof(buf)/sizeof(wchar_t)-1, TEXT("%s: %s\n"), szDescription[cnf], buffer);

		*pRet += buf;
		*pRet += L"\r\n";

        dwSize = sizeof(buffer);
        ZeroMemory(buffer, dwSize);
    }
}


//-------------------------------------------------------------------------------------------------------
// LOG SYSTEM ERROR
//-------------------------------------------------------------------------------------------------------
void log_system_error ( wchar_t * pcFile, int iLine, wchar_t * pcText, unsigned int dwSystemError )
{
	g_Log.log_system_error ( pcFile, iLine, pcText, dwSystemError );
}

//-------------------------------------------------------------------------------------------------------
// TEST
//-------------------------------------------------------------------------------------------------------
bool cLog::test ( cStrW * pError )
{
	pError->erase();

	int fd = _wopen(Filename.cstr(),_O_WRONLY | _O_APPEND);

	if ( -1 == fd )
	{
		*pError += L"Unable to open file: ";
		*pError += Filename;
		return false;
	}

	else
	{
		* pError = L"Log file opened: ";
		* pError += Filename;
	}


	cStrW sLine;
	format_line ( &sLine, __WFILE__, __LINE__, L"testing log file." );
	

	int iRetVal = _write ( fd, sLine.cstr(), sLine.len() );

	if ( iRetVal != sLine.len() )
	{
		*pError = L"Error while writing to file: ";
		*pError += Filename;
		_close ( fd );
		return false;
	}

	_commit ( fd );

#if 0
	if ( 0 != iComRetVal )
	{
		*pError = "Unable to _commit file: ";
		*pError += Filename;
		_close ( hFile );
		return false;
	}
#endif

	_close ( fd );

	return true;
}


//-------------------------------------------------------------------------------------------------------
//  LOG
//-------------------------------------------------------------------------------------------------------
void cLog::log ( const wchar_t * p )
{
	cLog::write ( p );
}


//-------------------------------------------------------------------------------------------------------
// LOG 
//-------------------------------------------------------------------------------------------------------
void cLog::log ( const wchar_t * pcFile, int iLine, const wchar_t * pcText, unsigned int dwX )
{
	wchar_t buf[500];
	swprintf ( buf, sizeof(buf)/sizeof(wchar_t)-1,  L"%s %x", pcText, dwX );
	log ( pcFile, iLine, buf);
}


//-------------------------------------------------------------------------------------------------------
// LOG SYSTEM ERROR
//-------------------------------------------------------------------------------------------------------
void cLog::log_system_error ( wchar_t * pcFile, int iLine, wchar_t * pcText, unsigned int dwSystemError )
{
	cStrW sSE;
	format_system_error ( &sSE, dwSystemError );

	wchar_t buf[500];
	_itow ( dwSystemError, buf, 10 );
	cStrW s;
	s = pcText;
	s += L"; ";
	s += L"system error ";
	s += buf;
	s += L": ";
	s += sSE;
	log ( pcFile, iLine, s.cstr() );
}


//-------------------------------------------------------------------------------------------------------
//  LOG - FORMAT LINE
//-------------------------------------------------------------------------------------------------------
void cLog::format_line ( cStrW * pRet, const wchar_t * pcFile, int iLine, const wchar_t * pcTxt )
{
	wchar_t acLine [500];
	_itow ( iLine, acLine, 10 );
	*pRet = pcFile;
	*pRet  += L" ";
	*pRet += acLine;
	*pRet  += L": ";
	*pRet += pcTxt;
	// *pRet += L"\n";	// new line is appended by lowest level function, i.e., cLog :: write()
}



//-------------------------------------------------------------------------------------------------------
//  LOG - log
//-------------------------------------------------------------------------------------------------------
void cLog::log ( const wchar_t * pcFile, int iLine, const wchar_t * pcTxt )
{
	// hFile = _wopen(Filename.cstr(),_O_WRONLY | _O_APPEND);
	cStrW s;
	format_line ( &s, pcFile, iLine, pcTxt );
	write ( s.cstr() );
}

//-------------------------------------------------------------------------------------------------------
//  LOG VARADIC
//-------------------------------------------------------------------------------------------------------
void cLog::log_v ( const wchar_t * pcFile, int iLine, const wchar_t * pTxt, ... )
{
	va_list args;
  	va_start ( args, pTxt );
	wchar_t awTxt [8192];
	replace_format_specification_fields ( awTxt, sizeof(awTxt)/sizeof(wchar_t), pTxt, args );
	cStrW s;
	format_line ( &s, pcFile, iLine, awTxt );
	write ( s.cstr() );
}



//-------------------------------------------------------------------------------------------------------
// WRITE
// writes a line, i.e., appends a newline char
//-------------------------------------------------------------------------------------------------------
void cLog::write ( const wchar_t * p ) // do all writing through this function
{
#if 0
	InitializeCriticalSectionAndSpinCount(&this->cs, 4000 );

	EnterCriticalSection ( &cs );
	{
#endif

#if 1 // UNICODE VERSION

	int fd = _wopen ( Filename.cstr(), _O_WRONLY | _O_APPEND | _O_U16TEXT );

	if ( -1 == fd )
	{
		assert(0);
	}

	unsigned uQtyBytes = (unsigned) wcslen(p) * sizeof(wchar_t);

	int iRetVal = _write ( fd, p, uQtyBytes );

	if ( (unsigned) iRetVal != uQtyBytes )
		assert(0);

	// iRetVal = _write ( fd, L"\x2028", 2 );  // line separator

	iRetVal = (int) _write ( fd, L"\n", 2 );

	if ( 2 != iRetVal )
		assert(0);

	if ( 0 != _commit ( fd ) )
		assert(0);

	_close ( fd );
#endif

#if 0 // ANSI VERSION
	cStrW w = p;
	cStrN  n;
	cStrW_to_cStrN ( &n, &w ); // ANSI not unicode for some reason ... why did I do this?

	hFile = _wopen ( Filename.cstr(), _O_WRONLY | _O_APPEND );
	int iRetVal = _write ( hFile, n.cstr(), n.len() );
	iRetVal = _write ( hFile, "\n", 1 );
	int iComRetVal = _commit ( hFile );
	_close ( hFile );
#endif

#if 0
	}
	LeaveCriticalSection ( &cs );
#endif

}


//-------------------------------------------------------------------------------------------------------
//  LOG - CONSTRUCTOR
//-------------------------------------------------------------------------------------------------------
cLog::cLog ( const wchar_t * pAppFolderName, const wchar_t * pAppTitle, mojo::cVersion * pVersion )
{
	sAppTitle = pAppTitle;
	Version = *pVersion;
	cStrW tail = pAppTitle;
	tail += L".log.txt";
	cStrW sPath;
	get_our_local_app_data_directory ( &sPath, pAppFolderName );
	sPath += tail;
	Filename = sPath;

	int fd = _wopen ( Filename.cstr(), _O_U16TEXT | _O_RDWR |_O_APPEND | _O_CREAT | _O_TRUNC ,  _S_IWRITE ); // the comma is correct
	// _wsopen_s ( &hFile, Filename.cstr(), _O_U16TEXT | _O_RDWR |_O_APPEND | _O_CREAT | _O_TRUNC , _SH_DENYNO , _S_IREAD | _S_IWRITE  );

	if ( -1 == fd )
		return;

	// _write ( hFile, L"\xFFEE", 2 );  // I think the _O_U16TEXT flag does this

	_close ( fd );

	cStrW sHeader = pAppTitle;
	sHeader += L" Log File";
	print_header ( sHeader.cstr() );
}

} // namespace j

#pragma warning ( pop )