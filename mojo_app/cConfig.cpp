/***********************************************************************************************************************
/*
/*    cConfig.cpp
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"


using namespace mojo;

//======================================================================================================================
//  DATA
//======================================================================================================================


//======================================================================================================================
//  PROTOTYPES
//======================================================================================================================


//======================================================================================================================
//  CODE
//======================================================================================================================


//----------------------------------------------------------------------------------------------------------------------
//  PATHNAME
//----------------------------------------------------------------------------------------------------------------------
const wchar_t * cConfig :: pathname ( cStrW * pRet )
{
	assert ( g_awAppTitle );

	pRet->erase();

	mojo::get_our_local_app_data_directory ( pRet, g_awAppTitle );
	*pRet += g_awAppTitle;
	*pRet += L".config.xml";

	return pRet->cstr();

}



//----------------------------------------------------------------------------------------------------------------------
//  SAVE
//----------------------------------------------------------------------------------------------------------------------
void cConfig :: save ()
{


}


//----------------------------------------------------------------------------------------------------------------------
//  LOAD
//----------------------------------------------------------------------------------------------------------------------
void cConfig :: load ()
{


}

//----------------------------------------------------------------------------------------------------------------------
//  CONSTRUCTOR
//----------------------------------------------------------------------------------------------------------------------
cConfig :: cConfig ()
{
	cStrW sPathname;
	pathname ( &sPathname );

	cFileIn f ( sPathname.cstr() );

	if ( 0 ==  f.h )
		return;

	cStrW sTxt ( 10000 );

	f.get_whole_thing_without_line_breaks ( &sTxt );

	const wchar_t * pNext = sTxt.cstr();

	cStrW sName;
	cStrW sAttribute;
	cStrW sContent;

	while ( xml_get_next_element ( &sName, &sAttribute, &sContent, &pNext ) )
	{
		if ( sName == L"Toon" )
		{
			cToon * p = new cToon;
			p->init_from_xml ( sContent.cstr() );
			p->dwSerialNumber = ++this->dwLastSerialNumberAssigned;
			this->ToonList.append ( p );
		}		
	}

	PostMessage ( g_hwnd, uWM_TOON_LIST_CHANGED, 0, 0 );
}



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