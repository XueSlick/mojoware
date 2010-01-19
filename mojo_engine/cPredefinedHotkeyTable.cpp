/***********************************************************************************************************************
/*
/*    cPredefinedHotkeyTable.cpp
/*   
/*    Copyright 2010 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"

using namespace mojo;

//======================================================================================================================
//  DATA
//======================================================================================================================



cPredefinedHotkeyTable::sFuncTableEntry cPredefinedHotkeyTable::aFunc [] =
{
	L"ToggleMojo",                   cPredefinedHotkeyTable::toggle_mojo,
	L"ToggleWindowBroadcast",        cPredefinedHotkeyTable::toggle_window_broadcast,
	L"ToggleComputerBroadcast",      cPredefinedHotkeyTable::toggle_computer_broadcast,
	L"ToggleMouseover",              cPredefinedHotkeyTable::toggle_mouseover,
	L"ToggleHotkeys",                cPredefinedHotkeyTable::toggle_hotkeys,
	L"BringMouseoverCursorHome",     cPredefinedHotkeyTable::bring_mouseover_cursor_home,
	L"HideShowMojo",                 cPredefinedHotkeyTable::hide_show_mojo,
};

//======================================================================================================================
//  PROTOTYPES
//======================================================================================================================

//======================================================================================================================
//  CODE
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
// BANG
//----------------------------------------------------------------------------------------------------------------------
bool cPredefinedHotkeyTable :: bang ( DWORD dwModState, cKeyboardStateEx * pKB )
{
	cHotkey * pHK = cHotkeyTable::bang ( dwModState, pKB );

	if ( 0 == pHK )
		return false;

	else
	{
		cPredefinedHotkey * pPHK = reinterpret_cast<cPredefinedHotkey*>(pHK);

		cStrW sT;
		pPHK->Trigger.print ( &sT );
		sT += L".";

		put_ad_lib_memo ( cMemo::success, L"Hotkey triggered", sT.cstr() );

		pPHK->pf();

		return true;
	}
}
	

//----------------------------------------------------------------------------------------------------------------------
//  NAME TO FUNC
//----------------------------------------------------------------------------------------------------------------------
fPredefinedHotkeyFunc *  cPredefinedHotkeyTable :: name_to_func ( const wchar_t * pName )
{
	for ( size_t i = 0; i < sizeof(aFunc)/sizeof(sFuncTableEntry); i++ )
	{
		if ( 0 == wcscmp ( pName, aFunc[i].pName ) )
			return aFunc[i].pf;
	}

	return 0;
}


//----------------------------------------------------------------------------------------------------------------------
//  FUNC TO HOTKEY
//----------------------------------------------------------------------------------------------------------------------
cPredefinedHotkey *  cPredefinedHotkeyTable :: func_to_hotkey ( fPredefinedHotkeyFunc * p )
{
	const int iSIZE = sizeof(aList)/sizeof(tList2<cHotkey>);

	cPredefinedHotkey * pRetVal = 0;

	for ( size_t i = 0; i < iSIZE ; i++ )
	{
		tList2<cHotkey> * pL = &aList[i];

		if ( 0 == pL->qty() )
			continue;

		pL->lock();

		for ( cHotkey * pHK = pL->pHead; pHK; pHK = pHK->pNext )
		{
			if ( p == reinterpret_cast<cPredefinedHotkey*>(pHK)->pf )
			{
				pRetVal = reinterpret_cast<cPredefinedHotkey*>(pHK);
				break;
			}
		}

		pL->unlock();

		if ( pRetVal )
			break;
	}

	return pRetVal;
}


//----------------------------------------------------------------------------------------------------------------------
//  CLEAR
//----------------------------------------------------------------------------------------------------------------------
bool cPredefinedHotkeyTable :: clear ( const wchar_t * pName )
{
	fPredefinedHotkeyFunc * pF = name_to_func ( pName );

	assert(pF);

	cPredefinedHotkey * pH = func_to_hotkey ( pF );

	if ( pH )
		remove ( pH );

	return true;
}


//----------------------------------------------------------------------------------------------------------------------
//  REMOVE
//----------------------------------------------------------------------------------------------------------------------
void cPredefinedHotkeyTable :: remove ( cPredefinedHotkey * a )
{
	tList2<cHotkey> * pL = &aList[a->Trigger.wLastExVK];

	pL->lock();

	for ( cHotkey * pHK = pL->pHead; pHK; pHK = pHK->pNext )
	{
		if ( pHK == static_cast<cHotkey*>(a) )
		{
			pL->rem_del ( static_cast<cHotkey*>(a) );
			break;
		}
	}

	pL->unlock();

}


//----------------------------------------------------------------------------------------------------------------------
//  INSERT
//----------------------------------------------------------------------------------------------------------------------
bool cPredefinedHotkeyTable :: insert ( const wchar_t * pName, mojo::cTrigger * pTrigger )
{
	fPredefinedHotkeyFunc * pF = name_to_func ( pName );
	assert(pF);

	cPredefinedHotkey * pNew = new cPredefinedHotkey;
	pNew->Trigger = *pTrigger;
	pNew->pf = pF;

	clear ( pName );

	aList[pTrigger->wLastExVK].append(pNew);

	return true;
}

//----------------------------------------------------------------------------------------------------------------------
//  MEMO
//----------------------------------------------------------------------------------------------------------------------
bool cPredefinedHotkeyTable :: memo ( const wchar_t * pName )
{
	put_ad_lib_memo ( cMemo::success, pName, L"Done." );
	return true;
}


//----------------------------------------------------------------------------------------------------------------------
//  TOGGLE MOJO
//----------------------------------------------------------------------------------------------------------------------
bool cPredefinedHotkeyTable :: toggle_mojo ()
{
	memo ( L"Toggle Mojo" );
	// to do
	
	return true;
}

//----------------------------------------------------------------------------------------------------------------------
//  TOGGLE WINDOW BROADCAST
//----------------------------------------------------------------------------------------------------------------------
bool cPredefinedHotkeyTable :: toggle_window_broadcast ()
{
	memo ( L"Toggle window broadcast" );
	g_Settings.bWindowBroadcastIsOn = g_Settings.bWindowBroadcastIsOn ? false : true;
	PostMessage ( g_hwndApp, uWM_WINDOW_BROADCAST_OFF_ON, (int)g_Settings.bWindowBroadcastIsOn, (int)g_Settings.bWindowBroadcastIsOn );
	return true;
}

//----------------------------------------------------------------------------------------------------------------------
//  TOGGLE COMPUTER BROADCAST
//----------------------------------------------------------------------------------------------------------------------
bool cPredefinedHotkeyTable :: toggle_computer_broadcast ()
{
	memo ( L"Toggle computer broadcast" );
	return true;
}


//----------------------------------------------------------------------------------------------------------------------
//  TOGGLE MOUSEOVER
//----------------------------------------------------------------------------------------------------------------------
bool cPredefinedHotkeyTable :: toggle_mouseover ()
{
	memo ( L"Toggle mouseover" );
	g_Settings.bMouseoverIsOn = g_Settings.bMouseoverIsOn ? false : true;
	PostMessage ( g_hwndApp, uWM_MOUSEOVER_OFF_ON, (int)g_Settings.bMouseoverIsOn, (int)g_Settings.bMouseoverIsOn );
	return true;
}


//----------------------------------------------------------------------------------------------------------------------
//  TOGGLE HOTKEYS
//----------------------------------------------------------------------------------------------------------------------
bool cPredefinedHotkeyTable :: toggle_hotkeys ()
{
	memo ( L"Toggle hotkeys" );
	return true;
}


//----------------------------------------------------------------------------------------------------------------------
//  BRING MOUSEOVER CURSOR HOME
//----------------------------------------------------------------------------------------------------------------------
bool cPredefinedHotkeyTable :: bring_mouseover_cursor_home ()
{
	memo ( L"Bring mouseover cursor home" );
	g_Mouseover.bring_cursor_home();
	return true;
}


//----------------------------------------------------------------------------------------------------------------------
//  HIDE SHOW MOJO
//----------------------------------------------------------------------------------------------------------------------
bool cPredefinedHotkeyTable :: hide_show_mojo ()
{
	memo ( L"Hide/show Mojo" );
	return true;
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