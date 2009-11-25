/**********************************************************************************************************************
/*
/*    cMemo.h / mojo_engine
/*
/*    A memo is a text message sent by the engine to the app.  The text can (and probably should) be displayed
/*    to the user.    In addition to text, a memo contains two other fields that the app can test for and react to.
/*
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#pragma once

#include "cScribPack.h"
#include <map>


namespace mojo
{

//======================================================================================================================
//  CLASS MEMO
//======================================================================================================================

class MOJO_ENGINE_API cMemo : public cScribPack
{
public:

	enum _eSeverity { success, error, warning, info } eSeverity;	
	cMemo () : eSeverity ( info ) {}
	cMemo ( _eSeverity e, const wchar_t * pKey, va_list pArgs );
	cMemo ( _eSeverity e, const wchar_t * pKey, ... );
	
	bool set_ad_lib ( _eSeverity e, const wchar_t * pHeadFormatString, const wchar_t * pBodyFormatString, va_list pArgs );
	bool set_ad_lib ( _eSeverity e, const wchar_t * pHeadFormatString, const wchar_t * pBodyFormatString, ... );
	const wchar_t * severity_cstr ();
	const wchar_t * head (); // { return head.cstr(); }
	const wchar_t * body (); // { return body.cstr(); }
	const wchar_t * key  (); // { return key.cstr(); }
};

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



