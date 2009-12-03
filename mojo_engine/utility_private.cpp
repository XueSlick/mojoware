/***********************************************************************************************************************
/*
/*	  utility_private.cpp / mojo_engine
/*
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"
#include <strsafe.h>

using namespace mojo;

//======================================================================================================================
//  CODE
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
//  REPLACE FORMAT SPEC FIELDS
//  va_list's are fragile and risky, so Structured Exception Handling (SEH) is used here to protect
//  against any and all problems.  The worst thing that can happen is that "ERROR" is copied to the
//  return string (or not even that if the return buffer is too small).
//----------------------------------------------------------------------------------------------------------------------
bool replace_format_specification_fields ( wchar_t * pRet, int iSize,  const wchar_t * pIn, va_list pVA )
{
	HRESULT hr;

	__try
	{
		hr = StringCchVPrintfW (  pRet, iSize - 1, pIn, pVA );
	}

	__except ( EXCEPTION_EXECUTE_HANDLER )
	{
		wcscpy_s ( pRet, iSize-1, L"ERROR" );
		return false;
	}

	if ( FAILED ( hr ) )
	{
		wcscpy_s ( pRet, iSize-1, L"ERROR" );
		return false;
	}

	return true;
}


//----------------------------------------------------------------------------------------------------------------------
//  LOG AND PUT MEMO
//  Called by LOG_AND_MEMO macro
//----------------------------------------------------------------------------------------------------------------------
#if 0
void log_and_put_memo ( const wchar_t * pFile, int iLine, mojo::cMemo::_eSeverity e, const wchar_t * pTxt, ... )
{
	va_list args;
  	va_start ( args, pTxt );

	mojo::cMemo m ( e, pTxt, args );

	mojo::put_memo ( &m ); // KEEP THIS BEFORE LOG

	g_Log.log ( pFile, iLine, L"The following cMemo has been logged:" );

	cStrW s = L"   Key: ";
	s += m.key();
	g_Log.log ( s.cstr() );

	s = L"   Severity: ";
	s += m.severity_cstr();
	g_Log.log ( s.cstr() );

	s = L"   Head: ";
	s += m.head();
	g_Log.log ( s.cstr() );


	s = L"   Body: ";
	s += m.body();
	s.replace ( L'\n', L' ' );
	g_Log.log ( s.cstr() );
}
#endif

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