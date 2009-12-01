/********************************************************************************************************
/*
/*    cScribMgr.h / mojo_engine
/*
/*    The scrib system exists in order to provide international (multi-language) support for all
/*    of the program's displayable text (dialog box captions, error messages, etc.)
/*
/*    The scrib system also allows the program to insert variable fields into displayed text.
/*
/*    A line of displayed text is called a cScrib.  Two cScribs (a headline and a body) are a 
/*    cScribPack.  The cScribMgr is responsible for loading language-specific text for all of
/*    the program's scribs from a language-specific file.  This file can be either a wchar_t
/*    UNICODE text file or an 8-bit ANSI text file.
/*
/*    Each line of a language-specific file has the following form:
/*
/*    key = value
/*
/*    Where both key and value are text.  The ScribMgr reads these files and loads 
/*    them into a C++ multimap.
/*
/*    A value has the same format as a format string in the C-language printf function.  This 
/*    allows the program to insert fields of various types into a line of text before displaying 
/*    it.
/*
/*    This sounds fragile and risky but the code that performs the substitution is wrapped in
/*    SEH try-catch blocks so it's actually bullet proof.  Thanks to SEH, the printf
/*    interface is used throughout the program for text messages and it's completely safe.
/*
/*    That code is in replace_format_specification_fields() in utility_private.cpp.
/*
/*    When the program needs to display text, it (1) uses a key to (2) locate appropriate values
/*    in the cScribMgr's multimap, which (3) are loaded into cScribs where (4) the format 
/*    specification fields are replaced by variables.
/*
/*    The program's cException class is derived from cScribPack so the text of exception messages
/*    is handled this way too.
/*
/*    Copyright 2009 Robert Sacks
/*
/*    This file is part of Mojo.  You may redistribute and/or modify Mojo under the terms of the GNU 
/*    General Public License, version 3, as published by the Free Software Foundation.  You should have
/*    received a copy of the license with mojo.  If you did not, go to http://www.gnu.org.
/*
/*    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR 
/*    IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND 
/*    FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
/*    CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
/*    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
/*    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER 
/*    IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT 
/*    OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
/*
/********************************************************************************************************/

#pragma once

#include <map>
#include "cException.h"

namespace mojo
{

//-------------------------------------------------------------------------------------------------------
//  CLASS SCRIB MANAGER
//-------------------------------------------------------------------------------------------------------

#pragma warning ( push )
#pragma warning ( disable : 4251 ) // class 'std::map...' needs to have dll-interface to be used by clients of class...
	
class MOJO_ENGINE_API cScribMgr
{
public:
	int load_scribs 	( const wchar_t * p [], 		bool bReplace );
	int load_scribs		( const wchar_t * pPathname, 	bool bReplace );
	bool load_sub 		( const wchar_t * pLine, 		bool bReplace );
	void log            (); // for debugging

private:

	void remove ( const wchar_t * pKey );

	typedef std::multimap<std::wstring, std::wstring> tMap;
	tMap Map;

	cStrW sPrevKey; // used by load functions

	friend class cScribPack;
};

#pragma warning ( pop )

} // namespace

