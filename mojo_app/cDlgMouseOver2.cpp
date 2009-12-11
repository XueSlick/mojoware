/***********************************************************************************************************************
/*
/*    cDlgMouseOver2.cpp / mojo_app
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"
#include "cDlgMouseOver.h"

using namespace Gdiplus;


//===============================================================================================
// DATA
//===============================================================================================

const int iSCALE_FACTOR = 8;
static COLORREF TRANSPARENT_COLOR = RGB ( 0, 255, 0 );


//===============================================================================================
// PROTOTYPES
//===============================================================================================

HBITMAP make_display_bitmap (  int x, int y, const wchar_t * pText, const wchar_t * pImageFilename, bool bBlackText );


//===============================================================================================
// CODE
//===============================================================================================

//----------------------------------------------------------------------------------------------
// MAKE BIG BITMAP
//----------------------------------------------------------------------------------------------
HBITMAP cDlgMouseOver :: make_big_bitmap ( int dx, int dy )
{
	HDC hWinDC = GetDC ( cWin::hwnd );
	HDC hdcD = CreateCompatibleDC ( hWinDC ); 
	HBITMAP hbmD = CreateCompatibleBitmap (hWinDC, dx, dy ); 
	ReleaseDC ( cWin::hwnd, hWinDC );

	SelectObject ( hdcD, (HGDIOBJ) hbmD );
	SetBkColor ( hdcD, TRANSPARENT_COLOR );

	RECT r;
	r.left = 0;
	r.top = 0;
	r.right = dx;
	r.bottom = dy;

	HBRUSH hBrushG = CreateSolidBrush ( TRANSPARENT_COLOR );
	FillRect ( hdcD, &r, hBrushG );
	DeleteObject ( HGDIOBJ ( hBrushG ) );

 	DeleteDC ( hdcD );
	return hbmD;
}


//----------------------------------------------------------------------------------------------
// MAKE IMAGE LIST -- MULTIPLE DISPLAYS
//----------------------------------------------------------------------------------------------
HIMAGELIST cDlgMouseOver :: make_image_list_for_multiple_displays ( HWND hwnd, cScrobImMouseOver * pScrob,
										   wchar_t * pImageFilenameLight,  wchar_t * pImageFilenameDark )
{
	cRectI rDeskScale;

	cMach * pMach = pScrob->pMach;
	
	pMach->DisplayList.calc_mult_virt_rect ( &rDeskScale );

	if ( rDeskScale.dx < 640 || rDeskScale.dy < 480 )
	{
		rDeskScale.dx = 1024;
	    rDeskScale.dy = 768;
	}

	rDeskScale /= iSCALE_FACTOR;

	HIMAGELIST hIL  = ImageList_Create ( rDeskScale.dx, rDeskScale.dy,    ILC_COLORDDB /* ILC_COLOR32 */ |  ILC_MASK , 2, 0);

	/// MAKE BIG BITMAPS
	HBITMAP hbmLight = make_big_bitmap ( rDeskScale.dx, rDeskScale.dy );
	HBITMAP hbmDark = make_big_bitmap ( rDeskScale.dx, rDeskScale.dy );

	HDC hdcWin = GetDC ( hwnd );
	HDC hdcLight = CreateCompatibleDC ( hdcWin );
	SelectObject ( hdcLight, hbmLight );
	HDC hdcDark = CreateCompatibleDC ( hdcWin );
	SelectObject ( hdcDark, hbmDark );
	ReleaseDC ( hwnd, hdcWin );

	/// BLT DISPLAY BITMAPS ONTO BIG ONES
	for ( cDisplay * d = pMach->DisplayList.pHead; d; d = d->pNext )
	{
		cRectI r = *static_cast<cRectI*>(d); // ->VirtRect;
		r /= iSCALE_FACTOR;

		r.x -= rDeskScale.x;
		r.y -= rDeskScale.y;

		// make display rectangle for collision detection
		sDispRect * dr = new sDispRect;
		dr->Offset.x = r.x;
		dr->Offset.y = r.y;
		dr->dx = r.dx;
		dr->dy = r.dy;
		dr->pScrob = pScrob;
		this->DispRectList.append ( dr );

		// MAKE TEXT THAT APPEARS ON LCD PICTURES
		cStrW sText = pMach->sName;

		if ( g_Settings.bShowIpOnMouseoverSettings )
		{
			if ( pMach->sDottedDec.len() )
			{
				sText += '\n';
				sText += pMach->sDottedDec;
			}

			else if ( pMach->bThisPC )
			{
				sText += '\n';
				sText += L"Local PC";
			}
		}

		// FILE NAME HERE
		HBITMAP hDispLightBM = make_display_bitmap ( r.dx, r.dy, sText.cstr(), /* pMach->sName.cstr(), */ pImageFilenameLight, false );
		HDC hdcDispLight = CreateCompatibleDC ( hdcLight );
		SelectObject ( hdcDispLight, hDispLightBM );
		BitBlt ( hdcLight,  r.x, r.y, r.dx, r.dy, hdcDispLight, 0, 0, SRCCOPY );
		DeleteDC ( hdcDispLight );
		DeleteObject ( HGDIOBJ ( hDispLightBM ) );

		HBITMAP hDispDarkBM = make_display_bitmap ( r.dx, r.dy, sText.cstr(), /* pMach->sName.cstr(), */ pImageFilenameDark, false );
		HDC hdcDispDark = CreateCompatibleDC ( hdcDark);
		SelectObject ( hdcDispDark, hDispDarkBM );
		BitBlt ( hdcDark,  r.x, r.y, r.dx, r.dy, hdcDispDark, 0, 0, SRCCOPY );
		DeleteDC ( hdcDispDark );
		DeleteObject ( HGDIOBJ ( hDispDarkBM ) );
	}

	DeleteDC ( hdcLight );
	DeleteDC ( hdcDark );
	
	ImageList_AddMasked ( hIL,  hbmLight, TRANSPARENT_COLOR );
	ImageList_AddMasked ( hIL,  hbmDark, TRANSPARENT_COLOR );

	DeleteObject ( (HGDIOBJ) hbmLight );
	DeleteObject ( (HGDIOBJ) hbmDark );

	return hIL;
}



