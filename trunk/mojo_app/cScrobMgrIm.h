/*************************************************************************************************
/*
/*   cScrobMgrIm.h
/*
/*   uses ImageLists
/*
/*   See cpp for instructions.
/*
/*   started October 16, 2008
/*   
/*************************************************************************************************/

#pragma once

#include <tPt.h>

#include "cScrobIm.h"


//------------------------------------------------------------------------------------------------
//  CLASS MANAGER
//------------------------------------------------------------------------------------------------
class cScrobMgrIm
{
public:
	cScrobMgrIm () : hwnd(0), pHitScrob(0), bMagnetismIsOn(true), bCollisionDetectionIsOn(true) {}

	virtual cScrobIm * get_hit_scrob ( int x, int y );
	virtual void on_mouse_left_down ( int x, int y );
	void on_mouse_move ( int x, int y );
	void on_mouse_left_up ( int x, int y );
	void on_close_window();

	bool add_scrob ( cScrobIm * pS );
	bool draw_scrobs ( HDC hdc );
	tList<cScrobIm> ScrobList;
	bool mouse_is_outside_our_client_area ( int x, int y );
	void get_last_inside_point ( cPtI * pRet, int x, int y );

	HWND hwnd;
	cScrobIm * pHitScrob;
	cPtI HotSpot;
	cPtI ClientOffset; // difference between window coord's and client coord's

	bool bMagnetismIsOn;
	bool bCollisionDetectionIsOn;
	
	bool is_overlapping ( cScrobIm * p );

	bool are_contiguous ( cScrobIm * p1, cScrobIm * p2 );
	bool they_overlap_x ( cScrobIm * p1, cScrobIm * p2 );
	bool they_overlap_y ( cScrobIm * p1, cScrobIm * p2 );
	int do_magnetism ( cScrobIm * pScrob );

	virtual void adjust_position_after_drag ( cScrobIm * pHitScrob );


	void move_to ( cScrobIm * pScrob, int x, int y ) { pScrob->Pos = cPtI ( x, y ); }
	int do_collision_detection ( cScrobIm * pScrob );
};
