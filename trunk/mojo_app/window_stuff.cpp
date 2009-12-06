/***********************************************************************************************************************
/*
/*    window_stuff.cpp / mojo_app
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
/**********************************************************************************************************************/

#include "stdafx.h"

#include "window_stuff.h"
#include <windowsx.h>


//======================================================================================================================
//  PROTOTYPES
//======================================================================================================================

// bool version_or_higher ( DWORD  dwMaj, DWORD dwMin );


//======================================================================================================================
//  CODE
//======================================================================================================================


//----------------------------------------------------------------------------------------------------------------------
//  SET CHILD WINDOW SIZE
//----------------------------------------------------------------------------------------------------------------------
bool set_child_window_size ( HWND hChild, int iDimX, int iDimY )
{
	assert ( hChild );

	POINT pt;
	pt.x = pt.y = 0;

	HWND hParent = GetParent ( hChild );

	if ( hParent )
		if ( ClientToScreen ( hChild, &pt ) )
			if ( ScreenToClient ( hParent, & pt ) )
				if ( MoveWindow ( hChild, pt.x, pt.y, iDimX, iDimY, FALSE ) )
					return true;

	return false;
}


//-------------------------------------------------------------------------------------------------------
// GET WINDOW TEXT
//-------------------------------------------------------------------------------------------------------
const wchar_t * get_window_text ( cStrW * pRet, HWND hwnd )
{
	wchar_t t [8192];
	GetWindowText ( hwnd, t, sizeof(t)/sizeof(wchar_t) / 1 );
	*pRet = t;
	return pRet->cstr();
}


//-------------------------------------------------------------------------------------------------------
// SET DIALOG FOCUS
//-------------------------------------------------------------------------------------------------------
void set_dialog_focus ( HWND hwnd, int iItemID )
{
	SendMessage ( hwnd, WM_NEXTDLGCTL, (WPARAM) GetDlgItem ( hwnd, iItemID), TRUE );
}


//-------------------------------------------------------------------------------------------------------
// CLIENT COORD TO PARENT CLIENT COORD
//-------------------------------------------------------------------------------------------------------
bool client_coord_to_parent_client_coord ( HWND hwnd, cPtI * pPt )
{
	assert ( hwnd );

	HWND hMom = GetParent ( hwnd );

	if ( ! hMom )
		return false;

	POINT ptKid, ptMom;
	ptKid.x = ptKid.y = ptMom.x = ptMom.y = 0;

	ClientToScreen ( hMom, &ptMom );
	ClientToScreen ( hwnd, &ptKid );

	cPtI Offset;

	Offset.x = ptKid.x - ptMom.x;
	Offset.y = ptKid.y - ptMom.y;

	*pPt += Offset;

	return true;
}


//-------------------------------------------------------------------------------------------------------
// GET CLIENT OFFSET
// client point 0,0 == what in whole-window coordinates?
//-------------------------------------------------------------------------------------------------------
void get_client_offset ( cPtI * pClientOffset, HWND hwnd )
{
	POINT c;
	c.x = c.y = 0;
	ClientToScreen ( hwnd, &c );

	RECT r;
	GetWindowRect ( hwnd, &r );

	pClientOffset->x = c.x - r.left;
	pClientOffset->y = c.y - r.top;
}


//-------------------------------------------------------------------------------------------------------
// IS TOPMOST
// hasn't been tested
//-------------------------------------------------------------------------------------------------------
bool is_topmost ( HWND h )
{
	return 0 == ( GetWindowLong ( h, GWL_EXSTYLE ) & WS_EX_TOPMOST ) ? false : true;
}

//-------------------------------------------------------------------------------------------------------
// GET WINDOW SIZE
//-------------------------------------------------------------------------------------------------------
void get_window_size ( int * pRetX, int * pRetY, HWND hwnd )
{
	RECT rect;
	GetWindowRect( hwnd, &rect );

	*pRetX = rect.right - rect.left;
	*pRetY = rect.bottom - rect.top;
}



//-------------------------------------------------------------------------------------------------------
// MOVE WIN TO CENTER
// moves main window to center of screen
//-------------------------------------------------------------------------------------------------------
void move_win_to_center ( HWND hwnd )
{
	RECT rWin, rDesk;
	GetWindowRect ( hwnd, &rWin );
	GetWindowRect ( GetDesktopWindow(), &rDesk );

	int iPosX = ( rDesk.right  - ( rWin.right  - rWin.left ) ) / 2;
	int iPosY = ( rDesk.bottom - ( rWin.bottom - rWin.top  ) ) / 2;

	iPosX = iPosX < 0 ? 0 : iPosX;
	iPosY = iPosY < 0 ? 0 : iPosY;

	move_window ( hwnd, iPosX, iPosY );
}


