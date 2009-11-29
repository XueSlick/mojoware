/***********************************************************************************************************************
/*
/*    cDisplay.cpp / mojo_engine
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"

using namespace mojo;


//======================================================================================================================
// PROTOTYPES
//======================================================================================================================
static BOOL CALLBACK enum_display_monitors_cb ( HMONITOR hMonitor,	HDC hdc, RECT * r, LPARAM lParam );


//======================================================================================================================
// CODE
//======================================================================================================================



//----------------------------------------------------------------------------------------------------------------------
//	COPY CONSTRUCTOR
//----------------------------------------------------------------------------------------------------------------------
cDisplay :: cDisplay ( const cDisplay & rh ) : pPrev(0), pNext(0), iIndex ( rh.iIndex), cRectI ( rh.x, rh.y, rh.dx, rh.dy )
{	
}

//----------------------------------------------------------------------------------------------------------------------
//	OPERATOR =
//----------------------------------------------------------------------------------------------------------------------
cDisplay & cDisplay :: operator= ( const cDisplay & rh )
{
	x		= rh.x;
	y		= rh.y;
	dx		= rh.dx;
	dy		= rh.dy;
	iIndex	= rh.iIndex;

	return *this;
}


//----------------------------------------------------------------------------------------------------------------------
//	IS POINT IN
//----------------------------------------------------------------------------------------------------------------------
bool cDisplayList :: is_point_in ( cPtI * pt )
{
	for ( cDisplay * p = first(); p; p = next(p) )
	{
		// cRectI * r = p;

		if ( is_point_in_rect ( static_cast<cRectI*>(p), pt ) )
			return true;
	}

	return false;
}


//---------------------------------------------------------------------------------------------
//	SET FROM TEXT
//---------------------------------------------------------------------------------------------
void cDisplayList :: set_from_text ( const wchar_t * pTxt )
{
	rem_del_all();

	cStrW aRay[4];
	const wchar_t * pNext = pTxt;

	while ( get_next_match ( aRay, sizeof(aRay)/sizeof(cStrW), & pNext, L"\\{(.+?) (.+?) (.+?) (.+?)\\}" ) )
	{
		cDisplay * pNew = new cDisplay;

		pNew->x  = _wtoi ( aRay[0].cstr() );
		pNew->y  = _wtoi ( aRay[1].cstr() );
		pNew->dx = _wtoi ( aRay[2].cstr() );
		pNew->dy = _wtoi ( aRay[3].cstr() );

		append ( pNew );
	}
}


//---------------------------------------------------------------------------------------------
//	GET TEXT
//---------------------------------------------------------------------------------------------
const wchar_t * cDisplayList :: text ( cStrW * pRet )
{
	for ( cDisplay * p = first(); p; p = next ( p ) )
	{
		wchar_t t [1000];
		wsprintf ( t, L"{ %d %d %d %d }", p->x, p->y, p->dx, p->dy );
		(*pRet) += t;
	}

	return pRet->cstr();
}


//---------------------------------------------------------------------------------------------
//	ENUM DISPLAY MONITORS CALL BACK (FOR UPDATE SCREEN RES FROM HARDWARE)
//---------------------------------------------------------------------------------------------
BOOL CALLBACK enum_display_monitors_cb ( HMONITOR hMonitor,	HDC hdc, RECT * r, LPARAM lParam )
{
	UNREFERENCED_PARAMETER ( hdc );
	UNREFERENCED_PARAMETER ( hMonitor );

	cDisplayList * L = reinterpret_cast<cDisplayList*>( lParam );

	cDisplay * d = new cDisplay;
	d->iIndex = L->qty();
	// d->VirtRect = cRectI ( r->left, r->top, r->right - r->left, r->bottom - r->top );
	*(cRectI*)d = cRectI ( r->left, r->top, r->right - r->left, r->bottom - r->top );
	L->append ( d );

	return TRUE;
}


//---------------------------------------------------------------------------------------------
//	UPDATE SCREEN RES FROM HARDWARE
//  get the virtual desktop for a particular machine and
//  the rectangles of each display that is part of that desktop
//---------------------------------------------------------------------------------------------
void cDisplayList :: set_from_local_hardware ()
{
	// Enumerate all displays that are part of the virtual desktop:
	EnumDisplayMonitors ( NULL, NULL, enum_display_monitors_cb, (LPARAM) this );
}


//---------------------------------------------------------------------------------------
//	CALC MULT VIRT RECT
//---------------------------------------------------------------------------------------
void cDisplayList :: calc_mult_virt_rect ( cRectI * pRet )
{
	int l = INT_MAX;
	int t = INT_MAX;
	int r = INT_MIN;  // coordinate not size
	int b = INT_MIN;

	// Calc the whole virtual desktop that comprises the individual display rectangles:
	for ( cDisplay * s = pHead; s; s = s->pNext )
	{
		cRectI * v = static_cast<cRectI*>(s); // ->VirtRect;

		if ( v->x < l )
			l = v->x;

		if ( v->y < t )
			t = v->y;

		if ( r < v->x + v->dx )
			r = v->x + v->dx;

		if ( b < v->y + v->dy )
			b = v->y + v->dy;
	}

	pRet->x  = l;
	pRet->y = t;
	pRet->dx = r - l;
	pRet->dy = b - t;
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