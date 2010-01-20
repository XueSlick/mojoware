/***********************************************************************************************************************
/*
/*    cWin_balloon.cpp / mojo_app
/*   
/*    Copyright 2010 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "StdAfx.h"
#include "cWin.h"

//======================================================================================================================
//  CODE
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
//  CREATE BALLOON
//----------------------------------------------------------------------------------------------------------------------
HWND cWin :: balloon ( HWND hCtrl, const wchar_t * pTitle, const wchar_t * pBody )
{
	if ( hBalloon )
		DestroyWindow ( hBalloon );

	hBalloon = ::display_balloon ( hCtrl, pTitle, pBody );
	SetCapture ( hwnd );
	int iDuration = GetDoubleClickTime() * 10; // This is how Microsoft says to do it
	SetTimer ( hwnd, UINT_PTR ( this ), iDuration, kill_balloon_cb );

	return hBalloon;
}


//----------------------------------------------------------------------------------------------------------------------
//  KILL BALLOON CALLBACK
//----------------------------------------------------------------------------------------------------------------------
VOID CALLBACK cWin :: kill_balloon_cb ( HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime )
{
	dwTime, uMsg, hwnd;

	cWin * pThis =  reinterpret_cast<cWin*> ( idEvent );
	KillTimer ( pThis->hwnd, idEvent );
	pThis->kill_balloon();
}


//----------------------------------------------------------------------------------------------------------------------
//  KILL BALLOON
//----------------------------------------------------------------------------------------------------------------------
void cWin :: kill_balloon ()
{
	if ( hBalloon )
	{
		DestroyWindow ( hBalloon );
		hBalloon = 0;
		ReleaseCapture();
	}
}


//----------------------------------------------------------------------------------------------------------------------
//  WM MOVE
//----------------------------------------------------------------------------------------------------------------------
void cWin::wm_move ( int x, int y )
{
	UNREFERENCED_PARAMETER (x);
	UNREFERENCED_PARAMETER (y);

	if ( hBalloon )
		kill_balloon();
}


//----------------------------------------------------------------------------------------------------------------------
//  WM BUTTONDOWN
//----------------------------------------------------------------------------------------------------------------------
#if 0
void cWin :: wm_mouseleave ()
{
	if ( hBalloon )
		kill_balloon();
}
#endif


//----------------------------------------------------------------------------------------------------------------------
//  WM BUTTONDOWN
//----------------------------------------------------------------------------------------------------------------------
void cWin :: wm_buttondown ()
{
	if ( hBalloon )
		kill_balloon();
}


//----------------------------------------------------------------------------------------------------------------------
//  WM MOUSEMOVE
//----------------------------------------------------------------------------------------------------------------------
void cWin :: wm_mousemove ()
{
	if ( hBalloon )
	{
		RECT r;
		GetWindowRect ( hBalloon, &r );
		POINT pt;
		GetCursorPos ( &pt );

		TOOLINFO ti;
		memset ( &ti, 0, sizeof(ti) );
		ti.cbSize = sizeof(TOOLINFO);

		const int iFUDGE = 110;

		if ( ( pt.x < r.left - iFUDGE ) ||
			 ( pt.y < r.top - iFUDGE ) ||
			 ( r.right + iFUDGE < pt.x ) ||
			 ( r.bottom + iFUDGE < pt.y ) )
		{
			kill_balloon();
		}
	}
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