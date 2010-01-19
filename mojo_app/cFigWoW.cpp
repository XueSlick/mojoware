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
	ENTRY_MEMBER ( Name,           sName,           cFigStr ),
	ENTRY_MEMBER ( Path,           sPath,           cFigStr ),
	ENTRY_MEMBER ( ComputerName,   sComputerName,   cFigStr ),
	{ 0, 0, 0 }
};


//======================================================================================================================
//  PROTOTYPES
//======================================================================================================================


//======================================================================================================================
//  CODE
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
//  SET TARGET
//----------------------------------------------------------------------------------------------------------------------
void cFigWoW :: set_target ( mojo::cTarget * t )
{
	t->bLaunchByMojo = this->bLaunchByMojo;
	t->dwID          = this->dwSerialNumber;
	t->hMach         = this->hMach;
	t->hwnd          = this->hwnd;
	t->dwProcessID   = this->dwProcessID;
	t->sName         = this->sName;
	t->sPath         = this->sPath;
	t->bIsRunning    = this->bRunning;
}


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
	pNew->bLaunchByMojo = true;
	pNew->hMach = 1;
	pNew->sComputerName = L"This computer";
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
	this->sComputerName = r.sComputerName;
	this->bLaunchByMojo = r.bLaunchByMojo; // = r.eOrigin;
	this->bRunning = r.bRunning;
	this->hwnd = r.hwnd;
	this->dwProcessID = r.dwProcessID;
	this->hMach = r.hMach;
	this->dwIP = r.dwIP;
	this->sDottedDec = r.sDottedDec;
	this->dwTargetID = r.dwTargetID;
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
		if ( ! bLaunchByMojo )
		{
			MessageBox ( g_hwnd, L"You can't delete a WoW that was found running.\n\n"
				                 L"Mojo will delete it automatically when it stops running.",
								 g_awAppTitle,
								 MB_OK );
			break;
		}

		if ( 1 != hMach )
		{
			MessageBox ( g_hwnd, L"You can only delete this WoW on the PC where it gets launched.",
								 g_awAppTitle,
								 MB_OK );
			break;
		}

		mojo::remove_launch_target ( dwSerialNumber );
		g_FigMgr.delete_fig ( dwSerialNumber );
		g_FigMgr.save_to_file();
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
	this->sComputerName = r.sComputerName;
	this->bLaunchByMojo = r.bLaunchByMojo; //  = r.eOrigin;
	this->bRunning = r.bRunning;
	this->hwnd = r.hwnd;
	this->dwProcessID = r.dwProcessID;
	this->hMach = r.hMach;
	this->dwIP = r.dwIP;
	this->sDottedDec = r.sDottedDec;
	this->dwTargetID = r.dwTargetID;
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