//-------------------------------------------------------------------------------------------------------
// SET MAIN WINDOW SIZE
// only for main window because it calls windows in screen units
// child windows are set with client units
//-------------------------------------------------------------------------------------------------------
void set_main_window_size ( HWND hwnd, int x, int y )
{
	RECT rect;
	GetWindowRect( hwnd, &rect );

	MoveWindow ( hwnd, rect.left, rect.top, x, y, TRUE );
}


//-------------------------------------------------------------------------------------------------------
// GET WINDOW RECT
//-------------------------------------------------------------------------------------------------------
void get_window_rect ( cRectI * pRect, HWND hwnd )
{
	RECT rect;
	GetWindowRect( hwnd, &rect );

	pRect->x = rect.left;
	pRect->y = rect.top;
	pRect->dx = rect.right - rect.left;
	pRect->dy = rect.bottom - rect.top;
}


//-------------------------------------------------------------------------------------------------------
// MOVE WINDOW
// in screen units
//-------------------------------------------------------------------------------------------------------
void move_window ( HWND hwnd, cPtI to )
{
#pragma warning ( push )
#pragma warning (disable : 4239 )
	cPtI size = get_window_size ( hwnd );
#pragma warning ( pop )

	MoveWindow ( hwnd, to.x, to.y, size.x, size.y, TRUE );
}


//-------------------------------------------------------------------------------------------------------
// MOVE WINDOW
// in screen units
//-------------------------------------------------------------------------------------------------------
void move_window ( HWND hwnd, int x, int y )
{
	int iDimX, iDimY;

	get_window_size ( &iDimX, &iDimY, hwnd );

	MoveWindow ( hwnd, x, y, iDimX, iDimY, TRUE );
}


//-------------------------------------------------------------------------------------------------------
// GET CLIENT SIZE
//-------------------------------------------------------------------------------------------------------
cPtI get_client_size ( HWND hwnd )
{
	RECT rect;
	GetClientRect( hwnd, &rect );
	cPtI ret;
	ret.x = rect.right - rect.left;
	ret.y = rect.bottom - rect.top;
	return ret;
}


//-------------------------------------------------------------------------------------------------------
// GET WINDOW SIZE
//-------------------------------------------------------------------------------------------------------
cPtI get_window_size ( HWND hwnd )
{
	RECT rect;
	GetWindowRect( hwnd, &rect );
	cPtI ret;
	ret.x = rect.right - rect.left;
	ret.y = rect.bottom - rect.top;
	return ret;
}


//-------------------------------------------------------------------------------------------------------
// GET WINDOW POSITION 
// in screen units
//-------------------------------------------------------------------------------------------------------
cPtI get_window_position ( HWND hwnd )
{
	RECT rect;
	GetWindowRect( hwnd, &rect );
	cPtI ret;
	ret.x = rect.left;
	ret.y = rect.top;
	return ret;
}

//-------------------------------------------------------------------------------------------------------
// GET SCREEN SIZE
//-------------------------------------------------------------------------------------------------------
cPtI get_screen_size ()
{
	HWND hwndMain = GetDesktopWindow();
	RECT rect;

	::GetWindowRect ( hwndMain, & rect );

#pragma warning ( push )
#pragma warning (disable : 4239 )
	return cPtI(rect.right, rect.bottom);
#pragma warning ( pop )

}


//-------------------------------------------------------------------------------------------------------
// GET SHOW STATE
//-------------------------------------------------------------------------------------------------------
unsigned get_show_state ( HWND hwnd )
{
	WINDOWPLACEMENT wp;

	memset( &wp, 0, sizeof(wp));
	wp.length = sizeof(wp);
	GetWindowPlacement( hwnd, &wp );
	return wp.showCmd;
}

//-------------------------------------------------------------------------------------------------------
// GET SCREEN SIZE
//-------------------------------------------------------------------------------------------------------
#if 0
cPtI get_screen_size ()
{
	HWND hwndMain = GetDesktopWindow();
	RECT rect;

	::GetWindowRect ( hwndMain, & rect );

	return cPtI(rect.right, rect.bottom);
	// pScrnDim->x = rect.right;
	// pScrnDim->y = rect.bottom;
}
#endif

