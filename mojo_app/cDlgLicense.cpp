/***********************************************************************************************************************
/*
/*    cDlgLicense.cpp / mojo_app
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"
#include <windowsx.h>
#include <richedit.h>
#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h> 

using namespace Gdiplus;

// #define GRADIENT

//======================================================================================================================
// DATA
//======================================================================================================================

static const wchar_t s_awText0 [] =

	L"Welcome to Mojo!";

static const wchar_t s_awText1 [] =

	L"In order to use this software, you must accept the terms of this agreement. "
	L"If you do not accept, click Decline.";

static const wchar_t s_awText2 [] =

	L"\n"

	L"This software is provided by the copyright holder and contributors \"as is\" " 
	L"and any express or implied warranties, including, but not limited to, the " 
	L"implied warranties of merchantability and fitness for a particular purpose " 
	L"are disclaimed. In no event shall the copyright owner or contributors be "
	L"liable for any direct, indirect, incidental, special, exemplary, or "
	L"consequential damages (including, but not limited to, procurement of "
	L"substitute goods or services; loss of use, data, or profits; or business "
	L"interruption) however caused and on any theory of liability, whether in "
	L"contract, strict liability, or tort (including negligence or otherwise) "
	L"arising in any way out of the use of this software, even if advised of the "
	L"possibility of such damage.\n\n"

// static const wchar_t s_awText3 [] =

	L"This software is distributed under the terms of the GNU General Public License "
	L"(GPL) version 3.  To obtain the source code, go to http://mojoware.org.  To "
	L"read the license, go to http://gnu.org.\n";


//======================================================================================================================
//  PROTOTYPES
//======================================================================================================================

void set_rich_edit_tabs_and_margins ( HWND hwnd );
void rich_edit_set_alignment_left ( HWND hwnd );
void rich_edit_set_alignment_center ( HWND hwnd );


//======================================================================================================================
//  CODE
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
// WM PAINT
//----------------------------------------------------------------------------------------------------------------------
void cDlgLicense::wm_paint ()
{

#ifdef GRADIENT
	//------------------------------------
	//  GRADIENT
	//------------------------------------
	
	HDC hdc = GetDC ( hwnd );
	RECT rCli;
	GetClientRect ( hwnd, &rCli );
	RectF r;
	r.X = (Gdiplus::REAL) rCli.left;
	r.Y = (Gdiplus::REAL) rCli.top;
	r.Width = (Gdiplus::REAL) rCli.right;
	r.Height = (Gdiplus::REAL) rCli.bottom;

	//------------------------------------
	//  GRADIENT BRUSH
	//------------------------------------

	LinearGradientBrush b ( r,  Color ( 240, 235, 250 ), Color ( 245, 235, 245 ), LinearGradientModeVertical );
	// REAL aFactors [] =   { 0.0f, 0.05f, /* 0.10f, */ 1.0f };
	// REAL aPositions [] = { 0.0f, 0.35f, /* 0.50f, */ 1.0f };
	// b.SetBlend ( aFactors, aPositions, sizeof(aFactors)/sizeof(REAL) );

	Graphics g ( hdc );
	g.FillRectangle ( &b, r );

	//------------------------------------
	//  CLEAN UP GRADIENT
	//------------------------------------

	ReleaseDC ( hwnd, hdc );

#endif

	//------------------------------------
	//  DRAW TEXT
	//------------------------------------

	cDlg::draw_text ( GetDlgItem ( hwnd, ID_TEXT_0 ), NULL, cDlg::subhead, s_awText0, true, DT_EXPANDTABS | DT_CENTER | DT_WORDBREAK );
	cDlg::draw_text ( GetDlgItem ( hwnd, ID_TEXT_1 ), NULL, cDlg::text, s_awText1, true );
}


