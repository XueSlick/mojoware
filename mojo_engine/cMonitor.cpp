/***********************************************************************************************************************
/*
/*    cMonitor.cpp / mojo_engine
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#if 0

#include "stdafx.h"
#include <windowsx.h>

using namespace mojo;

//======================================================================================================================
// DATA
//======================================================================================================================



//======================================================================================================================
// PROTOTYPES
//======================================================================================================================



//======================================================================================================================
// CODE
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
//  
//----------------------------------------------------------------------------------------------------------------------





//=============================================================================================
// DATA
//=============================================================================================

static const COLORREF clrRed	= RGB ( 0xCC, 0x00, 0x33 );
static const COLORREF clrBlue	= RGB ( 0x00, 0x00, 0xDD );
static const COLORREF clrGreen	= RGB ( 0x00, 0x99, 0x00 );
static const COLORREF clrBlack	= RGB ( 0x00, 0x00, 0x00 );

sMon cMonitor :: NullArg;

//=============================================================================================
// CODE
//=============================================================================================

#pragma warning ( push )
#pragma warning ( disable : 4239 ) // nonstandard extension used : 'argument' : conversion from 'sMon' to 'sMon &'



//---------------------------------------------------------------------------------------------
//  PRINT
//  for verb errors
//---------------------------------------------------------------------------------------------
void cMonitor :: print_error ( const wchar_t * pError, const wchar_t * pSource, const wchar_t * pHint)
{ 
	cStrW s;

	cStrW aRay [3];

	int iIndex = 0;

	if ( pSource && wcslen(pSource) )
	{
		aRay[iIndex] = L"Call:   ";
		aRay[iIndex] += pSource;
		iIndex++;
		// cMonitor::print ( s.cstr(), 1, red );
	}

	if ( pError && wcslen(pError) )
	{
		aRay[iIndex] = L"Error:  ";
		aRay[iIndex] += pError;
		iIndex ++;
		// cMonitor::print ( s.cstr(), 1, red );
	}

	if ( pHint && wcslen(pHint) )
	{
		aRay[iIndex] = L"Hint:   ";
		aRay[iIndex] += pHint;
		iIndex++;
		// cMonitor::print ( s.cstr(), 1, red );
	}

	g_Monitor.rcv ( sMon ( aRay[0].cstr(), 1, red ),
					sMon ( aRay[1].cstr(), 1, red ),
					sMon ( aRay[2].cstr(), 1, red ) );
}

#pragma warning ( pop )



//---------------------------------------------------------------------------------------------
//  CLEAR
//---------------------------------------------------------------------------------------------
void cMonitor :: clear ()
{
#if 0
	HWND hCtrl = g_WinMain.monitor();
	Edit_SetSel( hCtrl, 0, -1 );
	Edit_ReplaceSel (hCtrl, "" );
#endif
}


//---------------------------------------------------------------------------------------------
//  PRINT
//---------------------------------------------------------------------------------------------
void cMonitor :: print ( const wchar_t * p, int iIndentLevel, cColor color, eWeight weight )
{ 
	UNREFERENCED_PARAMETER ( p );
	UNREFERENCED_PARAMETER ( iIndentLevel );
	UNREFERENCED_PARAMETER ( color );
	UNREFERENCED_PARAMETER ( weight );
#if 0
	if ( ! p )
		return;

	HWND h = g_WinMain.monitor();

	cStrW m;

	for ( int i = 0; i < iIndentLevel; i++ )
		m += L'\t'; 
		
	m += p;

	if ( regular == weight )
		rich_edit_set_weight_regular ( h );

	else
		rich_edit_set_bold ( h );

	rich_edit_set_color ( h, color.colorref );

	write_line_to_rich_edit  ( h, m.cstr() );
#endif
}


//----------------------------------------------------------------------------------------------
//  DISPLAY
//----------------------------------------------------------------------------------------------
void cMonitor::display()
{
#if 0
	while ( 0 != b.qty() )
	{
		sMonEntry * p = b.get();

		for ( int i = 0; i < 5; i++ )
		{
			if ( p->a[i].sTxt.len() )
				print ( p->a[i].sTxt.cstr(), p->a[i].iIndent,  p->a[i].color, p->a[i].weight );

			else
			{
				write_line_to_rich_edit  ( g_WinMain.monitor(), L"" );
				break;
			}
		}
	}
#endif
}


//----------------------------------------------------------------------------------------------
// RCV
//----------------------------------------------------------------------------------------------
void cMonitor::rcv ( sMon & e0, sMon & e1,  sMon & e2,  sMon & e3,  sMon & e4 )
{
	sMonEntry * p = b.next_ref();
	p->a[0] = e0;
	p->a[1] = e1;
	p->a[2] = e2;
	p->a[3] = e3;
	p->a[4] = e4;

#if 0
	SendMessage ( g_hwnd, uWM_MONITOR, 0, 0 );
#endif
}

#endif

/***********************************************************************************************************************
/*
/*    This file is part of Mojo.  For more information, see http://mojoware.org.
/*
/*    You may redistribute and/or modify Mojo under the terms of the GNU General Public License, 
/*    version 3, as published by the Free Software Foundation.  You should have received a copy of the 
/*    license with mojo.  If you did not, go to http://www.gnu.org.
/*     
/*    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
/*    INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A 
/*    PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
/*    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
/*    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
/*    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
/*    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
/*    EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
/*
/***********************************************************************************************************************/