/***********************************************************************************************************************
/*
/*    xml.cpp / mojo_app
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"
#include <regex>

using namespace mojo;
using namespace std::tr1;

#if 0
bool get_next_match ( cStrW aRet[], int iArrayQty, const wchar_t * pRegex, const wchar_t ** ppNext )
{
	if ( ! ppNext )
		return false;

	if ( ! *ppNext )
		return false;
	
	boost::wregex  rgx ( pRegex );
	boost::wcmatch bmatch;

	if ( boost::regex_search ( *ppNext, bmatch, rgx) )
	{
		for ( int i = 0; i < iArrayQty; i++ )
			aRet[i] =  static_cast<std::wstring>(bmatch[i+1]).c_str();

		*ppNext =  bmatch [ bmatch.size() ] . second;

		return true;
	}

	return false;
}
#endif

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




#if 0


/*********************************************************************************
/*
/*   xml.cpp
/*
/*  adapted for Unicode October 13, 2008
/*   started 12/23/2005
/*
/*********************************************************************************/

#include "stdafx.h"

using namespace mojo;
using namespace std::tr1;

/************************************************************************************/
/*** PROTOTYPES *********************************************************************/
/************************************************************************************/

bool xml_key_to_val_sub  ( cStrW * pValue, wchar_t * pcSearchMe, wchar_t * pcKey );
bool xml_key_to_val_iter ( cStrW * pValue, wchar_t ** ppcSearchMe, wchar_t * pcKey );
bool xml_key_to_val      ( cStrW * pValue, wchar_t *    pSearchMe, wchar_t * pcKey );


/************************************************************************************/
/*** CODE ***************************************************************************/
/************************************************************************************/


#if 0
cStrW sXmlTest = 
"<one>"
	"<two>"
		"<three>text_three_a</three>"
		"<three>text_three_b</three>"
	"</two>"
"</one>";
#endif



/* ----------------------------------------------------------------------------------
   XML KEY TO VAL - this is the basic one you normally call...
                    it handles only non-nested, non-iterative keys
   ---------------------------------------------------------------------------------*/
bool xml_key_to_val ( cStrW * pValue, wchar_t * pcSearchMe, wchar_t * pcKey )
{
	return ( xml_key_to_val_sub ( pValue, pcSearchMe, pcKey ) );
}


/* ----------------------------------------------------------------------------------
   XML KEY TO VAL NESTED
                    this handles both single (non-nested) keys and nested
					keys (nested = x/y/z/blah)
   ---------------------------------------------------------------------------------*/
bool xml_key_to_val_nested ( cStrW * pValue, wchar_t * pcSearchMe, wchar_t * pcKey )
{
	// this handles nested keys of form key1/key2/key3 and also simple keys

	// first, let's see if we have nested keys

	wregex rex ( L"(.*?)\\\\(.*)" );
	wcmatch match;

	if ( regex_search ( pcKey, match, rex ))  // do we have nested keys?
	{
		std::wstring key1 = static_cast<std::wstring>(match[1]);
		std::wstring keyR = static_cast<std::wstring>(match[2]);

		wchar_t * pcKey1 = (wchar_t*)key1.c_str();
		wchar_t * pcKeyR = (wchar_t*)keyR.c_str();

		xml_key_to_val_sub ( pValue, pcSearchMe, pcKey1 ); // get value for 1st key

		return ( xml_key_to_val ( pValue, pValue->cstr(), pcKeyR ) ); // call this iteratively
	}

	else // nope, no nested keys
		return ( xml_key_to_val_sub ( pValue, pcSearchMe, pcKey ) );
}


/* ----------------------------------------------------------------------------------
   XML KEY TO VAL SUB - this one doesn't handle nested keys
   ---------------------------------------------------------------------------------*/
bool xml_key_to_val_sub ( cStrW * pValue, wchar_t * pcSearchMe, wchar_t * pcKey )
{
	// this doesn't handle nested keys of form key1/key2/key3

	cStrW re_parm;
    //   I had to change this jan 13 2006 to cover cases like
	//    <Error blah=blahblah>  and not just <Error>

	re_parm += L"<";
	re_parm += pcKey;
	re_parm += L"\\s*?.*?>"; // new 1/13/2006
	re_parm += L"(.*?)";
	re_parm += L"</" ;
	re_parm += pcKey;
	re_parm += L">" ;

	wregex re ( re_parm.cstr() );

	wcmatch match;

	wchar_t * p = pcSearchMe;

	if ( regex_search ( p, match, re ) )
	{
		std::wstring matchstr = static_cast<std::wstring>(match[1]);

		*pValue = matchstr.c_str(); 

		return true;
	}

	else
	{
		pValue->erase();
		return false;
	}
}


/* ----------------------------------------------------------------------------------
   XML KEY TO VAL ITER
   ---------------------------------------------------------------------------------*/
bool xml_key_to_val_iter ( cStrW * pValue, wchar_t ** ppcSearchMe, wchar_t * pcKey )
{

	// this doesn't handle nested keys of form key1/key2/key3

	if ( NULL == *ppcSearchMe )
		return false;

	if ( ! pcKey )
		return false;

	assert ( pValue );



	cStrW re_parm;

	re_parm += L"<";
	re_parm += pcKey;
	re_parm += L">";
	re_parm += L"(.*?)";
	re_parm += L"</" ;
	re_parm += pcKey;
	re_parm += L">" ;

	wregex re ( re_parm.cstr() );

	wcmatch match;

	wchar_t * p = *ppcSearchMe;

	if ( regex_search ( p, match, re ) )
	{
		std::wstring matchstr = static_cast<std::wstring>(match[1]);

		*ppcSearchMe = (wchar_t*)match[1].second;

		*pValue = matchstr.c_str(); 

		return true;
	}

	return false;
}


/* ----------------------------------------------------------------------------------
   XML ENCODE - replace e.g. & with &amp;
   ---------------------------------------------------------------------------------*/
void xml_encode ( cStrW * arg )
{
	cStrW n ( arg->len() );

	wchar_t * p = arg->cstr();

	if ( ! p )
	{
		assert(0);
		return;
	}

	for ( ; *p ; p++ )
	{
		switch ( *p )
		{
		case '&' :
			if ( *(p+1) == ' ' )
				n += L"&amp;";
			break;
		default:
			n += *p;
		}
	}

	*arg = n;
}

#endif /////////////////////////////////////////////////////

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

