/********************************************************************************************************
/*
/*    cInputEventBuffer.cpp / mojo_engine
/*
/*    Copyright 2009 Robert Sacks.
/*
/*    This file is part of Mojo.  You may redistribute and/or modify Mojo under the terms of the GNU 
/*    General Public License, version 3, as published by the Free Software Foundation.  You should have
/*    received a copy of the license with mojo.  If you did not, go to http://www.gnu.org.
/*
/*    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR 
/*    IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND 
/*    FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
/*    CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
/*    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
/*    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER 
/*    IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT 
/*    OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
/*
/********************************************************************************************************/

#include "stdafx.h"
#include <windowsx.h>

using namespace mojo;

//=============================================================================================
// CODE
//=============================================================================================

//---------------------------------------------------------------------------------------------
//  GET INPUT EVENT
//  This is called by EXE's thread
//---------------------------------------------------------------------------------------------
MOJO_ENGINE_API bool mojo::get_input_event ( cInputEvent * p )
{
	return g_EventBuffer.Buf.get ( p );
}


//---------------------------------------------------------------------------------------------
// PRINT
//---------------------------------------------------------------------------------------------
#if 0
void cInputEvent :: print ( cStrW * pRet )
{

#if 1

	cInputEvent e = *pArg;
	const wchar_t * pMsg = NULL;
	const wchar_t * pEvent = NULL;
	wchar_t awTxt [2000];

	if ( Buf.pop ( &e ) )
	// while ( Buf.get ( &e ) )
	{
		if ( cInputEvent::mouse == e.eType )
		{
			WORD wMouseVK = 0;

			switch ( e.wParam )
			{
			case WM_LBUTTONDOWN:
				pMsg = L"LButton"; pEvent = L"down"; wMouseVK = 1; break;
			case WM_LBUTTONUP:
				pMsg = L"LButton"; pEvent = L"up";   wMouseVK = 1; break;
			case WM_MOUSEWHEEL:
				if ( 0 < (int (e.u.m.mouseData ) >> 16 ))
					{pMsg = L"MouseWheel"; pEvent = L"forward"; wMouseVK = 0xA; }
				else
					{pMsg = L"MouseWheel"; pEvent = L"back"; wMouseVK = 0xB; }
				break;
			case WM_MBUTTONDOWN:
				pMsg = L"MButton"; pEvent = L"down"; wMouseVK = 4; break;
			case WM_MBUTTONUP:
				pMsg = L"MButton"; pEvent = L"up"; wMouseVK = 4; break;
			case WM_RBUTTONDOWN:
				pMsg = L"RButton"; pEvent = L"down"; wMouseVK = 2; break;
			case WM_RBUTTONUP:
				pMsg = L"RButton"; pEvent = L"up"; wMouseVK = 2; break;
			case WM_XBUTTONDOWN:
				if ( XBUTTON1 == HIWORD(e.u.m.mouseData) )
					{pMsg = L"Button4"; pEvent = L"down"; wMouseVK = 5; }
				else
					{pMsg = L"Button5"; pEvent = L"down"; wMouseVK = 6; }
				break;
			case WM_XBUTTONUP:
				if ( XBUTTON1 == HIWORD(e.u.m.mouseData) )
				{pMsg = L"Button4"; pEvent = L"up"; wMouseVK = 5; }
				else
				{pMsg = L"Button5"; pEvent = L"up"; wMouseVK = 6; }
				break;
#if 0
			case uWM_MOUSEOVER_MOVE_RELATIVE:
				GetCursorPos ( &e.u.m.pt );
			case WM_MOUSEMOVE:
			case uWM_MOUSEOVER_MOVE_ABSOLUTE:
				{
					wchar_t t [ 500 ];

					if ( g_Mouseover.eMode == cMouseover::master )
					{
						wsprintf ( t, L"\r\n"
									  L"   Mouse motion\r\n"
						              L"   scrn position:\t%s", g_Mouseover.get_slave()->sName.cstr() );
					}

					else
					{
						POINT ptClient = e.u.m.pt;
						ScreenToClient ( GetForegroundWindow(), &ptClient );
						
						wsprintf ( t, L"\r\n" 
									  L"   Mouse motion\r\n"
									  L"   scrn position:\tx = %d\ty = %d\r\n"
									  L"   win position:\tx = %d\ty = %d\r\n",
									  e.u.m.pt.x, 
									  e.u.m.pt.y,
									  ptClient.x, 
									  ptClient.y );
					}

					Edit_SetText ( hwnd, t );
					// continue;
					return;
				}
#endif
			}

			POINT pt = e.u.m.pt;
			ScreenToClient ( GetForegroundWindow(), &pt );

#if 0
			if ( g_Mouseover.eMode == cMouseover::master )
			{
				wsprintf ( awTxt,

						  L"\r\n"
						  L"   Key name:\t%s\r\n"
				          L"   event:\t%s\r\n"
				          L"   virtual key:\t%d\r\n"
						  L"   scrn position:\t%s",
						  pMsg,
						  pEvent,
						  wMouseVK,
						  g_Mouseover.get_slave()->sName.cstr() );
			}

			else

#endif
			{
				wsprintf ( awTxt,

						  L"\r\n"
						  L"   Key name:\t%s\r\n"
				          L"   event:\t%s\r\n"
				          L"   virtual key:\t%d\r\n"
						  L"   scrn position:\tx = %d\ty = %d\r\n"
						  L"   win position:\tx = %d\ty = %d\r\n",
						  pMsg,
						  pEvent,
						  wMouseVK,
						  e.u.m.pt.x, e.u.m.pt.y,
						  pt.x, pt.y );
			}
		}

		else // keyboard
		{


			wchar_t * pEvent = NULL;

			WORD wExVK = (WORD) e.u.k.vkCode;

#if 0
			// TEMP 
			static tArray<WORD> TempRay;
			TempRay.append(wExVK);
			// END TEMP
#endif

			if ( e.u.k.flags & LLKHF_EXTENDED )
				wExVK += 0x100;
			
			cStrW m = cKeyboard::ex_vk_to_pretty_name ( wExVK );

			switch ( e.wParam )
			{
			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
				pEvent = L"down";
				break;
			case WM_KEYUP:
			case WM_SYSKEYUP:
				pEvent = L"up";
				break;
			}

			wsprintf ( awTxt,	L"\r\n"
								L"   Key name:\t%s\r\n"
								L"   event:\t%s\r\n"
								L"   virtual key:\t%d\r\n"
								L"   scan code:\t%d\r\n"
								L"   extended:\t%s\r\n", 
								cKeyboard::ex_vk_to_pretty_name(wExVK),
								pEvent,
								e.u.k.vkCode, 
								e.u.k.scanCode, 
								e.u.k.flags & LLKHF_EXTENDED ? L"yes" : L"no" );

#if 0
			if ( g_Settings.bShowKeyboardEventsInMonitor )
			{
				wchar_t t [1000];
				wsprintf ( t,	L"%s %s: vk=%d; sc=%d; extended=%s; injected=%s.",
								cKeyboard::ex_vk_to_pretty_name(wExVK),
								pEvent,
								e.u.k.vkCode,
								e.u.k.scanCode,
								e.u.k.flags & LLKHF_EXTENDED ? L"yes" : L"no",
								e.u.k.flags & LLKHF_INJECTED ? L"yes" : L"no" );

				// g_Monitor.rcv ( sMon ( L"Keyboard event.", 0, black ), sMon ( t, 1, blue ) );
			}
#endif
		}

		*pRet = awTxt;

		// Edit_SetText ( hwnd, awTxt );
	}

#endif

}
#endif


