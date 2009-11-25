/********************************************************************************************************
/*
/*    regex.cpp / mojo_engine
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
#include <regex>
#include "mojo_engine.h"

using namespace mojo;
using namespace std::tr1;  // change namespace to boost and link to boost regex library
                           // if your compiler doesn't support regex.

namespace mojo
{


//----------------------------------------------------------------------------------------------------------------------
//  GET NEXT MATCH
//  *ppNext is the haystack
//----------------------------------------------------------------------------------------------------------------------
MOJO_ENGINE_API bool get_next_match ( cStrW aRet[], int iArrayQty, const wchar_t ** ppNext, const wchar_t * pRegex  )
{
	if ( ! ppNext )
		return false;

	if ( ! *ppNext )
		return false;
	
	wregex  rgx ( pRegex );
	wcmatch bmatch;

	if ( regex_search ( *ppNext, bmatch, rgx) )
	{
		for ( int i = 0; i < iArrayQty; i++ )
			aRet[i] =  static_cast<std::wstring>(bmatch[i+1]).c_str();

		*ppNext =  bmatch [ bmatch.size() ] . second;

		return true;
	}

	return false;
}



//-------------------------------------------------------------------------------------------------------
//  MATCH
//-------------------------------------------------------------------------------------------------------
MOJO_ENGINE_API bool match ( cStrW  aRet [], int iRetSize, const wchar_t * pHaystack, const wchar_t * pRgx )
{
	wregex  rgx ( pRgx );
	wcmatch bmatch;

	if ( regex_match ( pHaystack, bmatch, rgx ) )
	{
		for ( int i = 0; i < iRetSize; i++ )
		{
			aRet[i] = (static_cast<std::wstring>(bmatch[i+1])).c_str();
		}

		return true;
	}

	return false;
}
} // namespace