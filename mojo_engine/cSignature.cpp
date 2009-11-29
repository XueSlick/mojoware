/***********************************************************************************************************************
/*
/*    cSignature.cpp / mojo_engine
/*
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"


//======================================================================================================================
// DATA
//======================================================================================================================

struct sSigHead
{
	wchar_t          awEngineTitle [ sizeof ( awENGINE_TITLE ) / sizeof ( wchar_t ) ];
	mojo::cVersion   Version;
	int              iAppTitleOffset;    // from start of sig
	int              iHostNameOffset;    // from start of sig
	int              iDisplayListOffset; // from start of sig
	int              iSize;              // in bytes
};


//======================================================================================================================
// CODE
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
//  CONSTRUCTOR
//----------------------------------------------------------------------------------------------------------------------
cSignature :: cSignature () : pBuf(NULL)
{
	//-----------------------------------
	// GET COMPONENTS
	//-----------------------------------

	mojo::cStrW sHostName, sDisplayList;
	get_full_dns_name ( &sHostName );
	mojo::cDisplayList DL;
	DL.set_from_local_hardware();
	DL.text ( &sDisplayList );

	//-----------------------------------
	// ALLOCATE BUFFER
	//-----------------------------------

	iLen =	sizeof ( sSigHead )
		    + 2 * g_sAppTitle.len()    + 2       // app title with terminating NULL
			+ 2 * sHostName.len()      + 2	     // host name with terminating NULL
			+ 2 * ( sDisplayList.len() + 2 );	 // display list with terminating NULL

	//-----------------------------------
	// COPY FIXED-LENGTH DATA
	//-----------------------------------

	this->pBuf = new BYTE [ iLen ];
	sSigHead * pHead = reinterpret_cast<sSigHead *> (this->pBuf);

	wcscpy_s ( pHead->awEngineTitle, sizeof(awENGINE_TITLE), awENGINE_TITLE );
	pHead->Version = g_Version;
	pHead->iSize = iLen;

	//-----------------------------------
	// COPY VARIABLE-LENGTH DATA
	//-----------------------------------

	BYTE * p = pBuf + sizeof(sSigHead);

	pHead->iAppTitleOffset = p - pBuf;
	wcscpy_s ( (wchar_t*)(p), g_sAppTitle.len() + 1, g_sAppTitle.cstr() );
	p += ( 2 * g_sAppTitle.len() + 2 );

	pHead->iHostNameOffset = p - pBuf;
	wcscpy_s ( (wchar_t*)(p), sHostName.len() + 1, sHostName.cstr() );
	p += 2 * sHostName.len() + 2;

	pHead->iHostNameOffset = p - pBuf;
	wcscpy_s ( (wchar_t*)(p), sHostName.len() + 1, sHostName.cstr() );
	p += 2 * sHostName.len() + 2;

	// TO DO:  ADD DISPLAY LIST
}



//----------------------------------------------------------------------------------------------------------------------
//  PARSE
//----------------------------------------------------------------------------------------------------------------------

bool cSignature :: parse ( const wchar_t ** ppRetDisplayList, const wchar_t ** ppRetHostName, const wchar_t ** ppRetAppTitle, mojo::cVersion ** ppRetVersion, const char * pMsg )
{
	sSigHead * pSigHead = (sSigHead*) pMsg;

	if ( 0 != wcscmp ( awENGINE_TITLE, pSigHead->awEngineTitle ) )
		return false;

	*ppRetVersion         = &pSigHead->Version;
	*ppRetAppTitle        = ( wchar_t * ) ( pMsg + pSigHead->iAppTitleOffset );
	*ppRetHostName        = ( wchar_t * ) ( pMsg + pSigHead->iHostNameOffset ); 
	*ppRetDisplayList     = 0;  // TEMPORARY

	return true;
}



///////////////////////// DEAD CODE FOLLOWS /////////////////////////////////////


#if 0
struct sSig
{
	wchar_t		awAppTitle [ sizeof ( s_awAppTitle ) ];
	int			iSize;
	mojo::cVersion	Version;
	int			iDisplayListOffset; // includes terminating null
	wchar_t		awHostName [1]; // in reality this is longer
	// wchar_t	awDisplayList[];
};
#endif


//----------------------------------------------------------------------------------------------------------------------
//  PARSE
//----------------------------------------------------------------------------------------------------------------------
#if 0
bool cSignature :: parse ( const wchar_t ** ppRetDisplayList, const wchar_t ** ppRetHostName, mojo::cVersion ** ppRetVersion, const char * pMsg )
{
	sSig * pSig = (sSig*) pMsg;

	if ( 0 != wcscmp ( s_awAppTitle, pSig->awAppTitle ) )
		return false;

	if ( pSig->iSize < pSig->iDisplayListOffset )
		return false;

	*ppRetDisplayList = (wchar_t*) (pMsg + pSig->iDisplayListOffset);
	*ppRetHostName = pSig->awHostName;
	*ppRetVersion = &pSig->Version;

	return true;
}
#endif


//---------------------------------------------------------------------------------------------
// SIGNATURE CONSTRUCTOR
//---------------------------------------------------------------------------------------------
#if 0
cSignature :: cSignature() : pBuf(NULL)
{
	//-----------------------------------
	// GET COMPONENTS
	//-----------------------------------
	mojo::cStrW sHostName, sDisplayList;
	get_full_dns_name ( &sHostName );
	mojo::cDisplayList DL;
	DL.set_from_local_hardware();
	DL.text ( &sDisplayList );

	//-----------------------------------
	// ALLOCATE BUFFER
	//-----------------------------------
	iLen =	sizeof ( sSig )
			+ 2 * sHostName.len()				// host name with terminating NULL
			+ 2 * ( sDisplayList.len() + 1 );	// display list with terminating NULL

	this->pBuf = new char [ iLen ];
	sSig * p = ( sSig * ) pBuf;

	//-----------------------------------
	// COPY COMPONENTS TO BUFFER
	//-----------------------------------

	wcscpy_s ( p->awAppTitle, sizeof(s_awAppTitle), s_awAppTitle );

	p->iSize = iLen;
	p->Version = g_Version;

#pragma warning ( push )
#pragma warning ( disable :4996 )

	wcscpy ( p->awHostName, sHostName.cstr() );

	wchar_t * pDisplayName = p->awHostName + sHostName.len() + 1;
	wcscpy ( pDisplayName, sDisplayList.cstr() );

	p->iDisplayListOffset = ((char*)pDisplayName) - ((char*)p);

#pragma warning ( pop )
}
#endif


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