//---------------------------------------------------------------------------------------------
// CONSTRUCTOR
//---------------------------------------------------------------------------------------------
cInputEventBuffer::cInputEventBuffer () : Buf(32)
{

}


//---------------------------------------------------------------------------------------------
// SET HWND
//---------------------------------------------------------------------------------------------
void cInputEventBuffer :: set_hwnd ( HWND hArg )
{ 
	hwnd = hArg; 

#if 0
	HFONT hf  = ::get_message_hfont ();

	rich_edit_set_font				( hwnd, L"Segoe UI", false );
	rich_edit_set_text_height		( hwnd, 180 );

	rich_edit_set_line_space		( hwnd, 22, false );
	rich_edit_set_left_margin		( hwnd, 300 );
	rich_edit_set_tabs				( hwnd, 500);
#endif
}


//---------------------------------------------------------------------------------------------
// RECEIVE -- KEYBOARD
//---------------------------------------------------------------------------------------------
void cInputEventBuffer :: receive ( WPARAM wParam, KBDLLHOOKSTRUCT * p )
{
	mojo::cInputEvent se;

	se.u.k = *p;
	se.wParam = wParam;
	se.eType = mojo::cInputEvent::keyboard;
	Buf.put ( &se );

	// PostMessage ( g_hwnd, uWM_MONITOR_EVENT, 0, 0 );
}


//---------------------------------------------------------------------------------------------
// RECEIVE -- MOUSE
//---------------------------------------------------------------------------------------------
void cInputEventBuffer :: receive ( WPARAM wParam, MSLLHOOKSTRUCT * p )
{

#if 0
#ifndef _DEBUG
	// return if it's only movement
	if ( WM_MOUSEMOVE == wParam )
		return;
#endif
#endif

	mojo::cInputEvent se;

	se.u.m = *p;
	se.wParam = wParam;
	se.eType = mojo::cInputEvent::mouse;
	Buf.put ( &se );

	// PostMessage ( g_hwnd, uWM_MONITOR_EVENT, 0, 0 );
}


