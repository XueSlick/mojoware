/********************************************************************************************************
/*
/*	  tList.cpp / mojo_engine
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

#pragma once



#include "mojo_engine.h"
#include "tList.h"

namespace mojo
{
	
#if 0
	
//-------------------------------------------------------------------------------------------------------
// tLIST
//-------------------------------------------------------------------------------------------------------
template<class C> class MOJO_ENGINE_API tList
{
public:

	tList ();
	tList ( tList<C> & rh ); // copy constructor

	tList<C> & operator= ( tList<C> & rh );

   ~tList ();

   

   C * get_item  ( char * pcKey );

	void append      ( C * );
	void insert      ( C * );
	bool remove      ( C * );
	bool rem_del     ( char * pcKey  );  // find  and remove and delete it
	bool rem_del     ( C * );  // find  and remove and delete it


	void rem_del_all ();
	C *  pop         ();
	int  qty         ();

	bool get_by_index ( C * pRet, int iIndex ); // fills in info and returns
	C * first    () { return pHead; }
	bool get_next     ( C * pRet, C * pArg );
	C * get_prev     ( C * pArg );
	C * get_next     ( C * pArg = NULL ); 

	// tList<C> * dupe    (); use copy constructor instead
	// tList<C> * dupe   ( tList<C> * pDupe );  // use copy constructor instead

	int        get_pointers ( C ** pRet, int iSize ); // fills array with pointers to listed C's
	                                                 // caller uses this so can iterate through
	                                                 // links without worrying about another
	                                                 // thread altering the link order

    C * operator[] (int);  // where critical section is required, use
	                           // get_by_index() instead

	bool is_in_list ( C * rh );
	// void print_contents ( cStrN * s );
	C *pHead;

	C * end ();

private:
	CRITICAL_SECTION cs;
};

#endif


//===============================================================================================
// CODE
//===============================================================================================


#if 0
// ----------------------------------------------------------------------------------------------
//  COPY CONSTRUCTOR
// ----------------------------------------------------------------------------------------------
template<class C> tList<C> :: tList( tList<C> & rh )
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
template<class C> tList<C> & tList<C> :: operator= ( tList<C> & rh )
{
	EnterCriticalSection ( &cs );
	{
		rem_del_all();

		pHead = NULL;

		for ( C * p = rh.pHead; p; p = p->pNext )
		{
			C * n = new C ( *p );
			append ( n );
		}
	}
	LeaveCriticalSection ( &cs );

	return *this;
}


// ----------------------------------------------------------------------------------------------
//  GET POINTERS
//  Caller uses this so it can iterate through C's without worrying about another thread
//  altering the linkages in a way that would break the iteration.
//  Returns number of returned pointers.
// ----------------------------------------------------------------------------------------------
template<class C> int tList<C> :: get_pointers ( C ** ppRet, int iSize )
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
template<class C> C *  tList<C> :: get_prev ( C * pArg )
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
			pRet = end();
		}
	}
	LeaveCriticalSection ( &cs );
	
	return pRet;
}


// ------------------------------------------------------------
//  GET NEXT
// if arg is zero, returns head
// if next doesn't exist, returns NULL
// ------------------------------------------------------------
template<class C> C *  tList<C> :: get_next( C * pArg )
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
template<class C> bool tList<C> :: get_next( C * pRet, C * pArg )
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
template<class C> bool tList<C> :: get_by_index ( C * pRet, int iIndex ) // fills in info and returns
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
template<class C> bool tList<C>:: is_in_list ( C * rh )
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
template<class C> tList<C> * tList<C>::dupe  ( tList<C> * pDupe )
{
	// tList<C> * pDupe = new tList<C>;

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
template<class C> tList<C> * tList<C>::dupe  ()
{
	tList<C> * pDupe = new tList<C>;

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
template<class C> tList<C>::tList()
{
	if ( 0 == InitializeCriticalSectionAndSpinCount(&this->cs, 4000 ) )
	{
		int x = 3;
		// LOG_SYSTEM_ERROR_T ( "InitializeCriticalSectionAndSpinCount" );
	}

	pHead = NULL;
}




// ------------------------------------------------------------
//  DESTRUCTOR
// ------------------------------------------------------------
template<class C> tList<C>::~tList()
{
	rem_del_all ();
	DeleteCriticalSection ( & cs );
}



// ------------------------------------------------------------
//  QTY
// ------------------------------------------------------------
template<class C> int tList<C> :: qty ()
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
template<class C> bool tList<C> :: remove ( C * p )
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
template<class C> C * tList<C> :: pop ()
{
	C * p;

	EnterCriticalSection ( &cs );
	{
		p = end();

		remove ( p );
	}
	LeaveCriticalSection ( &cs );

	return p;
}


// ------------------------------------------------------------
//  REMOVE AND DELETE EVERYTHING ON THE LIST
// ------------------------------------------------------------
template<class C> void tList<C>::rem_del_all () // rewritten nov 3 2008
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
template<class C> void tList<C>::rem_del_all ()
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

template<class C> bool tList<C>::rem_del ( C * pArg )
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
template<class C> void tList<C>::insert ( C * pNew )
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
template<class C> void tList<C>::append ( C * pNew )
{
	EnterCriticalSection ( &cs );
	{
		C * e = end();

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
template<class C> C * tList<C>::end ()
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
template<class C> void tList<C>::print_contents ( cStrN * pStr   )
{
	for ( C * p = pHead; p; p = p->pNext )
	{
		p->print_contents(pStr);
		*pStr += ("\r\n");
	}
}
#endif

#endif

} // namespace
