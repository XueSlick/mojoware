/*************************************************************************************************
/*
/*   cScrobMIm.h
/*
/*   uses ImageLists
/*
/*   started October 17, 2008
/*   
/*************************************************************************************************/

#pragma once

#include <tPt.h>


//------------------------------------------------------------------------------------------------
//  CLASS SCROB
//------------------------------------------------------------------------------------------------
class cScrobIm
{
public:
	cScrobIm () : hIL(0), Dim(0,0), Pos(0,0), iImageIndex(-1) {}

	cScrobIm ( int x, int y, HIMAGELIST hArgIL ) :  hIL(hArgIL ), Dim(x,y) {}

	void set ( int x, int y, HIMAGELIST hArgIL );  // use this to change size and images -- it releases old list

	virtual int calc_image_index () { return 0; } // override this in derived class to control which image in image list

	bool point_is_transparent ( HDC hdc, int iIndex, int x, int y );

	void set_transparent_color ( COLORREF arg ) { TransparentColor = arg; }

	COLORREF TransparentColor;

	cPtI Pos;
	cPtI Dim;
	HIMAGELIST hIL;

	int iImageIndex;
	
	RECT * get_rect ( RECT * r );

	cScrobIm * pPrev;
	cScrobIm * pNext;
};

