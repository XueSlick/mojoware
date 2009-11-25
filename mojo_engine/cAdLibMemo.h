/**********************************************************************************************************************
/*
/*    cAdLibMemo.h / mojo_engine
/*
/*    This kind of memo is used to enter text directly in source code without looking up a key from the scrib map.
/*    Therefore this kind of memo doesn't get translated into other languages.  Whatever you type in the source code,
/*    that exact test goes into an ad lib memo.
/*
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#pragma once

#include "cMemo.h"

namespace mojo
{

//======================================================================================================================
//  CLASS MEMO
//======================================================================================================================

class MOJO_ENGINE_API cAdLibMemo : public cMemo
{
public:
	
	cAdLibMemo ( _eSeverity e, const wchar_t * pHeadFormatString, const wchar_t * pBodyFormatString, va_list pArgs ); // keep this before constructor (...)
	cAdLibMemo ( _eSeverity e, const wchar_t * pHeadFormatString, const wchar_t * pBodyFormatString, ... );			  // keep this after constructor ( va_args )
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



