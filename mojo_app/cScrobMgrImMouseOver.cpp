/***********************************************************************************************************************
/*
/*   cScrobMgrImMouseOver.cpp / mojo_app
/*
/*   With this version, the scrobs are ImageList controls.
/*   To use this, inherit this class in a cWin class.
/*   Set this class's hwnd member on initialization and
/*   call this class's various "on" functions from the child
/*   dialog's proc.
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"
#include "cDlgMouseOver.h"
#include "cScrobMgrImMouseOver.h"

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

//---------------------------------------------------------------------------------------
// IS OVERLAPPING
//---------------------------------------------------------------------------------------
bool cScrobMgrImMouseOver :: is_overlapping ( cScrobIm * p )
{
	assert ( p);

	// 	update_display_rects();

	for ( sDispRect * d1 = DispRectList.pHead; d1; d1 = d1->pNext )
	{
		// this loop only for p, i.e., d1 is the left side var
		if ( d1->pScrob != p ) 
			continue;

		for ( sDispRect * d2 = DispRectList.pHead; d2; d2 = d2->pNext )
		{
			// this loop only for not-p, i.e., d2 is the right side var
			if ( d2->pScrob == p )
				continue;

			if ( they_overlap_x ( d1, d2 ) && they_overlap_y ( d1, d2 ) )
				return true;
		}
	}

	return false;
}



//---------------------------------------------------------------------------------------
// ARE CONTIGUOUS
//---------------------------------------------------------------------------------------
bool cScrobMgrImMouseOver :: are_contiguous ( cScrobImMouseOver * p1, cScrobImMouseOver * p2 )
{
	assert ( p1 != p2 );

	// update_display_rects();

	for ( sDispRect * d1 = DispRectList.pHead; d1; d1 = d1->pNext )
	{
		if ( d1->pScrob != p1 )
			continue;

		for ( sDispRect * d2 = DispRectList.pHead; d2; d2 = d2->pNext )
		{
			if ( d2->pScrob != p2 )
				continue;

			int L1 = d1->x;
			int R1 = d1->x + d1->dx;
			int T1 = d1->y;
			int B1 = d1->y + d1->dy;

			int L2 = d2->x;
			int R2 = d2->x + d2->dx;
			int T2 = d2->y;
			int B2 = d2->y + d2->dy;

			if ( cScrobMgrImMouseOver::they_overlap_y ( d1, d2 ) )
			{
				if  ( ( R1 == L2 ) || ( R2 == L1 ) )
					return true;
			}

			else if ( cScrobMgrImMouseOver::they_overlap_x ( d1, d2 ) )
			{
				if  ( ( T1 == B2 ) || ( T2 == B1 ) )
					return true;
			}
		}
	}

	return false;
}



//----------------------------------------------------------------------------------------------
// ON MOUSE LEFT DOWN
//----------------------------------------------------------------------------------------------
void cScrobMgrImMouseOver::on_mouse_left_down ( int x, int y )
{
	cScrobIm * s = get_hit_scrob ( x, y );

	if ( s ) // move disp rect's to end of list
	{
		sDispRect *d = DispRectList.end(); 
		
		while ( d )
		{
			sDispRect * pPrev = d->pPrev;

			if ( d->pScrob == s )
			{
				DispRectList.remove ( d );
				DispRectList.append ( d );
			}

			d = pPrev;
		}
	}

	cScrobMgrIm::on_mouse_left_down ( x, y );
}
	


//----------------------------------------------------------------------------------------------
// GET HIT SCROB
//----------------------------------------------------------------------------------------------
cScrobIm * cScrobMgrImMouseOver::get_hit_scrob ( int x, int y )
{
	update_display_rects();

	// june 5 2009
	for ( sDispRect * d = DispRectList.end(); d; d = d->pPrev )
//	for ( sDispRect * d = DispRectList.pHead; d; d = d->pNext )
	{
		cPtI hit ( x, y );

		if ( is_point_in_rect ( d, &hit ) )
			return d->pScrob;
	}

	return NULL;
}



//---------------------------------------------------------------------------------------
// THEY OVERLAP
//---------------------------------------------------------------------------------------
bool cScrobMgrImMouseOver::they_overlap_x ( cRectI * r1, cRectI * r2 )
{
	int L1 = r1->x;
	int R1 = r1->x + r1->dx;

	int L2 = r2->x;
	int R2 = r2->x + r2->dx;

	if  ( ( R1 <= L2 ) || ( R2 <= L1 ) )
		return false;

	else
		return true;
}

//---------------------------------------------------------------------------------------
// THEY OVERLAP
//---------------------------------------------------------------------------------------
bool cScrobMgrImMouseOver::they_overlap_y ( cRectI * r1, cRectI * r2 )
{
	int T1 = r1->y;
	int B1 = r1->y + r1->dy;

	int T2 = r2->y;
	int B2 = r2->y + r2->dy;

	if ( ( B1 <= T2 ) || ( B2 <= T1 ) )
		return false;

	else
		return true;
}


//---------------------------------------------------------------------------------------
// DO COLLISION DETECTION
// returns qty changes
//---------------------------------------------------------------------------------------
int cScrobMgrImMouseOver::do_collision_detection ( cScrobImMouseOver * pHitScrob )
{
	int iQtyChanges = 0;

	assert ( pHitScrob );

	for ( sDispRect * dh = DispRectList.pHead; dh; dh = dh->pNext ) // dh == disp rect of hit scrob
	{
		if ( dh->pScrob != pHitScrob )
			continue;

		for ( sDispRect * dr = DispRectList.pHead; dr; dr = dr->pNext ) // r = right side of equation
		{
			if ( dh->pScrob == dr->pScrob )
				continue;

		// recalc first scrob's points each iteration because
		// it can change as result of move_to at bottom of loop

			int L1 = dh->x;
			int R1 = dh->x + dh->dx;
			int T1 = dh->y;
			int B1 = dh->y + dh->dy;

			int L2 = dr->x;
			int R2 = dr->x + dr->dx;
			int T2 = dr->y;
			int B2 = dr->y + dr->dy;

			if ( ( ! cScrobMgrImMouseOver::they_overlap_x ( dh, dr ) ) ||
				( ! cScrobMgrImMouseOver::they_overlap_y ( dh, dr ) ) )
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
				move_to ( pHitScrob, pHitScrob->Pos.x + iMoveX, pHitScrob->Pos.y );
				update_display_rects_for_scrob( pHitScrob );
				iQtyChanges++;
			}

			else
			{
				move_to ( pHitScrob, pHitScrob->Pos.x,  pHitScrob->Pos.y + iMoveY );
				update_display_rects_for_scrob ( pHitScrob );
				iQtyChanges++;
			}
		}
	}

	return iQtyChanges;
}


//----------------------------------------------------------------------------------------------
// UPDATE DISPLAY RECT LIST
//----------------------------------------------------------------------------------------------
void cScrobMgrImMouseOver :: update_display_rects ()
{
	// int iIndex = 0;

	for ( sDispRect * d = DispRectList.pHead; d; d = d->pNext )
	{
		// if ( d->pScrob == pHitScrob )
		{
			d->x = d->pScrob->Pos.x + d->Offset.x;
			d->y = d->pScrob->Pos.y + d->Offset.y;
		}
	}

	return;
}


//----------------------------------------------------------------------------------------------
// UPDATE DISPLAY RECT LIST
//----------------------------------------------------------------------------------------------
void cScrobMgrImMouseOver :: update_display_rects_for_scrob ( cScrobImMouseOver * pHitScrob )
{
	// int iIndex = 0;

	for ( sDispRect * d = DispRectList.pHead; d; d = d->pNext )
	{
		if ( d->pScrob == pHitScrob )
		{
			d->x = d->pScrob->Pos.x + d->Offset.x;
			d->y = d->pScrob->Pos.y + d->Offset.y;
		}
	}

	return;
}


//----------------------------------------------------------------------------------------------
// ADJUST POSITION AFTER DRAG
//----------------------------------------------------------------------------------------------
void cScrobMgrImMouseOver :: adjust_position_after_drag ( cScrobIm * pHitScrob )
{
	update_display_rects_for_scrob ( static_cast<cScrobImMouseOver*>(pHitScrob) );

	if ( bMagnetismIsOn )
		do_magnetism ( pHitScrob );	

	if ( bCollisionDetectionIsOn )
	{
		int iQty = this->ScrobList.qty();
			
		for ( int i = 0; i < iQty ; i++ )
		{
			do_collision_detection ( static_cast<cScrobImMouseOver*>(pHitScrob) );
		}
	}
}



//---------------------------------------------------------------------------------------
// DO MAGNETISM
// returns qty changes
//---------------------------------------------------------------------------------------
int cScrobMgrImMouseOver::do_magnetism ( cScrobIm * pScrob )
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

		if ( 0 != iMoveX && cScrobMgrIm::they_overlap_y ( pScrob, pOther ) )
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

		if ( 0 != iMoveY && cScrobMgrIm::they_overlap_x ( pScrob, pOther ) )
		{
			move_to ( pScrob, pScrob->Pos.x, pScrob->Pos.y + iMoveY);
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