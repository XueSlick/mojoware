/***********************************************************************************************************************
/*
/*    cDlgMonitor.cpp / mojo_app
/*
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#pragma once

#include "stdafx.h"

//======================================================================================================================
//  CODE
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
//  GET MEMO COLOR
//----------------------------------------------------------------------------------------------------------------------
COLORREF cDlgMonitor :: get_memo_color ( cMemo * pM )
{
	switch ( pM->eSeverity )
	{
	case cMemo::info:
		return RGB ( 0,0,0 );

	case cMemo::warning:
		return RGB ( 0xBB, 0, 0xFF );

	case cMemo::error:
		return RGB ( 0xCC, 0, 0x44 );

	case cMemo::success:
		return RGB ( 0, 0, 0xCC );

	default:
		assert(0);
	}

	return 0;
}


//----------------------------------------------------------------------------------------------------------------------
//  SHOW MEMO
//----------------------------------------------------------------------------------------------------------------------
void cDlgMonitor :: show_memo ()
{
	mojo::cMemo m;

	HWND h = this->Memos.hwnd;
	assert ( h );

	while ( mojo::get_memo ( &m ) )
	{
		// HEAD
		rich_edit_set_color ( h, RGB ( 0,0,0 ) );
		rich_edit_set_weight_regular ( h );
		rich_edit_append_line  ( h, m.head()  );

		// BODY
		rich_edit_set_indent ( h, 300 );
		rich_edit_set_color ( h, get_memo_color ( &m ) );
		rich_edit_append_line ( h, m.body() );
		rich_edit_append_line ( h, L"" );

		// RESET INDENT SO CURSOR BLINKS IN RIGHT SPOT
		rich_edit_set_indent ( h, 0 );
	}
}


//----------------------------------------------------------------------------------------------------------------------
// WM PAINT
//----------------------------------------------------------------------------------------------------------------------
void cDlgMonitor::wm_paint ()
{
	draw_head  ( &InputEventsHead  );
	draw_head  ( &MemosHead 	   );
	draw_head  ( &ConnectionsHead );
}


//----------------------------------------------------------------------------------------------------------------------
// DRAW HEAD
//----------------------------------------------------------------------------------------------------------------------
void cDlgMonitor::draw_head ( cWin * pHead )
{
	cStrW sText;
	get_window_text ( &sText, pHead->hwnd );

	if ( ! sText.len() )
		return;

	const wchar_t * pTxt = sText.cstr();

	const int iLeftMargin = 9;

   #define DOUBLE_BUFFER

	HDC hdcOriginal = GetDC ( pHead->hwnd );
	RECT rect;
	GetClientRect ( pHead->hwnd, &rect );

#ifdef DOUBLE_BUFFER
	HDC		hdcMem	= CreateCompatibleDC ( hdcOriginal );
	HBITMAP hbmMem	= CreateCompatibleBitmap(hdcOriginal, rect.right, rect.bottom);
	HANDLE	hOld	= SelectObject(hdcMem, hbmMem);
	HDC hdc			= hdcMem;
#else
	HDC hdc = hdcOriginal;
#endif

	HBRUSH hBrush	= CreateSolidBrush ( RGB ( 44, 44, 110 ) ); // /* (COLORREF) g_Settings.uHeaderColor */  );
	SelectObject	( hdc, HGDIOBJ ( hBrush ) );
	FillRect		( hdc, &rect, hBrush );
	DeleteObject	( (HGDIOBJ)  hBrush );
	DWORD dwStyles	= DT_VCENTER | DT_SINGLELINE;
	rect.left = iLeftMargin;
	cDlg::draw_text ( hdc, &rect, caption, pTxt, true, dwStyles );
	rect.left = 0;

#ifdef DOUBLE_BUFFER
	BitBlt ( hdcOriginal, rect.left, rect.top, rect.right, rect.bottom, hdcMem, 0, 0, SRCCOPY);
	SelectObject ( hdcMem, hOld );
	DeleteObject ( hbmMem );
	DeleteDC     ( hdcMem );
#endif

	ReleaseDC ( pHead->hwnd, hdcOriginal );
}


//----------------------------------------------------------------------------------------------------------------------
// SET TEXT
//----------------------------------------------------------------------------------------------------------------------
void cDlgMonitor::set_text ()
{
	set_item_text ( ID_INPUT_EVENTS_HEAD, 	L"DlgMonitor.InputEventsHead" );
	set_item_text ( ID_MEMOS_HEAD, 			L"DlgMonitor.MemosHead", g_awAppTitle );
	set_item_text ( ID_CONNECTIONS_HEAD, 	L"DlgMonitor.ConnectionsHead" );

	wm_paint();
}


