/***********************************************************************************************************************
/*
/*    tStr.h / mojo_engine
/*
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/*	  This template creates light-weight string classes that can be used both for text and for 
/*    conveniently handling buffers of binary BYTE and WCHAR data.
/*
/*	  A NULL is kept in the next array slot following the end of data to provide the option
/*    of using this class for null-terminated strings, but the class itself doesn't use the 
/*    zero to find the end of the data, so the data can be binary.
/*
/*	  Two specialized classes are created from this template, cStrN (narrow) and cStrW (wide) for
/*	  strings of char and wchar_t respectively.
/*
/*    The main reason this template is used instead of std::string is because this program derives 
/*    from code that was written before the STL existed.
/*
/**********************************************************************************************************************/

#pragma once

// #include <stdlib.h>
#include <windows.h>
#include <memory.h>
#include <assert.h>


namespace mojo
{
//----------------------------------------------------------------------------------------------------------------------
// tSTR
//----------------------------------------------------------------------------------------------------------------------
template<class C> class tStr
{
private:

	static const int iMIN_ALLOC = 12;

public:

	tStr();
	tStr ( int ); // allocates buffer of that size
	tStr ( const tStr<C> & Rh );
	tStr ( const C * pRh );

	~tStr ();

	void resize ( int );

	tStr<C> & operator= ( const tStr<C>  & Rh); 	// tStr
	tStr<C> & operator= ( const C * pRh );        	// null-terminated string

	bool operator== ( const tStr<C> & Rh ); 		// tStr
	bool operator== ( const C * pRh );        		// null-terminated string

	const C * cstr() const;

	int bufsize () { return iBufSize; }
	C * buffer () { return pBuf; } 					// not the same as cstr();

	C & operator[] ( int iArg ) { return  pBuf[iArg]; };

	void erase();

	tStr<C> & operator+= ( const tStr<C>  & Rh);   // array
	tStr<C> & operator+= ( const C * pRh );        // null-terminated string
	tStr<C> & operator+= ( const C );              // single C

	tStr<C> & append     ( const C * pRh, int iRhLen );
	tStr<C> & append     ( const C * pRh );

	int len () { return iLen; }; 

	void set_len_to_str_len () { iLen = strlen ( pBuf); }
	void truncate ( int iNewLen ) { if ( iNewLen < iBufSize ) { pBuf[iNewLen]=0; iLen=iNewLen; } } 

	tStr<C> sub ( int iIndex, int iSubLen );
	tStr<C> & toupper ();
	tStr<C> & trim_linefeeds(); 					// remove right-hand LFs
	tStr<C> & trim_spaces (); 						// remove right-hand space

	bool is_all_hex_digits () const;
	bool is_hex_num () const;
	bool is_num () const;
	int  trim_right ( C c );
	int  trim_left ( C c );
	int  trim_left_space (); 
	int  trim_right_space ();

	void remove_char ( C c );  // removes all occurences of that character
	void tStr<C>::replace ( C c, C r );
	bool tStr<C>::contains ( C c );

protected:

	void operator_equal_sub      ( const C * pRh, int iLen );
	bool operator_is_equal_sub   ( const C * pRh, int iLen );
	void operator_plus_equal_sub ( const C * pRh, int iLen );
	int strlen ( const C * p ); // just like CRT strlen() ... assumes the 
	                            // data in buffer is null-terminated
	// DATA MEMBERS

	mutable int iBufSize;   // size of buffer in C's -- includes room for terminating NULL
	mutable int iLen;       // number of C's ... doesn't include terminating NULL
	mutable C * pBuf;
};

//-------------------------------------------------------------------------------------------------------
//  CLASS cSTR N
//-------------------------------------------------------------------------------------------------------
class MOJO_ENGINE_API cStrW;
class MOJO_ENGINE_API cStrN : public tStr<char>
{
public:

	typedef tStr<char> B; // base

	cStrN () {}
	cStrN ( int i )      : B(i) {}
	cStrN ( const cStrN & Rh ) : B(Rh) {}
	cStrN ( const char  * Rh ) : B(Rh) {}
	cStrN ( const cStrW & Rh ); // wide to narrow

	cStrN & operator= ( const char * pRh ) { return static_cast<cStrN&>(B::operator=(pRh)); }
	int to_int (); // { return pBuf ? atoi ( pBuf ) : INT_MIN; }
};


//-------------------------------------------------------------------------------------------------------
//  CLASS cSTR W
//-------------------------------------------------------------------------------------------------------
class MOJO_ENGINE_API cStrW : public tStr<wchar_t>
{
public:

