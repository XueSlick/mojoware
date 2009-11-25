/********************************************************************************************************
/*
/*    cVersion.h / mojo_engine
/*
/*    A template for classes that represent points.   Typedefs are defined for a point
/*    of integers, a point of floats, and a point of doubles.
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

namespace mojo
{
//=======================================================================================================
// CLASS PT (POINT)
//=======================================================================================================
template<class C> class tPt
{

public:

	tPt () {;}
	tPt ( const tPt<C> & old );
	tPt ( C x, C y );
	tPt ( POINT & rh );

	~tPt () {;};

	tPt<C> & operator= ( const tPt<C>  & old );
	tPt<C> & operator+= ( const tPt<C>  & old );
	tPt<C> & operator-= ( const tPt<C>  & old );
	tPt<C> & operator*= (const C &rh );
	tPt<C> & operator/= (const C &rh );
	bool     operator== (const tPt<C> & rh );

	C x;
	C y;
};


typedef tPt<int>    cPtI;
typedef tPt<double> cPtD;
typedef tPt<float>  cPtF;

//=======================================================================================================
// CODE
//=======================================================================================================

#if 1
//-------------------------------------------------------------------------------------------------------
// OPERATOR ==
//-------------------------------------------------------------------------------------------------------
template<class C> bool tPt<C>::operator== ( const tPt<C> & rh )
{
	if ( x == rh.x && y == rh.y )
		return true;
	else
		return false;

}


//-------------------------------------------------------------------------------------------------------
//  CONSTRUCTOR
//-------------------------------------------------------------------------------------------------------
template<class C> tPt<C>::tPt ( POINT & rh )
{
	x = rh.x;
	y = rh.y;
}
template<class C> tPt<C>::tPt ( const tPt<C> & rh )
{
	x = rh.x;
	y = rh.y;
}
template<class C> tPt<C>::tPt ( C rh_x, C rh_y )
{
	x = rh_x;
	y = rh_y;
}

//-------------------------------------------------------------------------------------------------------
//  OPERATOR=
//-------------------------------------------------------------------------------------------------------
template<class C> tPt<C> & tPt<C>::operator= ( const tPt<C>  & rh )
{
	x = rh.x;
	y = rh.y;

	return *this;
}


//-------------------------------------------------------------------------------------------------------
// OPERATOR *=
//-------------------------------------------------------------------------------------------------------
template<class C> tPt<C> & tPt<C>::operator*= ( const C & rh )
{
	x *= rh;
	y *= rh;

	return *this;
}


//-------------------------------------------------------------------------------------------------------
// OPERATOR /=
//-------------------------------------------------------------------------------------------------------
template<class C> tPt<C> & tPt<C>::operator/= ( const C & rh )
{
	x /= rh;
	y /= rh;

	return *this;
}


//-------------------------------------------------------------------------------------------------------
// OPERATOR +=
//-------------------------------------------------------------------------------------------------------
template<class C> tPt<C> & tPt<C>::operator+= ( const tPt<C> & rh )
{
	x += rh.x;
	y += rh.y;

	return *this;
}


//-------------------------------------------------------------------------------------------------------
// OPERATOR -=
//-------------------------------------------------------------------------------------------------------
template<class C> tPt<C> & tPt<C>::operator-= ( const tPt<C> & rh )
{
	x -= rh.x;
	y -= rh.y;

	return *this;
}

#endif

} // namespace mojo

