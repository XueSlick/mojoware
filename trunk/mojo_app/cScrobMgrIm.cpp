/***********************************************************************************************************************
/*
/*    cScrobMgrIm.cpp / mojo_app
/*
/*    With this version, the scrobs are ImageList controls.
/*    To use this, inherit this class in a cWin class.
/*    Set this class's hwnd member on initialization and
/*    call this class's various "on" functions from the child
/*    dialog's proc.
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"
#include "cScrobMgrIm.h"

#include <commctrl.h>

//===============================================================================================
// DATA
//===============================================================================================


//===============================================================================================
// PROTOTYPES
//===============================================================================================


//===============================================================================================
// CODE
//===============================================================================================
	bool mouse_is_outside_our_client_area ( int x, int y );
	bool get_last_inside_point ( cPtI * pRet, int x, int y );
//----------------------------------------------------------------------------------------------
// ON MOUSE LEAVE
//----------------------------------------------------------------------------------------------
bool cScrobMgrIm :: mouse_is_outside_our_client_area ( int x, int y )
{
	RECT rect;
	GetClientRect ( hwnd, &rect );

	if ( x			 < rect.left  ||
		 y			 < rect.top   ||
		 rect.right  < x		  ||
		 rect.bottom < y               )

		 return true;
	else
		return false;
}

//----------------------------------------------------------------------------------------------
// ON MOUSE LEAVE
//----------------------------------------------------------------------------------------------
void cScrobMgrIm :: get_last_inside_point ( cPtI * pRet, int ArgX, int ArgY )
{
	assert(hwnd);

	int x = (INT16) ArgX; // the arguments are really unsigned words...
	int y = (INT16) ArgY; // ...from lParam in WM_MOUSEMOVE

	*pRet = cPtI ( x, y );

	RECT rect;
	GetClientRect ( hwnd, &rect );

	// DWORD dwError = GetLastError();

	if ( x < rect.left  )
		pRet->x = rect.left + 1;

	else if ( rect.right < x )
		pRet->x = rect.right - 1;

	if ( y < rect.top )
		pRet->y = rect.top + 1;

	else if ( rect.bottom < y )
		pRet->y = rect.bottom - 1;
}



//----------------------------------------------------------------------------------------------
// ON MOUSE MOVE
//----------------------------------------------------------------------------------------------
void cScrobMgrIm :: on_mouse_move ( int x, int y )
{
	if ( mouse_is_outside_our_client_area ( x, y ) )
	{
		cPtI Inside; 
		get_last_inside_point ( &Inside, x, y );
		on_mouse_left_up ( Inside.x, Inside.y );
	}

	else
	{
	   ImageList_DragMove ( x + ClientOffset.x, y + ClientOffset.y );
	}
}



//----------------------------------------------------------------------------------------------
// ON CLOSE WINDOW
//----------------------------------------------------------------------------------------------
void cScrobMgrIm::on_close_window()
{
	ReleaseCapture();
	ImageList_EndDrag(); 
    ImageList_DragLeave ( hwnd );
	ScrobList.rem_del_all();
	pHitScrob = 0;
}



//----------------------------------------------------------------------------------------------
// ADJUST POSITION AFTER DRAG
//----------------------------------------------------------------------------------------------
void cScrobMgrIm :: adjust_position_after_drag ( cScrobIm * pHitScrob )
{
	if ( bMagnetismIsOn )
		do_magnetism ( pHitScrob );	

	if ( bCollisionDetectionIsOn )
	{
		int iQty = this->ScrobList.qty();
			
		for ( int i = 0; i < iQty ; i++ )
		{
			do_collision_detection ( pHitScrob );
		}
	}
}


//----------------------------------------------------------------------------------------------
// ON MOUSE LEFT UP
//----------------------------------------------------------------------------------------------
void cScrobMgrIm :: on_mouse_left_up ( int x, int y )
{
	if ( ! pHitScrob )
		return;

	ImageList_EndDrag(); 
    ImageList_DragLeave ( hwnd );
 
    ReleaseCapture(); 

 	pHitScrob->Pos.x = x - HotSpot.x;
	pHitScrob->Pos.y = y - HotSpot.y;

	adjust_position_after_drag ( pHitScrob );

#if 0
	if ( bMagnetismIsOn )
		do_magnetism ( pHitScrob );	

	if ( bCollisionDetectionIsOn )
	{
		int iQty = this->ScrobList.qty();
			
		for ( int i = 0; i < iQty ; i++ )
		{
			do_collision_detection ( pHitScrob );
		}
	}
#endif

	pHitScrob = 0;

	// I'm repainting all of them because with MouseOver window,
	// moving one can change the colors of the others, but in 
	// general it would be necessary only to repaint pHitScrob,
	// so really the next bit should be in an override in a
	// a derived class

	for ( cScrobIm * s = ScrobList.pHead; s; s = s->pNext )
	{
		RECT r;

		InvalidateRect ( hwnd, s->get_rect(&r), FALSE );
		UpdateWindow ( hwnd );
	}
 } 


//----------------------------------------------------------------------------------------------
// GET HIT SCROB
//----------------------------------------------------------------------------------------------
cScrobIm * cScrobMgrIm::get_hit_scrob ( int x, int y )
{
	HDC hdc = GetDC ( hwnd );

	for ( cScrobIm * s = ScrobList.end(); s; s = s->pPrev ) // draw forwards, hit-test backwards
	{
		cRectI r ( s->Pos, s->Dim );
		cPtI hit ( x, y );

		if ( is_point_in_rect ( &r,  &hit ) )
		{
			int iIndex = 0;

			if ( ! s->point_is_transparent ( hdc, iIndex, x, y ) )
				return s;
		}
	}

	ReleaseDC ( hwnd, hdc );

	return NULL;
}


//----------------------------------------------------------------------------------------------
// ON MOUSE LEFT DOWN
//----------------------------------------------------------------------------------------------
void cScrobMgrIm::on_mouse_left_down ( int x, int y )
{
	pHitScrob = get_hit_scrob ( x, y );

	if ( 0 == pHitScrob )
		return;

	ScrobList.remove ( pHitScrob );
	ScrobList.append ( pHitScrob );
	RECT rectHit;
	rectHit.left = pHitScrob->Pos.x;
	rectHit.top  = pHitScrob->Pos.y;
	rectHit.right = pHitScrob->Pos.x + pHitScrob->Dim.x;
	rectHit.bottom = pHitScrob->Pos.y + pHitScrob->Dim.y;
	// InvalidateRect ( hwnd, &rectHit, FALSE );
	// UpdateWindow ( hwnd );

    SetCapture(hwnd); 
 
    // Erase the image from the client area.
	// SendMessage ( hwnd, WM_SETREDRAW, 0, 0 ); // EXPERIMENTAL
    InvalidateRect ( hwnd, &rectHit, TRUE ); 
    UpdateWindow ( hwnd ); 
	// SendMessage ( hwnd, WM_SETREDRAW, 1, 0 ); // EXPERIMENTAL

	HotSpot.x = x - rectHit.left;
	HotSpot.y = y - rectHit.top;
 
	if ( ! ImageList_BeginDrag ( pHitScrob->hIL, pHitScrob->iImageIndex,  HotSpot.x, HotSpot.y ) )
	{
		pHitScrob = 0;
		ReleaseCapture();
        return;
	}

    // Set the initial location of the image, and make it visible. 
    // Because the ImageList_DragEnter function expects coordinates to 
    // be relative to the upper-left corner of the given window, the 
    // width of the border, title bar, and menu bar need to be taken 
    // into account. 

	get_client_offset ( &ClientOffset, hwnd );

    ImageList_DragEnter ( hwnd, x + ClientOffset.x, y + ClientOffset.y ); 

     return;
}


//----------------------------------------------------------------------------------------------
// DRAW SCROBS
//----------------------------------------------------------------------------------------------
bool cScrobMgrIm :: draw_scrobs ( HDC hdc )
{
	for ( cScrobIm * s = ScrobList.pHead; s; s = s->pNext ) // draw forwards, hit-test backwards
	{
		int iIndex = s->calc_image_index ();
		s->iImageIndex = iIndex;

		if ( s != pHitScrob )
		{
			/* DEFAULT HERE */
			ImageList_SetBkColor ( s->hIL,   CLR_DEFAULT );

			/* TRANSPARENT HERE */
			BOOL iResult = ImageList_Draw ( s->hIL, iIndex, hdc, s->Pos.x, s->Pos.y, ILD_TRANSPARENT );
			UNREFERENCED_PARAMETER(iResult);
			#if 0
			IMAGELISTDRAWPARAMS ilp;
			ZeroMemory ( &ilp, sizeof(ilp) );
			ilp.cbSize = sizeof(ilp);
			ilp.himl = s->hIL;
			ilp.i = iIndex;
			ilp.hdcDst = hdc;
			ilp.x = s->Pos.x;
			ilp.y = s->Pos.y;
			ilp.xBitmap = 0;
			ilp.yBitmap = 0;
			ilp.rgbBk = CLR_NONE;
			ilp.rgbFg = CLR_NONE;
			ilp.fStyle =  ILD_ROP; // ILS_ALPHA;
			ilp.dwRop = SRCCOPY;
			ilp.fState = 0;
			ilp.Frame = 0;
			ilp.crEffect = 0; // 255;
			ImageList_DrawIndirect ( &ilp );
			#endif
		}
	}

	return true;
}


