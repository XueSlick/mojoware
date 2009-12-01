/**********************************************************************************************************************
/*
/*    cScribPack.h / mojo_engine
/*
/*    See cScribMgr.h for an explanation.
/*
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#pragma once

#include "cScrib.h"
// #include "cScribMgr.h"
#include <map>


namespace mojo
{
class cScribMgr;
//----------------------------------------------------------------------------------------------------------------------
//  CLASS SCRIB PACK
//----------------------------------------------------------------------------------------------------------------------
class MOJO_ENGINE_API cScribPack
{
public:
	
	cScribPack () {};
	cScribPack ( const wchar_t * pKey, va_list pArgs ); // keep this before constructor (...)
	cScribPack ( const wchar_t * pKey, ... );           // keep this after constructor (va_list)
	cScribPack ( const cScribPack * pRH ) { sHead = pRH->sHead; sBody = pRH->sBody; sKey = pRH->sKey; }

	const wchar_t * head () const { return sHead.cstr(); }
	const wchar_t * body () const { return sBody.cstr(); }
	const wchar_t * key  () const { return sKey.cstr(); }


protected:

	bool set_ad_lib ( const wchar_t * pHeadFormatString, const wchar_t * pBodyFormatString, va_list pArgs );
	bool load_from_multimap ( const wchar_t * pKey, va_list pArgs ); // keep this before other overload
	bool load_from_multimap ( const wchar_t * pKey, ... ); // keep thiss after other overload

	bool replace_format_specifications ( va_list pArgs );

protected:

	cScrib sHead;
	cScrib sBody;
	cScrib sKey;

	friend class cMemo;
};


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


}

