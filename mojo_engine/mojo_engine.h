/***********************************************************************************************************************
/*
/*    mojo_engine.h / mojo_engine
/*
/*    This file declares the DLL's exported interface.   Include this file in any program
/*    that uses this DLL.
/*
/*    Your program can use any function on this page.
/*
/*    Your program can also use any MOJO_ENGINE_API class in the included header files.
/*
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#pragma once

#ifndef MOJO_ENGINE_API
#ifdef MOJO_DLL_EXPORTS
#define MOJO_ENGINE_API __declspec(dllexport)
#else
#define MOJO_ENGINE_API __declspec(dllimport)
#endif
#endif

//======================================================================================================================
//  OTHER HEADER FILES
//======================================================================================================================

#include "tStr.h"
#include "tPt.h"
#include "regex.h"
#include "tRect.h"
#include "cVersion.h"
#include "files.h"
#include "tList.h"
#include "tList2.h"
#include "tArray2.h"
#include "tCircBuf.h"
#include "cLog.h"
#include "cScrib.h"
#include "cScribPack.h"
#include "cScribMgr.h"
#include "cException.h"
#include "cInputEvent.h"
#include "cKeyboard.h"
#include "utility.h"
#include "cMemo.h"
#include "cAdLibMemo.h"
#include "cSettingsBase.h"
#include "set.h"
#include "cDisplay.h"
#include "cMach.h"
#include "cMachlist.h"
#include <vector>



namespace mojo
{
//======================================================================================================================
//  DATA
//======================================================================================================================

const UINT uWM_INPUT_EVENT_READY 		= WM_APP + 0xBFFF - 0;
const UINT uWM_MEMO_READY 				= WM_APP + 0xBFFF - 1;
const UINT uWM_CONNECTIONS_CHANGED		= WM_APP + 0xBFFF - 2;

namespace nConnectionStatus
{
	enum eConnectionStatus { none=0, server=1, client=2, both=3 };
}


//======================================================================================================================
//  PROTOTYPES
//======================================================================================================================

MOJO_ENGINE_API void test					();	

MOJO_ENGINE_API void get_engine_version   ( cVersion * pRetVersion );
MOJO_ENGINE_API bool load_engine_settings	( const wchar_t * pAppDataDirectory );
MOJO_ENGINE_API bool initialize_engine 	( HINSTANCE hAppInstance, 
										  HWND hwndAppMessageLoop, 
										  const wchar_t * apScribs [],
										  const wchar_t * pScribPathname [],
	                                      const wchar_t * pAppDataDirectory, 
										  const cVersion * pVersionRequiredByApp );
MOJO_ENGINE_API void shut_down_engine 	();
MOJO_ENGINE_API bool get_input_event 		( mojo::cInputEvent * pRetInputEvent );
MOJO_ENGINE_API void put_memo				( mojo::cMemo::_eSeverity e, const wchar_t * pKey, ... );
MOJO_ENGINE_API void put_memo 			( mojo::cMemo * pMemo );
MOJO_ENGINE_API void put_ad_lib_memo		( mojo::cMemo::_eSeverity e, const wchar_t * pHeadFormatString, const wchar_t * pBodyFormatString=NULL, ... );
MOJO_ENGINE_API bool get_memo				( mojo::cMemo * pRetMemo );

MOJO_ENGINE_API int load_scribs			( const wchar_t * apScribs [],  bool bReplace );
MOJO_ENGINE_API int load_scribs			( const wchar_t * pPathname,	bool bReplace );
MOJO_ENGINE_API bool get_ip_addresses 	( mojo::cArrayU * pRet );
MOJO_ENGINE_API void ip_dword_to_cStrW	( cStrW * pRet, DWORD dw );
MOJO_ENGINE_API DWORD ip_pw_to_dword		( const wchar_t * );
MOJO_ENGINE_API void  get_machlist		( mojo::cMachlist * pRet );
MOJO_ENGINE_API mojo::nConnectionStatus::eConnectionStatus 
				get_connection_status 	( DWORD dwMachHandle );



} // namespace

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