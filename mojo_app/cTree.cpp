/***********************************************************************************************************************
/*
/*    cTree.cpp / mojo_app
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"
#include "cTree.h"

//======================================================================================================================
//  DATA
//======================================================================================================================


//======================================================================================================================
//  PROTOTYPES
//======================================================================================================================


//======================================================================================================================
//  CODE
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
//  INSERT LEFT
//----------------------------------------------------------------------------------------------------------------------
void cTree :: insert_left ( cTree * pNew )
{
	if ( this->pLeft )
		this->pLeft->pParent = pNew;

	pNew->pLeft = this->pLeft;

	this->pLeft = pNew;

	pNew->pParent = this;
};


//----------------------------------------------------------------------------------------------------------------------
//  APPEND LEFT
//----------------------------------------------------------------------------------------------------------------------
void cTree :: append_left ( cTree * pNew )
{
	cTree * pLast = this;

	while ( pLast->pLeft )
		pLast = pLast->pLeft;

	pLast->pLeft = pNew;
	pNew->pParent = pLast;
}


//----------------------------------------------------------------------------------------------------------------------
//  INSERT RIGHT
//----------------------------------------------------------------------------------------------------------------------
void cTree :: insert_right ( cTree * pNew )
{
	if ( this->pRight)
		this->pRight->pParent = pNew;

	pNew->pRight = this->pRight;

	this->pRight = pNew;

	pNew->pParent = this;
}


//----------------------------------------------------------------------------------------------------------------------
//  APPEND RIGHT
//----------------------------------------------------------------------------------------------------------------------
void cTree :: append_right ( cTree * pNew )
{
	cTree * pLast = this;

	while ( pLast->pRight )
		pLast = pLast->pRight;

	pLast->pRight = pNew;
	pNew->pParent = pLast;
}


//----------------------------------------------------------------------------------------------------------------------
//  COUNT
//----------------------------------------------------------------------------------------------------------------------
size_t cTree :: count () const
{
	unsigned uRetVal = 1;

	if ( pLeft )
		uRetVal += pLeft->count();

	if ( pRight )
		uRetVal += pRight->count();

	return uRetVal;
}


//----------------------------------------------------------------------------------------------------------------------
//  CLONE
//----------------------------------------------------------------------------------------------------------------------
cTree * cTree :: clone_tree () const
{
	cTree * pNew = clone_node();

	if ( pLeft )
		pNew->pLeft = pLeft->clone_tree();

	if ( pRight )
		pNew->pRight = pRight->clone_tree();

	return pNew;   	
}

//----------------------------------------------------------------------------------------------------------------------
//  PRINT
//----------------------------------------------------------------------------------------------------------------------
void cTree :: print ( mojo::cStrW * pRet ) const
{
	*pRet += L"    ";
	*pRet += sName;
	*pRet += L'\n';

	if ( pLeft )
		pLeft->print( pRet );

	if ( pRight )
		pRight->print ( pRet );
}


//----------------------------------------------------------------------------------------------------------------------
//  REMOVE
//----------------------------------------------------------------------------------------------------------------------
void cTree :: remove ()
{
	if ( 0 == pParent )
		return;

	// PRESERVE LEFT LINE OF DESCENT
	else if ( this == pParent->pLeft )
	{
		pParent->pLeft = pLeft;
		if ( pLeft )
		{
			pLeft->pParent = pParent;
			pLeft = 0;
		}
		pParent = 0;
	}

	// PRESERVE RIGHT LINE OF DESCENT
	else if ( this == pParent->pRight )
	{
		pParent->pRight = pRight;
		if ( pRight )
		{
			pRight->pParent = pParent;
			pRight = 0;
		}
		pParent = 0;
	}

	else
		assert(0);
}


//----------------------------------------------------------------------------------------------------------------------
//  DESTRUCTOR
//----------------------------------------------------------------------------------------------------------------------
cTree :: ~cTree ()
{
	if ( pLeft )
	{
		delete pLeft;
		pLeft = 0;
	}

	if ( pRight )
	{
		delete pRight;
		pRight = 0;
	}

	if ( pParent )
	{
		if ( this == pParent->pLeft )
			pParent->pLeft = 0;

		else if ( this == pParent->pRight )
			pParent->pRight = 0;

		pParent = 0;
	}
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