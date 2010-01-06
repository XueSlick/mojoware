/***********************************************************************************************************************
/*
/*    cFigWoWTree.cpp / mojo_app
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"
#include "cFig_define.h"


//======================================================================================================================
//  DATA
//======================================================================================================================

const cFigWoWTree cFigWoWTree::Default;

cFig::sEntry cFigWoWTree::aTable [] =
{
	ENTRY_MEMBERLESS ( WoW, cFigWoW ),
	{ 0, 0, 0 }
};


//======================================================================================================================
//  PROTOTYPES
//======================================================================================================================


//======================================================================================================================
//  CODE
//======================================================================================================================


//----------------------------------------------------------------------------------------------------------------------
//  GET BY HWND
//----------------------------------------------------------------------------------------------------------------------
cFigWoW * cFigWoWTree :: get_by_hwnd ( HWND hwnd ) const
{
	for ( cTree * p = pRight; p; p = p->pRight )
	{
		cFigWoW * w = reinterpret_cast < cFigWoW* > ( p );

		if ( w->hwnd == hwnd )
			return w;
	}

	return NULL;
}


//----------------------------------------------------------------------------------------------------------------------
//  SET FROM XML
//----------------------------------------------------------------------------------------------------------------------
void cFigWoWTree :: set_from_xml ( void * pvDest, const wchar_t * pTxt ) const
{
	cFigRoot * pRoot = reinterpret_cast<cFigRoot*>(pvDest);

	cFigWoWTree * pWoWTree = pRoot->get_wow_tree();

	if ( ! pWoWTree )
	{
		pWoWTree = new cFigWoWTree;
		pRoot->append_left ( pWoWTree );
	}

	pWoWTree->cFig::set_from_xml ( pWoWTree, pTxt );
}


//----------------------------------------------------------------------------------------------------------------------
//  WRITE TO XML
//  Writes the whole element
//----------------------------------------------------------------------------------------------------------------------
void cFigWoWTree :: write_to_xml ( cStrW * pRet, void * pObject, const wchar_t * pTagName ) const
{
	pRet; pObject; pTagName;

	cFigRoot * pRoot = reinterpret_cast<cFigRoot*>(pObject);

	cFigWoWTree * p = (cFigWoWTree *) pRoot->get_by_typeid ( typeid ( cFigWoWTree::Default ) . raw_name() );

	if ( ! p )
		return;

	// const sEntry * pEntry = table();
	// pEntry;

	xml_write_start_tag ( pRet, pTagName );
	*pRet += L'\n';

	for ( cTree * pTree = p->pRight; pTree; pTree = pTree->pRight )
	{
		cFigWoW * pWoW = reinterpret_cast<cFigWoW*>(pTree);

		if ( cFigWoW::launch_by_mojo == pWoW->eOrigin )
			pWoW->write_to_xml ( pRet, pWoW, table()->pwTag );
	}

	xml_write_end_tag ( pRet, pTagName );
	*pRet += L'\n';
}


//----------------------------------------------------------------------------------------------------------------------
//  SET WOW
//----------------------------------------------------------------------------------------------------------------------
#if 0
bool cFigWoWTree:: set_wow ( DWORD dwHandle, cFigWoW * pArg )
{
	bool bRetVal = false;

	List.lock();

	for ( cFigWoW * p = List.pHead; p; p = p->pNext )
	{
		if ( p->dwSerialNumber == dwHandle )
		{
			*p = *pArg;
			bRetVal = true;
			break;
		}
	}
	List.unlock();

	return bRetVal;
}
#endif

//----------------------------------------------------------------------------------------------------------------------
//  CREATE FIG VIEW ITEM LIST
//  Caller should delete the returned object after using it.
//----------------------------------------------------------------------------------------------------------------------
#if 0
cFigViewItemList * cFigWoWTree :: create_fig_view_item_list ()
{
	cFigViewItemList * pList = new cFigViewItemList;

	List.lock();
	for ( cFigWoW * p = List.pHead; p; p = p->pNext )
	{
		pList->append ( p->clone() );
	}
	List.unlock();

	return pList;
}
#endif


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