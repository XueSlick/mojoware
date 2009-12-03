/********************************************************************************************************
/*
/*    files.cpp / mojo_engine
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

#include "mojo_engine.h"
#include "directories.h"
#include <shlobj.h>

using namespace mojo;

//=======================================================================================================
//  CODE
//=======================================================================================================

namespace mojo {


//----------------------------------------------------------------------------------------------------------------------
//   GET DESKTOP DIRECTORY
//----------------------------------------------------------------------------------------------------------------------
const wchar_t * get_desktop_directory ( cStrW * s )
{
	wchar_t szPath [ MAX_PATH ];

	if ( SUCCEEDED ( SHGetFolderPath ( NULL, 
								       CSIDL_DESKTOP, 
									   NULL, 
									   0, 
									   szPath ) ) ) 
	{
		*s = szPath;
		*s += L'\\';
		return s->cstr();
	}

	return NULL;
}


//----------------------------------------------------------------------------------------------------------------------
//   GET START MENU DIRECTORY
//----------------------------------------------------------------------------------------------------------------------
const wchar_t * get_start_menu_directory ( cStrW * s )
{
	wchar_t szPath [ MAX_PATH ];

	if ( SUCCEEDED ( SHGetFolderPath ( NULL, 
								       CSIDL_STARTMENU, 
									   NULL, 
									   0, 
									   szPath ) ) ) 
	{
		*s = szPath;
		*s += L'\\';
		return s->cstr();
	}

	return NULL;
}


//-------------------------------------------------------------------------------------------------------
//   GET MODULE DIRECTORY
//   returns it with trailing backslash (e.g.) "c:\d\sou\"
//-------------------------------------------------------------------------------------------------------
const wchar_t * get_module_directory ( cStrW * s )
{
	wchar_t t[4096];

	DWORD retval = GetModuleFileNameW ( NULL, t, sizeof(t)-1 );

	if ( retval )
	{
		int l = (int) wcslen ( t );

		for ( wchar_t * p = t + l - 1; t < p; p-- )
		{
			if ( *p == '\\' )
			{
				*(p+1) = 0;
				break;
			}
		}

		*s = t;
		return s->cstr();
	}

	return NULL;
}


//-------------------------------------------------------------------------------------------------------
//   GET LOCAL APP DATA DIRECTORY
//-------------------------------------------------------------------------------------------------------
const wchar_t * get_local_app_data_directory ( cStrW * s )
{
	wchar_t szPath [ MAX_PATH ];

	if ( SUCCEEDED ( SHGetFolderPath ( NULL, 
								       CSIDL_LOCAL_APPDATA, 
									   NULL, 
									   0, 
									   szPath ) ) ) 
	{
		*s = szPath;
		*s += L'\\';
		return s->cstr();
	}

	return NULL;
}


//-------------------------------------------------------------------------------------------------------
//   GET APP DATA DIRECTORY
//-------------------------------------------------------------------------------------------------------
const wchar_t * get_app_data_directory ( cStrW * s )
{
	wchar_t szPath [ MAX_PATH ];

	if ( SUCCEEDED ( SHGetFolderPath ( NULL, 
								       CSIDL_APPDATA, 
									   NULL, 
									   0, 
									   szPath ) ) ) 
	{
		*s = szPath;
		return s->cstr();
	}

	return NULL;
}


//-------------------------------------------------------------------------------------------------------
//   GET OUR LOCAL APP DATA DIRECTORY
//   creates it if it doesn't exist
//-------------------------------------------------------------------------------------------------------
const wchar_t * get_our_local_app_data_directory ( cStrW * pRet, const wchar_t * pAppTitle )
{
	cStrW s;

	if ( ! get_local_app_data_directory ( &s) )
		return NULL;

	s += pAppTitle;

	if ( ! CreateDirectoryW ( s.cstr(), NULL ) ) // zero is error
	{
		if ( ERROR_ALREADY_EXISTS != GetLastError() )
			return NULL;
	}

	*pRet = s;
	*pRet += L"\\";
	return pRet->cstr();
}

} // namespace


