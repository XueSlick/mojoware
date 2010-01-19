/***********************************************************************************************************************
/*
/*    cFigTrigger.cpp / mojo_app
/*   
/*    Copyright 2010 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"

//======================================================================================================================
//  DATA
//======================================================================================================================

const cFigTrigger cFigTrigger :: Default;

//======================================================================================================================
//  PROTOTYPES
//======================================================================================================================

//======================================================================================================================
//  CODE
//======================================================================================================================



//----------------------------------------------------------------------------------------------------------------------
//  SET FROM XML
//----------------------------------------------------------------------------------------------------------------------
void cFigTrigger :: set_from_xml ( void * pvDest, const wchar_t * pTxt ) const
{
	cStrW * pDest = reinterpret_cast<cStrW*>(pvDest);
	*pDest = pTxt;
}


//----------------------------------------------------------------------------------------------------------------------
//  WRITE TO XML
//----------------------------------------------------------------------------------------------------------------------
void cFigTrigger:: write_to_xml ( mojo::cStrW * pRet, void * pObject, const wchar_t * pTagName ) const
{
	assert ( pObject );
	cStrW * pSource = reinterpret_cast<cStrW*>(pObject);
	if ( 0 == pSource->len() )
		return;
	xml_write_start_tag ( pRet, pTagName );
	*pRet += *pSource;
	xml_write_end_tag ( pRet, pTagName );
	*pRet += L'\n';
}


//----------------------------------------------------------------------------------------------------------------------
//  FIG TO CTRL -- STRING
//----------------------------------------------------------------------------------------------------------------------
void cFigTrigger :: fig_to_ctrl ( HWND hwnd, void * pVar ) const
{
	cStrW * pStr = (cStrW*) pVar;
	SetWindowText ( hwnd, pStr->cstr() );
}


//----------------------------------------------------------------------------------------------------------------------
//  CTRL TO FIG
//----------------------------------------------------------------------------------------------------------------------
void cFigTrigger :: ctrl_to_fig ( void * pVar, HWND hwnd ) const
{
	wchar_t b [ 8092 ];
	Edit_GetText ( hwnd, b, sizeof(b)/sizeof(wchar_t) );
	*(cStrW*)pVar = b;
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