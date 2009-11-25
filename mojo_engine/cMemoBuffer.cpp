/***********************************************************************************************************************
/*
/*    cMemoBuffer.cpp / mojo_engine
/*
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"

using namespace mojo;


//======================================================================================================================
//  CODE
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
//  GET MEMO
//----------------------------------------------------------------------------------------------------------------------
MOJO_ENGINE_API bool mojo::get_memo ( mojo::cMemo * pRetMemo )
{
	return g_MemoBuffer.get ( pRetMemo );
}

//----------------------------------------------------------------------------------------------------------------------
//  PUT AD LIB MEMO
//----------------------------------------------------------------------------------------------------------------------
MOJO_ENGINE_API void mojo::put_ad_lib_memo ( mojo::cMemo::_eSeverity e, const wchar_t * pHeadFormatString, const wchar_t * pBodyFormatString, ... )
{
	va_list args;
	va_start ( args, pBodyFormatString );
	cAdLibMemo m ( e, pHeadFormatString, pBodyFormatString, args );
	put_memo ( &m );
}


//----------------------------------------------------------------------------------------------------------------------
//  PUT MEMO
//----------------------------------------------------------------------------------------------------------------------
MOJO_ENGINE_API void mojo::put_memo ( mojo::cMemo::_eSeverity e, const wchar_t * pKey, ... )
{
	va_list args;
  	va_start ( args, pKey );
	cMemo m ( e, pKey, args );
	mojo::put_memo ( &m );
}


//----------------------------------------------------------------------------------------------------------------------
//  PUT MEMO
//----------------------------------------------------------------------------------------------------------------------
MOJO_ENGINE_API void mojo::put_memo ( mojo::cMemo * m )
{
	g_MemoBuffer.put ( m );
	PostMessage ( g_hwndApp, uWM_MEMO_READY, 0, 0 ); 
}

//----------------------------------------------------------------------------------------------------------------------
//  RECEIVE
//----------------------------------------------------------------------------------------------------------------------
void cMemoBuffer :: receive ( mojo::cMemo * pM )
{
	put ( pM );
}

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
