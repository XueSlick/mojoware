/***********************************************************************************************************************
/*
/*    cScribPack.cpp / mojo_engine
/*
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"

using namespace mojo;
using namespace std;

//======================================================================================================================
//  CODE
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
//  SET AD LIB
//----------------------------------------------------------------------------------------------------------------------
bool cScribPack :: set_ad_lib ( const wchar_t * pHeadFormatString, const wchar_t * pBodyFormatString, va_list pArgs )
{
	head = pHeadFormatString;
	body = pBodyFormatString;
	return replace_format_specifications ( pArgs );
}


//----------------------------------------------------------------------------------------------------------------------
//  REPLACE FORMAT SPECIFICATIONS
//----------------------------------------------------------------------------------------------------------------------
bool cScribPack :: replace_format_specifications ( va_list pArgs )
{
	bool bRetVal;

	if ( head.contains ( L'%' ) )
	{
		if ( body.contains ( L'%' ) )
		{
			//-----------------------------------------------------------------------
			// COMBINE HEAD AND BODY IN SINGLE STRING, DO REPLACEMENTS, THEN SPLIT 
			// THE STRINGS APART
			//-----------------------------------------------------------------------

			// Use a signature to mark the boundary between the two strings.  Ideally
			// we'd use a non-Unicode number (0xFFFF) to mark the boundary but that
			// generates an error from the Microsoft function that does the 
			// replacement.  The next-best choice is a combination of code points
			// that is fantastically unlikely to occur in anybody's strings.   The
			// four chosen code points are Tamil, Mongolian, Hangul, and Vai.

			const wchar_t awSig []  = L"\x0B90" L"\x1840" L"\x3182" L"\xA530";
		    head += awSig;
			head += body;
			bRetVal = head.replace_format_specification_fields ( pArgs );
			wchar_t * p = wcsstr ( head.buffer(), awSig );
			assert(p);
			body = p + sizeof(awSig)/sizeof(wchar_t)-1;
			head.truncate ( p - head.buffer() );
		}

		else
			bRetVal = head.replace_format_specification_fields ( pArgs );
	}

	else
	{
		if ( body.contains ( L'%' ) )
			bRetVal = body.replace_format_specification_fields ( pArgs );

		else
			bRetVal = true;
	}

	return bRetVal;
}

//----------------------------------------------------------------------------------------------------------------------
//  LOAD
//  loads from multimap and replaces format specification fields
//----------------------------------------------------------------------------------------------------------------------
bool cScribPack :: load_from_multimap ( const wchar_t * pKey, ... )
{
	va_list args;
	va_start ( args, pKey );
	return load_from_multimap ( pKey, args );
}


//----------------------------------------------------------------------------------------------------------------------
//  LOAD
//  loads from multimap and replaces format specification fields
//----------------------------------------------------------------------------------------------------------------------
bool cScribPack :: load_from_multimap ( const wchar_t * pKey, va_list pArgs )
{
	cScribMgr::tMap * pMap = &g_ScribMgr.Map;

	int iLineQty = 0;

	key = pKey;

	cScribMgr::tMap::iterator pBeg = pMap->find ( pKey );

	if ( pBeg == pMap->end() )
	{
		wchar_t t [1000];
		wsprintf ( t, L"Unable to find %s in multimap.", pKey );
		LOG ( t );
		this->body = L"LANGUAGE FILE ERROR";
		this->head = L"LANGUAGE FILE ERROR";
		return false;
	}

	cScribMgr::tMap::iterator pEnd = pMap->upper_bound ( pKey );

	body.erase();

	for ( ; pBeg != pEnd; pBeg++ )
	{
		if ( 0 == iLineQty )
			head = pBeg->second.c_str();

		else 
		{
			if ( 1 < iLineQty )
				body += L"\n";

			body += pBeg->second.c_str();
		}

		iLineQty ++;		
	}

	//-----------------------------------------------
	//  REPLACE FORMAT SPECIFICATIONS
	//-----------------------------------------------

	return replace_format_specifications ( pArgs );
}


//----------------------------------------------------------------------------------------------------------------------
//  CONSTRUCTOR
//  loads from multimap and replaces format specification fields
//----------------------------------------------------------------------------------------------------------------------
cScribPack :: cScribPack ( const wchar_t * pKey, ... )
{
	va_list args;
  	va_start ( args, pKey );
	load_from_multimap ( pKey, args ); 
}

//----------------------------------------------------------------------------------------------------------------------
//  CONSTRUCTOR
//  loads from multimap and replaces format specification fields
//----------------------------------------------------------------------------------------------------------------------
cScribPack :: cScribPack ( const wchar_t * pKey, va_list pArgs )
{
	load_from_multimap ( pKey, pArgs ); 
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
