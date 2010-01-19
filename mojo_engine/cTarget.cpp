/***********************************************************************************************************************
/*
/*    cTarget.cpp / mojo_engine
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
//  SERIALIZE
//----------------------------------------------------------------------------------------------------------------------
void cTarget :: serialize ( mojo::cStrW * pRet ) const
{
	wchar_t b[100];

	pRet->append ( dword_to_string ( b, sizeof(b)/sizeof(wchar_t), (DWORD) this->hwnd ) );
	*pRet += wchar_t(0);
	pRet->append ( dword_to_string ( b, sizeof(b)/sizeof(wchar_t), (DWORD) this->dwProcessID ) );
	*pRet += wchar_t(0);
	pRet->append ( dword_to_string ( b, sizeof(b)/sizeof(wchar_t), (DWORD) this->dwID ) );
	*pRet += wchar_t(0);
	pRet->append ( this->bLaunchByMojo ? L"T" : L"F" );
	*pRet += wchar_t(0);
	pRet->append ( this->bIsRunning ? L"T" : L"F" );
	*pRet += wchar_t(0);
	pRet->append ( this->sName.cstr() );
	*pRet += wchar_t(0);
	pRet->append ( this->sPath.cstr() );
	*pRet += wchar_t(0);
}

//----------------------------------------------------------------------------------------------------------------------
//  DESERIALIZE
//----------------------------------------------------------------------------------------------------------------------
size_t cTarget :: deserialize ( const wchar_t * a )
{
	wchar_t * p = const_cast<wchar_t*>(a);

	this->hwnd = (HWND) wcstoul ( p, &p, 16 );
	p ++;

	this->dwProcessID = wcstoul ( p, &p, 16 );
	p++;

	this->dwID = wcstoul ( p, &p, 16 );
	p++;

	this->bLaunchByMojo = ( *p == L'T' ? true : false );
	p += 2;

	this->bIsRunning = ( *p == L'T' ? true : false );
	p += 2;

	this->sName = p;
	p += sName.len();
	p++;

	this->sPath = p;
	p += sPath.len();
	p++;

	return p - a;
}

//----------------------------------------------------------------------------------------------------------------------
//  OPERATOR =
//----------------------------------------------------------------------------------------------------------------------
cTarget & cTarget :: operator= ( const cTarget & r )
{
	hwnd          = r.hwnd;
	sPath         = r.sPath;
	sName         = r.sName;
	hMach         = r.hMach;
	bLaunchByMojo = r.bLaunchByMojo;
	dwProcessID   = r.dwProcessID;
	dwID          = r.dwID;
	bIsRunning      = r.bIsRunning;
	memcpy ( aKeyboardState, r.aKeyboardState, sizeof(aKeyboardState) );

	return *this;
}


//----------------------------------------------------------------------------------------------------------------------
//  COPY CONSTRUCTOR
//----------------------------------------------------------------------------------------------------------------------
cTarget :: cTarget ( const cTarget & r ) : 
	hwnd ( r.hwnd ), 
	sPath ( r.sPath ),
	sName ( r.sName ),
	pPrev(0), 
	pNext(0), 
	hMach(r.hMach), 
	bLaunchByMojo(r.bLaunchByMojo),
	dwProcessID ( r.dwProcessID ),
	dwID ( r.dwID ),
	bIsRunning ( r.bIsRunning )
{
	memcpy ( aKeyboardState, r.aKeyboardState, sizeof(aKeyboardState) );
}


//----------------------------------------------------------------------------------------------------------------------
//  DEFAULT CONSTRUCTOR
//----------------------------------------------------------------------------------------------------------------------
cTarget :: cTarget () : 
	hwnd (0), 
	pPrev(0), 
	pNext(0), 
	hMach(0), 
	bLaunchByMojo(false),
	dwProcessID(0),
	dwID(0),
	bIsRunning(false)
{
	memset ( aKeyboardState, 0, sizeof(aKeyboardState ) );
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