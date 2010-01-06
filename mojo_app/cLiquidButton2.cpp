/***********************************************************************************************************************
/*
/*    cLiquidButton2.cpp / mojo_app
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"
#include "cDlg.h"

using namespace Gdiplus;


//======================================================================================================================
// DATA
//======================================================================================================================

//======================================================================================================================
//  PROTOTYPES
//======================================================================================================================

void create_rounded_rectangle_path ( GraphicsPath * p, int x, int y, int dx, int dy, int r );
void paint_gradient ( Graphics *g, RectF * r, GraphicsPath * gp, Color cTop, Color cBot );

//======================================================================================================================
//  CODE
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
// PAINT 
//----------------------------------------------------------------------------------------------------------------------

void cLiquidButton2::paint ( DRAWITEMSTRUCT* pDI, Gdiplus::Color cOutTop, Gdiplus::Color cOutBot, Gdiplus::Color cInTop, Gdiplus::Color cInBot, const wchar_t * pText, COLORREF clrTxt )
{

// #define DOUBLE_BUFFER
	HDC hdcOriginal = pDI->hDC;
#ifdef DOUBLE_BUFFER
	HDC		hdcMem	= CreateCompatibleDC ( hdcOriginal );
	HBITMAP hbmMem	= CreateCompatibleBitmap(hdcOriginal, pDI->rcItem.right, pDI->rcItem.bottom);
	HANDLE	hOld	= SelectObject(hdcMem, hbmMem);
	HDC hdc			= hdcMem;
#else
	HDC hdc = hdcOriginal;
#endif

	cInBot, cInTop;
	Graphics g ( hdc );
	g.SetSmoothingMode ( SmoothingModeAntiAlias );

	// paths
	GraphicsPath gpIn, gpOut;
	create_rounded_rectangle_path ( & gpOut, pDI->rcItem.left,  pDI->rcItem.top,   pDI->rcItem.right - pDI->rcItem.left - 1 , pDI->rcItem.bottom - pDI->rcItem.top - 1, 5 );
	create_rounded_rectangle_path ( & gpIn, pDI->rcItem.left,   pDI->rcItem.top,   pDI->rcItem.right - pDI->rcItem.left - 1, (( pDI->rcItem.bottom - pDI->rcItem.top ) / 2 ) - 1, 5 );

	// lower gradient

	RectF rOut (  (float) pDI->rcItem.left, (float) pDI->rcItem.top, (float) pDI->rcItem.right - pDI->rcItem.left, (float) pDI->rcItem.bottom - pDI->rcItem.top );
	::paint_gradient ( &g, &rOut, &gpOut, cOutTop, cOutBot );

#if 1

	// top gradient
	RectF rIn = rOut;
	rIn.Height /= 2;
	::paint_gradient ( &g, &rIn, &gpIn, cInTop, cInBot );
#endif

	// border

	draw_rounded_rectangle ( &g, &gpOut, Color ( 127, 127, 127 ) ); 

	// text

	SetTextColor    ( hdc, clrTxt );
	SetBkMode		( hdc, TRANSPARENT );
	// HFONT hOldFont = (HFONT ) SelectObject ( hdc, g_hCaptionFont );
	DrawText ( hdc, pText, -1, &pDI->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE );
	// SelectObject ( hdc, (HGDIOBJ) hOldFont );

#ifdef DOUBLE_BUFFER
	BitBlt ( hdcOriginal, pDI->rcItem.left, pDI->rcItem.top, pDI->rcItem.right, pDI->rcItem.bottom, hdcMem, 0, 0, SRCCOPY);
	SelectObject ( hdcMem, hOld );
	DeleteObject ( hbmMem );
	DeleteDC     ( hdcMem );
#endif
}


//----------------------------------------------------------------------------------------------------------------------
// PAINT GREEN
//----------------------------------------------------------------------------------------------------------------------
void cLiquidButton2::paint_green ( DRAWITEMSTRUCT* pDI, const wchar_t * pText )
{
	Color Border ( 0, 235, 0 );
#if 0
	Color c1 ( 178, 245, 178 );
	Color c2 ( 222, 255, 222 );
#endif

	Color c1 ( 210, 245, 210 );
	Color c2 ( 233, 255, 233 );

	paint ( pDI, c1, c1, c2, c2, pText, RGB ( 0, 0, 0 ) );
}


//----------------------------------------------------------------------------------------------------------------------
// PAINT GRAY
//----------------------------------------------------------------------------------------------------------------------
void cLiquidButton2::paint_gray ( DRAWITEMSTRUCT* pDI, const wchar_t * pText )
{
	//-----------------------------------------
	//  I = INNER BORDER,
	//  M == MIDDLE, O = OUTER
	//-----------------------------------------

	Color clrI ( 169, 169, 169 );
	Color clrM ( 0, 0, 0 );
	Color clrO ( 112, 112, 112 );

	Color clrBackground ( 70, 70, 70 );


	COLORREF clrTxt = RGB ( 255, 255, 255 );

	// paint ( pDI, ot, ob, it, ib, pText, RGB ( 0, 0, 0 ) );


	HDC hdc = pDI->hDC;
	Graphics g ( hdc );
	g.SetSmoothingMode ( SmoothingModeNone ); // SmoothingModeAntiAlias8x8 ); // SmoothingModeAntiAlias );

	// paths
	GraphicsPath gpI, gpM, gpO, gpL, gpU;

	// OUTER BORDER
	create_rounded_rectangle_path ( & gpO, pDI->rcItem.left,    pDI->rcItem.top,     pDI->rcItem.right - pDI->rcItem.left - 1 , pDI->rcItem.bottom - pDI->rcItem.top - 1, 5 );

	// MIDDLE BORDER
	create_rounded_rectangle_path ( & gpM, pDI->rcItem.left+1,  pDI->rcItem.top+1,   pDI->rcItem.right - pDI->rcItem.left - 3 , pDI->rcItem.bottom - pDI->rcItem.top - 3, 5 );

	// INNER BORDER
	create_rounded_rectangle_path ( & gpI, pDI->rcItem.left+2,  pDI->rcItem.top+2,   pDI->rcItem.right - pDI->rcItem.left - 5 , pDI->rcItem.bottom - pDI->rcItem.top - 5, 5 );

	// UPPER
	create_rounded_rectangle_path ( & gpL, pDI->rcItem.left,   pDI->rcItem.top,   pDI->rcItem.right - pDI->rcItem.left - 1, (( pDI->rcItem.bottom - pDI->rcItem.top ) / 2 ) - 1, 0 );

	//------------------------------------------
	// PAINT
	//------------------------------------------

	RectF rWhole = RectF ( (float)pDI->rcItem.left, (float)pDI->rcItem.top, (float)pDI->rcItem.right - pDI->rcItem.left, (float)pDI->rcItem.bottom - pDI->rcItem.top );
	SolidBrush bBackground ( clrBackground );
	g.FillRectangle ( &bBackground, rWhole );

	// PAINT UPPER GRADIENT
	RectF rUG = RectF ( (float)pDI->rcItem.left, (float)pDI->rcItem.top, (float)pDI->rcItem.right - pDI->rcItem.left, (float)pDI->rcItem.bottom - pDI->rcItem.top );
	// rUG.Height /= 2;
	paint_gradient ( &g, &rUG, &gpO, Color ( 170, 170, 170 ), Color ( 70, 70, 70 ) );


	draw_rounded_rectangle ( &g, &gpI, clrI  );
	draw_rounded_rectangle ( &g, &gpM, clrM  ); 
	draw_rounded_rectangle ( &g, &gpO, clrO  ); 

	// text

	SetTextColor    ( hdc, clrTxt );
	SetBkMode		( hdc, TRANSPARENT );
	DrawText ( hdc, pText, -1, &pDI->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE );

}


//----------------------------------------------------------------------------------------------------------------------
// CREATE ROUND RECTANGLE
//----------------------------------------------------------------------------------------------------------------------
void create_rounded_rectangle_path ( GraphicsPath * p, int x, int y, int dx, int dy, int r )  
{  
 	if ( 0 == r )  
		p->AddRectangle ( Rect( x, y, dx, dy ) );  

	else
	{  
		p->AddLine ( x + r, y, x + dx - r, y );  
		p->AddArc  ( x + dx - r, y, r, r, 270, 90 );  
		p->AddLine ( x + dx, y + r, x + dx, y + dy - r );  
		p->AddArc  ( x + dx - r, y + dy - r, r, r, 0, 90 );  
		p->AddLine ( x + dx - r, y + dy, x + r, y + dy );  
		p->AddArc  ( x, y + dy - r, r, r, 90, 90 );  
		p->AddLine ( x, y + dy - r, x, y + r );  
		p->AddArc  ( x, y, r, r, 180, 90 );  
		p->CloseFigure();  
	}
} 


//----------------------------------------------------------------------------------------------------------------------
// DRAW ROUNDED RECTANGLE
//----------------------------------------------------------------------------------------------------------------------
void cLiquidButton2 :: draw_rounded_rectangle ( Graphics * g, GraphicsPath * pGP, Color c )
{
	Pen p ( c, 1.0f ); 
    g->DrawPath ( &p, pGP );
}


//----------------------------------------------------------------------------------------------------------------------
// PAINT GRADIENT
//----------------------------------------------------------------------------------------------------------------------
void cLiquidButton2 :: paint_gradient ( Graphics *g, RectF * r, GraphicsPath * gp, Color cTop, Color cBot )
{
	LinearGradientBrush b ( *r,  cTop, cBot, LinearGradientModeVertical );
	g->SetSmoothingMode ( SmoothingModeAntiAlias );
	g->FillPath ( &b, gp );
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