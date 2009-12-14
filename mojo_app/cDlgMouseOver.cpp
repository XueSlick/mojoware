/***********************************************************************************************************************
/*
/*   cDlgMouseOver.cpp
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

static const int s_iSCALE_FACTOR = 8;


//===============================================================================================
// PROTOTYPES
//===============================================================================================

HIMAGELIST make_image_list ( HWND hwnd, int DimX, int DimY, wchar_t * pImageFilename, wchar_t * pText );


//===============================================================================================
// CODE
//===============================================================================================


//----------------------------------------------------------------------------------------------
// SET MOUSEOVER DEACTIVATES SCREEN SAVERS
//----------------------------------------------------------------------------------------------
void set_mouseover_deactivates_screen_savers()
{
	BOOL iResult = SystemParametersInfo (	SPI_SETBLOCKSENDINPUTRESETS,
											g_Settings.bMouseoverDeactivatesScreenSavers ? FALSE : TRUE,
											NULL,
											0 );
	if ( 0 == iResult )
		LOG_SYSTEM_ERROR_T ( L"SystemParametersInfo ( SPI_SETBLOCKSENDINPUTRESETS )" );
}


//----------------------------------------------------------------------------------------------
// ON RIGHT BUTTON DOWN
//----------------------------------------------------------------------------------------------
void cDlgMouseOver::on_right_button_down ( WORD x, WORD y )
{
	UNREFERENCED_PARAMETER(x);
	UNREFERENCED_PARAMETER(y);
}


//----------------------------------------------------------------------------------------------
// DRAW TEXT
// GDI version
//----------------------------------------------------------------------------------------------
void cDlgMouseOver :: draw_text ( HWND hwnd, HDC hdc, const cStrW * pStr )
{
	UNREFERENCED_PARAMETER(hwnd);

	SetBkMode ( hdc, TRANSPARENT );

	LOGFONT lf;
	ZeroMemory ( &lf, sizeof(lf) );

	wcscpy_s ( lf.lfFaceName, sizeof(lf.lfFaceName)/sizeof(wchar_t), L"Verdana" );
	lf.lfHeight = 17;
	lf.lfWeight = 400;

	HFONT hFont = CreateFontIndirect ( &lf );
	SelectObject ( hdc, hFont );

	RECT r;
	r.left = 15;
	r.top = 15;
	r.right = 500;
	r.bottom = 500;

	DrawText ( hdc, pStr->cstr(), pStr->len(), &r, DT_LEFT | DT_WORDBREAK);
}


//----------------------------------------------------------------------------------------------
// SET CONTIGUITY SUB
// recursive portion
//----------------------------------------------------------------------------------------------
void cDlgMouseOver :: set_contiguity_sub ( cScrobImMouseOver * pCtr )
{
	for ( cScrobIm * sb = ScrobList.pHead; sb; sb = sb->pNext )
	{
		cScrobImMouseOver * s = static_cast<cScrobImMouseOver*>(sb);

		if ( pCtr == s )
			continue;

		if ( are_contiguous ( pCtr, s ) )
		{
			if ( ! s->bIsContiguous && ! s->bIsOverlapping )
			{
				s->bIsContiguous = true;
				set_contiguity_sub ( s );
			}
		}
	}
}


//----------------------------------------------------------------------------------------------
// SET CONTIGUITY 
// entry to recursive portion
//----------------------------------------------------------------------------------------------
void cDlgMouseOver :: set_contiguity ()
{
	cScrobImMouseOver * pCtr = 0;

	update_display_rects();

	// erase all "connected" flags except for this PC
	for ( cScrobIm * sb = ScrobList.pHead; sb; sb = sb->pNext )
	{
		cScrobImMouseOver * s = static_cast<cScrobImMouseOver*>(sb);

		if ( s->pMach->bThisPC )
		{
			pCtr = s;
			s->bIsContiguous = true;
			s->bIsOverlapping = false;
		}
		
		else
		{
			s->bIsOverlapping = is_overlapping ( s );
			s->bIsContiguous = false;
		}
	}

	set_contiguity_sub ( pCtr );
}


//----------------------------------------------------------------------------------------------
// DRAW TEXT
//----------------------------------------------------------------------------------------------
void draw_text ( HWND hwnd, HDC hdc, Gdiplus::Color color, const wchar_t * pTxt, Gdiplus::Font * pFontArg  )
{
	UNREFERENCED_PARAMETER(hwnd);

 // gdi plus version

	Gdiplus::Font * pFont;
	Gdiplus::Graphics g ( hdc );
	HFONT hFont =  g_hMenuFont; // ::get_small_caption_hfont();
	Gdiplus::Font font ( hdc, hFont );;

	if ( pFontArg )
		pFont = pFontArg;

	else
		pFont = &font;

	Gdiplus::RectF rectG;

	rectG.X			=  15.0;
	rectG.Y			=  15.0;
	rectG.Height	= 200.0;
	rectG.Width		= 590.0;

	Gdiplus::SolidBrush  solidBrush ( color );

	Gdiplus::StringFormat format;
	format.SetAlignment(Gdiplus::StringAlignmentNear);
	format.SetLineAlignment(Gdiplus::StringAlignmentNear);

	g.DrawString ( pTxt, -1, pFont, rectG, &format, &solidBrush );
}


//----------------------------------------------------------------------------------------------
// GET IMAGE INDEX
//----------------------------------------------------------------------------------------------
int cScrobImMouseOver::calc_image_index ()
{
	if ( bIsContiguous )
		return 0;
	else
		return 1;
}


//----------------------------------------------------------------------------------------------
// SAVE DRAW POSITIONS
//----------------------------------------------------------------------------------------------
void cDlgMouseOver::save_draw_positions ()
{

	for ( cScrobIm * pS = ScrobList.pHead; pS; pS = pS->pNext )
	{
		cScrobImMouseOver * pO = static_cast<cScrobImMouseOver*>(pS);

		pO->pMach->DrawPos = pO->Pos;

		if ( pO->bIsContiguous && ! pO->bIsOverlapping )
			pO->pMach->bValidDrawPos = true;

		else
			pO->pMach->bValidDrawPos = false;
	}
}


//----------------------------------------------------------------------------------------------
// ON INIT
//----------------------------------------------------------------------------------------------
INT_PTR cDlgMouseOver::on_init ( HWND hwnd )
{

	const int iMarginY = 12;
	const int iGutterX = 6;
	const int iGutterY = 0;
	const int iMarginX = 12;
	const int iCtrlDimY = 23;
	const int iButtonDimX = 90;

	HICON hIcon = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE ( IDI_ICON ), IMAGE_ICON, 16, 16, 0);
	SendMessage ( hwnd, WM_SETICON, ICON_BIG, (LPARAM) hIcon );

	cStrW sTitle = g_awAppTitle;
	sTitle += L": Mouseover settings";
	SetWindowText ( hwnd, sTitle.cstr());

	// cMachlist Machlist ( g_Machlist );

	int i = 1;

	mojo::get_machlist ( &this->Machlist );

	for ( cMach * pM = this->Machlist.last(); pM; pM = pM->pPrev )
	{
		// if ( pM->bExclude )
		//	continue;

		if ( pM->DrawPos.x < 0 || pM->DrawPos.y < 0 )
		{
			pM->DrawPos.x = i * 40;
			pM->DrawPos.y = 100 + ( i * 20 );
			i++;
		}

		cScrobImMouseOver * pS = new cScrobImMouseOver ();

		pS->Pos = pM->DrawPos;
		pS->pMach = pM;
	
		pS->set_size_and_make_image_list ( this, hwnd );

		add_scrob ( pS );
	}

	//--------------------------------
	// PUT LOCAL PC ON TOP OF Z-ORDER
	//--------------------------------
#if 0
	if ( 1 < ScrobList.qty() )
	{
		for ( cScrobIm * pSI = ScrobList.pHead; pSI; pSI = pSI->pNext )
		{
			cScrobImMouseOver * p = static_cast<cScrobImMouseOver*>(pSI);

			cMach * pMach = p->pMach;

			if ( pMach->bThisPC )
			{
				ScrobList.remove ( p );
				ScrobList.append ( p );
				break;
			}
		}
	}
#endif




	//--------------------------------
	// REGISTER CHECKBOXES
	//--------------------------------

	ShowIP.hwnd = GetDlgItem ( hwnd, ID_SHOW_IP_ON_MOUSEOVER_SETTINGS );

	register_child ( &ShowIP,
							  nAnchor::left,		0,   iMarginX,
							  nAnchor::bottom,	    0,  - ( iMarginY + 5 * iCtrlDimY + 4 * iGutterY ),
							  nAnchor::left,		0,  iMarginX + 300,
							  nAnchor::bottom,	    0,  - ( iMarginY + 4 * iCtrlDimY + 4 * iGutterY ) );

	CheckDlgButton ( hwnd, ID_SHOW_IP_ON_MOUSEOVER_SETTINGS,
		g_Settings.bShowIpOnMouseoverSettings ? BST_CHECKED : BST_UNCHECKED );

	//-------------

	ScreenSaver.hwnd = GetDlgItem ( hwnd, ID_MOUSEOVER_DEACTIVATES_SCREEN_SAVERS );

	register_child ( &ScreenSaver,
							  nAnchor::left,		0,   iMarginX,
							  nAnchor::bottom,	    0,  - ( iMarginY + 4 * iCtrlDimY + 3 * iGutterY ),
							  nAnchor::left,		0,  iMarginX + 300,
							  nAnchor::bottom,	    0,  - ( iMarginY + 3 * iCtrlDimY + 3 * iGutterY ) );

	CheckDlgButton ( hwnd, ID_MOUSEOVER_DEACTIVATES_SCREEN_SAVERS,
		g_Settings.bMouseoverDeactivatesScreenSavers ? BST_CHECKED : BST_UNCHECKED );

	//-------------

	HideRemoteCursor.hwnd = GetDlgItem ( hwnd, ID_HIDE_REMOTE_CURSOR );

	register_child ( &HideRemoteCursor,
							  nAnchor::left,		0,   iMarginX,
							  nAnchor::bottom,	    0,  - ( iMarginY + 3 * iCtrlDimY + 2 * iGutterY ),
							  nAnchor::left,		0,  iMarginX + 300,
							  nAnchor::bottom,	    0,  - ( iMarginY + 2 * iCtrlDimY + 2 * iGutterY ) );

	CheckDlgButton ( hwnd, ID_HIDE_REMOTE_CURSOR,
		g_Settings.bHideRemoteCursor ? BST_CHECKED : BST_UNCHECKED );

	//--------------------------------
	// REGISTER HOTKEY STUFF
	//--------------------------------

	SetHotkey.hwnd = GetDlgItem ( hwnd, ID_SET_HOTKEY );
	register_child ( &SetHotkey,
							  nAnchor::left,		0,   iMarginX + 300 + iGutterX,
							  nAnchor::bottom,	    0,   - ( iMarginY + iCtrlDimY ),
							  
							  nAnchor::left,		0,   iMarginX + 300 + iGutterX + iButtonDimX,
							  nAnchor::bottom,	    0,   - iMarginY );

	HotkeyLabel.hwnd = GetDlgItem ( hwnd, ID_HOTKEY_LABEL );
	register_child ( &HotkeyLabel,
							  nAnchor::left,		0,   iMarginX,
							  nAnchor::bottom,	    0,   - ( iMarginY + 2 * iCtrlDimY + iGutterY),
							  
							  nAnchor::left,		0,   iMarginX + 300,
							  nAnchor::bottom,	    0,   - ( iMarginY + iGutterY + iCtrlDimY ) );

	Hotkey.hwnd = GetDlgItem ( hwnd, ID_HOTKEY );
	register_child ( &Hotkey,
							  nAnchor::left,		0,   iMarginX,
							  nAnchor::bottom,	    0,   - ( iMarginY + iCtrlDimY ),
							  nAnchor::left,		0,   iMarginX + 300,
							  nAnchor::bottom,	    0,   - iMarginY );


	//--------------------------------
	// REGISTER PUSH BUTTONS
	//--------------------------------
	OkayButton.hwnd = GetDlgItem ( hwnd, IDOK );

	register_child ( &OkayButton,
							  nAnchor::right,	    0,  - ( iMarginX + iButtonDimX  * 2 + iGutterX ),
							  nAnchor::bottom,	    0,  - ( iMarginY + iCtrlDimY ),
							  nAnchor::right,       0,  - ( iMarginX + iButtonDimX  * 1 + iGutterX ),
							  nAnchor::bottom,      0,  - iMarginY );

	CancelButton.hwnd = GetDlgItem ( hwnd, IDCANCEL );

	register_child ( &CancelButton,
							  nAnchor::right,	    0,  - ( iMarginX + iButtonDimX  ),
							  nAnchor::bottom,	    0,  - ( iMarginY + iCtrlDimY ),
							  nAnchor::right,       0,  - ( iMarginX ),
							  nAnchor::bottom,      0,  - iMarginY );


	RECT rect;
	GetClientRect ( hwnd, &rect );
	wm_size ( rect.right - rect.left, rect.bottom - rect.top ); // changed from on_size() when imported from HKN2
	set_size ( 800, 600 );
	move_to_center();

	return INT_PTR ( TRUE );
}


//----------------------------------------------------------------------------------------------
// DEFAULT PROC
//----------------------------------------------------------------------------------------------
INT_PTR CALLBACK cDlgMouseOver::dialog_proc (HWND hwnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	cWin * pWin = user_data_to_pWin ( hwnd );
	cDlgMouseOver * pThis = static_cast<cDlgMouseOver*>(pWin);

	switch ( uMessage )
	{
	case WM_LBUTTONDOWN:
		pThis->on_mouse_left_down ( LOWORD(lParam), HIWORD(lParam) );
		break;

	case WM_RBUTTONDOWN:
		pThis->on_right_button_down ( LOWORD(lParam), HIWORD(lParam) );
		break;

	case WM_MOUSEMOVE:
		pThis->on_mouse_move ( (INT16) LOWORD(lParam), (INT16) HIWORD(lParam) );
		break;

	case WM_LBUTTONUP:
		pThis->on_mouse_left_up ( LOWORD(lParam), HIWORD(lParam) );
		break;

	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);

			cStrW w =	L"Drag the monitors so they touch.  They light up when\n" 
						L"this PC's mouse can reach them. Click OK to save.";

#if 0       // GDI version
			pThis->draw_text ( hwnd, hdc, &w );  
#else

            // GDI+ version 
			{
				FontFamily  fontFamily ( L"Verdana" );
				Font        font ( &fontFamily, 14, FontStyleBold, UnitPixel);

				:: draw_text ( hwnd, hdc, Color ( 155, 0, 0, 111 ), w.cstr(), &font );
			}
#endif

			pThis->set_contiguity ();
			pThis->draw_scrobs ( hdc );
			EndPaint(hwnd, &ps);
			return (INT_PTR) TRUE;
		}
		break;

	case WM_CTLCOLORDLG :
	case WM_CTLCOLORSTATIC:
			return (INT_PTR) GetStockObject ( WHITE_BRUSH ); // CreateSolidBrush ( COLORREF ( 0xfffff) );
			break;

	case WM_INITDIALOG:
		{
			set_user_data ( hwnd, lParam );
			cWin * pWin = lParam_to_pWin ( hwnd, lParam );
			pThis = static_cast<cDlgMouseOver*>(pWin);

			pThis->cWin::hwnd = hwnd;
			pThis->cScrobMgrIm::hwnd = hwnd;
			pThis->sLabel = L"Mouseover";
			return pThis->on_init ( hwnd );
		}
		break;

	case WM_GETMINMAXINFO:
		{
			MINMAXINFO * p    = (MINMAXINFO*) lParam;
			POINT pt          = { 700, 500 };
			p->ptMinTrackSize = pt;
		}
		break;

	case WM_COMMAND:
		{
			switch ( LOWORD(wParam) )
			{
			case ID_SET_HOTKEY:
				{
					cTrigger Trigger;
					cDlgSetTrigger d;
					if ( 0 < d.make_dlg ( &Trigger ) )
					{
						cStrW s;
						Trigger.print ( &s );
						SetWindowText ( pThis->Hotkey.hwnd, s.cstr() );
					}
				}
				break;

			case ID_SHOW_IP_ON_MOUSEOVER_SETTINGS:
				if ( BN_CLICKED == HIWORD(wParam) )
				{
					g_Settings.bShowIpOnMouseoverSettings = IsDlgButtonChecked(hwnd, ID_SHOW_IP_ON_MOUSEOVER_SETTINGS) ? true : false;
					pThis->DispRectList.rem_del_all();

					for ( cScrobIm * pSI = pThis->ScrobList.pHead; pSI; pSI = pSI->pNext )
					{
						cScrobImMouseOver * p = static_cast<cScrobImMouseOver*>(pSI);
						p->set_size_and_make_image_list ( pThis, hwnd );
					}

					InvalidateRect ( hwnd, NULL, TRUE );
					UpdateWindow ( hwnd );
				}
				break;

			case IDOK:
				g_Settings.bShowIpOnMouseoverSettings        = IsDlgButtonChecked ( hwnd, ID_SHOW_IP_ON_MOUSEOVER_SETTINGS) ? true : false;
				g_Settings.bHideRemoteCursor                 = IsDlgButtonChecked ( hwnd, ID_HIDE_REMOTE_CURSOR ) ? true : false;
				g_Settings.bMouseoverDeactivatesScreenSavers = IsDlgButtonChecked ( hwnd, ID_MOUSEOVER_DEACTIVATES_SCREEN_SAVERS ) ? true : false;
				set_mouseover_deactivates_screen_savers();
				pThis->save_draw_positions ();

				mojo::set_mouseover_layout ( &pThis->Machlist );
				// TEMP TO DO
				// g_Machlist.save_draw_positions_to_file ();
				// SendMessage	( g_hwnd, uWM_SET_MOUSEOVER_DISPLAY_LIST, 0, 0 ); //g_Mouseover.set_mega_display_list ( &g_Machlist );
				pThis->on_close_window();
				break;

			case IDCANCEL:
				pThis->on_close_window();
				break;
			}
		}
		break;

	default:
		break;
	}

	return cDlgModal::dialog_proc ( hwnd, uMessage, wParam, lParam );
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
