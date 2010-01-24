/***********************************************************************************************************************
/*
/*    cFogPredefinedHotkeys.cpp / mojo_app
/*   
/*    Copyright 2010 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"
#include "cFog_define.h"



//======================================================================================================================
//  DATA
//======================================================================================================================



cFogTree::sEntry cFogPredefinedHotkeys::aTable [] =
{
	ENTRY_MEMBER ( ToggleMojo,               cFogPredefinedHotkeys,     sToggleMojo ),
	ENTRY_MEMBER ( ToggleWindowBroadcast,    cFogPredefinedHotkeys,     sToggleWindowBroadcast ),
	ENTRY_MEMBER ( ToggleComputerBroadcast,  cFogPredefinedHotkeys,     sToggleComputerBroadcast ),
	ENTRY_MEMBER ( ToggleMouseover,          cFogPredefinedHotkeys,     sToggleMouseover ),
	ENTRY_MEMBER ( ToggleHotkeys,            cFogPredefinedHotkeys,     sToggleHotkeys ),
	ENTRY_MEMBER ( HideShowMojo,             cFogPredefinedHotkeys,     sHideShowMojo ),
	ENTRY_MEMBER ( BringMouseoverCursorHome, cFogPredefinedHotkeys,     sBringMouseoverCursorHome ),
	{ 0, 0, 0 }
};

#if 0


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
void cFogPredefinedHotkeys :: set_engine ()
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
void cFogPredefinedHotkeys :: write_to_xml ( cStrW * pRet, void * pObject, const wchar_t * pTagName ) const
{

	pRet; pObject; pTagName;

	cFogRoot * pRoot = reinterpret_cast<cFogRoot*>(pObject);

	cFogPredefinedHotkeys * pThis = (cFogPredefinedHotkeys *) pRoot->get_by_typeid ( typeid ( cFogPredefinedHotkeys::Default ) . raw_name() );

	if ( ! pThis )
		return;

	cFog::write_to_xml ( pRet, pThis, pTagName /* table()->pwTag */ );
}


//----------------------------------------------------------------------------------------------------------------------
//  OPERATOR =
//----------------------------------------------------------------------------------------------------------------------
cFogPredefinedHotkeys & cFogPredefinedHotkeys:: operator= ( const cFog & ra )
{
	const cFogPredefinedHotkeys &r = (const cFogPredefinedHotkeys &) ra;

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
cFogPredefinedHotkeys * cFogPredefinedHotkeys :: clone () const
{
	return new cFogPredefinedHotkeys ( *this );
}


//----------------------------------------------------------------------------------------------------------------------
//  SET FROM XML
//  Tree version
//----------------------------------------------------------------------------------------------------------------------
void cFogPredefinedHotkeys :: set_from_xml ( void * pvDest, const wchar_t * pTxt ) const
{
	cFogRoot * pRoot = reinterpret_cast<cFogRoot*>(pvDest);

	cFogPredefinedHotkeys * pFPH = pRoot->get_predefined_hotkeys ();

	if ( ! pFPH )
	{
		pFPH = new cFogPredefinedHotkeys;
		pRoot->append_left ( pFPH );
	}

	pFPH->cFog::set_from_xml ( pFPH, pTxt );
}


//----------------------------------------------------------------------------------------------------------------------
//  CLONE
//----------------------------------------------------------------------------------------------------------------------
#if 0
cFogPredefinedHotkeys * cFogPredefinedHotkeys :: clone () const
{
	return new cFogPredefinedHotkeys ( *this );
}
#endif

//----------------------------------------------------------------------------------------------------------------------
//  OPERATOR =
//----------------------------------------------------------------------------------------------------------------------
#if 0
cFogPredefinedHotkeys & cFogPredefinedHotkeys :: operator= ( const cFog & r )
{
	this->sToggleMojo = r.sToggleMojo;
	return *this;
}
#endif





//----------------------------------------------------------------------------------------------------------------------
//  CONSTRUCTOR
//----------------------------------------------------------------------------------------------------------------------
#if 0
cFogPredefinedHotkeys :: cFogPredefinedHotkeys ( const cFogPredefinedHotkeys & r ) :
	sToggleMojo ( r.sToggleMojo )
{
}
#endif


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