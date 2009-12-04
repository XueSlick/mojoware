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

//======================================================================================================================
//  CODE
//======================================================================================================================

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
void cLiquidButton :: draw_rounded_rectangle ( DRAWITEMSTRUCT* pDI, GraphicsPath * pGP, Color c )
{
	Graphics g ( pDI->hDC );
	g.SetSmoothingMode ( SmoothingModeAntiAlias );
	Pen p ( c, 1.0f ); 
    g.DrawPath ( &p, pGP );
}


//----------------------------------------------------------------------------------------------------------------------
// PAINT GRADIENT
//----------------------------------------------------------------------------------------------------------------------
void cLiquidButton::paint_gradient ( DRAWITEMSTRUCT* pDI, GraphicsPath * pGP, Color aC[4] )
{
	//------------------------------------
	//  GRADIENT
	//------------------------------------
	
	HDC hdc = pDI->hDC;
	RECT rCli = pDI->rcItem;
	RectF r;
	r.X =      (Gdiplus::REAL) rCli.left;
	r.Y =      (Gdiplus::REAL) rCli.top;
	r.Width =  (Gdiplus::REAL) rCli.right;
	r.Height = (Gdiplus::REAL) rCli.bottom;

	RectF rTop  = r;
	RectF rBot  = r;
	rTop.Height = r.Height / 2;
	rBot.Y      =  r.Y + rTop.Height;
	rBot.Height =  r.Height - rTop.Height;

	//------------------------------------
	//  GRADIENT BRUSH
	//------------------------------------

	LinearGradientBrush b ( r,  Color ( 255, 0, 0 ), Color ( 111, 0 , 0 ), LinearGradientModeVertical );
	REAL aPositions [] = { 0.0f, .45f, .45f, 1.0f };
	b.SetInterpolationColors ( aC, aPositions, sizeof(aPositions)/sizeof(REAL) );
	Graphics g ( hdc );
	g.FillPath ( &b, pGP );
}


//----------------------------------------------------------------------------------------------------------------------
// PAINT GREEN
//----------------------------------------------------------------------------------------------------------------------
void cLiquidButton::paint_green ( DRAWITEMSTRUCT* pDI, const wchar_t * pText )
{
	Color Border ( 0, 235, 0 );
	// Color aColors [] = { Color ( 243, 255, 243 ), Color ( 200, 235, 200 ), Color ( 100, 222, 100 ), Color ( 100, 212, 100 ) };
	Color aColors [] = { Color ( 243, 255, 243 ), Color ( 210, 245, 210 ), Color ( 190, 235, 190 ), Color ( 100, 212, 100 ) };
	paint ( pDI, Border, aColors, pText );
}


//----------------------------------------------------------------------------------------------------------------------
// PAINT RED
//----------------------------------------------------------------------------------------------------------------------
void cLiquidButton::paint_red ( DRAWITEMSTRUCT* pDI, const wchar_t * pText )
{
	Color  Border ( 235, 0, 0 );
	// Color aColors [] =    { Color ( 255, 245, 245 ), Color ( 255, 195, 195 ), Color ( 242, 100, 100 ), Color ( 232, 100, 100 ) };
	Color aColors [] =    { Color ( 255, 245, 245 ), Color ( 255, 195, 195 ), Color ( 252, 140, 140 ), Color ( 222, 90, 90 ) };
	paint ( pDI, Border, aColors, pText );
}


//----------------------------------------------------------------------------------------------------------------------
// PAINT 
//----------------------------------------------------------------------------------------------------------------------
void cLiquidButton::paint ( DRAWITEMSTRUCT* pDI, Color cBorder, Color aC [4], const wchar_t * pText )
{
	pText;
	GraphicsPath gp;
	create_round_rectangle ( &gp, pDI->rcItem.left, pDI->rcItem.top, pDI->rcItem.right - pDI->rcItem.left - 1 , pDI->rcItem.bottom - pDI->rcItem.top - 1, 9 );
	paint_gradient ( pDI, &gp, aC );
	draw_rounded_rectangle ( pDI, &gp, cBorder );
	SetBkMode		( pDI->hDC, TRANSPARENT );
	DrawText ( pDI->hDC, pText, -1, &pDI->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE );
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