#pragma once
/***********************************************************************************************************************
/*
/*    cKeyBroadcaster.h
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#pragma once

#include "tArray.h"
#include "cTarget.h"
#include "tList.h"

//======================================================================================================================
//  CLASS
//======================================================================================================================

class cKeyBroadcaster
{
public:

	void inject_key_event		    ( const mojo::cTarget * pTarget, WPARAM wParam, KBDLLHOOKSTRUCT * p );
	void receive_from_keyboard_hook ( WPARAM wParam,  KBDLLHOOKSTRUCT * p );	
	void broadcast_to_local_windows ( const cMessageBroadcastKeyEvent * pMsg, HWND hExcludeThisWindow = 0 );
	// void get_broadcast_targets      ( mojo::cArrayTarget * pRet );
	// void receive_from_finder        ( mojo::cArrayTarget * p );

private:

	// mojo::cTarget * find_hwnd_in_list ( HWND hwnd );
	// bool hwnd_is_in_array ( mojo::cArrayTarget * pRay, HWND hwnd );
	// mojo::tList2<mojo::cTarget> List;

};


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