//----------------------------------------------------------------------------------------------------------------------
// SET SIZES
//----------------------------------------------------------------------------------------------------------------------
void cDlgLicense::set_sizes ()
{
	//----------------------------------------
	//  SET CONSTANTS
	//----------------------------------------

	const int iMargin = 12;
	const int iButtonDimY = ::g_NonClientMetrics.iCaptionHeight + 6;
	const int iButtonDimX = 95;

	int iText0_DimY = cDlg::draw_text ( GetDlgItem ( hwnd, ID_TEXT_0 ), NULL, cDlg::subhead, s_awText0, false );
	int iText1_DimY = cDlg::draw_text ( GetDlgItem ( hwnd, ID_TEXT_1 ), NULL, cDlg::text, s_awText1, false );

	//----------------------------------------
	//  TEXT
	//----------------------------------------

	const int iMarginMult = 2;

	register_child ( &Text0,
								nAnchor::left,		0,		iMarginMult * iMargin,
								nAnchor::top,	    0,		2 * iMargin,
								nAnchor::right,		0,	    - iMarginMult * iMargin,
								nAnchor::top,		0,		4 * iMargin + iText0_DimY );

	register_child ( &Text1,
								nAnchor::left,		0,		iMarginMult * iMargin,
								nAnchor::top,	    0,		4 * iMargin + iText0_DimY ,
								nAnchor::right,		0,	    - iMarginMult * iMargin,
								nAnchor::top,		0,		6 * iMargin + iText0_DimY + iText1_DimY );


	register_child ( &Text2,
								nAnchor::left,		0,		2 * iMargin, 
								nAnchor::top,	    0,		6 * iMargin + iText0_DimY + iText1_DimY,
								nAnchor::right,		0,	    - 2 * iMargin,
								nAnchor::bottom,	0,		- ( 2 * iMargin + iButtonDimY ) );

	//----------------------------------------
	//  AGREE BUTTON
	//----------------------------------------

	register_child ( &Accept,
								nAnchor::right,		0,		- ( 2 * iMargin + 2 * iButtonDimX ),
								nAnchor::bottom,	0,		- ( iMargin + iButtonDimY ),
								nAnchor::right,		0,		- ( 2 * iMargin + iButtonDimX ),
								nAnchor::bottom,	0,		- iMargin );

	//----------------------------------------
	//  DO NOT AGREE BUTTON
	//----------------------------------------


	register_child ( &Decline,
								nAnchor::right,		0,		- ( iMargin + iButtonDimX ),
								nAnchor::bottom,	0,		- ( iMargin + iButtonDimY ),
								nAnchor::right,		0,		- ( iMargin ),
								nAnchor::bottom,	0,		- iMargin );
}


//----------------------------------------------------------------------------------------------------------------------
// ON INIT
//----------------------------------------------------------------------------------------------------------------------
void cDlgLicense::wm_init ( HWND hwnd )
{
	//----------------------------------------
	//  SET WINDOW TITLE
	//----------------------------------------

	cStrW t = g_awAppTitle;
	t += L" license agreement";
	SetWindowText ( hwnd, t.cstr() );

	//----------------------------------------
	//  TEXT (STATIC)
	//----------------------------------------

 	Text0.hwnd = GetDlgItem ( hwnd, ID_TEXT_0 );
 	Text1.hwnd = GetDlgItem ( hwnd, ID_TEXT_1 );
 	Text3.hwnd = GetDlgItem ( hwnd, ID_TEXT_3 );
	//----------------------------------------
	//  TEXT (RICH EDIT)
	//----------------------------------------

	Text2.hwnd = GetDlgItem ( hwnd, ID_TEXT_2 );
	int iSize2 = wcslen ( s_awText2 );
	UNREFERENCED_PARAMETER(iSize2);

	set_rich_edit_tabs_and_margins ( Text2.hwnd );
	rich_edit_set_alignment_left ( Text2.hwnd );
	int iMask = ENM_SCROLLEVENTS | ENM_MOUSEEVENTS | ENM_SCROLL | ENM_REQUESTRESIZE ;
	LRESULT r = SendMessage ( Text2.hwnd, EM_SETEVENTMASK, 0, iMask );
	r = SendMessage ( Text2.hwnd, EM_SETEVENTMASK, 0, iMask );
	r = SendMessage ( Text2.hwnd, EM_GETEVENTMASK, 0, 0 );

	//----------------------------------------
	//  LOAD INTRO TEST
	//----------------------------------------

	Edit_ReplaceSel ( Text2.hwnd, s_awText2 );

	SendMessage ( Text2.hwnd, EN_REQUESTRESIZE , 0 , 0 );

	//----------------------------------------
	//  ACCEPT BUTTON
	//----------------------------------------

	Accept.hwnd = GetDlgItem ( hwnd, ID_ACCEPT );

	//----------------------------------------
	//  DECLINE BUTTON
	//----------------------------------------

	Decline.hwnd = GetDlgItem ( hwnd, ID_DECLINE );

	set_sizes ();

	reposition_children ();

	if ( rich_edit_last_char_is_visible (Text2.hwnd ) )
			EnableWindow ( Accept.hwnd, TRUE );

	set_dialog_focus ( hwnd, ID_DECLINE );
}


