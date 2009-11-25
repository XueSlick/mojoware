/********************************************************************************************************
/*
/*    tRect.h / mojo_engine
/*
/*    This template creates classes that represent rectangles.   Typedefs are defined for a rectangle
/*    of integers, a rectangle of floats, and a rectangle of doubles.
/*
/*    There are a bunch of templated non-member functions following the class definition.
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
#include "tPt.h"

namespace mojo
{

//=======================================================================================================
//  CLASS RECT
//=======================================================================================================

template<class C> class tRect
{

public:

	tRect () : x(0), y(0), dx(0), dy(0) {}
	tRect ( tRect<C> & rh );
	tRect ( C x, C y, C dx, C dy );
	tRect ( tPt<C> &Pos, tPt<C> &Dim );

	template<class C> friend tRect<C> & combine ( tRect<C> * combo, tRect<C> * r1, tRect<C> * r2 );

	tPt<C> pos ();
	tPt<C> dim ();

	~tRect () {;};

	tRect<C> & operator= ( const tRect<C>  & rh );
	bool     operator== (const tRect<C> & rh );
	tRect<C> & operator/= ( C rh );

	C x;  // x position
	C y;  // y position
	C dx; // width
	C dy; // height
};

//=======================================================================================================
//  TYPEDEFS
//=======================================================================================================

typedef tRect<int>    cRectI;
typedef tRect<double> cRectD;
typedef tRect<float>  cRectF;


//=======================================================================================================
//  CODE
//=======================================================================================================

//-------------------------------------------------------------------------------------------------------
// OPERATOR /=
//-------------------------------------------------------------------------------------------------------
template<class C> tRect<C>& tRect<C>::operator/= ( C rh )
{
	x  /= rh;
	y  /= rh;
	dx /= rh;
	dy /= rh;

	return *this;
}


//-------------------------------------------------------------------------------------------------------
//	IS POINT IN RECT
//  b = big, s = small; the small one is moved so none of it is outside b
//  if small is too big to fit, its top-left is inside and bottom-right is outside
//-------------------------------------------------------------------------------------------------------
template<class C> void move_rect_inside_rect ( tRect<C> * b, tRect<C> * s )
{
	tPt<C> ul ( s->x, s->y );
	tPt<C> lr ( s->x + s->dx, s->y + s->dy );

	if ( ! is_point_in_rect ( b, & lr ) )
	{
		if ( b->x + b->dx < lr.x )  // to far right
			s->x = b->x + b->dx - s->dx;

		if ( b->y + b->dy < lr.y )  // too far down
			s->y = b->y + b->dy - s->dy;
	}

	if ( ! is_point_in_rect ( b, & ul ) )
	{
		if ( ul.x < b->x )  // too far left
			s->x = b->x;

		if ( ul.y < b->y ) // too far up
			s->y = b->y;
	}
}


//-------------------------------------------------------------------------------------------------------
//	IS POINT IN RECT
//-------------------------------------------------------------------------------------------------------
template<class C> bool is_point_in_rect ( tRect<C> * r, tPt<C> * p ) 
{
	if ( r->x <= p->x &&
		 r->y <= p->y &&
		 p->x < r->x + r->dx &&
		 p->y < r->y + r->dy       )

		 return true;
	else
		return false;
}


//-------------------------------------------------------------------------------------------------------
// COMBINE aka UNION
//-------------------------------------------------------------------------------------------------------
template<class C> tRect<C>& combine ( tRect<C> * com, tRect<C> * r1, tRect<C> * r2 )
{
	com->x = r1->x < r2->x ? r1->x : r2->x;
	com->y = r1->y < r2->y ? r1->y : r2->y;

	C x1abs = r1->x + r1->dx;
	C x2abs = r2->x + r2->dx;

	if ( x1abs < x2abs )
		com->dx = x2abs - com->x;
	else
		com->dx = x1abs - com->x;

	C y1abs = r1->y + r1->dy;
	C y2abs = r2->y + r2->dy;

	if ( y1abs < y2abs )
		com->dy = y2abs - com->y;
	else
		com->dy = y1abs - com->y;

	return *com;
}


//-------------------------------------------------------------------------------------------------------
// POS
//-------------------------------------------------------------------------------------------------------
template<class C> mojo::tPt<C> tRect<C>::pos()
{
	mojo::tPt<C> ret ( x,y);
	return ret;
}


//-------------------------------------------------------------------------------------------------------
// DIM
//-------------------------------------------------------------------------------------------------------
template<class C> tPt<C> tRect<C>::dim()
{
	tPt<C> ret ( dx, dy);
	return ret;
}


// ------------------------------------------------------------
// OPERATOR ==
// ------------------------------------------------------------
template<class C> bool tRect<C>::operator== ( const tRect<C> & rh )
{
	if ( x == rh.x && y == rh.y && dx == rh.dx && dy == rh.dy )
		return true;
	else
		return false;
}


//-------------------------------------------------------------------------------------------------------
//  CONSTRUCTORS
//-------------------------------------------------------------------------------------------------------
template<class C> tRect<C>::tRect ( tRect<C> & rh )
{
	x = rh.x;
	y = rh.y;
	dx = rh.dx;
	dy = rh.dy;
}
template<class C> tRect<C>::tRect ( C rh_x, C rh_y, C rh_dx, C rh_dy )
{
	x = rh_x;
	y = rh_y;
	dx = rh_dx;
	dy = rh_dy;
}
template<class C> tRect<C>::tRect ( tPt<C> & Pos, tPt<C> & Dim )
{
	x = Pos.x;
	y = Pos.y;
	dx = Dim.x;
	dy = Dim.y;
}


//-------------------------------------------------------------------------------------------------------
//  OPERATOR=
//-------------------------------------------------------------------------------------------------------
template<class C> tRect<C> & tRect<C>::operator= ( const tRect<C>  & rh )
{
	x = rh.x;
	y = rh.y;
	dx = rh.dx;
	dy = rh.dy;

	return *this;
}


} // namespace 


