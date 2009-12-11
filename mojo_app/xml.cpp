/***********************************************************************************************************************
/*
/*    xml.cpp / mojo_app
/*
/*    This file implements a small subset for xml for the .config.txt files. 
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"
#include <regex>

using namespace mojo;
using namespace std::tr1;

//======================================================================================================================
//  CODE
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
//  XML GET NEXT ELEMENT
//  This is a partial implementation.
//  Attribute isn't implemented
//  Doesn't work right with nested elements that have same tag name  
//----------------------------------------------------------------------------------------------------------------------
bool xml_get_next_element ( mojo::cStrW * pRetName, mojo::cStrW * pRetAttribute, mojo::cStrW * pRetContent, const wchar_t ** ppHaystack )
{
	UNREFERENCED_PARAMETER ( pRetAttribute );

	assert (  ppHaystack );
	assert ( *ppHaystack );

	wregex rgx1 ( L"<(.*?)>" );
	wcmatch match1;

	if ( ! regex_search ( *ppHaystack, match1, rgx1 ) )
	{
		return false;
	}

	*pRetName = (static_cast<std::wstring>(match1[1])).c_str();

	cStrW sRgx;
	sRgx.f ( L"<%s>(.*?)(</%s>)", pRetName->cstr(), pRetName->cstr() );
	wregex rgx2 ( sRgx.cstr() );
	wcmatch match2;

	if ( ! regex_search ( *ppHaystack, match2, rgx2 ) )
	{
		return false;
	}
		
	*pRetContent = static_cast<std::wstring>(match2[1]).c_str();
	*ppHaystack = (wchar_t*)match2[2].second;

	return true;
}



//----------------------------------------------------------------------------------------------------------------------
//  XML NAME TO CONTENT
//----------------------------------------------------------------------------------------------------------------------
bool xml_name_to_content ( mojo::cStrW * pRet, const wchar_t ** ppHaystack, const wchar_t * pName )
{
	assert (  ppHaystack );
	assert ( *ppHaystack );
	assert (  pName );

	cStrW sRgx;
	sRgx.f ( L"<%s>(.*)(</%s>)", pName, pName );

	wregex re ( sRgx.cstr() );
	wcmatch match;

	const wchar_t * p = *ppHaystack;

	if ( regex_search ( p, match, re ) )
	{
		std::wstring matchstr = static_cast<std::wstring>(match[1]);

		*ppHaystack = (wchar_t*)match[2].second;
		// *ppHaystack =  match [ match.size() ] . second;
		*pRet = matchstr.c_str(); 

		return true;
	}

	return false;
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

