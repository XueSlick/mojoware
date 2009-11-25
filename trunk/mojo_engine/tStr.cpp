/********************************************************************************************************
/*
/*    tStr.cpp / mojo_engine
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

//-------------------------------------------------------------------------------------------------------
//  MEMORY DEBUG
//-------------------------------------------------------------------------------------------------------

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

//-------------------------------------------------------------------------------------------------------
//  INCLUDES
//-------------------------------------------------------------------------------------------------------
#include "tStr.h"
#include "stdlib.h"

bool replace_format_specification_fields ( wchar_t * pRet, int iSize,  const wchar_t * pIn, va_list pVA );


namespace mojo
{

//======================================================================================================================
// CODE
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
// cSTR N :: TO INT
//----------------------------------------------------------------------------------------------------------------------
int cStrN :: to_int ()
{ 
	return pBuf ? atoi ( pBuf ) : INT_MIN;
}

//----------------------------------------------------------------------------------------------------------------------
// cSTR W TO INT
//----------------------------------------------------------------------------------------------------------------------
int cStrW :: to_int ()
{ 
	return pBuf ? _wtoi ( pBuf ) : INT_MIN;
}


//-------------------------------------------------------------------------------------------------------
// FORMAT (LIKE SPRINTF)
//-------------------------------------------------------------------------------------------------------
const wchar_t * cStrW :: f ( const wchar_t * pTxt, ... )
{
	va_list args;
	va_start ( args, pTxt );
	return f ( pTxt, args );
}

//-------------------------------------------------------------------------------------------------------
// FORMAT (LIKE SPRINTF)
//-------------------------------------------------------------------------------------------------------
const wchar_t * cStrW :: f ( const wchar_t * pTxt, va_list pArgs )
{
	wchar_t t [8192];
	replace_format_specification_fields ( t, sizeof(t)/sizeof(wchar_t)-1,  pTxt, pArgs );
	*this = t;
	return pBuf;
}


//-------------------------------------------------------------------------------------------------------
// CONSTRUCTOR
// use this to convert a wide string to multibyte (narrow)
//-------------------------------------------------------------------------------------------------------
cStrN :: cStrN ( const cStrW & Rh )
{
	wchar_t * pRh = (( cStrW * ) &Rh) -> cstr(); // cast away constness

	int iRequiredBufSize = WideCharToMultiByte ( CP_ACP, 0, pRh, -1, 
						   NULL,
		                   0, // get size
						   NULL, NULL );

	pBuf = new char [ iRequiredBufSize ];
	iBufSize = iRequiredBufSize;

	iRequiredBufSize = WideCharToMultiByte ( CP_ACP, 0, pRh, -1,
		                   pBuf,
		                   iRequiredBufSize,
						   NULL, NULL );

	iLen = 0 == iRequiredBufSize ? 0 : iRequiredBufSize - 1;
}


//-------------------------------------------------------------------------------------------------------
// CONSTRUCTOR
// use this to convert a multibyte (narrow) string to wide
//-------------------------------------------------------------------------------------------------------
cStrW :: cStrW ( const cStrN & Rh )
{
	char * pRh = (( cStrN * ) &Rh) -> cstr(); // cast away constness

	int iRequiredBufSize = MultiByteToWideChar ( CP_ACP, 0, pRh, -1, 
						   NULL,
		                   0 ); // get size  );

	pBuf = new wchar_t [ iRequiredBufSize ];
	iBufSize = iRequiredBufSize;

	iRequiredBufSize = MultiByteToWideChar ( CP_ACP, 0, pRh, -1,
		                   pBuf,
		                   iRequiredBufSize );

	iLen = 0 == iRequiredBufSize ? 0 : iRequiredBufSize - 1;	
}


} // namespace

