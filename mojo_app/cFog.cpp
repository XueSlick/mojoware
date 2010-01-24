/***********************************************************************************************************************
/*
/*    cFog.cpp / mojo_app
/*   
/*    Copyright 2010 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"

//======================================================================================================================
//  DATA
//======================================================================================================================

DWORD cFog :: dwLastSerialNumberAssigned = 0;

//======================================================================================================================
//  PROTOTYPES
//======================================================================================================================

//======================================================================================================================
//  CODE
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
//  GET BY HANDLE
//----------------------------------------------------------------------------------------------------------------------
cFog * cFog :: get_by_handle ( DWORD dwHandle )
{
	if ( dwSerialNumber == dwHandle )
		return this;

	else
		return 0;
}


//----------------------------------------------------------------------------------------------------------------------
//  GET BY HANDLE
//----------------------------------------------------------------------------------------------------------------------
cFog * cFogTree :: get_by_handle ( DWORD dwHandle )
{
	//--------------------------------
	// THIS
	//--------------------------------

	if ( dwSerialNumber == dwHandle )
		return this;

	//--------------------------------
	// MEMBERS
	//--------------------------------

	else for ( const cFogTree::sEntry * pE = table(); pE->pwTag; pE++ )
	{
		if ( 0 == pE->pfCreate )
		{
			cFog * pEl = reinterpret_cast<cFog*>( (BYTE*)this + pE->iOffset );

			cFog * pRet = pEl->get_by_handle ( dwHandle );

			if ( pRet )
				return pRet;
		}
	}

	//--------------------------------
	// CHILDREN
	//--------------------------------

	if ( pLeft )
	{
		if ( cFog * p = (static_cast<cFogTree*>(pLeft))->get_by_handle ( dwHandle ) )
			return p;
	}

	if ( pRight )
	{
		if ( cFog * p = (static_cast<cFogTree*>(pRight))->get_by_handle ( dwHandle ) )
			return p;
	}

	return NULL;
}


//----------------------------------------------------------------------------------------------------------------------
// SET FROM XML
//----------------------------------------------------------------------------------------------------------------------
void cFogTree :: set_from_xml ( const wchar_t * pTxt )
{
	const wchar_t * pNext = pTxt;

	cStrW sName, sAttribute, sContent;

	int iQtyChildren = 0;

	while ( xml_get_next_element ( &sName, &sAttribute, &sContent, &pNext ) )
	{
		const sEntry * e = name_to_entry ( sName.cstr() );

		if ( ! e )
		{
			assert(e);
			continue;
		}

		cFog * pEl = 0;

		if ( 0 == e->pfCreate ) // MEMBER
			pEl = reinterpret_cast<cFog*>( (BYTE*)this + e->iOffset );

		else // /NEW NODE IN TREE
		{
			pEl = e->pfCreate();

			iQtyChildren++;

			if ( 1 == iQtyChildren )
				append_right ( (cFogTree*) pEl );

			else
				pRight->append_left ( (cFogTree*) pEl );
		}
			
		pEl->set_from_xml ( sContent.cstr() );
	}
}


//----------------------------------------------------------------------------------------------------------------------
// NAME TO ENTRY
//----------------------------------------------------------------------------------------------------------------------

const cFogTree::sEntry * cFogTree :: name_to_entry ( const wchar_t * pTxt ) const
{ 
	for ( const cFogTree::sEntry * pE = table(); pE->pwTag; pE++ )
	{
		if ( 0 == wcscmp ( pTxt, pE->pwTag ) )
			return ( pE );	
	}

	return 0;
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