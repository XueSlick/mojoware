/***********************************************************************************************************************
/*
/*    cScrobMgrImMouseOver.h / mojo_app
/*
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#pragma once

#include "cScrobMgrIm.h"


class cScrobImMouseOver;


//------------------------------------------------------------------------------------------------
//  DISPLAY RECTANGLE 
//  represents a single display monitor; used for collision detection
//  because a scrob can represent multiple monitors attached to 
//  a single PC.
//------------------------------------------------------------------------------------------------
struct sDispRect : public cRectI
{
	cScrobImMouseOver * pScrob;
	cPtI Offset; // from origin of scrob
	sDispRect * pNext;
	sDispRect * pPrev;
};


//------------------------------------------------------------------------------------------------
//  SCROB MGR IM MOUSEOVER
//------------------------------------------------------------------------------------------------
class cScrobMgrImMouseOver : public cScrobMgrIm
{
protected:
	virtual void on_mouse_left_down ( int x, int y );
	virtual cScrobIm *		get_hit_scrob ( int x, int y );
	bool			is_overlapping ( cScrobIm * p );
	tList<sDispRect> DispRectList; // NEW
	bool			are_contiguous ( cScrobImMouseOver * p1, cScrobImMouseOver * p2 );
	void			update_display_rects       ();

private:

	void			update_display_rects_for_scrob ( cScrobImMouseOver * pHitScrob );

	virtual void	adjust_position_after_drag ( cScrobIm * pHitScrob );
	bool			they_overlap_x ( cRectI * r1, cRectI * r2 );
	bool			they_overlap_y ( cRectI * r1, cRectI * r2 );
	int				do_collision_detection ( cScrobImMouseOver * pScrob );
	int				do_magnetism ( cScrobIm * pScrob );
};

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
