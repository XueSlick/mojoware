/***********************************************************************************************************************
/*
/*    cMessenger.h / mojo_engine
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#pragma once

#include "cMessage.h"

//======================================================================================================================
//  DATA
//======================================================================================================================

extern HWND g_hwndApp;
class mojo::cMach;

//----------------------------------------------------------------------------------------------------------------------
//   CLASS Messenger
//----------------------------------------------------------------------------------------------------------------------

class cMessenger
{
public:

	cMessenger             () : hwnd(0), hwndSwallow(0), hKeyEventNotificand(0) {}

	static bool keyboard_hook_service_routine ( WPARAM wParam, KBDLLHOOKSTRUCT * p );
	static bool mouse_hook_service_routine    ( WPARAM wParam, MSLLHOOKSTRUCT * p );

	void send                     ( DWORD hMachHandle, cMessage * pMessage );
	void receive                  ( struct sSocketInfo * pSI, const char * pBuffer, unsigned uLen );
	static const wchar_t * print_from_mach ( mojo::cStrW * pRet, const mojo::cMach * pMach );
	void put_receive_memo         ( cMessage * pMsg, const wchar_t * pBody2 );
	static void broadcast_message ( cMessage * pMsg );
	static void send_message      ( mojo::cMach * pMach, cMessage * pMsg );

	static const UINT uWM_SOCKET_HAS_CLOSED = WM_USER + 0;

	static void tell_app_that_machlist_changed ();
	static void tell_app_that_broadcast_targets_changed ();

	void start_swallowing_key_events ( HWND hwndSwallow );
	void stop_swallowing_key_events  ();

	void register_for_key_events     ( HWND hNotifyMe );
	void unregister_for_key_events   ( HWND hNotifyMe );

	DWORD start_thread 	();
	DWORD dwThreadID;
	HWND hwnd;
	HWND hwndSwallow; // swallow keystrokes if this window has focus

private:
	HWND hKeyEventNotificand;
	static unsigned _stdcall thread ( void * pArg );
};


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