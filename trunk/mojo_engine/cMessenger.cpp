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
//  TELL APP THAT CONNECTIONS CHANGED
//----------------------------------------------------------------------------------------------------------------------
void cMessenger :: tell_app_that_connections_changed ()
{
	PostMessage ( g_hwndApp, mojo::uWM_CONNECTIONS_CHANGED, 0, 0 );
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