/***********************************************************************************************************************
/*
/*    cKeyBroadcaster.cpp
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"

using namespace mojo;

//======================================================================================================================
//  DATA
//======================================================================================================================


//======================================================================================================================
//  PROTOTYPES
//======================================================================================================================


//======================================================================================================================
//  CODE
//======================================================================================================================


//----------------------------------------------------------------------------------------------------------------------
//  INJECT KEY EVENT
//----------------------------------------------------------------------------------------------------------------------
void cKeyBroadcaster :: inject_key_event ( const mojo::cTarget * pTarget, WPARAM wParam, KBDLLHOOKSTRUCT * p )
{
	pTarget; wParam; p;

	mojo::put_ad_lib_memo ( cMemo::info, L"Injecting key event", L"Handle: %x: %s", pTarget->hwnd, ((cTarget*)pTarget)->sPath.cstr() );	
}


//----------------------------------------------------------------------------------------------------------------------
//  RECEIVE FROM KEYBOARD HOOK
//----------------------------------------------------------------------------------------------------------------------
void cKeyBroadcaster :: receive_from_keyboard_hook ( WPARAM wParam, KBDLLHOOKSTRUCT * p )
{
	WORD wExVK = cKeyboardStateEx :: ex_vk ( p );

	HWND hForeground = GetForegroundWindow();

	if ( ( ( p->flags & LLKHF_UP ) && this->KeyboardState.is_down ( wExVK ) ) || 
		 g_TargetMgr.is_broadcast_source ( hForeground ) )
	{
		cMessageBroadcastKeyEvent m ( wParam, p );
		broadcast_to_local_windows ( &m, hForeground );
		cMessenger::broadcast_message ( &m );
		this->KeyboardState.receive ( p );
	}
}


//----------------------------------------------------------------------------------------------------------------------
//  BROADCAST TO LOCAL WINDOWS
//----------------------------------------------------------------------------------------------------------------------
void cKeyBroadcaster :: broadcast_to_local_windows ( const cMessageBroadcastKeyEvent * pMsg, HWND hExcludeThisWindow )
{
	unsigned uQty = 0;
	cStrW sBody;

	mojo::tList2<mojo::cTarget> * pList = &g_TargetMgr.List;

	pList->lock();
	{
		for ( cTarget * pTarget = pList->pHead; pTarget; pTarget = pTarget->pNext )
		{
			if ( 1 == pTarget->hMach && pTarget->bIsRunning && pTarget->hwnd != hExcludeThisWindow )
			{
				cSyringe::send_notify_message ( pTarget->hwnd, pMsg->wParam, &pMsg->kbhs, pTarget->aKeyboardState );

				//--------------------------------------
				// MAKE SCRIB BODY
				//--------------------------------------

				wchar_t t [300];

				if ( uQty )
					swprintf_s ( t, sizeof(t)/sizeof(wchar_t), L"\nWindow handle 0x%X.", pTarget->hwnd );

				else
					swprintf_s ( t, sizeof(t)/sizeof(wchar_t), L"Window handle 0x%X.",   pTarget->hwnd );

				sBody += t;

				uQty ++;
			}
		}
	}
	pList->unlock();

	cStrW sFrom;
	cMessenger::print_from_mach ( &sFrom, pMsg->pFromMach );

	cStrW sEvent;
	cKeyboard::pretty_key_event ( &sEvent, &pMsg->kbhs );

	if ( 0 == uQty )
		sBody = L"No windows were found.";

	put_ad_lib_memo ( cMemo::success, L"Command executed", L"%s\n" L"Broadcast %s to target windows on this PC.\n" L"%s", sFrom.cstr(), sEvent.cstr(), sBody.cstr()   );
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