//----------------------------------------------------------------------------------------------
// MAKE IMAGE LIST
//----------------------------------------------------------------------------------------------
HIMAGELIST cDlgMouseOver :: make_image_list_for_single_display ( HWND hwnd, int x, int y, wchar_t * pText, 
										   wchar_t * pImageFilenameLight, wchar_t * pImageFilenameDark )
{
	UNREFERENCED_PARAMETER(hwnd);
	HIMAGELIST hIL  = ImageList_Create( x, y,  ILC_COLORDDB | ILC_COLOR32| ILC_MASK , 2, 0);
	HBITMAP hBM;

	hBM = make_display_bitmap ( x, y, pText, pImageFilenameLight, false );
	ImageList_Add ( hIL, hBM, NULL );
	hBM = make_display_bitmap ( x, y, pText, pImageFilenameDark, false );
	ImageList_Add ( hIL, hBM, NULL );

	return hIL;
}


//----------------------------------------------------------------------------------------------
// SET SIZE AND MAKE IMAGE LIST
// This can be called repeatedly to change the size and images even
// while the scrob is being displayed
//----------------------------------------------------------------------------------------------
void cScrobImMouseOver :: set_size_and_make_image_list ( cDlgMouseOver * pDlg, HWND hwnd )
{
	int iQtyDisplays = pMach->DisplayList.qty();

	if ( 0 == iQtyDisplays )
	{
		cDisplay * s = new cDisplay;
		s->x = 0;
		s->y = 0;
		s->dx = 1680;
		s->dy = 1050;
		this->pMach->DisplayList.append ( s );
	}

	iQtyDisplays = pMach->DisplayList.qty();

	cRectI rMultVirtRect;
	this->pMach->DisplayList.calc_mult_virt_rect ( & rMultVirtRect );

	cPtI DimScale ( rMultVirtRect.dx, rMultVirtRect.dy );

	if ( DimScale.x < 640 || DimScale.y < 480 )
			DimScale = cPtI ( 1024, 768 );

	DimScale /= iSCALE_FACTOR;

	HIMAGELIST h = 0;

#if 0
	if ( iQtyDisplays < 1 )
	{
		h = pDlg->make_image_list_for_single_display ( hwnd, DimScale.x, DimScale.y, this->pMach->sName.cstr(),
				L"mouseover_gold.jpg",  L"mouseover_gray.jpg");
	}

	else

#endif
	{
		h = pDlg->make_image_list_for_multiple_displays ( hwnd, this, L"mouseover_blue.jpg",  L"mouseover_gray.jpg" );
	}


	this->set_transparent_color ( TRANSPARENT_COLOR );

	this->set ( DimScale.x, DimScale.y, h );
}


//----------------------------------------------------------------------------------------------
// MAKE DISPLAY_BITMAP
//----------------------------------------------------------------------------------------------
HBITMAP make_display_bitmap ( int x, int y, const wchar_t * pText, 
							   const wchar_t * pImageFilename, bool bBlackText )
{
	cStrW sPath;
	mojo::get_module_directory ( & sPath );
	sPath += pImageFilename;

	if ( ! file_exists ( sPath.cstr() ) )
	{
		mojo::put_ad_lib_memo ( mojo::cMemo::error, L"Image file not found",  sPath.cstr() );
		cStrW s;
		s.f ( L"Image file not found: %s", sPath.cstr() );
		LOG ( s.cstr() );
		return NULL;
	}

	Bitmap bmOriginalSize ( sPath.cstr() );
	Graphics gOld ( &bmOriginalSize );

	FontFamily  fontFamily ( L"Verdana" );
	Font        font ( &fontFamily, 13, FontStyleRegular, UnitPixel);

	SolidBrush  solidBrush ( Color ( 0, 0, 0, 255 ) );
	
	if ( bBlackText )
		solidBrush.SetColor ( Color ( 255, 0, 0, 0 ) );

	else
		solidBrush.SetColor ( Color ( 255, 255, 255, 255 ) );

	Bitmap bmNew ( x, y, & gOld );
	Graphics gNew ( &bmNew );
	gNew.DrawImage ( &bmOriginalSize, 0, 0, x, y );
	RectF rectG;
	rectG.Height = (float) bmNew.GetHeight();
	rectG.Width = (float) bmNew.GetWidth();
	StringFormat format;
	format.SetAlignment(StringAlignmentCenter);
	format.SetLineAlignment(StringAlignmentCenter);
	gNew.DrawString ( pText, -1, &font, rectG, &format, &solidBrush );

	HBITMAP hBM;
	bmNew.GetHBITMAP ( NULL, &hBM );
	return hBM;
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