	typedef tStr<wchar_t> B; // base

	cStrW () {}
	cStrW ( int i )        : B(i) {}
	cStrW ( const cStrW   & Rh ) : B(Rh) {}
	cStrW ( const wchar_t * Rh ) : B(Rh) {}
	cStrW ( const cStrN & Rh ); // narrow to wide

	const wchar_t * f		 	( const wchar_t * pTxt, va_list pArgs ); // sprintf
	const wchar_t * f 			( const wchar_t * pTxt, ... );			 // sprintf

#if 0
	const wchar_t * f_va_list 	( const wchar_t * pTxt, va_list pArgs ) // sprintf
	{
		wchar_t t [8192];
		bool bError = false;

		__try
		{
			if ( FAILED ( StringCchVPrintfW (  t, sizeof(t)/sizeof(wchar_t), pTxt, pArgs ) ) )
				bError = true;
		}

		__except ( EXCEPTION_EXECUTE_HANDLER )
		{
			bError = true;
		}

		if ( bError )
			*this = L"ERROR";

		else
			*this = t;

		return pBuf;
	}

	const wchar_t * f 			( const wchar_t * pTxt, ... ) // sprintf
	{
		va_list args;
		va_start ( args, pTxt );
		return f_va_list ( pTxt, args );
	}
#endif
	cStrW & operator= ( const wchar_t * pRh ) { return static_cast<cStrW&>(B::operator=(pRh)); }
	int to_int (); // { return pBuf ? _wtoi ( pBuf ) : INT_MIN; }
};

inline cStrN operator+ ( const cStrN   & a1, const cStrN   & a2 ) { cStrN a = a1; a += a2; return a; }
inline cStrN operator+ ( const char    * a1, const cStrN   & a2 ) { cStrN a = a1; a += a2; return a; }
inline cStrN operator+ ( const cStrN   & a1, const char    * a2 ) { cStrN a = a1; a += a2; return a; }

inline cStrW operator+ ( const cStrW   & a1, const cStrW   & a2 ) { cStrW a = a1; a += a2; return a; }
inline cStrW operator+ ( const wchar_t * a1, const cStrW   & a2 ) { cStrW a = a1; a += a2; return a; }
inline cStrW operator+ ( const cStrW   & a1, const wchar_t * a2 ) { cStrW a = a1; a += a2; return a; }

//-------------------------------------------------------------------------------------------------------
//  NON-MEMBER PROTOTYPES
//-------------------------------------------------------------------------------------------------------
void /* MOJO_ENGINE_API */ cStrN_to_cStrW ( cStrW * pW, cStrN * pN ); // deprecated -- use conversion constructor
void /* MOJO_ENGINE_API */ cStrW_to_cStrN ( cStrN * pN, cStrW * pW ); // deprecated -- use conversion constructor


//==========================================================================================
// CODE
//==========================================================================================

//------------------------------------------------------------------------------------------
// CONTAINS
// added nov 12 2009
//------------------------------------------------------------------------------------------
template<class C> bool tStr<C>::contains ( C c )
{
	if ( ! pBuf )
		return false;

	for ( C * p = pBuf; *p ; p++ )
		if ( *p == c )
			return true;

	return false;
}


//------------------------------------------------------------------------------------------
// REMOVE CHAR
// removes all occurrences of character
// added nov 11 2009
//------------------------------------------------------------------------------------------
template<class C> void tStr<C>::remove_char ( C c )
{
	if ( ! pBuf )
		return;

	C * pTo = pBuf;
	C * pFrom = pBuf;

	for ( ; *pFrom ; pFrom++ )
	{
		if ( *pFrom == c )
			continue;

		else
			*pTo++ = *pFrom;
	}

	iLen = pTo - pBuf;

	* pTo = 0;
}

//------------------------------------------------------------------------------------------
// REPLACE CHAR
// replaces all occurrences of character
// added nov 11 2009
//------------------------------------------------------------------------------------------
template<class C> void tStr<C>::replace ( C c, C r )
{
	if ( ! pBuf )
		return;

	C * pTo = pBuf;
	C * pFrom = pBuf;

	for ( ; *pFrom ; pFrom++ )
	{
		if ( *pFrom == c )
			*pTo++ = r;

		else
			*pTo++ = *pFrom;
	}
}


//------------------------------------------------------------------------------------------
//  TRIM RIGHT - remove right hand char
//------------------------------------------------------------------------------------------
template<class C> int tStr<C>::trim_right ( C c )
{
	int iRet = 0;

	if ( iLen )
	{
		C * p = pBuf + iLen - 1;
	
		while (  ( pBuf <= p ) && ( *p == c ) )
		{
			*p = 0;
			--p;
			iLen--;
			iRet++;
		}

		assert ( 0 <= iLen );
	}

	return iRet;
}


//------------------------------------------------------------------------------------------
// TRIM LEFT
//------------------------------------------------------------------------------------------
template<class C> int tStr<C>::trim_left ( C c )
{
	if ( ! pBuf )
		return 0;

	int i;

	for (  i = 0; pBuf[i] == c; i++ )
		;

	if ( i )
	{
		memcpy ( pBuf, pBuf+i, ( this->iBufSize - i ) * sizeof(C) );
		this->iLen -= i;
	}

	return i;
}


//------------------------------------------------------------------------------------------
//  TRIM RIGHT SPACE
//  removes tabs and spaces from right side
//------------------------------------------------------------------------------------------
template<class C> int tStr<C>::trim_right_space ()
{
	int iRet = 0;

	if ( iLen )
	{
		C * p = pBuf + iLen - 1;
	
		while (  ( pBuf <= p ) && 
			     (
			        ( ( *p == ' ' ) || ( *p == '\t' ) ) 
				 )
			  )
		{
			*p = 0;
			--p;
			iLen--;
			iRet++;
		}

		assert ( 0 <= iLen );
	}
	
	return iRet;
}


//------------------------------------------------------------------------------------------
// TRIM LEFT SPACE
// removes tabs and spaces from left side
//------------------------------------------------------------------------------------------
template<class C> int tStr<C>::trim_left_space ()
{
	if ( ! pBuf )
		return 0;

	int i;

	for (  i = 0; 
		   pBuf[i] == ' ' || pBuf[i] == '\t';  
		   i++ )
		;

	if ( i )
	{
		memcpy ( pBuf, pBuf+i, ( this->iBufSize - i ) * sizeof(C) );
		this->iLen -= i;
	}

	return i;

}



//------------------------------------------------------------------------------------------
//  NUKE
//  obliterates memory
//------------------------------------------------------------------------------------------
#if 0
void cStrN::nuke ()
{
	if ( pcTxt )
	{
		memset ( pcTxt, 'x', iBufSize );
		pcTxt[0] = 0;
	}
}
#endif


//------------------------------------------------------------------------------------------
//  IS ALL HEX DIGITS
//  checks whether the number consists 
//  exclusively of hex digit char's
//------------------------------------------------------------------------------------------
template<class C> bool tStr<C>::is_all_hex_digits () const
{
	if ( ! pBuf )
		return false;

	C * p = this->pBuf;

	if ( ! p[0] )
		return false;

	for ( int i = 0; p[i]; i++ )
		if ( ! isxdigit( p[i] ) )
		return false;

	return true;
}


//------------------------------------------------------------------------------------------
//  IS HEX NUMBER
// checks whether the number starts with "0x" and also
// whether it consists exclusively of hex digit char's
// imported from hotkeynet oct 11 2008, not tested
//------------------------------------------------------------------------------------------
template<class C> bool tStr<C>::is_hex_num () const
{
	if ( ! pBuf )
		return false;

	if ( 0 == iLen )
		return false;

	C * p = pBuf;

	if ( '-' == *p )
		p++;

	if ( '0' == *p  && ( 'x' ==  p[1] || 'X' == p[1] ) )
			p += 2;

	else
		return false;

	if ( ! p[0] )
		return false;

	for ( int i = 0; p[i]; i++ )
		if ( ! isxdigit( p[i] ) )
		return false;

	return true;
}


//------------------------------------------------------------------------------------------
//  IS NUMERIC
//------------------------------------------------------------------------------------------
template<class C> bool tStr<C>::is_num () const
{
	bool bIsHex = false;

	if ( ! pBuf )
		return false;

	if ( 0 == iLen )
		return false;

	C * p = pBuf;

	if ( '-' == *p )
		p++;

	if ( '0' == *p )
		if ( 'x' ==  p[1] || 'X' == p[1] )
		{
			bIsHex = true;
			p += 2;
		}

	if ( ! p[0] )
		return false;


	if ( bIsHex )
	{
		for ( int i = 0; p[i]; i++ )
			if ( ! isxdigit( p[i] ) )
			return false;
	}

	else
	{
		for ( int i = 0; p[i]; i++ )
			if ( ! isdigit( p[i] ) )
				return false;
	}

	return true;

}



//------------------------------------------------------------------------------------------
// TRIM LINEFEEDS
//------------------------------------------------------------------------------------------
template<class C> tStr<C> & tStr<C>::trim_linefeeds ()
{
	while ( 0 < iLen )
	{
		if ( 10 == pBuf [ iLen - 1 ] )
		{
			iLen--;

			pBuf[iLen] = 0;
		}

		else
			break;
	}

	return *this;
}


//------------------------------------------------------------------------------------------
// TRIM SPACES
//------------------------------------------------------------------------------------------
template<class C> tStr<C> & tStr<C>::trim_spaces ()
{
	while ( 0 < iLen )
	{
		if ( ' ' == pBuf [ iLen - 1 ] )
		{
			iLen--;

			pBuf[iLen] = 0;
		}

		else
			break;
	}

	return *this;
}


//------------------------------------------------------------------------------------------
// TOUPPER
//------------------------------------------------------------------------------------------
template<class C> tStr<C> & tStr<C>::toupper ()
{
	for ( int i = 0; i < iLen; i ++ )
		pBuf[i] = static_cast<C>(::toupper(pBuf[i]));

	return *this;
}


template<class C> tStr<C> operator+ ( const tStr<C> &a1, const tStr<C> &a2 );
template<class C> tStr<C> operator+ ( const C * p1,        const tStr<C> &a2 );
template<class C> tStr<C> operator+ ( const tStr<C> &a1, const C * p2 );


//------------------------------------------------------------------------------------------
// RESIZE
// this erases any current contents
// should be rewritten to save it and copy to new buffer
//------------------------------------------------------------------------------------------
template<class C> void tStr<C>::resize ( int iBufSizeArg )
{
	this->erase();

	iLen = 0;

	if ( iBufSizeArg < 1 )
	{
		if ( pBuf )
			delete pBuf; // nov 6 2009

		pBuf = 0;
		iBufSize = 0;
	}

	else
	{
		pBuf = new C [iBufSizeArg * sizeof(C)];

		if ( pBuf )
			iBufSize = iBufSizeArg;
		else
			iBufSize = 0;
	}
}


//------------------------------------------------------------------------------------------
//  OPERATOR+
//------------------------------------------------------------------------------------------
template<class C> tStr<C> operator+ ( const tStr<C> &a1, const tStr<C> &a2 )
{
	tStr<C> a = a1;
	a += a2;
	return a;
}

template<class C> tStr<C> operator+ ( const C * p1, const tStr<C> &a2 )
{
	tStr<C> a = p1;
	a += a2;
	return a;
}

template<class C> tStr<C> operator+ ( const tStr<C> &a1, const C * p2 )
{
	tStr<C> a = a1;
	a += p2;
	return a;
}




//------------------------------------------------------------------------------------------
// SUB -- GET SUBSTRING
//------------------------------------------------------------------------------------------
template<class C> tStr<C> tStr<C>::sub ( int iIndex, int iSubLen )
{
	tStr<C> ret;

	if ( this->iLen < iIndex + iSubLen )
		return ret;

	ret.iLen = iSubLen;
	ret.iBufSize = ret.iLen + 1;
	ret.pBuf = new C [ret.iBufSize * sizeof(C)];
	memcpy ( ret.pBuf, pBuf+iIndex, iSubLen );
	ret.pBuf[ret.iLen] = NULL;

	return ret;
}


//------------------------------------------------------------------------------------------
// STRLEN
//------------------------------------------------------------------------------------------
template<class C> int tStr<C>::strlen ( const C *p )
{
	if ( NULL == p )
		return 0;
	
	int i;

	for ( i = 0; p[i]; i++ )
		;

	return i;
}


//------------------------------------------------------------------------------------------
// OPERATOR +=
//------------------------------------------------------------------------------------------
template<class C> tStr<C> & tStr<C>::operator+= ( const C c )
{

	if ( ((iBufSize-1) <= iLen)  || (0 == iBufSize) )
	{
		int iNewSize = iLen + 2;  // allocate extra -- on initial allocation we need 2 spaces, one for char and one for zero
		C * p = new C [ iNewSize ];
		iBufSize = iNewSize;

		if ( pBuf )
		{
			// int iSizeOf = sizeof(C);
			memcpy ( p, pBuf, iLen * sizeof(C) );
			delete[] pBuf;
		}

		pBuf = p;
	}

	pBuf[iLen] = (C)c;
	iLen++;
	pBuf[iLen] = 0;

	return *this;
}


//------------------------------------------------------------------------------------------
// SPECIAL OPERATOR= FOR IMPLEMENTING operator+
// see Soustrup reference 3rd ed section 22.4.7
//------------------------------------------------------------------------------------------
#if 0
template<class C> tStr<C> & tStr<C>::operator= ( const tStrAdd<C> & ArrayAdd)
{
	tStr<C> t = ArrayAdd.a1;
	t += ArrayAdd.a2;

	(*this) = t;

	return *this;
}
#endif


//------------------------------------------------------------------------------------------
// CONSTRUCTOR FOR ENABLING INITIALIZION FROM ADDITION OF 2 STR's
// see Stroustrup reference 3rd ed section 22.4.7
//------------------------------------------------------------------------------------------
#if 0
template<class C> tStr<C>::tStr<C> ( const tStrAdd<C> & ArrayAdd)
{
	tStr<C> t = ArrayAdd.a1;
	t += ArrayAdd.a2;

	iLen = t.len();

	if ( iLen )
	{
		iBufSize = iLen + 1;
		pBuf = new C [ iBufSize * sizeof (C) ];
		::memcpy ( pBuf, t.pBuf, iLen * sizeof ( C ) ) ;
		pBuf [ iLen ] = NULL;

	}

	else
	{
		pBuf = 0;
		iBufSize = 0;
	}
}
#endif


//------------------------------------------------------------------------------------------
// APPEND
//------------------------------------------------------------------------------------------
template<class C> tStr<C> & tStr<C>::append ( const C * pRh, int iQty )
{
	operator_plus_equal_sub ( pRh, iQty );
	return *this;
}

template<class C> tStr<C> & tStr<C>::append ( const C * pRh )
{
	int iRhLen = tStr::strlen(pRh);
	operator_plus_equal_sub ( pRh, iRhLen );
	return *this;
}


//------------------------------------------------------------------------------------------
// OPERATOR +=
//------------------------------------------------------------------------------------------
template<class C> tStr<C> & tStr<C>::operator+= ( const tStr<C> & rh )
{
	operator_plus_equal_sub ( rh.pBuf, rh.iLen );
	return *this;
}

template<class C> tStr<C> & tStr<C>::operator+= ( const C * pRh )
{
	int iRhLen = tStr::strlen(pRh);
	operator_plus_equal_sub ( pRh, iRhLen );
	return *this;
}

//------------------------------------------------------------------------------------------
// OPERATOR PLUS EQUAL SUB
//------------------------------------------------------------------------------------------
template<class C> void tStr<C>::operator_plus_equal_sub ( const C * pRh, int iRhLen )
{
	int lenB; // length of both

	if ( 0 == iRhLen )
		return;

	lenB = iLen + iRhLen;

	if ( this->iBufSize < lenB + 1 )
	{
		iBufSize = iLen + 1 + 2 * iRhLen; // allocate twice as much as needed
		C * p = new C [ iBufSize *sizeof(C)];

		if ( pBuf )
		{
			::memcpy ( p, pBuf, iLen * sizeof ( C ) );
			delete[] pBuf;
		}

		pBuf = p;
	}

	C * pDest = pBuf + iLen;

	::memcpy ( pDest, pRh, iRhLen * sizeof ( C ) ) ;
	pDest[iRhLen] = NULL;

	iLen = lenB;
}


//------------------------------------------------------------------------------------------
// OPERATOR ==
//------------------------------------------------------------------------------------------
template<class C> bool tStr<C>::operator== ( const tStr<C> & rh )
{
	return ( operator_is_equal_sub ( rh.pBuf, rh.iLen ) );
}

template<class C> bool tStr<C>::operator== ( const C * pRh )
{
	int iRhLen = tStr::strlen(pRh);

	return ( operator_is_equal_sub ( pRh, iRhLen ) );
}

//------------------------------------------------------------------------------------------
// OPERATOR IS EQUAL SUB
//------------------------------------------------------------------------------------------
template<class C> bool tStr<C>::operator_is_equal_sub ( const C * pRh, int iRhLen )
{
	if ( ( 0 == iLen ) && ( 0 == iRhLen ) )
		return true;

	if ( iLen != iRhLen )
		return false;

	for ( int i = 0; i < iLen; i++ )
	{
		if ( this->pBuf[i] != pRh[i] )
			return false;
	}

	return true;
}


//------------------------------------------------------------------------------------------
//   OPERATOR = 
//------------------------------------------------------------------------------------------
template<class C> tStr<C> &  tStr<C>::operator= ( const C * pcRh )
{
	operator_equal_sub ( pcRh, tStr::strlen ( pcRh ) );
	return *this;
}

template<class C> tStr<C> & tStr<C>::operator= ( const tStr<C> & Rh )
{
	operator_equal_sub ( Rh.pBuf, Rh.iLen );
	return *this;
}


//------------------------------------------------------------------------------------------
//   OPERATOR EQUAL SUB
//------------------------------------------------------------------------------------------
template<class C> void tStr<C>::operator_equal_sub ( const C * pRh, int iRhLen )
{
	if  ( 0 == iRhLen )
	{
		this->erase();
	}

	else
	{
		assert ( NULL != pRh ); // this should not be possible because strlen() in calling
		                        // function should have set iRhLen to zero if this pointer
		                        // is null.

		if ( iBufSize < iRhLen + 1 )
		{
			if ( pBuf )
				delete[] pBuf;

			iBufSize = iRhLen + 1;

			pBuf = new C [ iBufSize ];
		}

		iLen = iRhLen;
		::memcpy ( pBuf, pRh, iRhLen * sizeof(C));
		pBuf [ iLen ] = 0;
	}
}


//-------------------------------------------------------------------------------------------------------
// ERASE
//-------------------------------------------------------------------------------------------------------
template<class C> void tStr<C>::erase ()
{
	if ( pBuf )
		pBuf[0] = 0;

	iLen = 0;
}



//-------------------------------------------------------------------------------------------------------
// CSTR
// rewritten april 25 2009
//-------------------------------------------------------------------------------------------------------
template<class C> const C * tStr<C>::cstr () const
{
	if ( ! pBuf )
	{
		pBuf = new C [iMIN_ALLOC];

		if ( pBuf )
		{
			iBufSize = iMIN_ALLOC;
			pBuf[0] = 0;
		}

		else
			iBufSize = 0;

		iLen = 0;
	}

	return pBuf;
}




//-------------------------------------------------------------------------------------------------------
// CONSTRUCTOR - with requested buffer size 7/15/2006 - checked 6/2007
//-------------------------------------------------------------------------------------------------------
template<class C> tStr<C>::tStr ( int iBufSizeArg )
{
	iLen = 0;

	if ( iBufSizeArg < 1 )
	{
		pBuf = 0;
		iBufSize = 0;
	}

	else
	{
		pBuf = new C [iBufSizeArg * sizeof(C)];

		if ( pBuf )
			iBufSize = iBufSizeArg;
		else
			iBufSize = 0;
	}
}


//-------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR - checked 6/2007
//-------------------------------------------------------------------------------------------------------
template<class C> tStr<C>::tStr()
{
	iBufSize = 0; 
	iLen = 0;
	pBuf = NULL;
}



//-------------------------------------------------------------------------------------------------------
//   COPY CONSTRUCTOR
//-------------------------------------------------------------------------------------------------------
template<class C> tStr<C>::tStr ( const tStr<C> & Rh )
{
	iBufSize = Rh.iBufSize; 
	iLen =     Rh.iLen;

	if ( iLen )
	{
		pBuf = new C [ iBufSize ];
		memcpy ( pBuf, Rh.pBuf, iLen * sizeof ( C ) );
		pBuf[iLen] = NULL;

	}

	else
		pBuf = NULL;
}

//-------------------------------------------------------------------------------------------------------
//   CONSTRUCTOR - FROM ARRAY
//-------------------------------------------------------------------------------------------------------
template<class C> tStr<C>::tStr ( const C * pRh )
{
	iLen = strlen ( pRh );

	if ( 0 < iLen )
	{
		iBufSize = iLen + 1;
		pBuf = new C [ iBufSize ];
		memcpy ( pBuf, pRh, iLen * sizeof ( C ) );
		pBuf[iLen] = NULL;

	}

	else
	{
		iLen = 0;
		iBufSize = 0;
		pBuf = NULL;
	}
}

//-------------------------------------------------------------------------------------------------------
//   DESTRUCTOR - checked 6/2007
//-------------------------------------------------------------------------------------------------------
template<class C> tStr<C>::~tStr()
{
	iBufSize = iLen = 0;
	if ( pBuf )
		delete[] pBuf;
}

} // namespace mojo


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