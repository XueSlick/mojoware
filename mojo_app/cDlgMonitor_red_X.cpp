/***********************************************************************************************************************
/*
/*    cDlgMonitor_red_X.cpp / mojo_app
/*
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#pragma once

#include "stdafx.h"

//======================================================================================================================
//  DATA
//======================================================================================================================


//======================================================================================================================
//  CODE
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
//  DRAW RED X
//----------------------------------------------------------------------------------------------------------------------
#ifdef RED_X
void cDlgMonitor :: draw_red_X ()
{
	// if ( ! g_Settings.bHotkeysAreOn )
	{
		HDC hdc = GetDC ( RedX.hwnd );
		RECT rect;
		GetClientRect ( RedX.hwnd, &rect );

		HBRUSH hBrush = (HBRUSH) GetStockObject ( WHITE_BRUSH );

		// double buffer it
		{
			HDC		hdcMem = CreateCompatibleDC ( hdc );
			HBITMAP hbmMem = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
			HANDLE	hOld   = SelectObject(hdcMem, hbmMem);

			// do the drawing 
	
			FillRect	( hdcMem, &rect, hBrush );

			rect.top += (LONG) ( .03 * rect.bottom );

			SetBkMode	( hdcMem, TRANSPARENT );
			SetTextColor    ( hdcMem, RGB ( 255, 233, 233 ) );

			LOGFONT lf;
			memset ( &lf, 0, sizeof(lf) );
			wcscpy_s  ( lf.lfFaceName, sizeof(lf.lfFaceName)/sizeof(wchar_t), L"Arial" );
			lf.lfHeight = (LONG) (rect.bottom * 1.2) ;
			lf.lfWeight = 100;
			lf.lfWidth = (LONG) (rect.right * .55);

			HFONT hFont = CreateFontIndirect(&lf);

			SelectFont ( hdcMem, hFont );

			DrawText	( hdcMem, L"X", 1, &rect,  DT_CENTER | DT_VCENTER | DT_SINGLELINE  );

			DeleteFont ( hFont );

			GetClientRect ( RedX.hwnd, &rect );
			rect.bottom = LONG ( rect.bottom * .97 );

			hFont = g_hMenuFont;
			SelectFont ( hdcMem, hFont );
			SetTextColor ( hdcMem, RGB ( 0, 0, 0 ) );

			DrawText ( hdcMem, L"Broadcast is off.", -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE );

			// Transfer the off-screen DC to the screen
			GetClientRect ( RedX.hwnd, &rect );
			BitBlt(hdc, 0, 0, rect.right, rect.bottom, hdcMem, 0, 0, SRCCOPY);

			// Free-up the off-screen DC
			SelectObject(hdcMem, hOld);
			DeleteObject(hbmMem);
			DeleteDC    (hdcMem);
		}

		ReleaseDC ( RedX.hwnd, hdc );
	}
}
#endif

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