/***********************************************************************************************************************
/*
/*    cWinMain_window_proc.cpp / mojo_app
/*
/*    This file contains the window proc for the main window.
/* 
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"
#include <WindowsX.h>

//======================================================================================================================
//  DATA
//======================================================================================================================

extern unsigned uWM_ARE_YOU_ME; // defined in ..\\mojo_engine\single_instance.cpp


//======================================================================================================================
//  CODE
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
//  MAIN WND PROC
//----------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK cWinMain::window_proc ( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	cWin		* pWin  = user_data_to_pWin ( hwnd );
	cWinMain	* pThis = static_cast<cWinMain*>(pWin);

	if ( uMsg == uWM_ARE_YOU_ME )
	{										// Another instance of this program is trying to start,
		ShowWindow ( hwnd, SW_RESTORE );	// so pop this one up instead.  The other one will abort
		ShowWindow ( hwnd, SW_NORMAL );		// itself when it sees this one.
		return uWM_ARE_YOU_ME;
	}

	switch ( uMsg )
	{

	case mojo::uWM_WINDOW_BROADCAST_OFF_ON:
		g_Settings.bWindowBroadcastIsOn = 0 == wParam ? true : false;
		pThis->toggle_broadcast();
		break;

	case mojo::uWM_MOUSEOVER_OFF_ON:
		g_Settings.bMouseoverIsOn = 0 == wParam ? true : false;
		pThis->toggle_mouseover();
		break;

	case WM_KEYDOWN:
		{
			int x = 3;
			x++;
		}
		break;

	case WM_KEYUP:
		{
			int x = 3;
			x++;
		}
		break;

	case WM_SYSKEYDOWN:
		{
			int x = 3;
			x++;
		}
		break;

	case WM_SYSKEYUP:
		{
			int x = 3;
			x++;
		}
		break;

	case uWM_TARGETS_CHANGED:
		{
			mojo::cArrayTarget ta;
			mojo::get_targets ( &ta );

			g_FigMgr.receive ( &ta );

#if 0

			g_Config.receive ( &ta );
			cStrW sAll;
			for ( unsigned u = 0; u < ta.qty(); u++ )
			{
				if ( 0 < u )
					sAll += L"\n";

				cStrW sOne;
				sOne.f ( L"Window handle 0x%X.", ta[u].hwnd );
				sAll += sOne;
			}
			mojo::put_ad_lib_memo ( mojo::cMemo::info, L"Broadcast targets:", sAll.cstr() );

#endif

			pThis->DlgWoWs.populate();
		}
		break;

	case WM_COMMAND:
		if ( pThis )
		{
			pThis->wm_command ( wParam, lParam );
			return cWin :: window_proc ( hwnd, uMsg, wParam, lParam );
		}
		break;

	case uWM_WOW_LIST_CHANGED:
		PostMessage ( pThis->DlgWoWs.hwnd, uWM_WOW_LIST_CHANGED, 0, 0 );
		break;

	case mojo::uWM_MACHLIST_CHANGED:
		{
			cMachlist l;
			mojo::get_machlist ( &l );
			pThis->DlgMonitor.Connections.populate ( &l );
			PostMessage ( pThis->DlgComputers.hwnd, uWM_MACHLIST_CHANGED, 0, 0 );
		}
		break;

	case uWM_TOON_LIST_CHANGED:
		{
			PostMessage ( pThis->DlgToons.hwnd, uWM_TOON_LIST_CHANGED, 0, 0 );
		}
		break;

	case WM_CREATE:
		{
			CREATESTRUCT * pCS = reinterpret_cast<CREATESTRUCT*>(lParam);
			cWin * pWin = static_cast<cWin*>(pCS->lpCreateParams);
			set_user_data ( hwnd, pWin );
			pThis = static_cast<cWinMain*>(pWin);
			assert(pThis);	
			pThis->wm_create ( hwnd );
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_ERASEBKGND:
		return 1;

	case WM_GETMINMAXINFO:
		{
			MINMAXINFO * p    = (MINMAXINFO*) lParam;
			POINT pt          = { 515, 330 };
			p->ptMinTrackSize = pt;
		}
		break;

	case uWM_HIDE_OR_SHOW_CURSOR:
		pThis->hide_or_show_cursor ( wParam );
		break;

	case WM_HOTKEY:
		if ( VK_F1 == wParam )
			exit(0);
		break;

	case mojo::uWM_INPUT_EVENT_READY:
		{
			mojo::cInputEvent ie;
			cStrW m;
			while ( mojo::get_input_event ( &ie ) )
			{
				ie.print ( &m );
				if ( pThis && pThis->DlgMonitor.InputEvents.hwnd )
				Edit_SetText ( pThis->DlgMonitor.InputEvents.hwnd, m.cstr() );

				// mojo::put_ad_lib_memo ( mojo::cMemo::warning, m.cstr() );
			}
		}
		break;

	case mojo::uWM_MEMO_READY:
		if ( pThis )
				PostMessage ( pThis->DlgMonitor.hwnd, mojo::uWM_MEMO_READY, 0, 0 );
		break;

	case WM_TIMER:
			mojo::put_ad_lib_memo ( cMemo::warning, L"Testing new scroll code", L"Aye I'm testing at rate of 33 messages per second." );
			break;
	


#if 0
	case uWM_SET_MOUSEOVER_DISPLAY_LIST:
		set_mouseover_display_list();
		break;





	case uWM_TRAY_ICON:
		wm_tray_icon ( wParam, lParam );
		break;

	case uWM_SOCKET_HAS_CLOSED:
		g_Pool.on_socket_close_notification ( wParam, lParam );
		break;

	case uWM_MACHLIST_RECEIPTS:
		g_Machlist.add_receipts();
		break;

	case uWM_MONITOR:
		g_Monitor.display();
		break;

	case uWM_MONITOR_EVENT:
		g_MonitorEvent.print();
		break;

	case uWM_MONITOR_CONNECTIONS:
		SendMessage ( pThis->DlgMain.hwnd, uWM_MONITOR_CONNECTIONS, 0, 0 );
		break;

	case WM_SIZE:
		{
			unsigned uShowState = get_show_state ( hWnd );

			if ( uShowState == SW_NORMAL )
			{
				RECT r;
				GetWindowRect (g_hwnd, &r);
				g_Settings.WinPos.dx = r.right-r.left;
				g_Settings.WinPos.dy = r.bottom-r.top;
			}
		}
		pThis->cWin::on_size( LOWORD(lParam), HIWORD(lParam) );
		break;

	case WM_MOVE:
		{
			unsigned uShowState = get_show_state ( hWnd );

			if ( uShowState == SW_NORMAL )
			{
				// RECT r;
				// GetWindowRect (g_hwnd, &r);
				// g_Settings.WinPos.x = r.left;
				// g_Settings.WinPos.y = r.top;

				// window placement functions use workspace coordinates

				WINDOWPLACEMENT wp;
				wp.length = sizeof(wp);
				GetWindowPlacement ( g_hwnd, &wp );
				g_Settings.WinPos.x = wp.rcNormalPosition.left;
				g_Settings.WinPos.y = wp.rcNormalPosition.top;
			}
		}
		break;

	case WM_COMMAND:

		pThis->wm_command ( wParam, lParam );
		return cWin :: window_proc ( hWnd, uMessage, wParam, lParam );
		break;

#if 0
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		EndPaint(hWnd, &ps);
		break;
#endif

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
#endif
	}

	return cWin :: window_proc ( hwnd, uMsg, wParam, lParam );
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

