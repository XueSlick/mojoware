/***********************************************************************************************************************
/*
/*    hooks.cpp / mojo_hooks
/*
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include <windows.h>
#include "hooks_private.h"


//======================================================================================================================
//  DATA
//======================================================================================================================

HWND g_hwndApp;

HHOOK 	s_hKeyboardHook;
HHOOK 	s_hMouseHook;

extern HMODULE g_hModule;

fKEYBOARDHOOKSERVICEROUTINE * s_pfKeyboardHookServiceRoutine;
fMOUSEHOOKSERVICEROUTINE 	* s_pfMouseHookServiceRoutine;


//======================================================================================================================
//  CODE
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
// LOW LEVEL KEYBOARD PROC
//----------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK low_level_keyboard_proc ( int nCode, WPARAM wParam, LPARAM lParam )
{
	if ( nCode < 0 )
		return CallNextHookEx ( s_hKeyboardHook, nCode, wParam, lParam );

#if 1

	bool bResult = s_pfKeyboardHookServiceRoutine ( wParam, (KBDLLHOOKSTRUCT*) lParam );

	if ( bResult )
		return CallNextHookEx ( s_hKeyboardHook, nCode, wParam, lParam );

	else
		return 1; 
#else
		return CallNextHookEx ( s_hKeyboardHook, nCode, wParam, lParam );
#endif

}


//----------------------------------------------------------------------------------------------------------------------
// LOW LEVEL MOUSE PROC
//----------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK low_level_mouse_proc ( int nCode, WPARAM wParam, LPARAM lParam )
{
	if ( nCode < 0 )
		return CallNextHookEx ( s_hMouseHook, nCode, wParam, lParam );
#if 1
	bool bResult = s_pfMouseHookServiceRoutine ( wParam, (MSLLHOOKSTRUCT*) lParam );

	if ( bResult )
		return CallNextHookEx ( s_hMouseHook, nCode, wParam, lParam );

	else
		return 1; 
#else
		return CallNextHookEx ( s_hMouseHook, nCode, wParam, lParam );
#endif
}


//----------------------------------------------------------------------------------------------------------------------
// CHECK VERSION
//----------------------------------------------------------------------------------------------------------------------
#if 0
mojo::cVersion * get_version ()
{
	return &s_Version;
}
#endif

//----------------------------------------------------------------------------------------------------------------------
// INSTALL KEYBOARD HOOK
//----------------------------------------------------------------------------------------------------------------------
bool install_keyboard_hook ( fKEYBOARDHOOKSERVICEROUTINE * pKB )
{
	if ( s_hKeyboardHook )
		return false;

	s_pfKeyboardHookServiceRoutine = pKB;

	s_hKeyboardHook = SetWindowsHookEx	( WH_KEYBOARD_LL, low_level_keyboard_proc, g_hModule, NULL );

	if ( s_hKeyboardHook )
		return true;

	else
		return false;
}


//----------------------------------------------------------------------------------------------------------------------
// INSTALL MOUSE HOOK
//----------------------------------------------------------------------------------------------------------------------
bool install_mouse_hook ( fMOUSEHOOKSERVICEROUTINE * pM )
{
	if ( s_hMouseHook )
		return false;

	s_pfMouseHookServiceRoutine = pM;

	s_hMouseHook = SetWindowsHookEx ( WH_MOUSE_LL, low_level_mouse_proc, g_hModule, NULL );

	return true;
}


//----------------------------------------------------------------------------------------------------------------------
// UNINSTALL KEYBOARD HOOK
//----------------------------------------------------------------------------------------------------------------------
bool uninstall_keyboard_hook ()
{
	if ( ! s_hKeyboardHook )
		return false;

	BOOL iResult = UnhookWindowsHookEx ( s_hKeyboardHook );

	s_hKeyboardHook = 0;

	return iResult ? true : false;
}

//----------------------------------------------------------------------------------------------------------------------
// UNINSTALL MOUSE HOOK
//----------------------------------------------------------------------------------------------------------------------
bool uninstall_mouse_hook ()
{
	if ( ! s_hMouseHook )
		return false;

	BOOL iResult = UnhookWindowsHookEx ( s_hMouseHook );

	s_hMouseHook = 0;

	return iResult ? true : false;
}


//----------------------------------------------------------------------------------------------------------------------
//  HOOK THREAD
//  This thread installs, uninstalls, and services the low-level hooks.
//----------------------------------------------------------------------------------------------------------------------
unsigned _stdcall thread ( void * pArg )
{
	sThreadArg * pTA = reinterpret_cast<sThreadArg*> ( pArg );

	g_hwndApp = pTA->hwndApp;
	s_pfMouseHookServiceRoutine    = pTA->pfM;
	s_pfKeyboardHookServiceRoutine = pTA->pfKB;

	delete pArg;

	install_keyboard_hook ( s_pfKeyboardHookServiceRoutine );
	install_mouse_hook    ( s_pfMouseHookServiceRoutine );

	MSG msg;

	//----------------------------------------------
	//  MESSAGE LOOP
	//----------------------------------------------

	while ( GetMessage ( &msg, NULL, 0, 0 ) )
	{
		switch ( msg.message )
		{
		case mojo_hooks::uWM_INSTALL_KEYBOARD_HOOK:
			install_keyboard_hook ( (fKEYBOARDHOOKSERVICEROUTINE *) msg.wParam );
			break;

		case mojo_hooks::uWM_UNINSTALL_KEYBOARD_HOOK:
			uninstall_keyboard_hook ();
			break;

		case mojo_hooks::uWM_INSTALL_MOUSE_HOOK:
			install_mouse_hook ( (fMOUSEHOOKSERVICEROUTINE *) msg.wParam );
			break;

		case mojo_hooks::uWM_UNINSTALL_MOUSE_HOOK:
			uninstall_mouse_hook ();
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
/*    license with Mojo.  If you did not, go to http://www.gnu.org.
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