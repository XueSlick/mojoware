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
//  OPERATOR =
//----------------------------------------------------------------------------------------------------------------------
cTarget & cTarget :: operator= ( const cTarget & r )
{
	hMachHandle = r.hMachHandle;
	hwnd        = r.hwnd;
	sPath       = r.sPath;
	memcpy ( aKeyboardState, r.aKeyboardState, sizeof(aKeyboardState) );

	return *this;
}


//----------------------------------------------------------------------------------------------------------------------
//  COPY CONSTRUCTOR
//----------------------------------------------------------------------------------------------------------------------
cTarget :: cTarget ( const cTarget & r ) : hwnd ( r.hwnd ), sPath ( r.sPath ), pPrev(0), pNext(0) 
{
	memcpy ( aKeyboardState, r.aKeyboardState, sizeof(aKeyboardState) );
}


//----------------------------------------------------------------------------------------------------------------------
//  DEFAULT CONSTRUCTOR
//----------------------------------------------------------------------------------------------------------------------
cTarget :: cTarget () : pPrev(0), pNext(0), hwnd(0)
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