/***********************************************************************************************************************
/*
/*    cKeyState.h
/*
/*    Stores EXTENDED virtual key codes, i.e., virtual key code + (if extended) 0x100.
/*    Used by keyboard hook handler to set the previous keystate bit.
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#pragma once

#include "cTrigger.h"

//======================================================================================================================
//  DATA
//======================================================================================================================
#if 0
namespace nTrigger
{
	// following bit flags are used to match the trigger
	// against the keyboard DLL's keyboard state:

	const DWORD dwNSHIFT        =	1<< 0; // no shift key pressed
	const DWORD dwLSHIFT        =   1<< 1;
	const DWORD dwRSHIFT        =   1<< 2;
	const DWORD dwSHIFT         =   1<< 3;

	const DWORD dwNCTRL         =	1<< 4;
	const DWORD dwLCTRL         =   1<< 5;
	const DWORD dwRCTRL         =   1<< 6;
	const DWORD dwCTRL          =   1<< 7;

	const DWORD dwNALT          =	1<< 8;
	const DWORD dwLALT          =   1<< 9;
	const DWORD dwRALT          =   1<<10;
	const DWORD dwALT           =   1<<11;

	const DWORD dwNUMLOCKON		=   1<<26;
	const DWORD dwNUMLOCKOFF	=   1<<27;
	const DWORD dwCAPSLOCKON	=   1<<28;
	const DWORD dwCAPSLOCKOFF	=   1<<29;
	const DWORD dwSCROLLLOCKON	=   1<<30;
	const DWORD dwSCROLLLOCKOFF	=   (DWORD) 1<<31;
}
#endif

//======================================================================================================================
//  CLASS
//======================================================================================================================

class cKeyState
{
public:

	cKeyState ();

	void           get_key_state_as_trigger  ( mojo::cTrigger * pRet );



	static WORD    ex_vk                     ( const KBDLLHOOKSTRUCT * p ) { return (WORD) p->vkCode + ( p->flags & LLKHF_EXTENDED ? 0x100 : 0 ); }
	DWORD          mod_state                 () const;

	void           receive                   ( const KBDLLHOOKSTRUCT * p );
	bool           is_up                     ( unsigned uExVK ) { return up   == ayTable[uExVK]; };
	bool           is_down                   ( unsigned uExVK ) { return down == ayTable[uExVK]; };

	friend mojo::cTrigger::cTrigger          ( const cKeyState * );

private:

	WORD wMostRecentPressedExVK;
	enum { virgin, up, down } eEvent;
	unsigned char ayTable [ 512 ];
	HWND hNotificand;
};


//----------------------------------------------------------------------------------------------------------------------
//  
//----------------------------------------------------------------------------------------------------------------------


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