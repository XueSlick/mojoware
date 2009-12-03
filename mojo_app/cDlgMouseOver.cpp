/*************************************************************************************************
/*
/*   cDlgMouseOver.cpp
/*   
/*   started October 16, 2008
/*   
/*************************************************************************************************/

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
							  nAnchor::left,		0,   20,
							  nAnchor::bottom,	0,  -79,
							  nAnchor::left,		0,  200,
							  nAnchor::bottom,	0,  -57 );

	CheckDlgButton ( hwnd, ID_SHOW_IP_ON_MOUSEOVER_SETTINGS,
		g_Settings.bShowIpOnMouseoverSettings ? BST_CHECKED : BST_UNCHECKED );

	//-------------

	ScreenSaver.hwnd = GetDlgItem ( hwnd, ID_MOUSEOVER_DEACTIVATES_SCREEN_SAVERS );

	register_child ( &ScreenSaver,
							  nAnchor::left,		0,   20,
							  nAnchor::bottom,	0,  -58,
							  nAnchor::left,		0,  360,
							  nAnchor::bottom,	0,  -36 );

	CheckDlgButton ( hwnd, ID_MOUSEOVER_DEACTIVATES_SCREEN_SAVERS,
		g_Settings.bMouseoverDeactivatesScreenSavers ? BST_CHECKED : BST_UNCHECKED );

	//-------------

	HideRemoteCursor.hwnd = GetDlgItem ( hwnd, ID_HIDE_REMOTE_CURSOR );

	register_child ( &HideRemoteCursor,
							  nAnchor::left,		0,   20,
							  nAnchor::bottom,	0,  -37,
							  nAnchor::left,		0,  360,
							  nAnchor::bottom,	0,  -15 );

	CheckDlgButton ( hwnd, ID_HIDE_REMOTE_CURSOR,
		g_Settings.bHideRemoteCursor ? BST_CHECKED : BST_UNCHECKED );

	//--------------------------------
	// REGISTER PUSH BUTTONS
	//--------------------------------
	OkayButton.hwnd = GetDlgItem ( hwnd, IDOK );

	register_child ( &OkayButton,
							  nAnchor::right,	  0, -170,
							  nAnchor::bottom,	  0,  -40,
							  nAnchor::right,    0,  -100,
							  nAnchor::bottom,   0,  -18 );

	CancelButton.hwnd = GetDlgItem ( hwnd, IDCANCEL );

	register_child ( &CancelButton,
							  nAnchor::right,	  0, -90,
							  nAnchor::bottom,	  0,  -40,
							  nAnchor::right,    0,  -20,
							  nAnchor::bottom,   0,  -18 );


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

	case WM_COMMAND:
		{
			switch ( LOWORD(wParam) )
			{
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


////////////////////////////////////////  DEAD CODE

#if 0
//----------------------------------------------------------------------------------------------
// MAKE IMAGE LIST SUB
//----------------------------------------------------------------------------------------------
HBITMAP make_image_list_sub (  HWND hwnd, int x, int y, wchar_t * pText, 
							   wchar_t * pImageFilename, bool bBlackText )
{
	cStrW sPath;
	j::get_module_path ( & sPath );
	sPath += pImageFilename;
	Bitmap bmOriginalSize ( sPath.cstr() );
	Graphics gOld ( &bmOriginalSize );

	FontFamily  fontFamily ( L"Verdana" );
	Font        font ( &fontFamily, 15, FontStyleRegular, UnitPixel);
	// PointF      pointF ( 30.0f, 10.0f );

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


//----------------------------------------------------------------------------------------------
// MAKE IMAGE LIST
//----------------------------------------------------------------------------------------------
HIMAGELIST cDlgMouseOver::make_image_list ( HWND hwnd, int x, int y, wchar_t * pText, 
										   wchar_t * pImageFilenameGold, /* wchar_t * pImageFilenameY, */ wchar_t * pImageFilenameGray )
{
	HIMAGELIST hIL  = ImageList_Create( x, y,  ILC_COLORDDB | ILC_COLOR32| ILC_MASK , 2, 0);
	HBITMAP hBM;
	hBM = make_image_list_sub ( hwnd, x, y, pText, pImageFilenameGold, true );
	ImageList_Add ( hIL, hBM, NULL );
	hBM = make_image_list_sub ( hwnd, x, y, pText, pImageFilenameGray, false );
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

	cPtI ScreenResScale ( pMach->VirtRect.dx, pMach->VirtRect.dy ); // this->pMach->ScreenRes;

	if ( ScreenResScale.x < 640 || ScreenResScale.y < 480 )
			ScreenResScale = cPtI ( 1024, 768 );

	ScreenResScale /= s_iSCALE_FACTOR;

	HIMAGELIST h = pDlg->make_image_list ( hwnd, ScreenResScale.x, ScreenResScale.y, this->pMach->sName.cstr(),
			L"mouseover_gold.jpg", /* L"art\\mouseover_blue.jpg", */ L"mouseover_gray.jpg");

	this->set ( ScreenResScale.x, ScreenResScale.y, h );
}
#endif