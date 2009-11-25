/***********************************************************************************************************************
/*
/*    cWinLabel.cpp / mojo_app
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"


//======================================================================================================================
//  CODE
//======================================================================================================================

void cWinLabel :: wm_init ()
{
	const int iRIGHT_PADDING = 3; // in pixels

	HWND hCtrl = hwnd;
	SendMessage ( hCtrl, EM_SETMARGINS, EC_LEFTMARGIN, 0 );
	wchar_t t [4096];
	GetWindowText ( hCtrl, t, sizeof(t)/sizeof(wchar_t ) );
	RECT r;
	SendMessage ( hCtrl, EM_GETRECT, 0, LPARAM ( &r ) );

	HDC hdc = GetDC ( hCtrl );
	HFONT hFont = (HFONT) SendMessage ( hCtrl, WM_GETFONT, 0, 0 );
	SelectObject ( hdc, (HGDIOBJ) hFont );
	DrawText ( hdc, t, (int) wcslen ( t ), &r, DT_CALCRECT );
	set_child_window_size ( hCtrl, r.right - r.left + iRIGHT_PADDING, r.bottom - r.top );
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