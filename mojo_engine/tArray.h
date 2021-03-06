/***********************************************************************************************************************
/*
/*    tArray.h / mojo_engine
/*
/*    This class has some weird (and potentially dangerous) features.   
/*
/*    (1) For array A, if you touch A[n] and n is past the end of the array, the array will automatically 
/*        expand to make A[n] exist.   If n is large this can result in a very big memory allocation!
/*
/*    (2) Also, erase() doesn't really erase anything.  It just sets the number of entries to zero.  If you
/*        erase and then touch A[10], entries A[0] through A[9] spring magically back to life!
/*
/*    The risks can be minimized by always writing to the array with append().
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#pragma once

namespace mojo
{

//----------------------------------------------------------------------------------------------------------------------
//  CLASS ARRAY
//----------------------------------------------------------------------------------------------------------------------
template <class C> class tArray
{
public:

	tArray () : pBuf(0), uQty(0), uBufLen(0) {}
	tArray ( unsigned a );
	~tArray () { if ( pBuf ) delete[] pBuf; }
	tArray<C> & operator= ( const tArray<C> & rh );
	void append ( const C & a );
	void push   ( const C & a ) { append (a); }
	C & last () const { assert ( 0 < uQty ); return pBuf [ uQty-1 ]; }
	C & pop () { assert ( 0 < uQty ); uQty--; return pBuf[uQty]; }
	C & operator [] ( unsigned a );
	unsigned qty () const { return uQty; }
	void erase () { uQty = 0; } // this doesn't clear anything, so be careful
	void truncate ( unsigned u ) { uQty = u; }  // everything from [u] onward inclusive is erased
	C * buf() { return pBuf; }
	void resize ( unsigned uSize ) { if ( uBufLen < uSize ) expand ( uSize - uBufLen ); }

protected:

	static const int MIN_ALLOC = 12;

	C * pBuf;
	unsigned uQty;
	unsigned uBufLen;

private:

	void expand ( unsigned uIncrement );

};

//======================================================================================================================
// SPECIALIZATIONS
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
//  tARRAY OF UNSIGNED
//  This is defined so it performs its allocations and deletions in the DLL regardless of whether
//  the code is called in the EXE or DLL.  This allows the class to be used to return values from
//  mojo_engine to mojo_app.
//----------------------------------------------------------------------------------------------------------------------
class MOJO_ENGINE_API cArrayU : public tArray<unsigned>
{
public:

	typedef tArray<unsigned> B; // base
	cArrayU ( unsigned a ) : B ( a ) {}
	cArrayU () {}
};


//----------------------------------------------------------------------------------------------------------------------
//  tARRAY OF WORDS
//  This is defined so it performs its allocations and deletions in the DLL regardless of whether
//  the code is called in the EXE or DLL.  This allows the class to be used to return values from
//  mojo_engine to mojo_app.
//----------------------------------------------------------------------------------------------------------------------
class MOJO_ENGINE_API cArrayW : public tArray<WORD>
{
public:

	typedef tArray<WORD> B; // base
	cArrayW ( WORD a ) : B ( a ) {}
	cArrayW () {}

	void sort ();
	bool operator== ( const cArrayW & r ) const;
	bool contains ( WORD a ) const;


private:
	static int compare ( const void *, const void * );
};

//======================================================================================================================
// CODE
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
//  OPERATOR EQUAL
//----------------------------------------------------------------------------------------------------------------------
template<class C> tArray<C> & tArray<C>::operator= ( const tArray<C> & r )
{
	if ( uBufLen < r.uQty )
		resize( r.uQty );

	for ( unsigned i = 0; i < r.uQty; i++ )
		pBuf[i] = r.pBuf[i];

		uQty = r.uQty;

	return *this;
}


//----------------------------------------------------------------------------------------------------------------------
// EXPAND
//----------------------------------------------------------------------------------------------------------------------
template<class C> void tArray<C> :: expand ( unsigned uIncrement )
{
	unsigned uNewBufLen = uBufLen + uIncrement;

	C * pNew = new C [ uNewBufLen ];

	if ( pBuf )
	{
		for ( unsigned i = 0; i < uQty; i++ )
			pNew[i] = pBuf[i];

		delete[] pBuf;
	}

	pBuf = pNew;
	uBufLen = uNewBufLen;
}


//----------------------------------------------------------------------------------------------------------------------
// OPERATOR
//----------------------------------------------------------------------------------------------------------------------
template<class C> C & tArray<C> :: operator [] ( unsigned a )
{
	if ( uBufLen <= a )
		expand ( a - uBufLen + MIN_ALLOC );

	if ( uQty <= a )
		uQty = a + 1;

	return pBuf[a];
}


//----------------------------------------------------------------------------------------------------------------------
// CONSTRUCTOR
//----------------------------------------------------------------------------------------------------------------------
template <class C> tArray<C> :: tArray ( unsigned a )
{
	uQty	= 0;
	pBuf	= new C[a];
	uBufLen = a; // in C's
}


//----------------------------------------------------------------------------------------------------------------------
// APPEND
//----------------------------------------------------------------------------------------------------------------------
template <class C> void tArray<C> :: append ( const C & a )
{
	if ( uQty == uBufLen )
	{
//		unsigned uNewBufLen = uBufLen + MIN_ALLOC;

		expand ( MIN_ALLOC );

#if 0
		C * pNew = new C [ uNewBufLen ];

		if ( pBuf )
		{
			for ( unsigned i = 0; i < uQty; i++ )
				pNew[i] = pBuf[i];

			delete[] pBuf;
		}

		pBuf = pNew;
		uBufLen = uNewBufLen;
#endif
	}

	pBuf[uQty] = a;
	uQty++;
}

} // namespace


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