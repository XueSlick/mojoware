/***********************************************************************************************************************
/*
/*    cMemo.cpp / mojo_engine
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
//  SET AD LIB
//----------------------------------------------------------------------------------------------------------------------
bool cMemo :: set_ad_lib ( _eSeverity e, const wchar_t * pHeadFormatString, const wchar_t * pBodyFormatString, va_list pArgs )
{
	eSeverity = e;
	cScribPack * pSB = static_cast<cScribPack *> ( this );
	return pSB->set_ad_lib ( pHeadFormatString, pBodyFormatString, pArgs );
}


//----------------------------------------------------------------------------------------------------------------------
//  SET AD LIB
//----------------------------------------------------------------------------------------------------------------------
bool cMemo :: set_ad_lib ( _eSeverity e, const wchar_t * pHeadFormatString, const wchar_t * pBodyFormatString, ... )
{
	va_list args;
	va_start ( args, pBodyFormatString );
	return set_ad_lib( e, pHeadFormatString, pBodyFormatString, args );
}


//----------------------------------------------------------------------------------------------------------------------
//  HEAD
//----------------------------------------------------------------------------------------------------------------------
const wchar_t * cMemo :: head ()
{
	return static_cast<cScribPack*>(this)->head.cstr();
}

//----------------------------------------------------------------------------------------------------------------------
//  BODY
//----------------------------------------------------------------------------------------------------------------------
const wchar_t * cMemo :: body ()
{
	return static_cast<cScribPack*>(this)->body.cstr();
}

//----------------------------------------------------------------------------------------------------------------------
//  KEY
//----------------------------------------------------------------------------------------------------------------------
const wchar_t * cMemo :: key ()
{
	return static_cast<cScribPack*>(this)->key.cstr();
}
//----------------------------------------------------------------------------------------------------------------------
//  SEVERITY CSTR
//----------------------------------------------------------------------------------------------------------------------
const wchar_t * cMemo :: severity_cstr ()
{
	switch ( eSeverity )
	{
	case error:
		return L"error";

	case warning:
		return L"warning";

	case info:
		return L"info";

	case success:
		return L"success";

	default:
		assert(0);
	}

	return NULL;
}

//----------------------------------------------------------------------------------------------------------------------
//  CONSTRUCTOR
//----------------------------------------------------------------------------------------------------------------------
cMemo :: cMemo ( _eSeverity e, const wchar_t * pKey, ... )
{
	va_list args;
  	va_start ( args, pKey );
	eSeverity = e;
	// cScribPack * pSP = static_cast<cScribPack*>(this);
	load_from_multimap ( pKey, args );
}


//----------------------------------------------------------------------------------------------------------------------
//  CONSTRUCTOR
//----------------------------------------------------------------------------------------------------------------------
cMemo :: cMemo ( _eSeverity e, const wchar_t * pKey, va_list pArgs ) : cScribPack ( pKey, pArgs )
{
	eSeverity = e;
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
