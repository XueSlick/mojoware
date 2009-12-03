/***********************************************************************************************************************
/*
/*    cInputEventBuffer.cpp / mojo_engine
/*
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

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
}


//---------------------------------------------------------------------------------------------
// RECEIVE -- KEYBOARD
//---------------------------------------------------------------------------------------------
void cInputEventBuffer :: receive ( WPARAM wParam, KBDLLHOOKSTRUCT * p )
{
	assert ( p->vkCode < 512 );
	mojo::cInputEvent se;
	se.u.k = *p;
	se.wParam = wParam;
	se.eType = mojo::cInputEvent::keyboard;
	Buf.put ( &se );
	PostMessage ( g_hwndApp, mojo::uWM_INPUT_EVENT_READY, 0, 0 );
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
	PostMessage ( g_hwndApp, mojo::uWM_INPUT_EVENT_READY, 0, 0 );
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