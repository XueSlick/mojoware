/***********************************************************************************************************************
/*
/*    cVersion.h / mojo_engine
/*
/*    This class represents the program's version number.  There are four digits.
/*
/*    It's defined inline so it can be included in all projects in the solution without the DLLs'
/*    becoming mutually dependent.  Edit:   On second thought, this file is no longer included in 
/*    all the projects, so inlining is no longer necessary, but I'm too lazy to move the functions
/*    back into a .cpp file.
/*
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#pragma once

#include "..\\mojo_engine\\tStr.h"
#include <regex>

namespace mojo
{

//----------------------------------------------------------------------------------------------------------------------
// CLASS VERSION
//----------------------------------------------------------------------------------------------------------------------
class cVersion
{
	public:

		//-------------------------------------------
		// CONSTRUCTORS
		//-------------------------------------------
		cVersion ( unsigned M, unsigned m, unsigned p, unsigned b ) : uMaj(M), uMin(m), uPip(p), uBld(b) {}
		cVersion () : uMaj(0),uMin(0),uPip(0), uBld(0) {}

		//-------------------------------------------
		// GET BUILD
		//-------------------------------------------
		unsigned build () { return uBld; }
		
		//-------------------------------------------
		// OPERATOR EQUAL
		//-------------------------------------------
		bool operator== ( const cVersion & r )
		{
			return ( uMaj == r.uMaj &&
				     uMin == r.uMin &&
					 uPip == r.uPip &&
					 uBld == r.uBld );
		}

		//-------------------------------------------
		// OPERATOR UNEQUAL
		//-------------------------------------------
		bool operator!= ( const cVersion & rh ) { return ! ( *this == rh ); };

		//-------------------------------------------
		// GET TEXT
		// i.e. "0.1.29 build 104";
		//-------------------------------------------
		const wchar_t * get_text ( cStrW * pRet ) const  	
		{
			assert ( pRet );
			wchar_t t [1000];
			swprintf ( t, sizeof(t)/sizeof(wchar_t), L"%u.%u.%u build %u", uMaj, uMin, uPip, uBld );
			*pRet = t;
			return pRet->cstr();
		}

		//-------------------------------------------
		// SET TEXT
		// i.e. "0.1.29 build 104"
		//-------------------------------------------
		bool set_from_text ( const wchar_t * pSig )
		{
			std::tr1::wregex  rgx ( L"(\\d+).(\\d+).(\\d+) build (\\d+)" );
			std::tr1::wcmatch bmatch;

			if ( std::tr1::regex_match ( pSig, bmatch, rgx ) )
			{
				uMaj = _wtoi ( static_cast<std::wstring>(bmatch[1]).c_str() );
				uMin = _wtoi ( static_cast<std::wstring>(bmatch[2]).c_str() );
				uPip = _wtoi ( static_cast<std::wstring>(bmatch[3]).c_str() );
				uBld = _wtoi ( static_cast<std::wstring>(bmatch[4]).c_str() );

				return true;
			}
			return false;
		}

		//-------------------------------------------
		// DATA
		//-------------------------------------------
		unsigned uMaj;
		unsigned uMin;
		unsigned uPip;
		unsigned uBld;

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