//----------------------------------------------------------------------------------------------
// ADD SCROB
//----------------------------------------------------------------------------------------------
bool cScrobMgrIm :: add_scrob ( cScrobIm * pS )
{
	ScrobList.append ( pS );

	return true;
}


//---------------------------------------------------------------------------------------
// ARE CONTIGUOUS
//---------------------------------------------------------------------------------------
bool cScrobMgrIm::are_contiguous ( cScrobIm * p1, cScrobIm * p2 )
{
	int L1 = p1->Pos.x;
	int R1 = p1->Pos.x + p1->Dim.x;
	int T1 = p1->Pos.y;
	int B1 = p1->Pos.y + p1->Dim.y;

	int L2 = p2->Pos.x;
	int R2 = p2->Pos.x + p2->Dim.x;
	int T2 = p2->Pos.y;
	int B2 = p2->Pos.y + p2->Dim.y;

	if ( they_overlap_y ( p1, p2 ) )
	{
		if  ( ( R1 == L2 ) || ( R2 == L1 ) )
			return true;
	}

	else if ( they_overlap_x ( p1, p2 ) )
	{
		if  ( ( T1 == B2 ) || ( T2 == B1 ) )
			return true;
	}

	return false;
}


//---------------------------------------------------------------------------------------
// THEY OVERLAP
//---------------------------------------------------------------------------------------
bool cScrobMgrIm::they_overlap_x ( cScrobIm * p1, cScrobIm * p2 )
{
	int L1 = p1->Pos.x;
	int R1 = p1->Pos.x + p1->Dim.x;

	int L2 = p2->Pos.x;
	int R2 = p2->Pos.x + p2->Dim.x;

	if  ( ( R1 <= L2 ) || ( R2 <= L1 ) )
		return false;

	else
		return true;
}

