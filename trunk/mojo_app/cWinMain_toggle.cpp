/***********************************************************************************************************************
/*
/*    cWinMain_toggle.cpp / mojo_app
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"


//======================================================================================================================
// PROTOTYPES
//======================================================================================================================


void             toggle_sub ( bool * pbSetting, const wchar_t * pSettingName, 
                              const wchar_t * pMenuTxtOn, const wchar_t * pMenuTxtOff,
							  const wchar_t * pToolbarTxtOn, const wchar_t * pToolbarTxtOff,
                              int iCtrlID );


//======================================================================================================================
//  CODE
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
//  TOGGLE BROADCAST
//----------------------------------------------------------------------------------------------------------------------
void cWinMain :: toggle_broadcast ()
{
	g_Settings.bWindowBroadcastIsOn = g_Settings.bWindowBroadcastIsOn ? false : true;

	if ( g_Settings.bWindowBroadcastIsOn )
		g_Settings.bMouseoverIsOn = false;

	toggle_sub ();
}


//----------------------------------------------------------------------------------------------------------------------
//  TOGGLE HOTKEYS
//----------------------------------------------------------------------------------------------------------------------
void cWinMain :: toggle_hotkeys ()
{
	g_Settings.bHotkeysAreOn = g_Settings.bHotkeysAreOn ? false : true;
	toggle_sub ();
}


//----------------------------------------------------------------------------------------------------------------------
//  TOGGLE MOUSEOVER
//----------------------------------------------------------------------------------------------------------------------
void cWinMain :: toggle_mouseover ()
{
	g_Settings.bMouseoverIsOn = g_Settings.bMouseoverIsOn ? false : true;

	if ( g_Settings.bMouseoverIsOn )
		g_Settings.bWindowBroadcastIsOn = false;

	toggle_sub ();
}

                              
//----------------------------------------------------------------------------------------------------------------------
//  TOGGLE SUB
//----------------------------------------------------------------------------------------------------------------------
void cWinMain :: toggle_sub ()
{
	mojo::set ( L"bWindowBroadcastIsOn", g_Settings.bWindowBroadcastIsOn );
	mojo::set ( L"bMouseoverIsOn", g_Settings.bMouseoverIsOn );
	mojo::set ( L"bHotkeysAreOn", g_Settings.bHotkeysAreOn );
	DlgModeStrip.redraw_buttons();
	InvalidateRect ( g_hwnd, NULL, TRUE );
	UpdateWindow ( g_hwnd );
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

