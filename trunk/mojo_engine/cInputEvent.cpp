/********************************************************************************************************
/*
/*    cInputEvent.cpp | mojo_engine
/*
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/*********************************************************************************************************/

#include "stdafx.h"
#include <windowsx.h>

using namespace mojo;

//=============================================================================================
//  DATA
//=============================================================================================

cStrW cInputEvent :: sMouseMotion;
cStrW cInputEvent :: sScrnPosition;
cStrW cInputEvent :: sWinPosition;
cStrW cInputEvent :: sKeyName;
cStrW cInputEvent :: sEvent;
cStrW cInputEvent :: sVirtualKey;
cStrW cInputEvent :: sScanCode;
cStrW cInputEvent :: sExtended;

//=============================================================================================
// CODE
//=============================================================================================

void cInputEvent :: load_scribs_sub ( cStrW * pRet, const wchar_t * pKey )
{
	cScribPack sp ( pKey );
	*pRet = sp.head;


}


void cInputEvent :: load_scribs ()
{
	load_scribs_sub ( &sMouseMotion, 	L"InputEvents.MouseMotion" );
	load_scribs_sub ( &sScrnPosition,	L"InputEvents.ScrnPosition" );
	load_scribs_sub ( &sWinPosition, 	L"InputEvents.WinPosition" );
	load_scribs_sub ( &sKeyName, 		L"InputEvents.KeyName" );
	load_scribs_sub ( &sEvent, 			L"InputEvents.Event" );
	load_scribs_sub ( &sVirtualKey, 	L"InputEvents.VirtualKey" );
	load_scribs_sub ( &sScanCode, 		L"InputEvents.ScanCode" );
	load_scribs_sub ( &sExtended, 		L"InputEvents.Extended" );
}

//---------------------------------------------------------------------------------------------
// PRINT
//---------------------------------------------------------------------------------------------
const wchar_t * cInputEvent :: print ( cStrW * pRet )
{

#if 1

	cInputEvent e = *this;
	const wchar_t * pMsg = NULL;
	const wchar_t * pEvent = NULL;
	wchar_t awTxt [2000];

	// if ( Buf.pop ( &e ) )
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
#if 1
			// case uWM_MOUSEOVER_MOVE_RELATIVE:
			// 	GetCursorPos ( &e.u.m.pt );
			case WM_MOUSEMOVE:
			// case uWM_MOUSEOVER_MOVE_ABSOLUTE:
				{
					wchar_t t [ 500 ];
#if 0
					if ( g_Mouseover.eMode == cMouseover::master )
					{
						wsprintf ( t, L"\r\n"
									  L"   Mouse motion\r\n"
						              L"   scrn position:\t%s", g_Mouseover.get_slave()->sName.cstr() );
					}

					else
#endif
					{
						POINT ptClient = e.u.m.pt;
						ScreenToClient ( GetForegroundWindow(), &ptClient );
						
						wsprintf ( t, L"\r\n" 
									  L"   %s\r\n"
									  L"   %s:\tx = %d\ty = %d\r\n"
									  L"   %s:\tx = %d\ty = %d\r\n",
									  sMouseMotion.cstr(),
									  sScrnPosition.cstr(),
									  e.u.m.pt.x, 
									  e.u.m.pt.y,
									  sWinPosition.cstr(),
									  ptClient.x, 
									  ptClient.y );
					}
					
					*pRet = t;
					// Edit_SetText ( hwnd, t );
					// continue;
					return pRet->cstr();
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
						  L"   %s:\t%s\r\n"
				          L"   %s:\t%s\r\n"
				          L"   %s:\t%d\r\n"
						  L"   %s:\tx = %d\ty = %d\r\n"
						  L"   %s:\tx = %d\ty = %d\r\n",
						  sKeyName.cstr(),
						  pMsg,
						  sEvent.cstr(),
						  pEvent,
						  sVirtualKey.cstr(),
						  wMouseVK,
						  sScrnPosition.cstr(),
						  e.u.m.pt.x, e.u.m.pt.y,
						  sWinPosition.cstr(),
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
								L"   %s:\t%s\r\n"
								L"   %s:\t%s\r\n"
								L"   %s:\t%d\r\n"
								L"   %s:\t%d\r\n"
								L"   %s:\t%s\r\n", 
								sKeyName.cstr(),
								cKeyboard::ex_vk_to_pretty_name(wExVK),
								sEvent.cstr(),
								pEvent,
								sVirtualKey.cstr(),
								e.u.k.vkCode,
								sScanCode.cstr(),
								e.u.k.scanCode,
								sExtended.cstr(),
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
		return pRet->cstr();

		// Edit_SetText ( hwnd, awTxt );
	}

#endif

}



/********************************************************************************************************
/*
/*    This file is part of Mojo.  For more information, see http://mojoware.org.
/*
/*    You may redistribute and/or modify Mojo under the terms of the GNU General Public License, 
/*    version 3, as published by the Free Software Foundation.  You should have received a copy of the 
/*    license with mojo.  If you did not, go to http://www.gnu.org.
/*     
/*    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
/*    INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A 
/*    PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
/*    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
/*    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
/*    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
/*    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
/*    EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
/*
/********************************************************************************************************/
