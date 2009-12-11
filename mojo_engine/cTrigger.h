/***********************************************************************************************************************
/*
/*    cTrigger.h
/*
/*    Represents the key combination that a user presses in order to trigger a hotkey.   The key combination consists
/*    of three parts:
/*
/*       -- The lock state of the keyboard (whether NumLock, CapsLock, and ScrollLock are on or off).
/*
/*       -- A chord which consists of any number of keys pressed in any order.
/*
/*       -- A main (or final) key which triggers the event, but only triggers it if the two preceeding factors
/*          are present.
/*
/*    Usually the chord is made of modifiers (shift, ctrl, and alt) but it can include any keys.
/*
/*    Internal representations:
/*   
/*       -- The lock state and modifiers are represented as bit flags in a DWORD.  The flags are in namespace
/*          nTrigger in this file.
/*
/*       -- Non-modifier chordal keys are represented as a cArrayW (tArray<WORD>) containing wExVK's (extended VKs).
/*
/*       -- The main (final) key is represented as a wExVK (extended virtual key code).
/*
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#pragma once




//======================================================================================================================
//  DATA
//======================================================================================================================

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


//======================================================================================================================
//  CLASS
//======================================================================================================================

class cKeyState;

namespace mojo
{
class MOJO_ENGINE_API cTrigger
{
public:

	cTrigger () : dwModState (0), wLastExVK (0), aMain(0) {}
	cTrigger ( const cKeyState * pKeyState );
	cTrigger & operator= ( const cTrigger & r );
	bool init ( DWORD dwLock, cArrayW * paMod, cArrayW * paMain, WORD wLastExVK );
	const wchar_t * print ( mojo::cStrW * pRet, bool bIncludeLocks = true );
	WORD ex_vk_to_vk ( WORD wExVK ) { return 0xFF & wExVK; }

	DWORD dwModState;
	WORD  wLastExVK;
	mojo::cArrayW aMain; // extra mains excluding wMainExVK

private:

	void remove_mod_from_modifier_state ( WORD wExVK );
};
} // namespace


//======================================================================================================================
//  PROTOTYPES
//======================================================================================================================


bool bang ( mojo::cTrigger * pTrigger, DWORD dwKeyboardModState );


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