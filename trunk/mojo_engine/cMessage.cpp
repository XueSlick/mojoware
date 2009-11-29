/***********************************************************************************************************************
/*
/*    cMessage.cpp
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"

using namespace mojo;


//======================================================================================================================
//  DATA
//======================================================================================================================


//======================================================================================================================
//  PROTOTYPES
//======================================================================================================================


//======================================================================================================================
//  CODE
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
//  PRINT -- BASE
//----------------------------------------------------------------------------------------------------------------------
const wchar_t * cMessage :: print ( mojo::cStrW * pRet )
{
	return pRet->cstr();
}


//----------------------------------------------------------------------------------------------------------------------
//  PRINT -- BROADCAST KEY EVENT
//----------------------------------------------------------------------------------------------------------------------
const wchar_t * cMessageBroadcastKeyEvent :: print ( mojo::cStrW * pRet )
{
#if 0
	WORD wExVK = (WORD) kbhs.vkCode + (( this->kbhs.flags & LLKHF_EXTENDED ) ? 0x100 : 0 );
	const wchar_t * pwTrans = kbhs.flags & LLKHF_UP ? L"up" : L"down" ;
	const wchar_t * pwName = mojo::cKeyboard::ex_vk_to_pretty_name(wExVK);
#endif
	mojo::cStrW sPretty;
	cKeyboard::pretty_key_event ( &sPretty, &kbhs );
	pRet->f ( L"Broadcast %s to World of Warcraft.", sPretty.cstr() );
	return pRet->cstr();
}


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


