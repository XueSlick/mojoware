/***********************************************************************************************************************
/*
/*    cMessenger.cpp / mojo_engine
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/***********************************************************************************************************************/

#include "stdafx.h"

#include <process.h>

#pragma once

using namespace mojo;

//======================================================================================================================
// DATA
//======================================================================================================================

//======================================================================================================================
// PROTOTYPES
//======================================================================================================================

//======================================================================================================================
// CODE
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
//  REGISTER FOR KEY EVENTS
//----------------------------------------------------------------------------------------------------------------------
void cMessenger :: register_for_key_events   ( /* tCircBuf<WORD> * pBuf, */ HWND hNotifyMe )
{
	// pKeyEventNotifyBuf  = pBuf;
	hKeyEventNotificand = hNotifyMe;
}


//----------------------------------------------------------------------------------------------------------------------
//  UNREGISTER FOR KEY EVENTS
//----------------------------------------------------------------------------------------------------------------------
void cMessenger :: unregister_for_key_events   ( HWND hNotifyMe )
{
	if ( hKeyEventNotificand == hNotifyMe )
	   hKeyEventNotificand = 0;

	else
		assert ( 0 );
}

//----------------------------------------------------------------------------------------------------------------------
//  START SWALLOWING KEY EVENTS
//----------------------------------------------------------------------------------------------------------------------
void cMessenger :: start_swallowing_key_events ( HWND hwndSwallowArg )
{
	hwndSwallow = hwndSwallowArg;
}


//----------------------------------------------------------------------------------------------------------------------
//  STOP SWALLOWING KEY EVENTS
//----------------------------------------------------------------------------------------------------------------------
void cMessenger :: stop_swallowing_key_events ()
{
	hwndSwallow = 0;
}


//----------------------------------------------------------------------------------------------------------------------
// PRINT FROM MACH
//----------------------------------------------------------------------------------------------------------------------
const wchar_t * cMessenger :: print_from_mach ( mojo::cStrW * pRet, const cMach * pMach )
{
	if ( 0 == pMach )
		*pRet = L"From this computer.";

	else
		pRet->f ( L"From %s at %s.", ((cMach*)pMach)->sName.cstr(), ((cMach*)pMach)->sDottedDec.cstr() );

	return pRet->cstr();
}


//----------------------------------------------------------------------------------------------------------------------
// SEND MESSAGE (TO ONE PC)
//----------------------------------------------------------------------------------------------------------------------
void cMessenger :: send_message ( cMach * pMach, cMessage * pMsg )
{
	if ( ! g_Settings.bConnect )
		return;

	g_Pool.send ( pMach->dwIP, (char *) pMsg, pMsg->uLen );

	cStrW m;
	pMsg->print ( & m );

	put_ad_lib_memo ( cMemo::success, L"Message sent", L"To %s at %s.\n"
		                                              L"%s",
													  pMach->sName.cstr(),
													  pMach->sDottedDec.cstr(),
													  m.cstr() );
}


//----------------------------------------------------------------------------------------------------------------------
// BROADCAST MESSAGE (TO ALL PCS)
//----------------------------------------------------------------------------------------------------------------------
void cMessenger :: broadcast_message ( cMessage * pMsg )
{
	assert ( pMsg );

	if ( ! g_Settings.bConnect )
		return;

	cStrW sBody1;

	g_Machlist.lock();
	{
		for ( cMach * pMach = g_Machlist.pHead; pMach; pMach = pMach->pNext )
		{
			if ( pMach->bThisPC )
				continue;

			g_Pool.send ( pMach->dwIP, (char *) pMsg, pMsg->uLen );

			sBody1 += L"To ";
			sBody1 += pMach->sName.cstr();
			sBody1 += L" at ";
			sBody1 += pMach->sDottedDec.cstr();
			sBody1 += L".\n";
		}
	}

	g_Machlist.unlock();

	cStrW sBody2;
	pMsg->print ( & sBody2 );

	put_ad_lib_memo ( cMemo::success, L"Message sent", L"%s%s", sBody1.cstr(), sBody2.cstr() );
}


//----------------------------------------------------------------------------------------------------------------------
// ARE EQUAL (KBDLLHOOKSTRUCT)
// ignores time
//----------------------------------------------------------------------------------------------------------------------
bool are_equal ( KBDLLHOOKSTRUCT * p1, KBDLLHOOKSTRUCT * p2 )
{
	if ( p1->flags   == p2->flags    &&
		p1->scanCode == p2->scanCode &&
		p1->vkCode   == p2->vkCode       )
	{
		return true;
	}

	else
		return false;
}


