/***********************************************************************************************************************
/*
/*    cDlgViewWoWs.cpp / mojo_app
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"


//======================================================================================================================
// DATA
//======================================================================================================================

const int iButtonHeight = 27;
const int iMargin = 9;
const int iButtonStripDimY = 23;

//======================================================================================================================
// PROTOTYPES
//======================================================================================================================

//======================================================================================================================
// CODE
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
//  POPULATE
//----------------------------------------------------------------------------------------------------------------------
#if 0
void  cDlgViewWoWs :: populate()
{
	this->ListView.populate ( &g_Config.WoWList );
}
#endif


//----------------------------------------------------------------------------------------------------------------------
//  ITEM LIST
//----------------------------------------------------------------------------------------------------------------------
#if 0
const cFigViewItemList * cDlgViewWoWs :: item_list ()
{
	return & g_Config.WoWList;
}
#endif

//----------------------------------------------------------------------------------------------------------------------
// WM INIT
//----------------------------------------------------------------------------------------------------------------------
#if 0
void cDlgViewWoWs::wm_init ()
{
	const int iMargin = 9;
	const int iLeftMargin = 0;

	ListView.hwnd = GetDlgItem ( hwnd, ID_LIST_VIEW );
	register_child ( &ListView,

							  nAnchor::left,		0,		iLeftMargin,
							  nAnchor::top,			0,		iMargin,
							  nAnchor::right,		0,		-iMargin,
							  nAnchor::bottom,		0,      -iMargin );
}
#endif

//----------------------------------------------------------------------------------------------------------------------
// WM PAINT
//----------------------------------------------------------------------------------------------------------------------
#if 0
void cDlgViewWoWs :: wm_paint ()
{


	PAINTSTRUCT ps;
	BeginPaint ( hwnd, &ps );
	HBRUSH hBrush = ( HBRUSH ) GetStockObject ( BLACK_BRUSH );
	// SelectObject ( hdc, (HGDIOBJ) hBrush );
	RECT r;
	GetClientRect ( hwnd, &r );
	r.bottom = iButtonStripDimY;
	FillRect ( ps.hdc, &r, hBrush );

	

	EndPaint ( hwnd, &ps );
}
#endif

//----------------------------------------------------------------------------------------------------------------------
// DEFAULT PROC
//----------------------------------------------------------------------------------------------------------------------
INT_PTR CALLBACK cDlgViewWoWs::dialog_proc (HWND hwnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	cWin * pWin = user_data_to_pWin ( hwnd );
	cDlgViewWoWs * pThis = static_cast<cDlgViewWoWs*>(pWin);

	switch ( uMessage )
	{
	case uWM_WOW_LIST_CHANGED:
		pThis->populate();
		break;

	case WM_INITDIALOG:
		{
			set_user_data ( hwnd, lParam );
			cWin * pWin = lParam_to_pWin ( hwnd, lParam );
			pThis = static_cast<cDlgViewWoWs*>(pWin);
			pThis->hwnd = hwnd;
		}
		break;

	case WM_COMMAND:
		switch ( LOWORD ( wParam ) )
		{
		case ID_ADD:
			{
				cDlgFigWoW d;
				d.make_dlg();
			}
		}
		break;

	default:
		break;
	}

	return cDlgView::dialog_proc ( hwnd, uMessage, wParam, lParam );
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