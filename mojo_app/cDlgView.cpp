/***********************************************************************************************************************
/*
/*    cDlgView.cpp / mojo_app
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"

#include "cDlgFigWoW.h"

//======================================================================================================================
// DATA
//======================================================================================================================

const int iButtonHeight = 27;
const int iMargin = 9;
static int s_iStripDimY = 20;
static int s_iStripButtonDimX = 80;

//======================================================================================================================
// PROTOTYPES
//======================================================================================================================

//======================================================================================================================
// CODE
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
// DRAW STRIP
//----------------------------------------------------------------------------------------------------------------------
void cDlgView :: draw_strip ( HDC hdcOriginal, const wchar_t * pText )
{

	const COLORREF clrCOLOR = RGB ( 44, 44, 110 );

	const int iLeftMargin = 9;


	RECT rBuf;
	GetClientRect ( hwnd, &rBuf );
	rBuf.right -= iMargin;
	rBuf.bottom = s_iStripDimY;

	HDC		hdcMem	= CreateCompatibleDC ( hdcOriginal );
	HBITMAP hbmMem	= CreateCompatibleBitmap(hdcOriginal, rBuf.right, rBuf.bottom);
	HANDLE	hOld	= SelectObject(hdcMem, hbmMem);
	HDC hdc			= hdcMem;

	HBRUSH hBrush	= CreateSolidBrush ( clrCOLOR );
	SelectObject	( hdc, HGDIOBJ ( hBrush ) );
	FillRect		( hdc, &rBuf, hBrush );
	DeleteObject	( (HGDIOBJ)  hBrush );

	if ( wcslen ( pText ) )
	{
		DWORD dwStyles	= DT_VCENTER | DT_SINGLELINE;
		rBuf.left = iLeftMargin;
		cDlg::draw_text ( hdc, &rBuf, caption, pText, true, dwStyles );
	}

	rBuf.left = 0;

	BitBlt ( hdcOriginal, 0, iMargin, rBuf.right + iMargin, rBuf.bottom + iMargin, hdcMem, 0, 0, SRCCOPY);
	SelectObject ( hdcMem, hOld );
	DeleteObject ( hbmMem );
	DeleteDC     ( hdcMem );



}


//----------------------------------------------------------------------------------------------------------------------
// WM PAINT
//----------------------------------------------------------------------------------------------------------------------
void cDlgView :: wm_paint ()
{
	PAINTSTRUCT ps;
	BeginPaint ( hwnd, &ps );
	draw_strip ( ps.hdc, L"WoWs" );
	EndPaint ( hwnd, &ps );
}


//----------------------------------------------------------------------------------------------------------------------
// WM INIT
//----------------------------------------------------------------------------------------------------------------------
void cDlgView::wm_init ()
{
	const int iMargin = 9;
	const int iLeftMargin = 0;

	//--------------------------------
	// MAIN LIST VIEW CONTROL
	//--------------------------------

	pListView->hwnd = GetDlgItem ( hwnd, ID_LIST_VIEW );
	register_child ( pListView,

							  nAnchor::left,		0,		iLeftMargin,
							  nAnchor::top,			0,		iMargin * 1 + s_iStripDimY,
							  nAnchor::right,		0,		-iMargin,
							  nAnchor::bottom,		0,      -iMargin );

	//--------------------------------
	// SMALL TOGGLE VIEW BUTTON
	//--------------------------------

	ToggleView.hwnd = GetDlgItem ( hwnd, ID_TOGGLE_VIEW );
	register_child ( &ToggleView,

							  nAnchor::right,		0,		- ( iMargin * 2 + s_iStripButtonDimX ),
							  nAnchor::top,			0,		iMargin + 1,
							  nAnchor::right,		0,		-( iMargin * 2 ),
							  nAnchor::top,		    0,      iMargin + s_iStripDimY - 1 );

	//--------------------------------
	// SMALL ADD BUTTON
	//--------------------------------

	Add.hwnd = GetDlgItem ( hwnd, ID_ADD  );
	register_child ( &Add,

							  nAnchor::right,		0,		- ( iMargin * 3 + s_iStripButtonDimX * 2),
							  nAnchor::top,			0,		iMargin + 1,
							  nAnchor::right,		0,		-( iMargin * 3 + s_iStripButtonDimX ),
							  nAnchor::top,		    0,      iMargin + s_iStripDimY - 1 );

	ShowWindow ( pListView->hwnd, TRUE );

	pListView->init();
	pListView->populate (); //  item_list() );


}


//----------------------------------------------------------------------------------------------------------------------
//  TOGGLE VIEW
//----------------------------------------------------------------------------------------------------------------------
void cDlgView :: toggle_view ()
{
	pListView->toggle_view(); 
}


//----------------------------------------------------------------------------------------------------------------------
// DEFAULT PROC
//----------------------------------------------------------------------------------------------------------------------
INT_PTR CALLBACK cDlgView::dialog_proc (HWND hwnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	cWin * pWin = user_data_to_pWin ( hwnd );
	cDlgView * pThis = static_cast<cDlgView*>(pWin);

	switch ( uMessage )
	{
	case WM_COMMAND:

		switch ( LOWORD ( wParam ) )
		{
		case ID_TOGGLE_VIEW:
			pThis->toggle_view ();
			break;


		}

#if 0
	case WM_ERASEBKGND:
		return TRUE;
#endif

	case WM_PAINT:
		pThis->wm_paint();
		break;
#if 0
	case uWM_TOON_LIST_CHANGED:
		{
		#if 0
			// cToonList tl ( g_Config.ToonList );
			pThis->pListView->populate ( &g_Config );
		#endif
		}
		break;
#endif

	case WM_INITDIALOG:
		{
			set_user_data ( hwnd, lParam );
			cWin * pWin = lParam_to_pWin ( hwnd, lParam );
			pThis = static_cast<cDlgView*>(pWin);
			pThis->hwnd = hwnd;
			pThis->wm_init ();
		}
		break;

	case WM_NOTIFY:
		if ( pThis )
		{
			NMHDR * pN = (NMHDR*)lParam;

			if ( pN->code == NM_RCLICK )
			{
				DWORD dwHandle = pThis->pListView->hot_item();

				if ( (DWORD)-1 != dwHandle )
				{
					cFigViewItem * p = pThis->pListView->pList->get_item_clone ( dwHandle );

					if ( p )
					{

						int iCommand = p->show_context_menu ( hwnd );
						p->handle_context_menu ( iCommand );

#if 0

						switch ( iCommand )
						{
						case ID_DELETE:
							g_FigMgr.delete_fig ( dwHandle );
							PostMessage ( g_hwnd, uWM_WOW_LIST_CHANGED, 0, 0 );
							break;

						case ID_LAUNCH:
							message_box ( L"Sorry, Mojo can't launch programs yet." );
							break;

						case ID_PROPERTIES:
							{
								cDlgFigWoW d;
								d.make_dlg ( p );
							}
							break;

						default:
							break;
						}
#endif

						delete p;
					}
				}
			}
		}
		break;

	default:
		break;
	}

	return cDlg::dialog_proc ( hwnd, uMessage, wParam, lParam );
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