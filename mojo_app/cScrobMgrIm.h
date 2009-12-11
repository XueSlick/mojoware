/***********************************************************************************************************************
/*
/*    cScrobMgrIm.h / mojo_app
/*
/*    uses ImageLists
/*
/*    See cpp for instructions.
/*
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#pragma once

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