//----------------------------------------------------------------------------------------------------------------------
// WM INIT
//----------------------------------------------------------------------------------------------------------------------
void cDlgMonitor::wm_initdialog ()
{
	set_text ();

	const int iMargin = 9;
	int iHeadHeight = g_NonClientMetrics.iCaptionHeight;
	int iVertDiv = 235;

	//----------------------------------------
	//  YOUR LAST ACTION (INPUT EVENTS) HEADER
	//----------------------------------------

	InputEventsHead.hwnd = GetDlgItem ( hwnd, ID_INPUT_EVENTS_HEAD );

	register_child ( &InputEventsHead,

							  nAnchor::left,		0,		iMargin,
							  nAnchor::top,	    	0,		iMargin,
							  nAnchor::left,		0,		iVertDiv,
							  nAnchor::top,	   	 	0,		iMargin + iHeadHeight   );

	//----------------------------------------
	//  YOUR LAST ACTION (INPUT EVENTS) 
	//----------------------------------------

	InputEvents.hwnd = GetDlgItem ( hwnd, ID_INPUT_EVENTS );

	SendMessage ( InputEvents.hwnd, WM_SETFONT, (WPARAM) g_hMenuFont, TRUE );

	unsigned aTabs[] = { 62, 100 };
	SendMessage ( InputEvents.hwnd, EM_SETTABSTOPS, (WPARAM) sizeof(aTabs)/sizeof(unsigned), (LPARAM) aTabs );

	register_child ( &InputEvents,

							  nAnchor::left,		0,		iMargin,
							  nAnchor::top,	    	0,		iMargin + iHeadHeight,
							  nAnchor::left,		0,		iVertDiv,
							  nAnchor::top,			45,		-(iMargin/2)     );

	//----------------------------------------
	//  CONNECTIONS HEADER
	//----------------------------------------

	ConnectionsHead.hwnd = GetDlgItem ( hwnd, ID_CONNECTIONS_HEAD );

	register_child ( &ConnectionsHead,

							  nAnchor::left,		0,		iMargin,
							  nAnchor::top,			45,		iMargin/2,
							  nAnchor::left,		0,		iVertDiv,
							  nAnchor::top,			45,		iMargin/2 + iHeadHeight   );

	//----------------------------------------
	//  CONNECTIONS (LISTVIEW CONTROL)
	//----------------------------------------

	Connections.hwnd = GetDlgItem ( hwnd, ID_CONNECTIONS );
	Connections.init();

	register_child ( &Connections,
	
							  nAnchor::left,		0,		iMargin,
							  nAnchor::top,			45,		iMargin/2 + iHeadHeight,
							  nAnchor::left,		0,		iVertDiv,
							  nAnchor::bottom,		0,		-iMargin     );

	//----------------------------------------
	//  PROGRAM'S LAST ACTION (MEMOS) HEADER
	//----------------------------------------

	MemosHead.hwnd = GetDlgItem ( hwnd, ID_MEMOS_HEAD );

	register_child ( &MemosHead,

							  nAnchor::left,		0,		iVertDiv + iMargin,
							  nAnchor::top,			0,		iMargin,
							  nAnchor::right,		0,		-iMargin,
							  nAnchor::top,	    	0,		iMargin + iHeadHeight   );


	//-----------------------------------------
	// PROGRAM'S LAST ACTION (MEMOS) (RICH EDIT CONTROL)
	//-----------------------------------------

	Memos.hwnd = GetDlgItem ( hwnd, ID_MEMOS );

	register_child ( &Memos,

							  nAnchor::left,		0,		iVertDiv + iMargin,
							  nAnchor::top,			0,		iMargin + iHeadHeight,
							  nAnchor::right,		0,		-iMargin,
							  nAnchor::bottom,		0,		-iMargin  );

	SendMessage ( Memos.hwnd, WM_SETFONT, (WPARAM) g_hMenuFont, TRUE );

	RECT rect;
	SendMessage ( Memos.hwnd, EM_GETRECT, 0, (LPARAM)&rect);
	rect.top += 12;
	rect.left += 16;
	rect.right -= 12;
	rect.bottom -= 12;
	SendMessage ( Memos.hwnd, EM_SETRECT, 0, (LPARAM)&rect );
}


