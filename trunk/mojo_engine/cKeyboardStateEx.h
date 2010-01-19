/***********************************************************************************************************************
/*
/*    cKeyboardStateEx.h
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


//======================================================================================================================
//  CLASS
//======================================================================================================================

class cKeyboardStateEx
{
public:

	cKeyboardStateEx ();

	void           get_key_state_as_trigger  ( mojo::cTrigger * pRet );

	static WORD    ex_vk                     ( const KBDLLHOOKSTRUCT * p ) { return (WORD) p->vkCode + ( p->flags & LLKHF_EXTENDED ? 0x100 : 0 ); }
	DWORD          mod_state                 () const;

	void           receive                   ( const KBDLLHOOKSTRUCT * p );
	bool           is_down                   ( unsigned uExVK ) { return down == ayTable[uExVK]; };

	friend mojo::cTrigger::cTrigger          ( const cKeyboardStateEx * );

	WORD wMostRecentPressedExVK;
	WORD wMostRecentExVK;
	enum eEvent { up, down };
	eEvent eMostRecentEvent;

private:



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