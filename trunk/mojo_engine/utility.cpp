/***********************************************************************************************************************
/*
/*	  utility.cpp / mojo_engine
/*
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"
#include "cMemo.h"

using namespace mojo;


//======================================================================================================================
//  CODE
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
//  DWORD TO STRING
//----------------------------------------------------------------------------------------------------------------------
MOJO_ENGINE_API wchar_t * mojo::dword_to_string ( wchar_t * pwRet, size_t size, DWORD a )
{
	_ultow_s ( a, pwRet, size, 16 );
	return pwRet;
}


//----------------------------------------------------------------------------------------------------------------------
//   VERSION OR HIGHER (OF OPERATING SYSTEM)
//----------------------------------------------------------------------------------------------------------------------
bool MOJO_ENGINE_API mojo::os_version_or_higher ( DWORD  dwMaj, DWORD dwMin )
{
	bool bRetVal = false;

	OSVERSIONINFOEX osvi;
	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	BOOL iGetVersionExRetVal = GetVersionEx ((OSVERSIONINFO *) &osvi);

	if ( iGetVersionExRetVal )
	{
		bRetVal = 	

		( (osvi.dwMajorVersion > dwMaj) ||
		( (osvi.dwMajorVersion == dwMaj ) && (osvi.dwMinorVersion >= dwMin )) );
	}

	return bRetVal;
}


//----------------------------------------------------------------------------------------------------------------------
//	FORMAT SYSTEM ERROR
//  Convert system error code into text
//----------------------------------------------------------------------------------------------------------------------
void MOJO_ENGINE_API mojo::format_system_error ( cStrW * pRet, unsigned int dwError )
{
    // Retrieve the system error message for the last-error code

    LPVOID lpMsgBuf;

    int iMsgLen = FormatMessageW(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dwError,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (wchar_t *) &lpMsgBuf,
        0, NULL );

	if ( iMsgLen )
	{
		*pRet = (wchar_t*)lpMsgBuf;
		pRet->trim_right ( 0xA );
		pRet->trim_right ( 0xD );
	}

	else
		pRet->erase();

	LocalFree(lpMsgBuf);
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