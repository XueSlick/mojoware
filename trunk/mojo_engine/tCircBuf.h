/********************************************************************************************************
/*
/*    tCircBuf.h / mojo_engine
/*
/*    Circular buffer.  Threadsafe (protected by critical section).  The class in a nutshell:
/*
/*        -- The buffer is empty when *pHead == *pTail.
/*
/*        -- It's full when pHead - pTail == 1
/*  
/*        -- pHead == empty position ready for writing.
/*
/*        -- pTail == filled position ready for reading.
/*
/*    A unit test is included at bottom of file
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

#include <windows.h>
#include <winbase.h>
#include <assert.h>

namespace mojo
{
	
//-------------------------------------------------------------------------------------------------------
//  TEMPLATE CIRCULAR BUFFER
//-------------------------------------------------------------------------------------------------------	
template<class C> class tCircBuf
{
public:

	tCircBuf ( int iArgMaxQty = 127 );

	~tCircBuf ();

	C * next_ref (); // puts "null" and returns pointer so caller can write the data; risky

	void put ( C  );  // add something at head
	void put ( const C * );

	bool get  ( C  *); // remove something from tail, i.e, FIFO like a circular buffer
	bool pop  ( C  *); // remove something from head, i.e. LIFO like a stack
	C * get(); // returns pointer; risky

	void clear (); // set head and tail equal so it appears
	               // nothing is in the Buf and qty() will
	               // return zero; equivalent of removing
	               // everything

    C operator[] (int); 
	
	int qty ();

#ifdef _test_tCircBuf
	bool test_state ( int iSizeArg, int iMaxQtyArg, int iQty );
#endif

private:
	CRITICAL_SECTION cs;

	void inc ( C ** pp );
	void dec ( C ** pp );

	int iSize;   // of buffer
	int iMaxQty; // one less than buffer
	C * pBuf;
	C * pHead;
	C * pTail;
};


//-------------------------------------------------------------------------------------------------------
//   OPERATOR []
// this is unsafe ... could look like it's
// returning valid value of zero when it's
// acually trying to indicate error with NULL
// this would only work if it returns a pointer
//-------------------------------------------------------------------------------------------------------
template<class C> C tCircBuf<C>::operator[] ( int n )
{
	C *p;

	EnterCriticalSection ( &cs );
	{
		if ( (n<0) || (qty() <= n) )
			p = NULL;

		else
		{
			// First item in index order was the earliest
			// one put in, which is the tail

			int iTailIndex = pTail - pBuf;

			int iOurIndex = (iTailIndex + n) % iSize;
			p = pBuf + iOurIndex;
		}
	}
	LeaveCriticalSection ( &cs );

	if ( p )
		return *p;
	else
		return NULL;
}

//-------------------------------------------------------------------------------------------------------
//   CLEAR
//-------------------------------------------------------------------------------------------------------
template<class C> void tCircBuf<C>::clear ()
{
 	pHead = pTail = pBuf; 
}


//-------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR
//-------------------------------------------------------------------------------------------------------
template<class C> tCircBuf<C>::tCircBuf ( int iArgMaxQty )
{
	InitializeCriticalSectionAndSpinCount(&this->cs, 4000 );
	iMaxQty = iArgMaxQty; 
	iSize = 1+iArgMaxQty; 
	pBuf = new C[iSize]; 
	pHead = pTail = pBuf; 
}


//-------------------------------------------------------------------------------------------------------
//   DESTRUCTOR
//-------------------------------------------------------------------------------------------------------
template<class C> tCircBuf<C>::~tCircBuf()
{
	delete[] pBuf;
	DeleteCriticalSection ( & cs );
}


//-------------------------------------------------------------------------------------------------------
//   INC
//-------------------------------------------------------------------------------------------------------
template<class C> void tCircBuf<C>:: inc ( C ** pp )
{
	EnterCriticalSection ( &cs );
	{
		(*pp)++;

		if ( ( pBuf + iSize ) <= (*pp) )
			*pp = pBuf;
	}
	LeaveCriticalSection ( &cs );
}


//-------------------------------------------------------------------------------------------------------
//   QTY
//-------------------------------------------------------------------------------------------------------
template<class C> int tCircBuf<C>:: qty ()
{
	int iRetVal;

	EnterCriticalSection ( &cs );
	{
		iRetVal = (int ) ( pHead - pTail );

		if ( iRetVal < 0 )
		{
			iRetVal += iSize; // put this in crit section in case resizing is added to class in future
		}
	}
	LeaveCriticalSection ( &cs );

	return iRetVal;
}

//-------------------------------------------------------------------------------------------------------
//   NEXT REF
//-------------------------------------------------------------------------------------------------------
template<class C> C * tCircBuf<C>:: next_ref ()
{
	C * pRetVal = NULL;

	EnterCriticalSection ( &cs );
	{
		if ( iMaxQty <= qty() )
			inc ( &pTail ); // throw away oldest value

		pRetVal = pHead;
		// *pHead = Arg ;

		inc ( &pHead );
	}
	LeaveCriticalSection ( &cs );

	return pRetVal;
}


//-------------------------------------------------------------------------------------------------------
//   PUT
//-------------------------------------------------------------------------------------------------------
template<class C> void tCircBuf<C>:: put ( C Arg )
{
	EnterCriticalSection ( &cs );
	{
		if ( iMaxQty <= qty() )
			inc ( &pTail ); // throw away oldest value

		*pHead = Arg ;
		inc ( &pHead );
	}
	LeaveCriticalSection ( &cs );
}


//-------------------------------------------------------------------------------------------------------
//   PUT
//-------------------------------------------------------------------------------------------------------
template<class C> void tCircBuf<C>:: put ( const C * pArg )
{
	EnterCriticalSection ( &cs );
	{
		if ( iMaxQty <= qty() )
			inc ( &pTail ); // throw away oldest value

		*pHead = *pArg;
		inc ( &pHead );
	}
	LeaveCriticalSection ( &cs );
}

//-------------------------------------------------------------------------------------------------------
//   GET
//-------------------------------------------------------------------------------------------------------
template<class C> C * tCircBuf<C>::  get ()
{
	C * pRetVal = NULL;

	EnterCriticalSection ( &cs );
	{
		if ( 0 < qty() )
		{
			pRetVal = pTail;
			inc ( &pTail );
		}
	}
	LeaveCriticalSection ( &cs );

	return pRetVal;
}

//-------------------------------------------------------------------------------------------------------
//   GET
//-------------------------------------------------------------------------------------------------------
template<class C> bool tCircBuf<C>::  get ( C * pRet )
{
	bool bRetVal;

	EnterCriticalSection ( &cs );
	{
		if ( 0 < qty() )
		{
			*pRet = *pTail;
			inc ( &pTail );
			bRetVal = true;
		}

		else
			bRetVal = false;
	}
	LeaveCriticalSection ( &cs );

	return bRetVal;
}


//-------------------------------------------------------------------------------------------------------
//   DEC
//   NEW JUNE 13 2009
//-------------------------------------------------------------------------------------------------------
template<class C> void tCircBuf<C>:: dec ( C ** pp )
{
	EnterCriticalSection ( &cs );
	{
			if ( *pp == pBuf )
				*pp = pBuf + iSize;

			else
				(*pp)--;

	}
	LeaveCriticalSection ( &cs );
}


//-------------------------------------------------------------------------------------------------------
//   POP
//   new june 13 2009
//-------------------------------------------------------------------------------------------------------
template<class C> bool tCircBuf<C>::  pop ( C * pRet )
{
	bool bRetVal;

	EnterCriticalSection ( &cs );
	{
		if ( pTail == pHead )
			bRetVal = false;

		else
		{
			dec ( &pHead );
			*pRet = *pHead;
			bRetVal = true;
		}
	}
	LeaveCriticalSection ( &cs );

	return bRetVal;
}


/*******************************************************************************************************/
/*
/*    UNIT TEST FOR CLASS tCIRCBUF
/* 
/*    directions for use:
/* 
/*       -- #define _test_tCircBuf in a source file;
/*       -- create an object such as tCircList<int> cb;
/*       -- check to see whether cb.test_tCircBuf() is true
/* 
/*******************************************************************************************************/

