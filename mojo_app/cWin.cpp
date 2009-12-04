/***********************************************************************************************************************
/*
/*    cWin.cpp / mojo_app
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "StdAfx.h"
#include "cWin.h"

//======================================================================================================================
//  CODE
//======================================================================================================================

void cWin :: balloon ( HWND hCtrl, const wchar_t * pTitle, const wchar_t * pBody )
{
	if ( hBalloon )
		DestroyWindow ( hBalloon );

	hBalloon = ::display_balloon ( hCtrl, pTitle, pBody );
}


//----------------------------------------------------------------------------------------------------------------------
//  SET ITEM TEXT
//  If uID is non-zero, it's the ID of a child item (e.g. control)
//  If it's zero, set the window title
//----------------------------------------------------------------------------------------------------------------------
void cWin :: set_item_text ( int iID, const wchar_t * pKey, ... )
{
	va_list args;
	va_start ( args, pKey );
	cScribPack sp ( pKey, args );

	if ( 0 == iID )
		SetWindowText ( hwnd, sp.head() );

	else
		SetWindowText ( GetDlgItem ( hwnd, iID ),sp.head() );
}


//----------------------------------------------------------------------------------------------------------------------
//  SET TEXT RECURSIVE
//----------------------------------------------------------------------------------------------------------------------
void cWin :: set_text_recursive ()
{
	for ( sWinPtr * ptr = ChildList.pHead; ptr; ptr = ptr->pNext )
	{
		cWin * p = ptr->pW;
		p->set_text ();
	}
}

//----------------------------------------------------------------------------------------------------------------------
//  USER DATA TO * WIN
//----------------------------------------------------------------------------------------------------------------------
cWin * cWin::user_data_to_pWin ( HWND hwnd )
{
	return reinterpret_cast<cWin*>( static_cast<LONG_PTR> ( GetWindowLongPtr ( hwnd, GWLP_USERDATA ) ) );
}


//----------------------------------------------------------------------------------------------------------------------
//  LPARAM TO USER DATA 
//----------------------------------------------------------------------------------------------------------------------
void cWin::set_user_data ( HWND hwnd, cWin * pWin )
{
	SetWindowLongPtr ( hwnd, GWLP_USERDATA, (LONG)reinterpret_cast<LONG_PTR>(pWin) );
}


//----------------------------------------------------------------------------------------------------------------------
//  ANCHOR
//  converts anchor type to pixel index
//----------------------------------------------------------------------------------------------------------------------
int cWin::anchor ( int x, int y, nAnchor::eAnchor a )
{
	if ( a == nAnchor::left )
		return 0;

	else if ( a == nAnchor::top )
		return 0;

	else if ( a == nAnchor::right )
		return x;

	else if ( a == nAnchor::bottom )
		return y;

	else
	{
		assert(0);
		return 0;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//  ON SIZE
//----------------------------------------------------------------------------------------------------------------------
void cWin::reposition_children ()
{
	RECT r;
	GetClientRect ( hwnd, &r );
	wm_size ( r.right - r.left, r.bottom - r.top );
}


//----------------------------------------------------------------------------------------------------------------------
//  WM MOVE
//----------------------------------------------------------------------------------------------------------------------
void cWin::wm_move ( int x, int y )
{
	UNREFERENCED_PARAMETER (x);
	UNREFERENCED_PARAMETER (y);

	if ( hBalloon )
	{
		DestroyWindow ( hBalloon );
		hBalloon = 0;
	}
}


//----------------------------------------------------------------------------------------------------------------------
//  ON SIZE
//----------------------------------------------------------------------------------------------------------------------
void cWin::wm_size ( int x, int y )
{
	for ( sWinPtr * ptr = ChildList.pHead; ptr; ptr = ptr->pNext )
	{
		cWin * p = ptr->pW;

		int iAnchorPosX = (int) ( anchor ( x, y, p->left.anchor ) + x * p->left.pct / 100 );
		int iPosX = iAnchorPosX + p->left.offset;

		int iAnchorPosY = (int) ( anchor ( x, y, p->top.anchor ) + y * p->top.pct / 100 );
		int iPosY = iAnchorPosY + p->top.offset;	
		
		int iAnchorRight = (int) ( anchor ( x, y, p->right.anchor ) + x * p->right.pct / 100 );
		int iRight = iAnchorRight + p->right.offset;	

		int iAnchorBottom = (int) ( anchor ( x, y, p->bottom.anchor ) + y * p->bottom.pct / 100 );
		int iBottom = iAnchorBottom + p->bottom.offset;	

		int iDimX = iRight - iPosX;
		int iDimY = iBottom - iPosY;

		MoveWindow ( p->hwnd, iPosX, iPosY, iDimX, iDimY, TRUE );
		InvalidateRect ( p->hwnd, NULL, FALSE );
		UpdateWindow ( p->hwnd );

		p->wm_size ( iDimX, iDimY );
	}
}


//----------------------------------------------------------------------------------------------------------------------
//  REGISTER CHILD SUB
//----------------------------------------------------------------------------------------------------------------------
void cWin::register_child_sub ( sWinPt * pt, nAnchor::eAnchor anch, double pct, int off )
{
	pt->anchor = anch;
	pt->offset = off;
	pt->pct = pct;
}


//----------------------------------------------------------------------------------------------------------------------
//   REGISTER CHILD
//
//   The syntax is:
//
//   Anchor is the left, top, right, or bottom border of the parent's client area.
//
//   The double and int arg's are both offsets from the anchor.  One is a percentage
//   of the linear dimension.  The other is in pixels.  Both are added to anchor.
//   Both can be negative.
//----------------------------------------------------------------------------------------------------------------------
void cWin::register_child ( cWin * p,
						    nAnchor::eAnchor al, double pl, int ol,
		                    nAnchor::eAnchor at, double pt, int ot,
						    nAnchor::eAnchor ar, double pr, int or,
							nAnchor::eAnchor ab, double pb, int ob )
{
	register_child_sub ( &p->left,		al, pl, ol );
	register_child_sub ( &p->top,		at, pt, ot );
	register_child_sub ( &p->right,		ar, pr, or );
	register_child_sub ( &p->bottom,	ab, pb, ob );

	sWinPtr * pPtr = new sWinPtr;
	pPtr->pW = p;

	ChildList.append ( pPtr );
}


//----------------------------------------------------------------------------------------------------------------------
//  WINDOW PROC
//----------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK cWin :: window_proc ( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	cWin * pThis  = user_data_to_pWin ( hwnd );

	switch ( uMsg )
	{
	case WM_SIZE:
		pThis->wm_size ( LOWORD ( lParam ), HIWORD ( lParam ) );
		break;

	case WM_MOVE:
		pThis->wm_move ( LOWORD ( lParam ), HIWORD ( lParam ) );
		break;
	}

	return DefWindowProc ( hwnd, uMsg, wParam, lParam);	
}


/***********************************************************************************************************************
/*
/*    This file is part of Mojo.  For more information, see http://mojoware.org.
/*
/*    You may redistribute and/or modify Mojo under the terms of the GNU General Public License, version 3, as
/*    published by the Free Software Foundation.  You should have received a copy of the license with mojo.  If you
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