/***********************************************************************************************************************
/*
/*    cConfigRoot.cpp / mojo_app
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"
#include "cFig_define.h"


//======================================================================================================================
//  DATA
//======================================================================================================================

const cFigRoot cFigRoot::Default;

cFig::sEntry cFigRoot::aTable [] =
{
	ENTRY_MEMBERLESS ( WoWTree, cFigWoWTree ),
	{ 0, 0, 0 }
};


//======================================================================================================================
//  PROTOTYPES
//======================================================================================================================


//======================================================================================================================
//  CODE
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
//  APPEND WOW
//----------------------------------------------------------------------------------------------------------------------
void cFigRoot :: append_wow ( cFigWoW * pWoW )
{
	cFigWoWTree * t = get_wow_tree ();

	if ( ! t )
		return;

	else
		t->append_right ( pWoW );
}


//----------------------------------------------------------------------------------------------------------------------
//  GET WOW TREE
//----------------------------------------------------------------------------------------------------------------------
cFigWoWTree * cFigRoot :: get_wow_tree ()
{
	// memory leak

	const char * pRawName = typeid ( cFigWoWTree::Default ) . raw_name ();

	return reinterpret_cast<cFigWoWTree*> ( get_by_typeid ( pRawName ) ); // "class cFigWoWTree" ) );

#if 0
	for ( cTree * t = pLeft; t; t = t->pLeft )
	{
		const char * n = typeid(*t).name();

		if ( 0 == strcmp ( "class cFigWoWTree", n ) )
			return reinterpret_cast<cFigWoWTree*>(t);
	}

	return 0;
#endif
}


//----------------------------------------------------------------------------------------------------------------------
//  CONSTRUCTOR
//----------------------------------------------------------------------------------------------------------------------
cFigRoot :: cFigRoot ()
{
	cFigWoWTree * p = new cFigWoWTree;
	append_left ( p );
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