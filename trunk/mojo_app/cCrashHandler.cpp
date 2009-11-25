/***********************************************************************************************************************
/*
/*	  cCrashHandler.cpp / mojo_app
/*
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"


//======================================================================================================================
//  CODE
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
// WRITE MINI DUMP
//----------------------------------------------------------------------------------------------------------------------
bool cCrashHandler::write_mini_dump ( const wchar_t * pFile, EXCEPTION_POINTERS * pExceptionInfo )
{
    HANDLE hFile = CreateFile ( pFile, 
								GENERIC_READ|GENERIC_WRITE, 
								FILE_SHARE_DELETE|FILE_SHARE_READ|FILE_SHARE_WRITE, 
								NULL, 
								CREATE_ALWAYS, 
								FILE_ATTRIBUTE_NORMAL, 
								NULL );

	if ( NULL == hFile )
		false;

    HANDLE hProcess		= GetCurrentProcess ();
    DWORD  dwProcessID	= GetProcessId ( hProcess );

    MINIDUMP_EXCEPTION_INFORMATION mei;
	mei.ThreadId = GetCurrentThreadId();
	mei.ExceptionPointers = pExceptionInfo;
	mei.ClientPointers = FALSE;

	MiniDumpWriteDump (
		hProcess,
		dwProcessID,
		hFile,
		MiniDumpNormal,
		&mei,
		NULL,
		NULL );

	return true;
}


//----------------------------------------------------------------------------------------------------------------------
// UNHANDLED EXCEPTION HANDLER
//----------------------------------------------------------------------------------------------------------------------
LONG WINAPI cCrashHandler::unhandled_exception_filter ( EXCEPTION_POINTERS * pExceptionInfo )
{
	cStrW sPathname;
	cCrashHandler::make_file_name ( & sPathname );
	EXCEPTION_RECORD * er = pExceptionInfo->ExceptionRecord;

	wchar_t b [ 2000 ];

	if ( write_mini_dump ( sPathname.cstr(), pExceptionInfo ) )
	{
		wsprintf ( b,	L"%s has crashed.  Could you please email this file:\r\r"

						L"%s\r\r"

						L"to freddie@hotkeynet.com.\r\r"

						L"And here's some additional info that Freddie will want to know:\r\r"

						L"Code:\t\t0x%X\r"
						L"Address:\t\t0x%p\r",

						g_awAppTitle,
						sPathname.cstr(),
						er->ExceptionCode,
						er->ExceptionAddress );
	}

	else
	{
		wsprintf ( b,	L"%s has crashed, and it was unable to create\r"
						L"a dump file.\r\r"

						L"Code:\t\t0x%X\r"
						L"Address:\t\t0x%p\r",

						g_awAppTitle,
						er->ExceptionCode,
						er->ExceptionAddress );
	}

	MessageBox ( NULL, b, g_awAppTitle, MB_OK );

	return EXCEPTION_EXECUTE_HANDLER;
}


//----------------------------------------------------------------------------------------------------------------------
// MAKE FILE NAME
//----------------------------------------------------------------------------------------------------------------------
void cCrashHandler :: make_file_name ( cStrW * pRet )
{
	wchar_t awBuild [500];
	wsprintf ( awBuild, L"%d%", g_Version.build() );

	get_our_local_app_data_directory ( pRet, g_awAppTitle );
	*pRet += g_awAppTitle;
	*pRet += L'.';
	*pRet += awBuild;
	*pRet += L".dmp";
}


//----------------------------------------------------------------------------------------------------------------------
// CONSTRUCTOR
//----------------------------------------------------------------------------------------------------------------------
cCrashHandler :: cCrashHandler ()
{
	SetUnhandledExceptionFilter ( &unhandled_exception_filter );
}


/***********************************************************************************************************************
/*
/*    This file is part of Mojo.  For more information, see http://mojoware.org.
/*
/*    You may redistribute and/or modify Mojo under the terms of the GNU General Public License, version 3, as
/*    published by the Free Software Foundation.  You should have received a copy of the license with mojo.  If you
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