#ifdef _test_tCircBuf

//-------------------------------------------------------------------------------------------------------
//   TEST 
//-------------------------------------------------------------------------------------------------------

bool test_tCircBuf ();

template<class C> bool tCircBuf<C>::test_state ( int iMaxQtyArg, int iSizeArg, int iQtyArg )
{
	// check ptrs not out of bounds
	{
		int iDiff;

		if ( pTail <= pHead )
			iDiff = pHead - pTail;

		else
			iDiff = pTail - pHead;

		if ( iMaxQty < iDiff)
			return false;
	}

	if ( iSize != iSizeArg )
		return false;

	else if (iMaxQtyArg != iMaxQtyArg )
		return false;

	else if ( iQtyArg != qty() )
		return false;

	return true;
}

#define TEST_VALS(x,y) \
	do { \
		if ( x != y ) \
			return false; \
	} while (0)


#define TEST_STATE(C,a,b,c) \
	do { \
		if ( ! C.test_state(a,b,c ) ) \
			return false; \
	} while (0)


bool test_tCircBuf ()
{
	int g;
	bool r;

	tCircBuf<int> cb(3);
	TEST_STATE(cb,3,4,0);

	cb.put(5);
	TEST_STATE(cb,3,4,1);

	r = cb.get(&g);
	TEST_VALS(r,true);
	TEST_VALS(g,5);
	TEST_STATE(cb,3,4,0);

	cb.put(1);
	TEST_STATE(cb,3,4,1);
	cb.put(2);
	TEST_STATE(cb,3,4,2);

	r = cb.get(&g);
	TEST_VALS(r,true);
	TEST_VALS(g,1);
	TEST_STATE(cb,3,4,1);
	
	r = cb.get(&g);
	TEST_VALS(r,true);
	TEST_VALS(g,2);
	TEST_STATE(cb,3,4,0);

	r = cb.get(&g);
	TEST_VALS(r,false);
	TEST_STATE(cb,3,4,0);

	cb.put(1);
	cb.put(2);
	cb.put(3);
	cb.put(4);

	TEST_STATE(cb,3,4,3);

	r = cb.get(&g);
	TEST_VALS(r,true);
	TEST_VALS(g,2);
	TEST_STATE(cb,3,4,2);

	for ( int i = 1; i <= 1003; i ++ )
	{
		cb.put(i);
	}

	TEST_STATE(cb,3,4,3);
	r = cb.get(&g);
	TEST_VALS(r,true);
	TEST_VALS(g,1001);
	TEST_STATE(cb,3,4,2);
	
	r = cb.get(&g);
	TEST_VALS(r,true);
	TEST_VALS(g,1002);
	TEST_STATE(cb,3,4,1);

	r = cb.get(&g);
	TEST_VALS(r,true);
	TEST_VALS(g,1003);
	TEST_STATE(cb,3,4,0);

	r = cb.get(&g);
	TEST_VALS(r,false);
	TEST_STATE(cb,3,4,0);

	for ( int j = 1; j <= 1003; j ++ )
	{
		cb.put(j);
	}

	TEST_STATE(cb,3,4,3);
	r = cb.get(&g);
	TEST_VALS(r,true);
	TEST_VALS(g,1001);
	TEST_STATE(cb,3,4,2);
	
	r = cb.get(&g);
	TEST_VALS(r,true);
	TEST_VALS(g,1002);
	TEST_STATE(cb,3,4,1);

	r = cb.get(&g);
	TEST_VALS(r,true);
	TEST_VALS(g,1003);
	TEST_STATE(cb,3,4,0);

	r = cb.get(&g);
	TEST_VALS(r,false);
	TEST_STATE(cb,3,4,0);

	
	return true;
}
#endif // test stuff


} // namespace 


