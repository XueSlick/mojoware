/***********************************************************************************************************************
/*
/*    cDlgViewStrip.cpp / mojo_app
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"

//======================================================================================================================
// DATA
//======================================================================================================================

const int iButtonHeight = 44;
const int iMargin = 9;

//======================================================================================================================
// PROTOTYPES
//======================================================================================================================

//======================================================================================================================
// CODE
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
// WM INIT
//----------------------------------------------------------------------------------------------------------------------
void cDlgViewStrip::wm_init ()
{	
	ViewComputers.hwnd = GetDlgItem ( hwnd, ID_VIEW_COMPUTERS );
	register_child ( &ViewComputers,

							  nAnchor::left,		0,		iMargin,
							  nAnchor::top,			0,		iMargin,
							  nAnchor::right,		0,		-iMargin,
							  nAnchor::top,			0,      iMargin + iButtonHeight );

	ViewWoWs.hwnd = GetDlgItem ( hwnd, ID_VIEW_WOWS );
	register_child ( &ViewWoWs,

							  nAnchor::left,		0,		iMargin,
							  nAnchor::top,			0,		iMargin * 2 + iButtonHeight,
							  nAnchor::right,		0,		-iMargin,
							  nAnchor::top,			0,      iMargin * 2 + iButtonHeight * 2 );

	ViewToons.hwnd = GetDlgItem ( hwnd, ID_VIEW_TOONS );
	register_child ( &ViewToons,

							  nAnchor::left,		0,		iMargin,
							  nAnchor::top,			0,		iMargin * 3 + 2 * iButtonHeight,
							  nAnchor::right,		0,		-iMargin,
							  nAnchor::top,			0,      iMargin * 3 + iButtonHeight * 3 );

	ViewTeams.hwnd = GetDlgItem ( hwnd, ID_VIEW_TEAMS );
	register_child ( &ViewTeams,

							  nAnchor::left,		0,		iMargin,
							  nAnchor::top,			0,		iMargin * 4 + 3 * iButtonHeight,
							  nAnchor::right,		0,		-iMargin,
							  nAnchor::top,			0,      iMargin * 4 + iButtonHeight * 4 );

	ViewMonitor.hwnd = GetDlgItem ( hwnd, ID_VIEW_MONITOR );
	register_child ( &ViewMonitor,

							  nAnchor::left,		0,		iMargin,
							  nAnchor::top,			0,		iMargin * 5 + iButtonHeight * 4,
							  nAnchor::right,		0,		-iMargin,
							  nAnchor::top,			0,      iMargin * 5 + iButtonHeight * 5 );
}


//----------------------------------------------------------------------------------------------------------------------
//  DIALOG PROC
//----------------------------------------------------------------------------------------------------------------------
INT_PTR CALLBACK cDlgViewStrip::dialog_proc (HWND hwnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	cWin * pWin = user_data_to_pWin ( hwnd );
	cDlgViewStrip * pThis = static_cast<cDlgViewStrip*>(pWin);

	switch ( uMessage )
	{
	case WM_COMMAND:
		{
			if ( BN_CLICKED == HIWORD ( wParam ) )
			{
				switch 	( LOWORD ( wParam ) )
				{
				case ID_VIEW_COMPUTERS:
					PostMessage ( g_hwnd, WM_COMMAND, ID_VIEW_COMPUTERS, 0 );
					break;

				case ID_VIEW_MONITOR:
					PostMessage ( g_hwnd, WM_COMMAND, ID_VIEW_MONITOR, 0 );
					break;

				case ID_VIEW_WOWS:
					PostMessage ( g_hwnd, WM_COMMAND, ID_VIEW_WOWS, 0 );
					break;

				case ID_VIEW_TOONS:
					PostMessage ( g_hwnd, WM_COMMAND, ID_VIEW_TOONS, 0 );
					break;
				}
			}
		}
		break;

	case WM_INITDIALOG:
		{
			set_user_data ( hwnd, lParam );
			cWin * pWin = lParam_to_pWin ( hwnd, lParam );
			pThis = static_cast<cDlgViewStrip*>(pWin);
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