//----------------------------------------------------------------------------------------------------------------------
// KEYBOARD HOOK SERVICE ROUTINE
//----------------------------------------------------------------------------------------------------------------------
bool cMessenger :: keyboard_hook_service_routine ( WPARAM wParam, KBDLLHOOKSTRUCT * p )
{
	//-----------------------------------------
	//  KILL SWITCH FOR DEBUGGING MOUSEOVER
	//-----------------------------------------

#if 0
	if ( p->vkCode == VK_F1 )
		exit(0);
#endif

	//-----------------------------------------
	//  SET "PREVIOUS KEY STATE" BIT WHICH
	//  IS USED IN WM_KEY* MESSAGES
	//-----------------------------------------
	
	p->dwExtraInfo = 0;

	WORD wExVK = cKeyState :: ex_vk ( p );

	if ( g_KeyState.is_down ( wExVK ) )   // DO THIS BEFORE RECEIVE
		p->dwExtraInfo |= ( 1<<30 );      // THIS BIT INDICATES PREV KEY STATE
	                                      // IT'S USED E.G. BY cSyringe

	//------------------------------------
	//  GIVE cKEYSTATE FIRST CRACK AT IT
	//------------------------------------

	g_KeyState.receive ( p );

	//------------------------------------
	//  DISPLAY EVENT
	//------------------------------------

	g_EventBuffer.receive ( wParam, p );

	//------------------------------------
	//  NOTIFY IF IT WAS REQUESTED
	//------------------------------------

	if ( g_Messenger.hKeyEventNotificand )
	{
		//---------------------------------
		//  NON-TYPEMATIC PRESSES ONLY
		//---------------------------------

		LPARAM lParam = p->flags & LLKHF_UP ? 1 : 0;

		if ( ! lParam && ! ( p->dwExtraInfo & (1<<30) )  )
		{
			PostMessage ( g_Messenger.hKeyEventNotificand, mojo::uWM_KEY_EVENT_OCCURRED, wExVK, lParam );
			// g_Messenger.pKeyEventNotifyBuf->put ( wExVK );
		}
	}

	//------------------------------------
	//  FIRST, SWALLOW IF A PARTICULAR
	//  APP WINDOW (PRESUMABLY MODAL)
	//  ASKED FOR KEYSTROKES TO BE
	//  BLOCKED.  (THE "GET TRIGGER"
	//  DIALOG DOES THIS.)
	//------------------------------------

	if ( g_Messenger.hwndSwallow )
		if ( g_Messenger.hwndSwallow == GetForegroundWindow() )
			return false;

	//------------------------------------
	//  SECOND, MOUSEOVER
	//------------------------------------

	if ( ! g_Mouseover.on_keyboard_hook ( wParam, p ) )
		return false;

	// if ( g_Settings.bMouseoverIsOn )
	// 	return g_Mouseover.on_keyboard_hook ( wParam, p );

	//------------------------------------
	//  THIRD, BROADCAST
	//------------------------------------

	if ( g_Settings.bBroadcastIsOn )
		g_KeyBroadcaster.receive_from_keyboard_hook ( wParam, p );

	//------------------------------------
	//  FOURTH, CHAIN NEXT HOOK
	//------------------------------------

	return true; // true means "call next hook in chain"
}


//----------------------------------------------------------------------------------------------------------------------
// MOUSE HOOK SERVICE ROUTINE
//----------------------------------------------------------------------------------------------------------------------
bool cMessenger :: mouse_hook_service_routine ( WPARAM wParam, MSLLHOOKSTRUCT * p )
{
	//------------------------------------
	//  DISPLAY EVENT
	//------------------------------------

	g_EventBuffer.receive ( wParam, p );

	//------------------------------------
	//  FIRST, MOUSEOVER
	//------------------------------------

	if ( ! g_Mouseover.on_mouse_hook ( wParam, p ) )
		return false;

//	if ( g_Settings.bMouseoverIsOn )
//		return g_Mouseover.on_mouse_hook ( wParam, p );

	//------------------------------------
	//  LAST, CHAIN NEXT HOOK
	//------------------------------------

	return true; // true means "call next hook in chain"
}


//----------------------------------------------------------------------------------------------------------------------
// PUT RECEIVE MEMO
//----------------------------------------------------------------------------------------------------------------------
void cMessenger :: put_receive_memo ( cMessage * pMsg, const wchar_t * pBody2 )
{
	put_ad_lib_memo ( cMemo::success, L"Message received", L"From %s at %s.\n%s",

														   pMsg->pFromMach->sName.cstr(),
														   pMsg->pFromMach->sDottedDec.cstr(),
														   pBody2 );
}


