/*************************************************************************************************
/*
/*   cScrobMgrImMouseOver.h
/* 
/*   new version for multi-display scrobs
/*
/*   started November 6, 2008
/*   
/*************************************************************************************************/

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
