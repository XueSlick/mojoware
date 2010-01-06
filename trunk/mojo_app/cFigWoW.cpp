/***********************************************************************************************************************
/*
/*    cFigWoW.cpp / mojo_app
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"
#include "cFig_define.h"



//======================================================================================================================
//  DATA
//======================================================================================================================

const cFigWoW cFigWoW::Default;

cFig::sEntry cFigWoW::aTable [] =
{
	ENTRY_MEMBER ( Name,       sName,       cFigStr ),
	ENTRY_MEMBER ( Path,       sPath,       cFigStr ),
	ENTRY_MEMBER ( Computer,   sComputer,   cFigStr ),
	{ 0, 0, 0 }
};


//======================================================================================================================
//  PROTOTYPES
//======================================================================================================================


//======================================================================================================================
//  CODE
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
//  SET FROM XML
//  Tree version
//----------------------------------------------------------------------------------------------------------------------
void cFigWoW :: set_from_xml ( void * pvDest, const wchar_t * pTxt ) const
{
	pTxt;
	cFigWoWTree * pTree = reinterpret_cast<cFigWoWTree*>(pvDest);
	cFigWoW * pNew = new cFigWoW;
	pNew->cFig::set_from_xml ( pNew, pTxt );
	pNew->eOrigin = launch_by_mojo;
	pTree->append_right ( pNew );
}


//----------------------------------------------------------------------------------------------------------------------
//  SET FROM XML
//----------------------------------------------------------------------------------------------------------------------
#if 0 // list version pre dec 27 2009
void cFigWoW :: set_from_xml ( void * pvDest, const wchar_t * pTxt ) const
{
	pTxt;
	tList2<cFigWoW> * pList = reinterpret_cast<tList2<cFigWoW>*>(pvDest);
	pList;
	cFigWoW * pNew = new cFigWoW;
	pNew->cFig::set_from_xml ( pNew, pTxt );
	pNew->eOrigin = launch_by_mojo;
	pList->append ( pNew );
}
#endif

//----------------------------------------------------------------------------------------------------------------------
//  CLONE
//----------------------------------------------------------------------------------------------------------------------
cFigWoW * cFigWoW :: clone () const
{
	return new cFigWoW ( *this );
}


//----------------------------------------------------------------------------------------------------------------------
//  OPERATOR =
//----------------------------------------------------------------------------------------------------------------------
cFigWoW & cFigWoW :: operator= ( const cFig & ra )
{
	const cFigWoW &r = (const cFigWoW &) ra;
	this->dwSerialNumber = r.dwSerialNumber;
	this->sName = r.sName;
	this->sPath = r.sPath;
	this->sComputer = r.sComputer;
	this->eOrigin = r.eOrigin;
	this->bRunning = r.bRunning;
	return *this;
}


//----------------------------------------------------------------------------------------------------------------------
//  HANDLE CONTEXT MENU
//----------------------------------------------------------------------------------------------------------------------
void cFigWoW :: handle_context_menu ( int iID )
{
	switch ( iID )
	{
	case ID_DELETE:
		if ( this->found == this->eOrigin )
		{
			MessageBox ( g_hwnd, L"You can't delete a WoW that was found running.\n\n"
				                 L"Mojo will delete it automatically when it stops running.",
								 g_awAppTitle,
								 MB_OK );
			break;
		}
		g_FigMgr.delete_fig ( dwSerialNumber );
		PostMessage ( g_hwnd, uWM_WOW_LIST_CHANGED, 0, 0 );
		break;

	case ID_LAUNCH:
		message_box ( L"Sorry, Mojo can't launch programs yet." );
		break;

	case ID_PROPERTIES:
		{
			cDlgFigWoW d;
			d.make_dlg ( this );
		}
		break;
	}
}


//----------------------------------------------------------------------------------------------------------------------
//  CONSTRUCTOR
//----------------------------------------------------------------------------------------------------------------------
cFigWoW :: cFigWoW ( const cFigWoW & r )
{
	this->dwSerialNumber = r.dwSerialNumber;
	this->sName = r.sName;
	this->sPath = r.sPath;
	this->sComputer = r.sComputer;
	this->eOrigin = r.eOrigin;
	this->bRunning = r.bRunning;
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