/***********************************************************************************************************************
/*
/*    hooks_public.cpp / mojo_hooks
/*
/*    The functions in this file are exported and are called by mojo_engine.
/*
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include <windows.h>
#include "hooks_private.h"
#include "..//mojo_app//global_data_version.h"


#include <process.h>

using namespace mojo_hooks;

DWORD g_dwHookThreadID = 0;

using namespace mojo;



//======================================================================================================================
//  CODE
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
//  GET VERSION
//----------------------------------------------------------------------------------------------------------------------
void mojo_hooks::get_version ( unsigned *pMaj, unsigned *pMin, unsigned *pPip, unsigned * pBld )
{
	*pMaj = uVERSION_MAJ;
	*pMin = uVERSION_MIN;
	*pPip = uVERSION_PIP;
	*pBld = uVERSION_BLD;
}


//----------------------------------------------------------------------------------------------------------------------
//  START THREAD
//----------------------------------------------------------------------------------------------------------------------
DWORD mojo_hooks::start_thread ( HWND hwndApp, fKEYBOARDHOOKSERVICEROUTINE * pfKB, fMOUSEHOOKSERVICEROUTINE * pfM  )
{
	sThreadArg * pArg 	= new sThreadArg; // will get deleted by receiving function
	pArg->hwndApp 		= hwndApp;
	pArg->pfKB 			= pfKB;
	pArg->pfM  			= pfM;
	_beginthreadex (  NULL, 0, &thread, reinterpret_cast<void*>( pArg ), 0, (UINT *)(&g_dwHookThreadID) );
	return g_dwHookThreadID;
}


//----------------------------------------------------------------------------------------------------------------------
//  INSTALL MOUSE HOOK
//  called from non-hook thread
//----------------------------------------------------------------------------------------------------------------------
bool mojo_hooks::install_keyboard_hook ( fKEYBOARDHOOKSERVICEROUTINE * pF )
{
	if ( 0 == g_dwHookThreadID )
		return false;

	PostThreadMessage ( g_dwHookThreadID, mojo_hooks::uWM_INSTALL_KEYBOARD_HOOK, (WPARAM) pF, 0 );
	return true;
}


//----------------------------------------------------------------------------------------------------------------------
//  INSTALL MOUSE HOOK
//  called from non-hook thread
//----------------------------------------------------------------------------------------------------------------------
bool mojo_hooks::install_mouse_hook ( fMOUSEHOOKSERVICEROUTINE * pF )
{
	if ( 0 == g_dwHookThreadID )
		return false;

	PostThreadMessage ( g_dwHookThreadID, mojo_hooks::uWM_INSTALL_MOUSE_HOOK, (WPARAM) pF, 0 );
	return true;
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