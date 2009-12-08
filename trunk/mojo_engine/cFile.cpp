/********************************************************************************************************
/*
/*    cFile.cpp / mojo_engine
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

#include "stdafx.h"
#include "cFile.h"
#include "wchar.h"

using namespace mojo;

//=======================================================================================================
// PROTOTYPES
//=======================================================================================================

static FILE * open_text_file_for_reading ( const wchar_t * pwFile );
static FILE * open_text_file_for_writing ( const wchar_t * pwFile );

//=======================================================================================================
// CODE
//=======================================================================================================

//-------------------------------------------------------------------------------------------------------
// GET WHOLE THING
//-------------------------------------------------------------------------------------------------------
bool cFileIn :: get_whole_thing ( mojo::cStrW * pRet )
{
	wint_t c;
	unsigned uQtyRead = 0;

	while ( WEOF != ( c = getwc ( h ) ) )
	{
		uQtyRead ++;
		*pRet += static_cast<wchar_t> ( c );
	}

	return uQtyRead ? true : false;
}

//-------------------------------------------------------------------------------------------------------
// GET WHOLE THING 
//-------------------------------------------------------------------------------------------------------
bool cFileIn :: get_whole_thing_without_line_breaks ( mojo::cStrW * pRet )
{
	wint_t c;
	unsigned uQtyRead = 0;

	while ( WEOF != ( c = getwc ( h ) ) )
	{
		uQtyRead ++;

		if ( 0x2028 == c )  // unicode line separator
			continue;

		else if ( 0x0A == c )
			continue;

		else if ( 0x0D == c )
			continue;

		else
			*pRet += static_cast<wchar_t> ( c );
	}

	return uQtyRead ? true : false;
}


//-------------------------------------------------------------------------------------------------------
// GET LINE
//-------------------------------------------------------------------------------------------------------
bool cFileIn :: get_line ( mojo::cStrW * s )
{
	wint_t c, iQtyRead = 0;

	while ( WEOF != ( c = getwc ( h ) ) )
	{
		iQtyRead ++;

		if ( 0x2028 == c )  // unicode line separator
			break;

		else if ( 0x0A == c )
			break;

		else if ( 0x0D == c )
			;

		else
		{
			*s += static_cast<wchar_t> ( c );
		}
	}

	return iQtyRead ? true : false;
}



//-------------------------------------------------------------------------------------------------------
// CONSTRUCTOR
//-------------------------------------------------------------------------------------------------------
cFileIn :: cFileIn ( const wchar_t * pName ) : sName ( pName )
{
	h = open_text_file_for_reading ( pName );
}


//-------------------------------------------------------------------------------------------------------
// DESTRUCTOR
//-------------------------------------------------------------------------------------------------------
cFileIn :: ~cFileIn ()
{
	if ( h )
		fclose ( h );
}


//-------------------------------------------------------------------------------------------------------
// OPEN TEXT FILE FOR READING
// This will automatically open a file correctly 
// REGARDLESS OF WHETHER IT'S ANSI, UTF-8, or UTF-16
//-------------------------------------------------------------------------------------------------------
static FILE * open_text_file_for_reading ( const wchar_t * pwFile )
{
	FILE * h;

	if ( 0 != _wfopen_s ( & h, pwFile, L"rt, ccs=UTF-8" ) )
		return 0;

	else
		return h;
}



//-------------------------------------------------------------------------------------------------------
// CONSTRUCTOR
//-------------------------------------------------------------------------------------------------------
cFileOut :: cFileOut ( const wchar_t * pName ) : sName ( pName )
{
	h = open_text_file_for_writing ( pName );
}


//-------------------------------------------------------------------------------------------------------
// DESTRUCTOR
//-------------------------------------------------------------------------------------------------------
cFileOut :: ~cFileOut ()
{
	if ( h )
		fclose ( h );
}



//-------------------------------------------------------------------------------------------------------
// OPEN TEXT FILE FOR WRITING
// This will automatically open a file correctly 
// REGARDLESS OF WHETHER IT'S ANSI, UTF-8, or UTF-16
//-------------------------------------------------------------------------------------------------------
static FILE * open_text_file_for_writing ( const wchar_t * pwFile )
{
	FILE * h;

	if ( 0 != _wfopen_s ( & h, pwFile, L"wt, ccs=UTF-8" ) )
		return 0;

	else
		return h;
}