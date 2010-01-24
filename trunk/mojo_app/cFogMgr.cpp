/***********************************************************************************************************************
/*
/*    cFigMgr.cpp / mojo_app
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"

//======================================================================================================================
//  DATA
//======================================================================================================================

DWORD cFogMgr :: dwLastSerialNumberAssigned = 0;


//======================================================================================================================
//  PROTOTYPES
//======================================================================================================================


//======================================================================================================================
//  CODE
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
//  GET BY HANDLE
//----------------------------------------------------------------------------------------------------------------------
cFog * cFogMgr :: get_by_handle ( DWORD dwHandle )
{
	return Root.get_by_handle ( dwHandle );
}


//----------------------------------------------------------------------------------------------------------------------
//  PATHNAME
//----------------------------------------------------------------------------------------------------------------------
const wchar_t * cFogMgr :: pathname ( cStrW * pRet )
{
	assert ( g_awAppTitle );
	pRet->erase();
	mojo::get_our_local_app_data_directory ( pRet, g_awAppTitle );
	*pRet += g_awAppTitle;
	*pRet += L".fog.xml";
	return pRet->cstr();
}


//----------------------------------------------------------------------------------------------------------------------
//  LOAD
//----------------------------------------------------------------------------------------------------------------------
void cFogMgr :: load_from_file ( cFog * pFog )
{
	cStrW sPathname;
	pathname ( &sPathname );

	cFileIn f ( sPathname.cstr() );

	if ( 0 ==  f.h )
		return;

	cStrW sTxt ( 10000 );

	f.get_whole_thing_without_line_breaks ( &sTxt );

	pFog->set_from_xml ( sTxt.cstr() );

	PostMessage ( g_hwnd, uWM_TOON_LIST_CHANGED, 0, 0 );
	PostMessage ( g_hwnd, uWM_WOW_LIST_CHANGED, 0, 0 );
}


#if 0
//----------------------------------------------------------------------------------------------------------------------
//  SAVE TO FILE
//----------------------------------------------------------------------------------------------------------------------
void cFogMgr :: save_to_file ()
{

	cStrW sPathname;
	pathname ( &sPathname );

	FILE * h;

	if ( 0 != _wfopen_s ( & h, sPathname.cstr(), L"wt, ccs=UTF-8" ) )
	{
		LOG_V ( L"Unable to open configuration file for writing: %s.", sPathname.cstr() );
		return;
	}

	cStrW s (10000);

	Root.write_to_xml ( &s, &Root, 0 );

	const wchar_t * pTxt = s.cstr();
	int iLen = s.len() * 2;
	pTxt;
	iLen;

	// fwprintf ( h, L"Test string" );
	fwrite ( pTxt, iLen, 1, h );
	fclose(h);

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