//----------------------------------------------------------------------------------------------------------------------
// DIALOG PROC
//----------------------------------------------------------------------------------------------------------------------
INT_PTR CALLBACK cDlgLicense::dialog_proc (HWND hwnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	cWin * pWin = user_data_to_pWin ( hwnd );
	cDlgLicense * pThis = static_cast<cDlgLicense*>(pWin);

	switch ( uMessage )
	{
#ifdef GRADIENT
	case WM_ERASEBKGND:
		return TRUE;
#endif

	case WM_PAINT:
		if ( pThis )
			pThis->wm_paint();
		break;

	case WM_INITDIALOG:
		{
			set_user_data ( hwnd, lParam );
			cWin * pWin = lParam_to_pWin ( hwnd, lParam );
			pThis = static_cast<cDlgLicense*>(pWin);
			pThis->hwnd = hwnd;
			pThis->wm_init ( hwnd );
		}
		break;

	case WM_NOTIFY:
		{
			int iID = (int)wParam;

			if ( iID == ID_TEXT_2 )
			{
			static bool bNow = false;

			if ( bNow && pThis )
				if ( pThis->thumb_is_at_bottom() )
				{
					EnableWindow ( pThis->Accept.hwnd, TRUE );
				}

#if 0			// THE FOLLOWING CODE WAS NEEDED WHEN THE
				// PROGRAM USED A FREE-STANDING SCROLL BAR
				// INSTEAD OF A BUILT-IN PSEUDO CONTROL IN
				// THE RICH EDIT CONTROL

				NMHDR * p = (LPNMHDR) lParam;

				//---------------------------------------
				// SET SCROLL BAR
				//---------------------------------------

				if ( p->code == EN_REQUESTRESIZE )
				{
					bNow = true;

					REQRESIZE * rr = (REQRESIZE*)lParam;
					RECT r;
					GetClientRect ( GetDlgItem ( hwnd, ID_TEXT ), &r );

					pThis->iEditCtrlDimY = ( rr->rc.bottom - ( r.bottom - r.top ) ) + 1;

					const int iSCALE_FACTOR = 8;
					pThis->iScrollRange = pThis->iEditCtrlDimY / iSCALE_FACTOR; 

 					SCROLLINFO si;
					memset ( &si, 0, sizeof(si) );
					si.cbSize = sizeof ( si ) ;
					si.nPage = 1 + ( r.bottom - r.top ) / iSCALE_FACTOR ;
					si.nMin = 0;
					si.nMax = pThis->iScrollRange;
					si.fMask = SIF_RANGE | SIF_PAGE;
					SetScrollInfo ( GetDlgItem ( hwnd, IDC_SCROLLBAR ), SB_CTL, &si, TRUE );
				}
#endif
			}
		}
		break;

	case WM_COMMAND:
		{
			switch ( LOWORD(wParam) )
			{
			case ID_ACCEPT:
				LOG ( L"License accepted." );
				g_Settings.bLicenseAccepted = true;
				g_Settings.save_to_file ();
				EndDialog ( hwnd, ID_ACCEPT );
				return TRUE;
	
			case ID_DECLINE:
			case ID_CANCEL:
				LOG ( L"License declined." );
				g_Settings.bLicenseAccepted = false;
				g_Settings.save_to_file ();
				EndDialog ( hwnd, ID_DECLINE );
				return TRUE;
			}
		}
		break;

	default:
		break;
	}

	return cDlgModal::dialog_proc ( hwnd, uMessage, wParam, lParam );
}


//----------------------------------------------------------------------------------------------------------------------
//  SET RICH EDIT TABS
//----------------------------------------------------------------------------------------------------------------------
void set_rich_edit_tabs_and_margins ( HWND hwnd )
{
	PARAFORMAT2 pf;
	memset ( &pf, 0, sizeof (pf) );
	pf.cbSize = sizeof(pf);

	pf.dwMask = PFM_TABSTOPS | PFM_RIGHTINDENT |  PFM_STARTINDENT | PFM_OFFSET | PFM_LINESPACING;

	pf.dxRightIndent = 500;
	pf.dxStartIndent = 400;
	pf.bLineSpacingRule = 5;
	pf.dyLineSpacing = 25;
	pf.cTabCount = 4;

	for ( int i = 0; i <= 3; i ++ )
		pf.rgxTabs[i] = i * 350;

	SendMessage ( hwnd, EM_SETPARAFORMAT, 0, (LPARAM) &pf );
}


//----------------------------------------------------------------------------------------------------------------------
//  RICH EDIT ALIGNMENT CENTER
//----------------------------------------------------------------------------------------------------------------------
void rich_edit_set_alignment_center ( HWND hwnd )
{
	PARAFORMAT2 pf;
	memset ( &pf, 0, sizeof (pf) );
	pf.cbSize = sizeof(pf);
	pf.dwMask = PFM_ALIGNMENT;
	pf.wAlignment = PFA_CENTER;
	SendMessage ( hwnd, EM_SETPARAFORMAT, 0, (LPARAM) &pf );
}


//----------------------------------------------------------------------------------------------------------------------
//  RICH EDIT ALIGNMENT LEFT
//----------------------------------------------------------------------------------------------------------------------
void rich_edit_set_alignment_left ( HWND hwnd )
{
	PARAFORMAT2 pf;
	memset ( &pf, 0, sizeof (pf) );
	pf.cbSize = sizeof(pf);
	pf.dwMask = PFM_ALIGNMENT;
	pf.wAlignment = PFA_LEFT;
	SendMessage ( hwnd, EM_SETPARAFORMAT, 0, (LPARAM) &pf );
}


//----------------------------------------------------------------------------------------------------------------------
//  IS THUMB AT BOTTOM
//----------------------------------------------------------------------------------------------------------------------
bool cDlgLicense :: thumb_is_at_bottom ()
{
	SCROLLINFO si;
	memset ( &si, 0, sizeof(si) );
	si.cbSize = sizeof (si);
	si.fMask  = SIF_ALL;
	GetScrollInfo ( Text2.hwnd, SB_VERT, &si) ;

	if ( (int)si.nMax <= (int)si.nTrackPos + (int)si.nPage )
		return true;

	else
		return false;
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