//---------------------------------------------------------------------------------------
// THEY OVERLAP
//---------------------------------------------------------------------------------------
bool cScrobMgrIm::they_overlap_y ( cScrobIm * p1, cScrobIm * p2 )
{
	int T1 = p1->Pos.y;
	int B1 = p1->Pos.y + p1->Dim.y;

	int T2 = p2->Pos.y;
	int B2 = p2->Pos.y + p2->Dim.y;

	if ( ( B1 <= T2 ) || ( B2 <= T1 ) )
		return false;

	else
		return true;
}

//---------------------------------------------------------------------------------------
// DO MAGNETISM
// returns qty changes
//---------------------------------------------------------------------------------------
int cScrobMgrIm::do_magnetism ( cScrobIm * pScrob )
{
	int iQtyChanges = 0;

	assert ( pScrob );

	const int iMaxGap = 10;

	for ( cScrobIm * pOther = this->ScrobList.pHead; pOther; pOther = pOther->pNext )
	{
		if ( pScrob == pOther )
			continue;

		// recalc first scrob's points each iteration because
		// it can change as result of move_to at bottom of loop

		int L1 = pScrob->Pos.x;
		int R1 = pScrob->Pos.x + pScrob->Dim.x;
		int T1 = pScrob->Pos.y;
		int B1 = pScrob->Pos.y + pScrob->Dim.y;

		int L2 = pOther->Pos.x;
		int R2 = pOther->Pos.x + pOther->Dim.x;
		int T2 = pOther->Pos.y;
		int B2 = pOther->Pos.y + pOther->Dim.y;

		int iGapX1, iGapX2, iGapY1, iGapY2;

		iGapX1 = L2 - R1;
		iGapX2 = L1 - R2;
		iGapY1 = T2 - B1;
		iGapY2 = T1 - B2;

		// move x dimension?

		int iMoveX = 0;

		if ( 0 < iGapX1 && iGapX1 < iMaxGap )
			iMoveX = L2 - R1;

		else if ( 0 < iGapX2 && iGapX2 < iMaxGap )
			iMoveX = R2 - L1;

		if ( 0 != iMoveX && they_overlap_y ( pScrob, pOther ) )
		{
			move_to ( pScrob, pScrob->Pos.x + iMoveX, pScrob->Pos.y );
			iQtyChanges++;
		}

		// move y dimension?

		int iMoveY = 0;

		if ( 0 < iGapY1 && iGapY1 < iMaxGap )
			iMoveY = T2 - B1;

		else if ( 0 < iGapY2 && iGapY2 < iMaxGap )
			iMoveY = B2 - T1;

		if ( 0 != iMoveY && they_overlap_x ( pScrob, pOther ) )
		{
			move_to ( pScrob, pScrob->Pos.x, pScrob->Pos.y + iMoveY);
			iQtyChanges++;
		}
	}

	return iQtyChanges;
}

