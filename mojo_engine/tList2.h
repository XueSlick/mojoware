/***********************************************************************************************************************
/*
/*	  tList2.h / mojo_engine
/*
/*    Some of the differences between this template and earlier version, tList, is the iterators 
/*    next(), prev(), etc., and lock() and unlock();
/*
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#pragma once

#ifndef _WIN32_WINNT
#define _WIN32_WINNT  0x500
#endif

#include <winbase.h>
#include <windows.h>
#include <assert.h>

//===============================================================================================
// CLASS tList2
//===============================================================================================
template<class C> class tList2
{

public:

	tList2 ();
	tList2 ( const tList2<C> & rh ); // copy constructor

	tList2<C> & operator= ( const tList2<C> & rh );

   ~tList2 ();

   void lock	() { EnterCriticalSection ( &cs ); }
   void unlock	() { LeaveCriticalSection ( &cs ); }

   // C * get_item  ( char * pcKey );

	void	append			( C * );
	void	insert			( C * );
	bool	remove			( C * );
	// bool	rem_del			( char * pcKey  );  // find  and remove and delete it
	bool	rem_del			( C * );  // find  and remove and delete it


	void	rem_del_all		();
	C *		pop				();
	int		qty				();

	bool	get_by_index	( C * pRet, int iIndex ); // fills in info and returns
	C *		first			() { pLastIter = pHead; return pLastIter; }
	bool	next			( C * pRet, C * pArg );
	C *		prev			( C * pArg );
	C *		next			( C * pArg ); 
	C *     next			();

	// tList2<C> * dupe    (); use copy constructor instead

	int        get_pointers ( C ** pRet, int iSize ); // fills array with pointers to listed C's
	                                                  // caller uses this so can iterate through
	                                                  // links without worrying about another
	                                                  // thread altering the link order

    C * operator[] (unsigned);		// where critical section is required, use
									// get_by_index() instead

	bool is_in_list ( C * rh );

	C *pHead;
	C *pLastIter;

	C * last ();

private:
	CRITICAL_SECTION cs;
};


//===============================================================================================
// CODE
//===============================================================================================

// ----------------------------------------------------------------------------------------------
//  OPERATOR[]
//  where critical section required, use get_by_index instead
// ----------------------------------------------------------------------------------------------
template<class C> C * tList2<C>::operator [] ( unsigned uArg )
{
	unsigned u = 0;

	for ( C * p = pHead; p; p = p->pNext, u++ )
	{
		if ( u == uArg )
			return p;
	}
	
	return NULL;
}


// ----------------------------------------------------------------------------------------------
//  COPY CONSTRUCTOR
// ----------------------------------------------------------------------------------------------
template<class C> tList2<C> :: tList2( const tList2<C> & rh )
{
	InitializeCriticalSectionAndSpinCount(&this->cs, 4000 );

	EnterCriticalSection ( &cs );
	{
		pHead = NULL;

		for ( C * p = rh.pHead; p; p = p->pNext )
		{
			C * n = new C ( *p );
			append ( n );
		}
	}
	LeaveCriticalSection ( &cs );
}

// ----------------------------------------------------------------------------------------------
//  OPERATOR EQUALS
// ----------------------------------------------------------------------------------------------
template<class C> tList2<C> & tList2<C> :: operator= ( const tList2<C> & rh )
{
	EnterCriticalSection ( &cs );
	{
		rem_del_all();

		assert ( 0 == pHead );

		for ( C * p = rh.pHead; p; p = p->pNext )
		{
			C * n = new C ( *p );
			append ( n );
		}
	}
	LeaveCriticalSection ( &cs );

	return * this;
}


// ----------------------------------------------------------------------------------------------
//  GET POINTERS
//  Caller uses this so it can iterate through C's without worrying about another thread
//  altering the linkages in a way that would break the iteration.
//  Returns number of returned pointers.
// ----------------------------------------------------------------------------------------------
template<class C> int tList2<C> :: get_pointers ( C ** ppRet, int iSize )
{
	assert ( ppRet );

	int i = 0;

	EnterCriticalSection ( &cs );
	{
		for ( C * p = pHead; p; p = p->pNext )
		{
			if ( i < iSize )
			{
				ppRet[i] = p;
				i++;
			}
		}
	}
	LeaveCriticalSection ( &cs );

	return i;
}

	
// ------------------------------------------------------------
//  GET PREV
// if next doesn't exist, returns NULL
// pass NULL to get last one
// ------------------------------------------------------------
template<class C> C *  tList2<C> :: prev ( C * pArg )
{
	C * pRet = NULL;

	EnterCriticalSection ( &cs );
	{
		if ( pArg )
		{
			if ( pArg->pPrev )
			{
				pRet = pArg->pPrev;
			}
		}

		else 
		{
			pRet = last();
		}
	}
	LeaveCriticalSection ( &cs );
	
	return pRet;
}



// ------------------------------------------------------------
//  NEXT
// if next doesn't exist, returns NULL
// ------------------------------------------------------------
template<class C> C *  tList2<C> :: next()
{
	C * pRet = NULL;

	EnterCriticalSection ( &cs );
	{
		if ( ! pLastIter )
			pRet = NULL;

		else
		{
			pLastIter = pLastIter->pNext;
			pRet = pLastIter;
		}
	}
	LeaveCriticalSection ( &cs );
	
	return pRet;
}

// ------------------------------------------------------------
//  GET NEXT
// if next doesn't exist, returns NULL
// ------------------------------------------------------------
template<class C> C *  tList2<C> :: next( C * pArg )
{
	C * pRet = NULL;

	EnterCriticalSection ( &cs );
	{
		if ( pArg )
		{
			if ( pArg->pNext )
			{
				pRet = pArg->pNext;
			}
		}

		else if ( pHead )
		{
			pRet = this->pHead;
		}
	}
	LeaveCriticalSection ( &cs );
	
	return pRet;
}


// ------------------------------------------------------------
//  GET NEXT
// if next doesn't exist, returns false
// set pArg = NULL to get first one
// this func is cumbersome, the single-arg versions are more elegant
// ------------------------------------------------------------
template<class C> bool tList2<C> :: next( C * pRet, C * pArg )
{
	bool bRetVal = false;

	EnterCriticalSection ( &cs );
	{
		if ( pArg )
		{
			if ( pArg->pNext )
			{
				*pRet = *(pArg->pNext);
				bRetVal = true;
			}
		}

		else if ( pHead )
		{
			* pRet = *(this->pHead);
			bRetVal = true;
		}

	}
	LeaveCriticalSection ( &cs );
	

	return bRetVal;
}


// ------------------------------------------------------------
//  GET BY INDEX
// ------------------------------------------------------------
template<class C> bool tList2<C> :: get_by_index ( C * pRet, int iIndex ) // fills in info and returns
{
	int iQty = qty();

	if ( (iQty-1) < iIndex )
		return false;

	EnterCriticalSection ( &cs );
	{
		int iCount = 0;

		for ( C * p = pHead; p; p = p->pNext )
		{
			if ( iCount == iIndex )
			{
				*pRet = *p;
				break;
			}

			iCount ++;
		}
	}
	LeaveCriticalSection ( &cs );

	return true;
}


// ------------------------------------------------------------
// IS_IN_LIST
// ------------------------------------------------------------
template<class C> bool tList2<C>:: is_in_list ( C * rh )
{
	EnterCriticalSection ( &cs );
	{
		for ( C * p = pHead; p; p = p->pNext )
		{
			if ( p == rh )  // changed aug 20 2007
				return true;
		}
	}
	LeaveCriticalSection ( &cs );

	return false;

}

// ------------------------------------------------------------
//  DUPE
//  make a copy of this list
// ------------------------------------------------------------
#if 0
template<class C> tList2<C> * tList2<C>::dupe  ( tList2<C> * pDupe )
{
	// tList2<C> * pDupe = new tList2<C>;

	EnterCriticalSection ( &cs );
	{
		int iQty = qty();

		for ( C * p = pHead; p; p = p->pNext )
		{
			C * pNewItem = new C ( *p );
			pDupe->append ( pNewItem );
		}
	}
	LeaveCriticalSection ( &cs );

	return pDupe;
}
#endif

// ------------------------------------------------------------
//  DUPE
//  make a copy of this list
// ------------------------------------------------------------
#if 0
template<class C> tList2<C> * tList2<C>::dupe  ()
{
	tList2<C> * pDupe = new tList2<C>;

	EnterCriticalSection ( &cs );
	{
		int iQty = qty();

		for ( C * p = pHead; p; p = p->pNext )
		{
			C * pNewItem = new C ( *p );
			pDupe->append ( pNewItem );
		}
	}
	LeaveCriticalSection ( &cs );

	return pDupe;
}
#endif


// ------------------------------------------------------------
//  CONSTRUCTOR
// ------------------------------------------------------------
template<class C> tList2<C>::tList2()
{
	if ( 0 == InitializeCriticalSectionAndSpinCount(&this->cs, 4000 ) )
	{

		// LOG_SYSTEM_ERROR_T ( "InitializeCriticalSectionAndSpinCount" );
	}

	pHead = NULL;
}




// ------------------------------------------------------------
//  DESTRUCTOR
// ------------------------------------------------------------
template<class C> tList2<C>::~tList2()
{
	rem_del_all ();
	DeleteCriticalSection ( & cs );
}



// ------------------------------------------------------------
//  QTY
// ------------------------------------------------------------
template<class C> int tList2<C> :: qty ()
{

	int iQty = 0;

	EnterCriticalSection ( &cs );
	{
		for ( C * p = pHead; p; p = p->pNext )
			iQty ++;

	}
	LeaveCriticalSection ( &cs );

	return iQty;
}


// ------------------------------------------------------------
//  REMOVE
// ------------------------------------------------------------
template<class C> bool tList2<C> :: remove ( C * p )
{
	if ( NULL == p )
		return false;

#ifdef _DEBUG
	int iStartQty = qty();
#endif

	EnterCriticalSection ( &cs );
	{
		if ( p->pPrev ) // not first item
			p->pPrev->pNext = p->pNext;

		else  // NULL == p->pPrev ) // this means it's the first item
		{
			pHead = p->pNext;
		}

		if ( p->pNext )
			p->pNext->pPrev = p->pPrev;
	}
	LeaveCriticalSection ( &cs );

#ifdef _DEBUG
	int iEndQty = qty();
	assert ( iStartQty == iEndQty + 1 );
#endif

	return true;
}


// ------------------------------------------------------------
//  POP
// ------------------------------------------------------------
template<class C> C * tList2<C> :: pop ()
{
	C * p;

	EnterCriticalSection ( &cs );
	{
		p = last();

		remove ( p );
	}
	LeaveCriticalSection ( &cs );

	return p;
}


// ------------------------------------------------------------
//  REMOVE AND DELETE EVERYTHING ON THE LIST
// ------------------------------------------------------------
template<class C> void tList2<C>::rem_del_all () // rewritten nov 3 2008
{
	EnterCriticalSection (&cs);
	{
		while ( pHead )
		{
			C * p = pHead;
			pHead = p->pNext;
			delete p;
		}
	}
	LeaveCriticalSection ( &cs );

	assert ( 0 == pHead );
}


#if 0
template<class C> void tList2<C>::rem_del_all ()
{
	int iQty = 0;

	EnterCriticalSection ( & cs );
	{
		for ( C * p = end(); p; p = end() )
			if ( rem_del(p) )
				iQty ++;
	}
	LeaveCriticalSection ( &cs );

	assert ( 0 == pHead );
}
#endif

// ------------------------------------------------------------
//  REMOVE AND DELETE AN ENTRY
// THIS PROBABLY DOES NOT WORK FOR FIRST ITEM
// ------------------------------------------------------------

template<class C> bool tList2<C>::rem_del ( C * pArg )
{
	if ( 0 == pArg )
		return false;

	bool retval = false;

	EnterCriticalSection ( &cs );
	{
		for ( C * p = pHead; p; p = p->pNext )
		{
			if ( pArg == p )
			{
				if ( p->pPrev )
					p->pPrev->pNext = p->pNext;

				else
					pHead = p->pNext;

				if ( p->pNext )
					p->pNext->pPrev = p->pPrev;

				delete p;

				retval = true;

				break;
			}
		}
	}
	LeaveCriticalSection ( &cs );

	return retval;
}
// ------------------------------------------------------------
//  INSERT
// ------------------------------------------------------------
template<class C> void tList2<C>::insert ( C * pNew )
{

	EnterCriticalSection ( &cs );
	{
		pNew->pNext = pHead;
		pNew->pPrev = NULL;
		pHead = pNew;

		if ( pNew->pNext)
			( pNew->pNext->pPrev = pNew );
	}
	LeaveCriticalSection ( &cs );
}


// ------------------------------------------------------------
//  APPEND
// ------------------------------------------------------------
template<class C> void tList2<C>::append ( C * pNew )
{
	EnterCriticalSection ( &cs );
	{
		C * e = last ();

		if ( NULL == e )
			pHead = pNew;

		else
			e->pNext = pNew;

		pNew->pNext = 0;
		pNew->pPrev = e;
	}
	LeaveCriticalSection ( &cs );
}


// ------------------------------------------------------------
//   END -- GET FINAL MACHINE IN LINKED LIST
//          returns 0 if list is empty
// ------------------------------------------------------------
template<class C> C * tList2<C>::last ()
{
	C * p;

	EnterCriticalSection ( & cs );
	{
		p = pHead;

		if (p)
		{
			while (p->pNext)
				p = p->pNext;
		}
	}
	LeaveCriticalSection ( &cs );

	return p;
}

// ------------------------------------------------------------
//   PRINT CONTENTS
// ------------------------------------------------------------
#if 0
template<class C> void tList2<C>::print_contents ( cStrN * pStr   )
{
	for ( C * p = pHead; p; p = p->pNext )
	{
		p->print_contents(pStr);
		*pStr += ("\r\n");
	}
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


