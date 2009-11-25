/********************************************************************************************************
/*
/*    hooks.h / mojo_hooks
/*
/*    This file declares functions used privately (not exported) by hooks.dll.  It's
/*    done this way to to ensure that the "hook thread" is the only thread that installs
/*    hooks.
/*
/*    Copyright 2009 Robert Sacks
/*
/*    This file is part of Mojo.  You may redistribute and/or modify Mojo under the terms of the GNU 
/*    General Public License, version 3, as published by the Free Software Foundation.  You should have
/*    received a copy of the license with Mojo.  If you did not, go to http://www.gnu.org.
/*
/*    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR 
/*    IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND 
/*    FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
/*    CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
/*    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
/*    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER 
/*    IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT 
/*    OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
/*
/********************************************************************************************************/

#pragma once

//---------------------------------
//  MEMORY DEBUG (in this order)
//---------------------------------

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <stdio.h>

//---------------------------------
//  INCLUDES
//---------------------------------

#include "hooks_public.h"

//=======================================================================================================
//  DATA
//=======================================================================================================

extern HWND 						g_hwndMainDLL;
// extern mojo::cVersion			g_Version;

struct sThreadArg
{
	HWND hwndApp;
	fKEYBOARDHOOKSERVICEROUTINE * 	pfKB;
	fMOUSEHOOKSERVICEROUTINE 	* 	pfM;
};


//=======================================================================================================
//  PROTOTYPES
//=======================================================================================================

bool install_keyboard_hook 		( fKEYBOARDHOOKSERVICEROUTINE * pKB );
bool uninstall_keyboard_hook 	();
bool install_mouse_hook 		( fMOUSEHOOKSERVICEROUTINE * pM );
bool uninstall_keyboard_hook 	();
unsigned _stdcall thread 		( void * pArg );
