/***********************************************************************************************************************
/*
/*    cLog.h / mojo_engine
/*
/*    For logging trace statements, etc., in a log file.
/*
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#pragma once

#include "tStr.h"
#include <stdio.h>
#include "cVersion.h"
#include "cMemo.h"

//=======================================================================================================
// DEFINES
//=======================================================================================================

#define WIDEN2(x) L ## x
#define WIDEN(x) WIDEN2(x)
#define __WFILE__ WIDEN(__FILE__)
#define __WDATE__ WIDEN(__DATE__)
#define __WTIME__ WIDEN(__TIME__)

namespace mojo
{

//=======================================================================================================
// PROTOTYPES
//=======================================================================================================

void log_system_error ( wchar_t *, int, wchar_t * pcText, unsigned int );


//=======================================================================================================
// MACROS
//=======================================================================================================

#define LOG(a)													\
	do															\
	{															\
		g_Log.log (  __WFILE__, __LINE__, a );					\
	__pragma ( warning ( push ) ) 								\
	__pragma ( warning ( disable : 4127 ) )						\
	} while(0) 													\
	__pragma ( warning ( pop ) )

#define LOG_TX(t,x)												\
	do															\
	{															\
		g_Log.log (  __WFILE__, __LINE__, t, x );				\
	__pragma ( warning ( push ) ) 								\
	__pragma ( warning ( disable : 4127 ) )						\
	} while(0) 													\
	__pragma ( warning ( pop ) )

#define LOG_V(t,...) 											\
	do															\
	{															\
	   g_Log.log_v ( __WFILE__, __LINE__, t, __VA_ARGS__ );		\
	__pragma ( warning ( push ) ) 								\
	__pragma ( warning ( disable : 4127 ) )						\
	} while(0) 													\
	__pragma ( warning ( pop ) )


#define LOG_SYSTEM_ERROR_TE(t,e)								\
	do															\
	{															\
		log_system_error ( __WFILE__, __LINE__, t, e );			\
	__pragma ( warning ( push ) ) 								\
	__pragma ( warning ( disable : 4127 ) )						\
	} while(0) 													\
	__pragma ( warning ( pop ) )


#define LOG_SYSTEM_ERROR_T(t)									\
	do															\
	{															\
		unsigned int dwError = GetLastError();					\
		log_system_error ( __WFILE__, __LINE__, t, dwError );	\
	__pragma ( warning ( push ) ) 								\
	__pragma ( warning ( disable : 4127 ) )						\
	} while(0) 													\
	__pragma ( warning ( pop ) )


//=======================================================================================================
//  CLASS
//=======================================================================================================

#pragma warning ( push )
#pragma warning ( disable : 4251 ) // class 'mojo::cStrW' needs to have dll-interface to be used by clients of class 'mojo::cLog'



class MOJO_ENGINE_API cLog
{
public:

	cLog ( const wchar_t * pAppFolderName, const wchar_t * pAppTitle, cVersion * pVersion );

	void log_v ( const wchar_t * pcFile, int iLine, const wchar_t * pKey, ... );

	void log ( const wchar_t * pcFile, int iLine, const mojo::cMemo * pMemo );
	void log ( const wchar_t * );
	void log ( const wchar_t * pcFile, int iLine, const wchar_t * pcTxt );
	void log_system_error ( wchar_t * pcFile, int iLine, wchar_t * pcText, unsigned int dwSystemError );
	void log ( const wchar_t * pcFile, int iLine, const wchar_t * pcTxt, unsigned int dwX );
	void print_header ( const wchar_t * p);
	void format_line ( cStrW * pRet, const wchar_t * pcFile, int iLine, const wchar_t * pcTxt );
	bool test ( cStrW * pError );
	const wchar_t * filename () { return Filename.cstr(); } // complete pathname
	void get_text ( cStrW * );

private:
	// CRITICAL_SECTION cs;

	void write ( const wchar_t * p ); // do all writing through this function

	mojo::cStrW Filename; // complete pathname
	cStrW sAppTitle;
	cVersion Version;
};

#pragma warning ( pop )

} // namepace


//=======================================================================================================
//  EXTERN DATA
//=======================================================================================================

extern mojo::cLog g_Log;


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