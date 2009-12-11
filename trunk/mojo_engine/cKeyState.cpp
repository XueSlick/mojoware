/***********************************************************************************************************************
/*
/*    cKeyState.cpp
/*
/*    Handles keys as extended virtual keycodes, aka wExVK's, which are
/*    virtual key codes + (if extended) 0x100
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"


using namespace nTrigger;

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
//  MOD STATE
//----------------------------------------------------------------------------------------------------------------------
DWORD cKeyState :: mod_state () const
{
	DWORD dwModState = 0;

	//----------------------------------
	// LOCKS
	//----------------------------------

	short iCapsLock = GetKeyState ( VK_CAPITAL );

	if ( iCapsLock & 1 )  // on
	{
		dwModState &= (~dwCAPSLOCKOFF);
		dwModState |=   dwCAPSLOCKON;
	}

	else
	{
		dwModState &= (~dwCAPSLOCKON);
		dwModState |=   dwCAPSLOCKOFF;
	}

	short iNumLock = GetKeyState ( VK_NUMLOCK );

	if ( iNumLock & 1 )  // on
	{
		dwModState &= (~dwNUMLOCKOFF);
		dwModState |=   dwNUMLOCKON;
	}

	else
	{
		dwModState &= (~dwNUMLOCKON);
		dwModState |=   dwNUMLOCKOFF;
	}

	short iScrollLock = GetKeyState ( VK_SCROLL );

	if ( iScrollLock & 1 )  // on
	{
		dwModState &= (~dwSCROLLLOCKOFF);
		dwModState |=   dwSCROLLLOCKON;
	}

	else
	{
		dwModState &= (~dwSCROLLLOCKON);
		dwModState |=   dwSCROLLLOCKOFF;
	}

	//----------------------------------
	// MODIFIERS
	//----------------------------------

	if ( down == ayTable [ VK_LSHIFT ] )
		dwModState |= ( dwLSHIFT | dwSHIFT );

	if ( down == ayTable [ VK_RSHIFT + 0x100 ] )
		dwModState |= ( dwRSHIFT | dwSHIFT );

	if ( ! (down==ayTable[VK_LSHIFT] || down==ayTable[VK_RSHIFT+0x100] ) )
		dwModState |= dwNSHIFT;


	if ( down == ayTable [ VK_LMENU ] )
		dwModState |= ( dwLALT | dwALT );

	if ( down == ayTable [ VK_RMENU + 0x100 ] )
		dwModState |= ( dwRALT | dwALT );

	if ( ! (down==ayTable[VK_LMENU] || down==ayTable[VK_RMENU+0x100] ) )
		dwModState |= dwNALT;


	if ( down == ayTable [ VK_LCONTROL ] )
		dwModState |= ( dwLCTRL | dwCTRL );

	if ( down == ayTable [ VK_RCONTROL + 0x100 ] )
		dwModState |= ( dwRCTRL | dwCTRL );

	if ( ! (down==ayTable[VK_LCONTROL] || down==ayTable[VK_RCONTROL+0x100] ) )
		dwModState |= dwNCTRL;

	
	return dwModState;
}




//----------------------------------------------------------------------------------------------------------------------
//  RECEIVE EVENT
//----------------------------------------------------------------------------------------------------------------------
void cKeyState :: receive ( const KBDLLHOOKSTRUCT * p )
{
	WORD wExVK = ex_vk ( p );

	if ( p->flags & LLKHF_UP )
		ayTable [ wExVK ] = up;

	else
	{
		ayTable [ wExVK ] = down;
		wMostRecentPressedExVK = wExVK;

	}

#if 0
	if ( hNotificand )
	{
		LPARAM lParam = p->flags & LLKHF_UP ? 1 : 0;
		PostMessage ( hNotificand, mojo::uWM_KEY_EVENT_OCCURRED, wExVK, lParam );
	}
#endif
}


//----------------------------------------------------------------------------------------------------------------------
//  CONSTRUCTOR
//----------------------------------------------------------------------------------------------------------------------
cKeyState :: cKeyState ()
{
	hNotificand = 0;
	wMostRecentPressedExVK = 0;
	memset ( ayTable, virgin, sizeof ( ayTable ) );
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