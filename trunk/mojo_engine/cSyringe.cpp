/***********************************************************************************************************************
/*
/*    cSyringe.cpp
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
//  PRINT
//----------------------------------------------------------------------------------------------------------------------
const wchar_t * cSyringe :: print ( cStrW * pRet, const KBDLLHOOKSTRUCT * p )
{
	cStrW sEvent;
	cKeyboard::pretty_key_event ( &sEvent, p );
	pRet->f ( L"Inject %s into windows.", sEvent.cstr() );
	return pRet->cstr();
}


//----------------------------------------------------------------------------------------------------------------------
//  SEND INPUT (KEYBOARD INPUT FROM HOOK DATA)
//----------------------------------------------------------------------------------------------------------------------
void cSyringe :: send_input ( WPARAM wParamHook, const KBDLLHOOKSTRUCT * p )
{	
	wParamHook;
	p;
}

//----------------------------------------------------------------------------------------------------------------------
//  SEND NOTIFY MESSAGE
//
//  We maintain our own keyboard state for each target window.  We use it to convert key strokes into characters
//  with the system's ToAscii function.  Then we synthesize and send both WM_KEY* and WM_CHAR messages with
//  SendNotifyMessage.  This procedure bypasses the normal mechanism by which the target receives keystroke messages
//  in its posted message queue and then translates them into characters by calling TranslateMessage with the 
//  system-provided keyboard state.
//
//  The reason for replacing the normal procedure is that the system-provided keyboard state doesn't know which
//  keys the target has received via our injections.   It would be simpler to update the system-provided keyboard 
//  state, but unfortunately, in order to do that, we have to use a thread which is permanently attached to the 
//  target's GUI thread.   This would require us to maintain a dedicated thread for each target and switch to that
//  thread every time we inject which would be a pretty severe performance hit.
//----------------------------------------------------------------------------------------------------------------------
void cSyringe :: send_notify_message ( HWND hwnd, WPARAM uMsg, const KBDLLHOOKSTRUCT * p, BYTE aKeyboardState [256] )
{
	LPARAM lParam = 1;
	lParam |= ( p->scanCode << 16 );
	lParam |= ( ( p->flags & LLKHF_EXTENDED )  ? ( 1 << 24 ) : 0 ); // extended
	lParam |= ( ( p->flags & LLKHF_ALTDOWN  )  ? ( 1 << 29 ) : 0 ); // context code
	lParam |= ( ( p->dwExtraInfo & ( 1<<30 ) ) ? ( 1 << 30 ) : 0 ); // previous key state; our keyboard hook handler inserted this bit
	lParam |= ( ( p->flags & LLKHF_UP       )  ? ( 1 << 31 ) : 0 ); // transition state

	//---------------------------------------
	//  SEND WM_KEY* OR WM_SYSKEY* MESSAGE
	//---------------------------------------

	SendNotifyMessage ( hwnd, uMsg, (WPARAM) p->vkCode, lParam );

	//---------------------------------------
	//  IF APPROPRIATE, SEND WM_CHAR MESSAGE
	//
	//  To do:  ToAscii may return two
	//  chars.
	//
	//  To do:  maybe Alt-Numpad-release-Alt
	//  keystrokes can be detected here.
	//---------------------------------------

	if ( ! ( p->flags & LLKHF_UP ) )
	{
		WORD wChar = 0;
		int iQty = ToAscii ( p->vkCode, p->scanCode, aKeyboardState, &wChar, 0 );

		if ( iQty )
			SendNotifyMessage ( hwnd, WM_CHAR, (WPARAM) wChar, lParam );		
	}

	//---------------------------------------------
	// ADJUST THIS TARGET'S PRIVATE KEYBOARD STATE
	//---------------------------------------------

	if ( p->flags & LLKHF_UP )
		aKeyboardState[p->vkCode] &= ~128;

	else
		aKeyboardState[p->vkCode] |= 128;

	if ( 0 == aKeyboardState[VK_LMENU] && 0 == aKeyboardState[VK_RMENU] )
		aKeyboardState[VK_MENU] = 0;

	else
		aKeyboardState[VK_MENU] = 128;

	if ( 0 == aKeyboardState[VK_LCONTROL] && 0 == aKeyboardState[VK_RCONTROL] )
		aKeyboardState[VK_CONTROL] = 0;

	else
		aKeyboardState[VK_CONTROL] = 128;

	if ( 0 == aKeyboardState[VK_LSHIFT] && 0 == aKeyboardState[VK_RSHIFT] )
		aKeyboardState[VK_SHIFT] = 0;

	else
		aKeyboardState[VK_SHIFT] = 128;
}


////// DEAD CODE ///////////////////////////////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------------------------------------------------
//  POST MESSAGE (FROM HOOK DATA)
//----------------------------------------------------------------------------------------------------------------------
#if 0
void cSyringe :: post_message ( HWND hwnd, WPARAM wParamHook, const KBDLLHOOKSTRUCT * p )
{
	//---------------------------------------------
	// IF THIS IS A SHIFT KEY, CHANGE SHIFT STATE
	//---------------------------------------------

#if 1
	int iResult = AttachThreadInput ( GetCurrentThreadId(), GetWindowThreadProcessId ( hwnd, NULL ), TRUE );
	iResult;

	BYTE KeyboardState [256];
	memset ( &KeyboardState, 0, 256 );
	KeyboardState [ VK_SHIFT ] = 128;
	KeyboardState [ VK_LSHIFT ] = 128;

	SetKeyboardState ( KeyboardState );
#endif


	unsigned uMsg = wParamHook;
	WPARAM wParam = p->vkCode;

	LPARAM lParam = 1;

	DWORD dwTransition = p->flags & LLKHF_UP;
	dwTransition;

	lParam |= ( p->scanCode << 16 );
	lParam |= ( ( p->flags & LLKHF_EXTENDED )  ? ( 1 << 24 ) : 0 ); // extended
	lParam |= ( ( p->flags & LLKHF_ALTDOWN  )  ? ( 1 << 29 ) : 0 ); // context code
	lParam |= ( ( p->dwExtraInfo & ( 1<<30 ) ) ? ( 1 << 30 ) : 0 ); // previous key state; our keyboard hook handler inserted this bit
	lParam |= ( ( p->flags & LLKHF_UP       )  ? ( 1 << 31 ) : 0 ); // transition state

	PostMessage ( hwnd, uMsg, wParam, lParam );

#if 0
	iResult = AttachThreadInput ( GetCurrentThreadId(), GetWindowThreadProcessId ( hwnd, NULL ), FALSE );
#endif
}
#endif

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