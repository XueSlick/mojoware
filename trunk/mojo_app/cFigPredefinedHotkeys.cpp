/***********************************************************************************************************************
/*
/*    cFigPredefinedHotkeys.cpp / mojo_app
/*   
/*    Copyright 2010 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"
#include "cFig_define.h"

//======================================================================================================================
//  DATA
//======================================================================================================================

const cFigPredefinedHotkeys cFigPredefinedHotkeys::Default;

cFig::sEntry cFigPredefinedHotkeys::aTable [] =
{
	ENTRY_MEMBER ( ToggleMojo,               sToggleMojo,               cFigStr ),
	ENTRY_MEMBER ( ToggleWindowBroadcast,    sToggleWindowBroadcast,    cFigStr ),
	ENTRY_MEMBER ( ToggleComputerBroadcast,  sToggleComputerBroadcast,  cFigStr ),
	ENTRY_MEMBER ( ToggleMouseover,          sToggleMouseover,          cFigStr ),
	ENTRY_MEMBER ( ToggleHotkeys,            sToggleHotkeys,            cFigStr ),
	ENTRY_MEMBER ( HideShowMojo,             sHideShowMojo,             cFigStr ),
	ENTRY_MEMBER ( BringMouseoverCursorHome, sBringMouseoverCursorHome, cFigStr ),
	{ 0, 0, 0 }
};




//======================================================================================================================
//  PROTOTYPES
//======================================================================================================================

static void set_engine_sub ( const wchar_t * pName, cStrW * pTxt );

//======================================================================================================================
//  CODE
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
//  SET ENGINE
//----------------------------------------------------------------------------------------------------------------------
void set_engine_sub ( const wchar_t * pName, cStrW * pTxt )
{
	if ( 0 == pTxt->len() )
		mojo::clear_predefined_hotkey (pName);

	else
	{
		cTrigger t;
		t.init ( pTxt->cstr() );
		mojo::set_predefined_hotkey ( pName, &t );
	}
}



//----------------------------------------------------------------------------------------------------------------------
//  SET ENGINE
//----------------------------------------------------------------------------------------------------------------------
void cFigPredefinedHotkeys :: set_engine ()
{
	struct sEntry
	{
		const wchar_t * pName;
		cStrW * pStr;
	};

	sEntry aTable [] = 
	{
		L"ToggleMojo",               &sToggleMojo,
		L"ToggleWindowBroadcast",    &sToggleWindowBroadcast,
		L"ToggleComputerBroadcast",  &sToggleComputerBroadcast,
		L"ToggleMouseover",          &sToggleMouseover,
		L"ToggleHotkeys",            &sToggleHotkeys,
		L"HideShowMojo",             &sHideShowMojo,
		L"BringMouseoverCursorHome", &sBringMouseoverCursorHome,
	};

	for ( int i = 0; i < sizeof(aTable)/sizeof(sEntry); i++ )
	{
		set_engine_sub ( aTable[i].pName, aTable[i].pStr ); 
	}
}






//----------------------------------------------------------------------------------------------------------------------
//  WRITE TO XML
//  Writes the whole element
//----------------------------------------------------------------------------------------------------------------------
void cFigPredefinedHotkeys :: write_to_xml ( cStrW * pRet, void * pObject, const wchar_t * pTagName ) const
{

	pRet; pObject; pTagName;

	cFigRoot * pRoot = reinterpret_cast<cFigRoot*>(pObject);

	cFigPredefinedHotkeys * pThis = (cFigPredefinedHotkeys *) pRoot->get_by_typeid ( typeid ( cFigPredefinedHotkeys::Default ) . raw_name() );

	if ( ! pThis )
		return;

	cFig::write_to_xml ( pRet, pThis, pTagName /* table()->pwTag */ );
}


//----------------------------------------------------------------------------------------------------------------------
//  OPERATOR =
//----------------------------------------------------------------------------------------------------------------------
cFigPredefinedHotkeys & cFigPredefinedHotkeys:: operator= ( const cFig & ra )
{
	const cFigPredefinedHotkeys &r = (const cFigPredefinedHotkeys &) ra;

	this->dwSerialNumber                = r.dwSerialNumber;
	this->sToggleMojo                   = r.sToggleMojo;
	this->sToggleWindowBroadcast        = r.sToggleWindowBroadcast;
	this->sToggleComputerBroadcast      = r.sToggleComputerBroadcast;
	this->sToggleMouseover              = r.sToggleMouseover;
	this->sToggleHotkeys                = r.sToggleHotkeys;
	this->sBringMouseoverCursorHome     = r.sBringMouseoverCursorHome;
	this->sHideShowMojo                 = r.sHideShowMojo;

	return *this;
}


//----------------------------------------------------------------------------------------------------------------------
//  CLONE
//----------------------------------------------------------------------------------------------------------------------
cFigPredefinedHotkeys * cFigPredefinedHotkeys :: clone () const
{
	return new cFigPredefinedHotkeys ( *this );
}


//----------------------------------------------------------------------------------------------------------------------
//  SET FROM XML
//  Tree version
//----------------------------------------------------------------------------------------------------------------------
void cFigPredefinedHotkeys :: set_from_xml ( void * pvDest, const wchar_t * pTxt ) const
{
	cFigRoot * pRoot = reinterpret_cast<cFigRoot*>(pvDest);

	cFigPredefinedHotkeys * pFPH = pRoot->get_predefined_hotkeys ();

	if ( ! pFPH )
	{
		pFPH = new cFigPredefinedHotkeys;
		pRoot->append_left ( pFPH );
	}

	pFPH->cFig::set_from_xml ( pFPH, pTxt );
}


//----------------------------------------------------------------------------------------------------------------------
//  CLONE
//----------------------------------------------------------------------------------------------------------------------
#if 0
cFigPredefinedHotkeys * cFigPredefinedHotkeys :: clone () const
{
	return new cFigPredefinedHotkeys ( *this );
}
#endif

//----------------------------------------------------------------------------------------------------------------------
//  OPERATOR =
//----------------------------------------------------------------------------------------------------------------------
#if 0
cFigPredefinedHotkeys & cFigPredefinedHotkeys :: operator= ( const cFig & r )
{
	this->sToggleMojo = r.sToggleMojo;
	return *this;
}
#endif





//----------------------------------------------------------------------------------------------------------------------
//  CONSTRUCTOR
//----------------------------------------------------------------------------------------------------------------------
#if 0
cFigPredefinedHotkeys :: cFigPredefinedHotkeys ( const cFigPredefinedHotkeys & r ) :
	sToggleMojo ( r.sToggleMojo )
{
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