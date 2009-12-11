/***********************************************************************************************************************
/*
/*    cScrobIm.cpp / mojo_app
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"
#include "cScrobIm.h"

using namespace Gdiplus;

//===============================================================================================
// DATA
//===============================================================================================



//===============================================================================================
// PROTOTYPES
//===============================================================================================


//===============================================================================================
// CODE
//===============================================================================================

//----------------------------------------------------------------------------------------------
// POINT IS TRANSPARENT
//----------------------------------------------------------------------------------------------
bool cScrobIm :: point_is_transparent ( HDC hdc, int iIndex, int x, int y )
{
	IMAGEINFO ii;

	if ( 0 == ImageList_GetImageInfo ( hIL, iIndex, &ii ) )
	{
		assert(0);
		LOG_SYSTEM_ERROR_T ( L"ImageList_GetImageInfo" );
		return false;
	}
	
	SelectObject ( hdc, HGDIOBJ ( ii.hbmMask ) );

	COLORREF c = GetPixel ( hdc, x, y );
	
	if ( c == 0x00FFFFFF )
		return true;

	else
		return false;
}



//----------------------------------------------------------------------------------------------
// SET
//----------------------------------------------------------------------------------------------
void cScrobIm::set ( int x, int y, HIMAGELIST hArgIL )  // use this to change size and images -- it releases old list
{
	if ( this->hIL )
		ImageList_Destroy ( hIL );

	this->Dim.x = x;
	this->Dim.y = y;
	hIL = hArgIL;
}



//----------------------------------------------------------------------------------------------
// GET RECT
//----------------------------------------------------------------------------------------------
RECT * cScrobIm::get_rect ( RECT * r )
{

	r->left = Pos.x;
	r->top = Pos.y;
	r->right = Pos.x + Dim.x;
	r->bottom = Pos.y + Dim.y;

	return r;
}


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