//---------------------------------------------------------------------------------------
// IS OVERLAPPING
//---------------------------------------------------------------------------------------
bool cScrobMgrIm :: is_overlapping ( cScrobIm * p )
{
	assert ( p);

#if 0
	bool bHasBeenFound = false; // only look at scrobs that are beneath (after)
	                            // this one in the Z order
#endif

	for ( cScrobIm * pOther = this->ScrobList.pHead; pOther; pOther = pOther->pNext )
	{
		if ( p == pOther )
		{
			// bHasBeenFound = true;
			continue;
		}
#if 0
		else if ( ! bHasBeenFound )
			continue;
#endif

		else if ( they_overlap_x ( p, pOther ) && they_overlap_y ( p, pOther ) )
		{
			return true;
		}
	}

	return false;
}

//---------------------------------------------------------------------------------------
// DO COLLISION DETECTION
// returns qty changes
//---------------------------------------------------------------------------------------
int cScrobMgrIm::do_collision_detection ( cScrobIm * pScrob )
{
	int iQtyChanges = 0;

	assert ( pScrob );

	for ( cScrobIm * pOther = this->ScrobList.pHead; pOther; pOther = pOther->pNext )
	{
		if ( pScrob == pOther )
			continue;

		// recalc first scrob's points each iteration because
		// it can change as result of move_to at bottom of loop

		int L1 = pScrob->Pos.x;
		int R1 = pScrob->Pos.x + pScrob->Dim.x;
		int T1 = pScrob->Pos.y;
		int B1 = pScrob->Pos.y + pScrob->Dim.y;

		int L2 = pOther->Pos.x;
		int R2 = pOther->Pos.x + pOther->Dim.x;
		int T2 = pOther->Pos.y;
		int B2 = pOther->Pos.y + pOther->Dim.y;

		if ( ( ! they_overlap_x ( pScrob, pOther ) ) ||
			( ! they_overlap_y ( pScrob, pOther ) ) )
			continue;

		int MX1 = ( L1 + R1 ) / 2;
		int MX2 = ( L2 + R2 ) / 2;

		int MY1 = ( B1 + T1 ) / 2;
		int MY2 = ( B2 + T2 ) / 2;

		int iMoveX;
		int iMoveY;

		iMoveX = MX1 < MX2 ?  L2 - R1 : iMoveX = R2 - L1;
		iMoveY = MY1 < MY2 ?  T2 - B1 : iMoveY = B2 - T1;

		if ( ( 0 == iMoveX ) && ( 0 == iMoveY ) )
			NULL ;

		else if ( abs ( iMoveX ) < abs ( iMoveY ) )
		{
			move_to ( pScrob, pScrob->Pos.x + iMoveX, pScrob->Pos.y );
			iQtyChanges++;
		}

		else
		{
			move_to ( pScrob, pScrob->Pos.x,  pScrob->Pos.y + iMoveY );
			iQtyChanges++;
		}
	}

	return iQtyChanges;
}


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