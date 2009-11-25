/***********************************************************************************************************************
/*
/*    tList.h / mojo_engine
/*
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/*    This class is non-intrusive, i.e., doesn't require the contained class
/*    to have any special characteristics
/*
/*    This class is NOT safe for multithreading.
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#pragma once

#include <winbase.h>
#include <windows.h>
#include <assert.h>


//======================================================================================================================
// CLASSES
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
// Node
//----------------------------------------------------------------------------------------------------------------------
template<class C> struct sNodeNI
{
	sNodeNI ( C * p ) : pData ( p ), pNext(0) {}
	sNodeNI () : pData ( 0 ), pNext(0) {}
	~sNodeNI () { if ( pData ) delete pData; }
	sNodeNI<C> * pNext;
	C * pData;
};


//----------------------------------------------------------------------------------------------------------------------
// LIST NON-INTRUSIVE
//----------------------------------------------------------------------------------------------------------------------
template<class C> class tListNI
{

public:

	tListNI () : pHead(0), pLastEnumerated(0), iQty(0) {}
   ~tListNI ();

   void insert ( C * );
   int qty () { return iQty; }

   C * first ();  // for iterating
   C * next  ();  // for iterating

private:

	sNodeNI<C>		*	pLastEnumerated;
	sNodeNI<C>		*	pHead;
	int iQty;
};


//======================================================================================================================
// CODE
//======================================================================================================================

//-----------------------------------------------------------------------------------------------
// DESTRUCTOR
//-----------------------------------------------------------------------------------------------
template<class C> tListNI<C> :: ~tListNI ()
{
	while ( pHead )
	{
		sNodeNI<C> * p = pHead;
		pHead = p->pNext;
		delete p;
	}
}



//-----------------------------------------------------------------------------------------------
// FIRST
//-----------------------------------------------------------------------------------------------
template<class C> C * tListNI<C>::first ()
{
	if ( pHead )
	{
		pLastEnumerated = pHead;
		return pLastEnumerated->pData;
	}

	else
	{
		pLastEnumerated = 0;
		return 0;
	}
}


//-----------------------------------------------------------------------------------------------
// NEXT
//-----------------------------------------------------------------------------------------------
template<class C> C * tListNI<C> :: next ()
{
	if ( pLastEnumerated )
		pLastEnumerated = pLastEnumerated->pNext;

	// consecutive if's are deliberate

	if ( pLastEnumerated )  // consecutive if's are deliberate
		return pLastEnumerated->pData;

	else
		return 0;
}


//-----------------------------------------------------------------------------------------------
// INSERT
//-----------------------------------------------------------------------------------------------
template<class C> void tListNI<C>::insert( C * d )
{
	sNodeNI<C> * n = new sNodeNI<C>( d );
	n->pNext = pHead;
	pHead = n;
	iQty++;
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