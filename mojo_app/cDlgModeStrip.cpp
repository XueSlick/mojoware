/***********************************************************************************************************************
/*
/*    cDlgModeStrip.cpp / mojo_app
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
const int iLeftMargin = 0;

//======================================================================================================================
// PROTOTYPES
//======================================================================================================================

LRESULT CALLBACK tool_proc ( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


//======================================================================================================================
// CODE
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
// WM INIT
//----------------------------------------------------------------------------------------------------------------------
int cDlgModeStrip::get_height ()
{
	return ( 1 * iMargin ) + iButtonHeight;
}


//----------------------------------------------------------------------------------------------------------------------
// WM INIT
//----------------------------------------------------------------------------------------------------------------------
void cDlgModeStrip::wm_init ()
{
	//--------------------------------------------
	//  IF WE'RE ON XP, REMOVE COMPOSITED STYLE
	//  BECAUSE IT MAKES THE OWNER DRAWN BUTTONS
	//  INVISIBLE
	//--------------------------------------------
	if ( ! os_version_or_higher ( 6, 0 ) )
	{
		LONG_PTR dwStyle = GetWindowLongPtr ( hwnd, GWL_EXSTYLE );
		dwStyle &= ~WS_EX_COMPOSITED;
		SetWindowLongPtr ( hwnd, GWL_EXSTYLE, dwStyle );
	}

	const int iButtonWidth = 150;

	ToggleMouseover.hwnd = GetDlgItem ( hwnd, ID_TOGGLE_MOUSEOVER );
	register_child ( &ToggleMouseover,

							  nAnchor::left,		0,		iLeftMargin,
							  nAnchor::top,			0,		iMargin,
							  nAnchor::left,		0,		iLeftMargin + iButtonWidth,
							  nAnchor::top,			0,      iMargin + iButtonHeight );

	ToggleHotkeys.hwnd = GetDlgItem ( hwnd, ID_TOGGLE_HOTKEYS );
	register_child ( &ToggleHotkeys,

							  nAnchor::left,		0,		iLeftMargin + iMargin + iButtonWidth,
							  nAnchor::top,			0,		iMargin,
							  nAnchor::left,		0,		iLeftMargin + iMargin + iButtonWidth * 2,
							  nAnchor::top,			0,      iMargin + iButtonHeight );

	ToggleWindowBroadcast.hwnd = GetDlgItem ( hwnd, ID_TOGGLE_WINDOW_BROADCAST );
	register_child ( &ToggleWindowBroadcast,

							  nAnchor::left,		0,		iLeftMargin + iMargin * 2 + iButtonWidth * 2,
							  nAnchor::top,			0,		iMargin,
							  nAnchor::left,		0,		iLeftMargin + iMargin * 2 + iButtonWidth * 3,
							  nAnchor::top,			0,      iMargin + iButtonHeight );

	ToggleComputerBroadcast.hwnd = GetDlgItem ( hwnd, ID_TOGGLE_COMPUTER_BROADCAST );
	register_child ( &ToggleComputerBroadcast,

							  nAnchor::left,		0,		iLeftMargin + iMargin * 3 + iButtonWidth * 3,
							  nAnchor::top,			0,		iMargin,
							  nAnchor::left,		0,		iLeftMargin + iMargin * 3 + iButtonWidth * 4,
							  nAnchor::top,			0,      iMargin + iButtonHeight );



}


//----------------------------------------------------------------------------------------------------------------------
//  REDRAW BUTTONS
//----------------------------------------------------------------------------------------------------------------------
void cDlgModeStrip::redraw_buttons ()
{
	InvalidateRect ( ToggleMouseover.hwnd, NULL, TRUE );
	UpdateWindow   ( ToggleMouseover.hwnd );

	InvalidateRect ( ToggleHotkeys.hwnd, NULL, TRUE );
	UpdateWindow   ( ToggleHotkeys.hwnd );

	InvalidateRect ( ToggleWindowBroadcast.hwnd, NULL, TRUE );
	UpdateWindow   ( ToggleWindowBroadcast.hwnd );

	InvalidateRect ( ToggleComputerBroadcast.hwnd, NULL, TRUE );
	UpdateWindow   ( ToggleComputerBroadcast.hwnd );
}


//----------------------------------------------------------------------------------------------------------------------
// WM DRAWITEM
//----------------------------------------------------------------------------------------------------------------------
void cDlgModeStrip::wm_drawitem ( int iID, DRAWITEMSTRUCT* pDI )
{
	switch ( iID )
	{
	case ID_TOGGLE_MOUSEOVER:
		if ( g_Settings.bMouseoverIsOn )
			ToggleMouseover.paint_green ( pDI, L"Mouseover is on" );
		else
			ToggleMouseover.paint_gray   ( pDI, L"Mouseover is off" );
		break;

	case ID_TOGGLE_HOTKEYS:
		if ( g_Settings.bHotkeysAreOn )
			ToggleHotkeys.paint_green ( pDI, L"Hotkeys are on" );
		else
			ToggleHotkeys.paint_gray   ( pDI, L"Hotkeys are off" );
		break;

	case ID_TOGGLE_WINDOW_BROADCAST:
		if ( g_Settings.bWindowBroadcastIsOn )
			ToggleWindowBroadcast.paint_green ( pDI, L"Window broadcast is on" );
		else
			ToggleWindowBroadcast.paint_gray   ( pDI, L"Window broadcast is off" );
		break;

	case ID_TOGGLE_COMPUTER_BROADCAST:
		if ( g_Settings.bComputerBroadcastIsOn )
			ToggleComputerBroadcast.paint_green ( pDI, L"Computer broadcast is on" );
		else
			ToggleComputerBroadcast.paint_gray   ( pDI, L"Computer broadcast is off" );
		break;
	}
}


//----------------------------------------------------------------------------------------------------------------------
// DEFAULT PROC
//----------------------------------------------------------------------------------------------------------------------
INT_PTR CALLBACK cDlgModeStrip::dialog_proc (HWND hwnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	cWin * pWin = user_data_to_pWin ( hwnd );
	cDlgModeStrip * pThis = static_cast<cDlgModeStrip*>(pWin);

	switch ( uMessage )
	{
	case WM_COMMAND:
		{
			if ( BN_CLICKED == HIWORD ( wParam ) )
			{
				switch 	( LOWORD ( wParam ) )
				{
				case ID_TOGGLE_MOUSEOVER:
					PostMessage ( g_hwnd, WM_COMMAND, ID_TOGGLE_MOUSEOVER, ID_TOGGLE_MOUSEOVER );
					break;

				case ID_TOGGLE_HOTKEYS:
					pThis->cWin::balloon ( GetDlgItem ( hwnd, ID_TOGGLE_HOTKEYS ), L"Sorry.", L"Hotkeys aren't implemented yet." );
					break;

				case ID_TOGGLE_WINDOW_BROADCAST:
					PostMessage ( g_hwnd, WM_COMMAND, ID_TOGGLE_WINDOW_BROADCAST, ID_TOGGLE_WINDOW_BROADCAST );
					break;

				case ID_TOGGLE_COMPUTER_BROADCAST:
					pThis->cWin::balloon ( GetDlgItem ( hwnd, ID_TOGGLE_COMPUTER_BROADCAST ), L"Sorry.", L"Computer broadcast isn't implemented yet." );
					break;

				case ID_VIEW_MONITOR:
					PostMessage ( g_hwnd, WM_COMMAND, ID_VIEW_MONITOR, 0 );
					break;

				case ID_VIEW_WOWS:
					PostMessage ( g_hwnd, WM_COMMAND, ID_VIEW_WOWS, 0 );
					break;
				}
			}
		}
		break;

	case WM_DRAWITEM:
		if ( pThis )
			pThis->wm_drawitem ( (int) wParam, (DRAWITEMSTRUCT*) lParam );
		break;

	case WM_INITDIALOG:
		{
			set_user_data ( hwnd, lParam );
			cWin * pWin = lParam_to_pWin ( hwnd, lParam );
			pThis = static_cast<cDlgModeStrip*>(pWin);
			pThis->hwnd = hwnd;
			pThis->wm_init ();
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