//----------------------------------------------------------------------------------------------------------------------
// RECEIVE
//----------------------------------------------------------------------------------------------------------------------
void cMessenger :: receive ( struct sSocketInfo * pSI, const char * pBuffer, unsigned uLen )
{
	pSI;
	assert ( pBuffer );

	const char * pEnd = pBuffer + uLen;

	//--------------------------------------
	// THIS LOOP DE-COALESCES TCP MESSAGES
	//--------------------------------------

	for ( const char * p = pBuffer; p < pEnd; p += ((cMessage*)p)->uLen )
	{
		cMessage * pMsg = (cMessage*)p;
		pMsg->pFromMach = pSI->pMach;

		if ( pMsg->uLen < 5 )
		{
			LOG ( L"Disregarding short message." );
			break;
		}

		cStrW sPrint;

		switch ( pMsg->Type )
		{
		case cMessage::broadcast_key_event:
			((cMessageBroadcastKeyEvent*)pMsg)->cMessageBroadcastKeyEvent::print(&sPrint);
			put_receive_memo ( pMsg, sPrint.cstr() );
			g_KeyBroadcaster.broadcast_to_local_windows ( ( cMessageBroadcastKeyEvent * ) pMsg );
			break;

		case cMessage::mouseover:
			g_Mouseover.receive_command ( pMsg );
			break;
		}
	}
}


//----------------------------------------------------------------------------------------------------------------------
//  SEND
//  This is the top-level chokepoint function for sending stuff to other PCs. pMessage can be allocated on the
//  stack.  It gets copied to heap storage by cPool.
//----------------------------------------------------------------------------------------------------------------------
void cMessenger :: send ( DWORD hMachHandle, cMessage * pMessage )
{
	DWORD dwIP = g_Machlist.handle_to_ip ( hMachHandle );
	g_Pool.send ( dwIP, (char*) pMessage, pMessage->uLen );
}


//----------------------------------------------------------------------------------------------------------------------
//  TELL APP THAT CONNECTIONS CHANGED
//----------------------------------------------------------------------------------------------------------------------
void cMessenger :: tell_app_that_machlist_changed ()
{
	PostMessage ( g_hwndApp, mojo::uWM_MACHLIST_CHANGED, 0, 0 );
}


//----------------------------------------------------------------------------------------------------------------------
//  TELL APP THAT BROADCAST TARGETS CHANGED
//----------------------------------------------------------------------------------------------------------------------
void cMessenger :: tell_app_that_targets_changed ()
{
	PostMessage ( g_hwndApp, mojo::uWM_TARGETS_CHANGED, 0, 0 );
}


//----------------------------------------------------------------------------------------------------------------------
//  START THREAD
//----------------------------------------------------------------------------------------------------------------------
DWORD cMessenger::start_thread ()
{
	_beginthreadex (  NULL, 0, &thread, reinterpret_cast<void*>( this ), 0, (UINT *)(&this->dwThreadID) );
	return this->dwThreadID;
}

//----------------------------------------------------------------------------------------------------------------------
//  Messenger THREAD
//----------------------------------------------------------------------------------------------------------------------
unsigned _stdcall cMessenger::thread ( void * pArg )
{
	//--------------------------------------
	// MAKE SURE THIS IS g_Messenger
	//--------------------------------------

	cMessenger * pThis = reinterpret_cast<cMessenger *>( pArg );
	assert ( &g_Messenger == pThis );

	//-----------------------------------------------------
	//  CREATE MESSAGE-ONLY WINDOW
	//  Doing this so cPool can call WSAAsyncSelect
	//  because cPool was already written that way.
	//-----------------------------------------------------

	g_Messenger.hwnd = CreateWindow ( L"Button", L"mojoMessenger", 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, g_hAppInstance, 0 );

	//-----------------------------------------------------
	//  MESSAGE LOOP
	//-----------------------------------------------------

	MSG msg;

	while ( GetMessage ( &msg, NULL, 0, 0 ) )
	{
		switch ( msg.message )
		{
		case uWM_SOCKET_HAS_CLOSED:
			g_Pool.on_socket_close_notification ( msg.wParam, msg.lParam );
			break;

		default:
			break;
		}
	}

	return 0;
}


/***********************************************************************************************************************
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
/***********************************************************************************************************************/