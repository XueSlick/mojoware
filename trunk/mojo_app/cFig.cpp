/***********************************************************************************************************************
/*
/*    cFig.cpp / mojo_app
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"

//======================================================================================================================
//  DATA
//======================================================================================================================

const cFigUnsigned cFigUnsigned::Default;
const cFigStr      cFigStr     ::Default;

//======================================================================================================================
//  PROTOTYPES
//======================================================================================================================

//======================================================================================================================
//  CODE
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
//  GET BY TYPEID
//  Use raw_name() instead of name() because (1) it's faster and (2) name() causes a memory leak in Visual Studio 2010.
//----------------------------------------------------------------------------------------------------------------------
cFig * cFig :: get_by_typeid ( const char * pRawName )
{
	if ( 0 == strcmp ( typeid(*this).raw_name(), pRawName ) )
		return this;

	if ( pLeft )
	{
		if ( cFig * p = (reinterpret_cast<cFig*>(pLeft))->get_by_typeid ( pRawName ) )
			return p;
	}

	if ( pRight )
	{
		if ( cFig * p = (reinterpret_cast<cFig*>(pRight))->get_by_typeid ( pRawName ) )
			return p;
	}

	return NULL;
}


//----------------------------------------------------------------------------------------------------------------------
//  GET BY HANDLE
//----------------------------------------------------------------------------------------------------------------------
cFig * cFig :: get_by_handle ( DWORD dwHandle )
{
	if ( dwSerialNumber == dwHandle )
		return this;

	if ( pLeft )
	{
		if ( cFig * p = (reinterpret_cast<cFig*>(pLeft))->get_by_handle ( dwHandle ) )
			return p;
	}

	if ( pRight )
	{
		if ( cFig * p = (reinterpret_cast<cFig*>(pRight))->get_by_handle ( dwHandle ) )
			return p;
	}

	return NULL;
}


//----------------------------------------------------------------------------------------------------------------------
//  SET FROM XML -- UNSIGNED
//----------------------------------------------------------------------------------------------------------------------
void cFigUnsigned :: set_from_xml ( void * pvDest, const wchar_t * pTxt ) const
{
	unsigned * puDest = reinterpret_cast<unsigned*>(pvDest);

	wchar_t * pwStop;
	*puDest = wcstoul ( pTxt, &pwStop, 10 ); 
}


//----------------------------------------------------------------------------------------------------------------------
//  STRING -- SET FROM XML
//----------------------------------------------------------------------------------------------------------------------
void cFigStr :: set_from_xml ( void * pvDest, const wchar_t * pTxt ) const
{
	cStrW * pDest = reinterpret_cast<cStrW*>(pvDest);
	*pDest = pTxt;
}


//----------------------------------------------------------------------------------------------------------------------
//  STRING -- WRITE TO XML
//----------------------------------------------------------------------------------------------------------------------
void cFigStr :: write_to_xml ( mojo::cStrW * pRet, void * pObject, const wchar_t * pTagName ) const
{
	assert ( pObject );
	cStrW * pSource = reinterpret_cast<cStrW*>(pObject);
	if ( 0 == pSource->len() )
		return;
	xml_write_start_tag ( pRet, pTagName );
	*pRet += *pSource;
	xml_write_end_tag ( pRet, pTagName );
	*pRet += L'\n';
}


//----------------------------------------------------------------------------------------------------------------------
//  STRING -- FIG TO CTRL -- STRING
//----------------------------------------------------------------------------------------------------------------------
void cFigStr :: fig_to_ctrl ( HWND hwnd, void * pVar ) const
{
	cStrW * pStr = (cStrW*) pVar;
	SetWindowText ( hwnd, pStr->cstr() );
}


//----------------------------------------------------------------------------------------------------------------------
//  STRING -- CTRL TO FIG
//----------------------------------------------------------------------------------------------------------------------
void cFigStr :: ctrl_to_fig ( void * pVar, HWND hwnd ) const
{
	wchar_t b [ 8092 ];
	Edit_GetText ( hwnd, b, sizeof(b)/sizeof(wchar_t) );
	*(cStrW*)pVar = b;
}

//----------------------------------------------------------------------------------------------------------------------
// NAME TO ENTRY
//----------------------------------------------------------------------------------------------------------------------

const cFig::sEntry * cFig :: name_to_entry ( const wchar_t * pTxt ) const
{
	for ( const cFig::sEntry * pE = table(); pE->pwTag; pE++ )
	{
		if ( 0 == wcscmp ( pTxt, pE->pwTag ) )
			return ( pE );	
	}

	return 0;
}


//----------------------------------------------------------------------------------------------------------------------
//  WRITE TO XML
//  Writes an element
//----------------------------------------------------------------------------------------------------------------------
void cFig :: write_to_xml ( cStrW * pRet, void * pObject, const wchar_t * pTagName ) const
{
	assert ( pObject );

	if ( pTagName )
	{
		xml_write_start_tag ( pRet, pTagName );
		*pRet += L'\n';
	}

	for ( const cFig::sEntry * pEntry = table(); pEntry->pwTag; pEntry++ )
	{
		pEntry->pDefaultFig->write_to_xml ( pRet, ( (BYTE*)pObject ) + pEntry->iOffset, pEntry->pwTag );
	}

	if ( pTagName )
	{
		xml_write_end_tag ( pRet, pTagName );
		*pRet += L'\n';
	}
}


//----------------------------------------------------------------------------------------------------------------------
//  SET FROM XML
//----------------------------------------------------------------------------------------------------------------------
void cFig :: set_from_xml ( void * pvDest, const wchar_t * pTxt ) const
{
	assert ( pvDest );

	const wchar_t * pNext = pTxt;

	cStrW sName, sAttribute, sContent;

	while ( xml_get_next_element ( &sName, &sAttribute, &sContent, &pNext ) )
	{
		const sEntry * p = name_to_entry ( sName.cstr() );

		if ( ! p )
		{
			assert(p);
			continue;
		}

		p->pDefaultFig->set_from_xml ( ( (BYTE*)pvDest ) + p->iOffset, sContent.cstr() );
	}
}


//----------------------------------------------------------------------------------------------------------------------
//  CONSTRUCTOR
//----------------------------------------------------------------------------------------------------------------------
cFig :: cFig () : dwSerialNumber ( ++cFigMgr::dwLastSerialNumberAssigned )
{
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