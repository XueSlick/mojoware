/***********************************************************************************************************************
/*
/*    cScrobMIm.h / mojo_app
/*
/*    A scrob is a "screen object," a thing that gets moved around the screen.  "Im" stands for ImageList (a Windows
/*    control) because this particular implementation uses them.
/*
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

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

