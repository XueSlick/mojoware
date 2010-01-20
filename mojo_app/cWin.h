/***********************************************************************************************************************
/*
/*    cWin.h / mojo_app
/*
/*    This class is a wrapper for a window.  The eAnchor stuff is used to change the size and position of child
/*    windows when the parent resizes and/or moves.
/*
/*    All of the application's windows and dialog boxes are represented by classes that derive from this class.
/*
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#pragma once
#include "..\\mojo_engine\\tList.h"
#include "..\\mojo_engine\\tStr.h"
#include "stdafx.h"

using namespace mojo;

//======================================================================================================================
//  PRELIMINARIES
//======================================================================================================================

namespace nAnchor
{
	enum eAnchor { left, top, right, bottom };
}

struct sWinPt
{
	double				pct;
	int					offset;
	nAnchor::eAnchor 	anchor;
};


class cWin;

struct sWinPtr
{
	cWin    * 	pW;
	sWinPtr * 	pNext;
	sWinPtr *	pPrev;
};


//======================================================================================================================
//  MAIN CLASS
//======================================================================================================================

class cWin
{
public:

	cWin () 		: hwnd(0), pUserData(0), hBalloon(0) {}
	cWin ( HWND a ) : hwnd(a), pUserData(0), hBalloon(0) {}

	virtual void wm_size ( int x, int y );
	void wm_move ( int x, int y );

	virtual void set_text () {};
	void set_text_recursive ();
	void set_item_text ( int iID, const wchar_t * pKey, ... );
	void reposition_children ();

	void set_size ( int x, int y ) { MoveWindow ( hwnd, 0, 0, x, y, TRUE ); }
	void move_to_center () { move_win_to_center ( hwnd ); }

	int anchor ( int x, int y, nAnchor::eAnchor a );

	void cWin::register_child ( cWin * p,
						    nAnchor::eAnchor al, double pl, int ol,
		                    nAnchor::eAnchor at, double pt, int ot,
						    nAnchor::eAnchor ar, double pr, int or,
						    nAnchor::eAnchor ab, double pb, int ob );

	void register_child_sub ( sWinPt * pt, nAnchor::eAnchor anch, double pct, int off );

	tList<sWinPtr> ChildList;  	// make this a list of pointers so the list entries can 
	                           	// be deleted independently of the global window objects
	cStrW sLabel;

	HWND hwnd;

	sWinPt left;
	sWinPt top;
	sWinPt right;
	sWinPt bottom;

	void * pUserData; // available for user of this class

protected:

	static VOID CALLBACK kill_balloon_cb ( HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime );
	void wm_mousemove ();
	void wm_buttondown();
	// void wm_mouseleave();
	HWND balloon ( HWND hCtrl, const wchar_t * pTitle, const wchar_t * pBody );
	HWND hBalloon; // new nov 20 2009
	void kill_balloon ();

	static cWin *	user_data_to_pWin	( HWND hwnd );
	static void		set_user_data		( HWND hwnd, cWin * pWin );
	
	static LRESULT CALLBACK window_proc ( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
};


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