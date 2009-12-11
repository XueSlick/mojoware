/*************************************************************************************************
/*
/*    cDlgMouseOver.h / mojo_app
/*
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#pragma once

#include "cDlg.h"
#include "cScrobMgrImMouseOver.h"
#include "cScrobIm.h"

using namespace mojo;

class mojo::cMach;

class cDlgMouseOver;

//======================================================================================================================
//  CLASSES
//======================================================================================================================

//------------------------------------------------------------------------------------------------
//  CLASS SCROB
//------------------------------------------------------------------------------------------------
class cScrobImMouseOver : public cScrobIm
{
public:
	cScrobImMouseOver () {}
	cScrobImMouseOver ( int x, int y, HIMAGELIST hArgIL ) :  cScrobIm ( x, y, hArgIL ) {}

	void set_size_and_make_image_list ( cDlgMouseOver * pDlg, HWND hwnd );

	virtual int calc_image_index ();

	bool bIsContiguous;  // adjacent to adjacent to adjacent ... to local pc
	                     // used for colors on screen

	bool bIsOverlapping; // used for colors on screen

	cMach * pMach;


};


//------------------------------------------------------------------------------------------------
//  CLASS cDlg
//------------------------------------------------------------------------------------------------
class cDlgMouseOver : public cDlgModal, public cScrobMgrImMouseOver
{
public:

	void save_draw_positions ();


	HIMAGELIST make_image_list_for_single_display ( HWND hwnd, int x, int y, wchar_t * pText, 
										   wchar_t * pImageFilenameGold, wchar_t * pImageFilenameGray );

	HIMAGELIST make_image_list_for_multiple_displays ( HWND hwnd, cScrobImMouseOver * pScrob,
										   wchar_t * pImageFilenameGold,  wchar_t * pImageFilenameGray );

	void set_contiguity ();
	void set_contiguity_sub ( cScrobImMouseOver * );

	void draw_text ( HWND hwnd, HDC hdc, const cStrW * pStr );

	void on_right_button_down ( WORD x, WORD y );

	INT_PTR on_init ( HWND hwnd );
	virtual int idd () { return IDD_MOUSEOVER; }
	virtual DialogProc * dialog_proc () { return dialog_proc; }
	static DialogProc dialog_proc;

	cWin OkayButton;
	cWin CancelButton;
	cWin ShowIP;
	cWin HideRemoteCursor;
	cWin ScreenSaver;
	cWin Hotkey;
	cWin HotkeyLabel;
	cWin SetHotkey;

private:

	cMachlist Machlist;
	HBITMAP make_big_bitmap ( int dx, int dy );
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