/***********************************************************************************************************************
/*
/* 	  stdafx.h / mojo_engine
/*
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/


#pragma once



//-------------------------------------------------------------------------------------------------------
// INCLUDES
//-------------------------------------------------------------------------------------------------------

//---------------------------------
//  MEMORY DEBUG (in this order)
//---------------------------------

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

//---------------------------------
//  WINDOWS
//---------------------------------

#define WIN32_LEAN_AND_MEAN

#include "targetver.h"
#include <windows.h>
#include <winsock2.h> // for SOCKET

//---------------------------------
//  C RUNTIME LIBRARY
//---------------------------------

#include <stdio.h>
#include <string>
#include <map>

//---------------------------------
//  DLL EXPORTS
//---------------------------------

#include "mojo_engine.h"


//---------------------------------
//  DLL IMPORTS
//---------------------------------

#include "..\\mojo_hooks\\hooks_public.h"

//---------------------------------
//  EMGINE HEADERS
//---------------------------------

#include "cSettings.h"
#include "cFile.h"
#include "hooks.h"
#include "cInputEventBuffer.h"
#include "cFinder.h"
#include "cSignature.h"
#include "cPool.h"
#include "tArray.h"
#include "cMessenger.h"
#include "net_stuff.h"
#include "cMemoBuffer.h"
#include "utility_private.h"
#include "cKeyBroadcaster.h"
#include "cSyringe.h"
#include "cPreviousKeyState.h"
#include "cDisplay.h"
#include "cMouseover.h"


//---------------------------------
//  GLOBAL DATA (KEEP THIS LAST)
//---------------------------------

#include "global_data.h" // keep this last)



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






