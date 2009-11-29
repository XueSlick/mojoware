/***********************************************************************************************************************
/*
/*    cSyringe.cpp
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
//  PRINT
//----------------------------------------------------------------------------------------------------------------------
const wchar_t * cSyringe :: print ( cStrW * pRet, const KBDLLHOOKSTRUCT * p )
{
	cStrW sEvent;
	cKeyboard::pretty_key_event ( &sEvent, p );
	pRet->f ( L"Inject %s into windows.", sEvent.cstr() );
	return pRet->cstr();
}


//----------------------------------------------------------------------------------------------------------------------
//  POST MESSAGE (FROM HOOK DATA)
//----------------------------------------------------------------------------------------------------------------------
void cSyringe :: post_message ( HWND hwnd, WPARAM wParamHook, const KBDLLHOOKSTRUCT * p )
{
	unsigned uMsg = wParamHook;
	WPARAM wParam = p->vkCode;

	LPARAM lParam = 1;

	DWORD dwTransition = p->flags & LLKHF_UP;
	dwTransition;

	lParam |= ( p->scanCode << 16 );
	lParam |= ( ( p->flags & LLKHF_EXTENDED ) ? ( 1 << 24 ) : 0 );
	lParam |= ( ( p->flags & LLKHF_ALTDOWN  ) ? ( 1 << 29 ) : 0 );
	lParam |= ( ( p->flags & LLKHF_UP       ) ? ( 1 << 31 ) : 0 );

	PostMessage ( hwnd, uMsg, wParam, lParam );
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