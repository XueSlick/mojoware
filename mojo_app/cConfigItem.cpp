/***********************************************************************************************************************
/*
/*    cConfigItem.cpp
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"


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
//   DO CONTEXT MENU
//----------------------------------------------------------------------------------------------------------------------
int cConfigItem :: do_context_menu ( HWND hwnd )
{
	HMENU hMenu =  LoadMenu ( g_hInstance, MAKEINTRESOURCE ( menu() ) );
	HMENU hSubMenu = GetSubMenu ( hMenu, 0 );
	POINT pt;
	GetCursorPos ( &pt );
	return TrackPopupMenuEx ( hSubMenu, TPM_RETURNCMD | TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, hwnd, NULL );
}


//----------------------------------------------------------------------------------------------------------------------
//  INIT FROM XML
//----------------------------------------------------------------------------------------------------------------------
bool cConfigItem :: init_from_xml ( const wchar_t * pTxt )
{
	cStrW sName, sAttribute, sContent;

	while ( xml_get_next_element ( &sName, &sAttribute, &sContent, &pTxt ) )
	{
		if ( sName == L"Name" )
			this->sName = sContent;

		else if ( sName == L"IconPath" )
			this->sIconPath = sContent;

		else
		{
			assert(0);
			return false;
		}
	}

	return true;
}


//----------------------------------------------------------------------------------------------------------------------
//  WRITE XML
//----------------------------------------------------------------------------------------------------------------------
void write_xml_element ( cStrW * pRet, const wchar_t * pName, const wchar_t * pContents )
{
	*pRet += L"<";
	*pRet += pName;
	*pRet += L">\n";
	*pRet += pContents;
	*pRet += L"</";
	*pRet += pName;
	*pRet += L">\n";
}

//----------------------------------------------------------------------------------------------------------------------
//  WRITE XML ELEMENT
//----------------------------------------------------------------------------------------------------------------------
void cConfigItem :: write_xml_element (  cStrW * pRet )
{
	pRet;
}

//----------------------------------------------------------------------------------------------------------------------
//  GET DUPE
//----------------------------------------------------------------------------------------------------------------------
cConfigItem * cConfigItem :: get_dupe ()
{
	cConfigItem * pNew = new cConfigItem ( *this );
	return pNew;
}


//----------------------------------------------------------------------------------------------------------------------
//  OPERATOR =
//----------------------------------------------------------------------------------------------------------------------
cConfigItem & cConfigItem :: operator= ( const cConfigItem & r )
{
	this->dwSerialNumber = r.dwSerialNumber;
	this->sIconPath = r.sIconPath;
	this->sName = r.sName;

	return *this;
}


//----------------------------------------------------------------------------------------------------------------------
//  CONSTRUCTOR
//----------------------------------------------------------------------------------------------------------------------
cConfigItem :: cConfigItem ( const cConfigItem & r )
{
	this->dwSerialNumber = r.dwSerialNumber;
	this->sIconPath = r.sIconPath;
	this->sName = r.sName;
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