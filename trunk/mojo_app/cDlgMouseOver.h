/*************************************************************************************************
/*
/*   cDlgMouseOver.h
/* 
/*   new version using image lists
/*
/*   started October 16, 2008
/*   
/*************************************************************************************************/

#pragma once

#include "cDlg.h"
#include "cScrobMgrImMouseOver.h"
#include "cScrobIm.h"

using namespace mojo;

class mojo::cMach;

class cDlgMouseOver;

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


private:
	cMachlist Machlist;
	HBITMAP make_big_bitmap ( int dx, int dy );
};
