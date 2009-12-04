/***********************************************************************************************************************
/*
/*    cLiquidButton.cpp / mojo_app
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

void create_round_rectangle ( GraphicsPath * p, int x, int y, int width, int height, int radius ) ;
void paint_gradient ( Graphics *g, RectF * r, GraphicsPath * gp, Color cTop, Color cBot );

//======================================================================================================================
//  CODE
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
// PAINT 
//----------------------------------------------------------------------------------------------------------------------
void cLiquidButton::paint ( DRAWITEMSTRUCT* pDI, Gdiplus::Color cOutTop, Gdiplus::Color cOutBot, Gdiplus::Color cInTop, Gdiplus::Color cInBot, const wchar_t * pText, COLORREF clrTxt )
{
	cInBot, cInTop;
	Graphics g ( pDI->hDC );
	g.SetSmoothingMode ( SmoothingModeAntiAlias );

	// paths
	GraphicsPath gpIn, gpOut;
	create_round_rectangle ( & gpOut, pDI->rcItem.left,  pDI->rcItem.top,   pDI->rcItem.right - pDI->rcItem.left - 1 , pDI->rcItem.bottom - pDI->rcItem.top - 1, 5 );
	create_round_rectangle ( & gpIn, pDI->rcItem.left,   pDI->rcItem.top,   pDI->rcItem.right - pDI->rcItem.left - 1, (( pDI->rcItem.bottom - pDI->rcItem.top ) / 2 ) - 1, 5 );

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

	SetTextColor    ( pDI->hDC, clrTxt );
	SetBkMode		( pDI->hDC, TRANSPARENT );
	// HFONT hOldFont = (HFONT ) SelectObject ( pDI->hDC, g_hCaptionFont );
	DrawText ( pDI->hDC, pText, -1, &pDI->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE );
	// SelectObject ( pDI->hDC, (HGDIOBJ) hOldFont );
}


//----------------------------------------------------------------------------------------------------------------------
// PAINT GREEN
//----------------------------------------------------------------------------------------------------------------------
void cLiquidButton::paint_green ( DRAWITEMSTRUCT* pDI, const wchar_t * pText )
{
	Color Border ( 0, 235, 0 );
	// paint ( pDI, Color ( 200, 235, 200 ), Color ( 0, 100, 0 ), Color ( 100, 185, 100 ), Color ( 215, 235, 215 ), pText );
	// paint ( pDI, Color ( 180, 220, 180 ), Color ( 44,  60, 44 ), Color ( 100, 120,100 ), Color ( 211, 234, 211 ), pText );
	// paint ( pDI, Color ( 111, 255, 111 ), Color ( 0, 234, 0 ), Color ( 190, 244, 190 ), Color ( 222, 255, 222 ), pText, RGB ( 0, 0, 0 ) );
	// paint ( pDI, Color ( 211, 255, 211 ), Color ( 111, 255, 111 ), Color ( 199, 255, 199 ), Color ( 232, 255, 232 ), pText, RGB ( 0, 0, 0 ) );
	Color c1 ( 178, 245, 178 );
	Color c2 ( 222, 255, 222 );
	paint ( pDI, c1, c1, c2, c2, pText, RGB ( 0, 0, 0 ) );
}


//----------------------------------------------------------------------------------------------------------------------
// PAINT RED
//----------------------------------------------------------------------------------------------------------------------
void cLiquidButton::paint_red ( DRAWITEMSTRUCT* pDI, const wchar_t * pText )
{
	Color  Border ( 235, 0, 0 );
	// paint ( pDI, Color ( 255, 200, 200 ), Color ( 0, 0, 0 ), Color ( 255, 245, 245 ), Color ( 255, 105, 105 ), pText );
	// paint ( pDI, Color ( 255, 200, 200 ), Color ( 100, 0, 0 ), Color ( 205, 100, 100 ), Color ( 255, 235, 235 ), pText, RGB ( 255, 255, 255 ) );
	// paint ( pDI, Color ( 235, 180, 180 ), Color ( 80, 0, 0 ), Color ( 185, 80, 80 ), Color ( 235, 215, 215 ), pText, RGB ( 235, 235, 235 ) );
	// paint ( pDI, Color ( 211, 130, 130 ), Color ( 70, 20, 20 ), Color ( 180, 100, 130 ), Color ( 225, 155, 155 ), pText, RGB ( 255, 255, 255 ) );
	// paint ( pDI, Color ( 60,60, 60 ), Color ( 0, 0, 0 ), Color ( 160, 160, 160 ), Color ( 180, 180, 180 ), pText, RGB ( 255, 255, 255 ) );
	Color ot ( 230, 230, 230 );
	Color ob ( 210, 210, 210 );
	Color it ( 230, 230, 230 );
	Color ib ( 240, 240, 240 );
	paint ( pDI, ot, ob, it, ib, pText, RGB ( 0, 0, 0 ) );
}


//----------------------------------------------------------------------------------------------------------------------
// CREATE ROUND RECTANGLE
//----------------------------------------------------------------------------------------------------------------------
void create_round_rectangle ( GraphicsPath * p, int x, int y, int width, int height, int radius )  
{  
 	if (radius == 0)  
		p->AddRectangle ( Rect( x, y, width, height ) );  

	else
	{  
		p->AddLine ( x + radius, y, x + width - radius, y );  
		p->AddArc  ( x + width - radius, y, radius, radius, 270, 90 );  
		p->AddLine ( x + width, y + radius, x + width, y + height - radius );  
		p->AddArc  ( x + width - radius, y + height - radius, radius, radius, 0, 90 );  
		p->AddLine ( x + width - radius, y + height, x + radius, y + height );  
		p->AddArc  ( x, y + height - radius, radius, radius, 90, 90 );  
		p->AddLine ( x, y + height - radius, x, y + radius );  
		p->AddArc  ( x, y, radius, radius, 180, 90 );  
		p->CloseFigure();  
	}
} 


//----------------------------------------------------------------------------------------------------------------------
// DRAW ROUNDED RECTANGLE
//----------------------------------------------------------------------------------------------------------------------
void cLiquidButton :: draw_rounded_rectangle ( Graphics * g, GraphicsPath * pGP, Color c )
{

	Pen p ( c, 1.0f ); 
    g->DrawPath ( &p, pGP );
}


//----------------------------------------------------------------------------------------------------------------------
// PAINT GRADIENT
//----------------------------------------------------------------------------------------------------------------------
void paint_gradient ( Graphics *g, RectF * r, GraphicsPath * gp, Color cTop, Color cBot )
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