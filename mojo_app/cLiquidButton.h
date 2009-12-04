/***********************************************************************************************************************
/*
/*   cLiquidButton.h / mojo_app
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#pragma once

#include "cWin.h"

//-------------------------------------------------------------------------------------------------------
//  CLASS cDlg
//-------------------------------------------------------------------------------------------------------
class cLiquidButton : public cWin
{
public:

	void paint_green ( DRAWITEMSTRUCT* pDI, const wchar_t * pText );
	void paint_red   ( DRAWITEMSTRUCT* pDI, const wchar_t * pText );

private:
	void paint                  ( DRAWITEMSTRUCT* pDI, Gdiplus::Color cOutTop, Gdiplus::Color cOutBot, Gdiplus::Color cInTop, Gdiplus::Color cInBot, const wchar_t * pText, COLORREF clrTxt  );
	// void paint_old              ( DRAWITEMSTRUCT* pDI, Gdiplus::Color cBorder, Gdiplus::Color aC [4], const wchar_t * pText );
	void draw_rounded_rectangle ( Gdiplus::Graphics * g, Gdiplus::GraphicsPath * pGP, Gdiplus::Color c );
	// void paint_gradient         ( DRAWITEMSTRUCT* pDI, Gdiplus::GraphicsPath * pGP, Gdiplus::Color aC [4] ); 
	void paint_gradient         ( Gdiplus::Graphics *g, Gdiplus::RectF * r, Gdiplus::GraphicsPath * gp, Gdiplus::Color cTop, Gdiplus::Color cBot );
};


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