/********************************************************************************************************
/*
/*    cScribMgr.cpp / mojo_engine
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

#include "stdafx.h"
#include <string>
#include <map>
#include <stdarg.h>
#include <strsafe.h>

using namespace mojo;
using namespace std;

//=======================================================================================================
//  CODE
//=======================================================================================================
//-------------------------------------------------------------------------------------------------------
//  LOAD SCRIBS
//-------------------------------------------------------------------------------------------------------
MOJO_ENGINE_API int mojo :: load_scribs ( const wchar_t * apScribs [], bool bReplace )
{
	return g_ScribMgr.load_scribs ( apScribs, bReplace );
}


//-------------------------------------------------------------------------------------------------------
//  LOAD SCRIBS
//-------------------------------------------------------------------------------------------------------
MOJO_ENGINE_API int mojo :: load_scribs ( const wchar_t * pPathname, bool bReplace )
{
	return g_ScribMgr.load_scribs ( pPathname, bReplace );
}



//-------------------------------------------------------------------------------------------------------
//  LOAD SUB
//-------------------------------------------------------------------------------------------------------
bool cScribMgr :: load_sub ( const wchar_t * pLine, bool bReplace )
{

	wchar_t * pRgx = L"\\s*(\\S*)\\s*=\\s*" L"((?:.|\\n)+)";

	cStrW aRay[3];

	if ( ! match ( aRay, sizeof(aRay)/sizeof(cStrW), pLine, pRgx ) ) 
	{
		cStrW m = L"Unable to match scrib with regex: ";
		m += pLine;
		LOG ( m.cstr() );
		return false;
	}

	if ( bReplace )
	{
		//------------------------------------------------
		// IF THIS IS FIRST TIME WE'VE ENTERED THIS
		// KEY, REMOVE EARLIER ENTRIES WITH SAME KEY
		//------------------------------------------------

		if ( _wcsicmp ( sPrevKey.cstr(), aRay[0].cstr() ) )
		{
			remove ( aRay[0].cstr() );
			sPrevKey = aRay[0];
		}
	}

	Map.insert ( pair<wstring,wstring> ( aRay[0].cstr(), aRay[1].cstr() ) );

	return true;
}

//-------------------------------------------------------------------------------------------------------
//  LOAD FROM SOURCE CODE
//-------------------------------------------------------------------------------------------------------
int cScribMgr::load_scribs ( const wchar_t * pRay [], bool bReplace )
{
	sPrevKey.erase();

	int iQty = 0;

	wchar_t * t = L"\n";

	t++; t--;

	for ( const wchar_t ** ppLine = &pRay[0]; *ppLine; ppLine++ )
		if ( load_sub ( *ppLine, bReplace ) )
			iQty ++;

	// LOG
	{
		wchar_t t [1000];
		swprintf_s ( t, sizeof(t)/sizeof(wchar_t), L"Scribs loaded from memory: %d scribs.", iQty );
		LOG ( t );
	}

	return iQty;
}



//-------------------------------------------------------------------------------------------------------
//  LOAD FROM FILE
//-------------------------------------------------------------------------------------------------------
int cScribMgr :: load_scribs ( const wchar_t * pPathname, bool bReplace )
{
	sPrevKey.erase();

	int iQty = 0;

	assert ( pPathname );


		cFileIn f ( pPathname );

		if ( ! f.h )

		{
			cStrW m = "cScribMgr::load_from_file() unable to find ";
			m += pPathname;
			LOG ( m.cstr() );
			return 0;
		}

	cStrW s;

	while ( f.get_line ( &s ) )
	{
		if ( load_sub ( s.cstr(), bReplace ) )
			iQty++;

		s.erase();
	}

	// LOG
	{
		wchar_t t [1000];
		swprintf_s ( t, sizeof(t)/sizeof(wchar_t), L"Scribs loaded from file: %d scribs in %s.", iQty, pPathname );
		LOG ( t );
	}

	return iQty;
}


//-------------------------------------------------------------------------------------------------------
//  REMOVE
//  Remove all entries with specified key.
//-------------------------------------------------------------------------------------------------------
void cScribMgr::remove ( const wchar_t * pKey )
{
	typedef std::multimap<wstring, wstring> tMap;
	typedef std::pair<tMap::iterator, tMap::iterator> tPair ;
	tPair Pair = Map.equal_range ( pKey );
	Map.erase ( Pair.first, Pair.second );
}


#if 0
//-------------------------------------------------------------------------------------------------------
//  INSERT DEFAULT SCRIBS
//  Add a few English scribs to multimap that the program needs in order to display
//  a fatal error when it cannot load a scrip file.
//-------------------------------------------------------------------------------------------------------
void cScribMgr:: insert_emergency_scribs ()
{
	Map.insert ( pair<wstring,wstring> ( L"CannotLoadDefaultScribFile", L"Unable to load default language file." ) );
	Map.insert ( pair<wstring,wstring> ( L"CannotLoadDefaultScribFile", L"Try reinstalling the program." ) );
}
#endif