//----------------------------------------------------------------------------------------------------------------------
// DIALOG PROC
//----------------------------------------------------------------------------------------------------------------------
INT_PTR CALLBACK cDlgMonitor :: dialog_proc ( HWND hDlg, UINT uMessage, WPARAM wParam, LPARAM lParam )
{

	cWin * pWin = user_data_to_pWin ( hDlg );
	cDlgMonitor * pThis = static_cast<cDlgMonitor*>(pWin);

	switch ( uMessage )
	{
	case mojo::uWM_MEMO_READY:
		if ( pThis )
			pThis->show_memo ();
		break;
		 
	case WM_PAINT:
		pThis->wm_paint ();
		break;

	case WM_INITDIALOG:
		{
			set_user_data ( hDlg, lParam );
			cWin * pWin = lParam_to_pWin ( hDlg, lParam );
			pThis = static_cast<cDlgMonitor*>(pWin);
			pThis->hwnd = hDlg;

			HICON hIcon = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE ( IDI_ICON ), IMAGE_ICON, 16, 16, 0);
			SendMessage ( pThis->hwnd, WM_SETICON, ICON_BIG, (LPARAM) hIcon ) ;

			pThis->wm_initdialog ();
			return (INT_PTR)TRUE;
		}

	case WM_CTLCOLORBTN:
	case WM_CTLCOLOREDIT:
	case WM_CTLCOLORDLG:
	case WM_CTLCOLORSTATIC:

		if ( lParam == (long) GetDlgItem ( hDlg, ID_INPUT_EVENTS ) ||
			 lParam == (long) GetDlgItem ( hDlg, ID_MEMOS ) )
		{
			HDC hdc = (HDC)wParam;
			SetTextColor(hdc, RGB(0,0,0));
			SetBkMode(hdc, OPAQUE);
			SetBkColor( hdc, RGB(0xFF,0xFF,0xFF) );
			return (LONG) GetStockObject ( WHITE_BRUSH );
		}
		break;

	case WM_NOTIFY:
		NMHDR * pN = (NMHDR*)lParam;

		if ( pN->idFrom == ID_CONNECTIONS ) // pThis->MonitorConnections.hwnd )
		{
			switch ( pN->code )
			{
			case NM_CUSTOMDRAW:
				{
					LPNMLVCUSTOMDRAW  lplvcd = (LPNMLVCUSTOMDRAW)lParam;

					switch ( lplvcd->nmcd.dwDrawStage ) 
					{
						case CDDS_PREPAINT :
							SetWindowLongPtr ( hDlg, DWL_MSGRESULT, (LONG_PTR) CDRF_NOTIFYITEMDRAW );
							return TRUE;

						case CDDS_ITEMPREPAINT:
							{
								DWORD dwSerialNumber = lplvcd->nmcd.lItemlParam;

								mojo::nConnectionStatus::eConnectionStatus eStat = mojo::get_connection_status ( dwSerialNumber );

								COLORREF clrFG=0, clrBG=0;

								switch (eStat)
								{
								case nConnectionStatus::none: 
									clrFG = RGB ( 0, 0, 0 );          // BLACK ON GRAY
									clrBG = RGB ( 235, 235, 235 );
									break;

								case nConnectionStatus::server:
								case nConnectionStatus::client:
									clrFG = RGB ( 0xBB, 0, 0xCC );    // PURPLE ON PURPLE
									clrBG = RGB ( 245, 240, 255 );
									break;

								case nConnectionStatus::both:
									clrFG = RGB ( 0, 0, 0xFF );       // BLUE ON WHITE
									clrBG = RGB ( 255, 255, 255 );
									break;

								default:
									assert(0);
								}

								lplvcd->clrText   = clrFG;
								lplvcd->clrTextBk = clrBG;

								SetWindowLongPtr ( hDlg, DWL_MSGRESULT, (LONG_PTR) CDRF_NEWFONT );

								return TRUE;
							}
					}
				}
				break;

			case NM_RCLICK:
				{
#if 0
					int iIndex = ListView_GetHotItem ( pThis->MonitorConnections.hwnd );

					if ( pThis->pConnectionsMenuMach = pThis->MonitorConnections.hot_mach() )
						pThis->do_connections_menu ();
#endif
				}
				break;
			}
		}

		break;
	}

	return cDlg::dialog_proc ( hDlg, uMessage, wParam, lParam );
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