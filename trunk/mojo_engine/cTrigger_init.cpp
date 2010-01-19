/***********************************************************************************************************************
/*
/*    cTrigger.cpp
/*
/*    Modifiers are represented by three sorts of things:
/*
/*    -- Microsoft VK_ codes.
/*    -- nTrigger bit flags.
/*    -- Mojo wExVK codes.
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"

using namespace mojo;
using namespace nTrigger;

//======================================================================================================================
//  DATA
//======================================================================================================================

//======================================================================================================================
//  PROTOTYPES
//======================================================================================================================

//======================================================================================================================
//  CODE
//======================================================================================================================


//----------------------------------------------------------------------------------------------
//  INIT
//  Call with human-readable text representation, e.g. "NumLockOn LShift RAlt F10"
//----------------------------------------------------------------------------------------------
bool cTrigger :: init ( const wchar_t * pArg, cStrW * pRetError )
{
	cArrayW aMain, aMod, aAll;
	DWORD dwLock = 0;
	WORD wLastExVK = 0; wLastExVK;

	wchar_t awDelim [] = L" ";
	wchar_t * pwContext;
	cStrW sArgCopy = pArg;

	for ( wchar_t * p = wcstok_s ( const_cast<wchar_t*>(sArgCopy.cstr()), awDelim, &pwContext );
	      p;
	      p = wcstok_s ( NULL, awDelim, &pwContext ) )
	{
		//---------------------------------
		//  LOCKS
		//---------------------------------

		if ( ! _wcsicmp ( p, L"CapsLockOn" ) )
			dwLock |= nTrigger::dwCAPSLOCKON;

		else if ( ! _wcsicmp ( p, L"CapsLockOff" ) )
			dwLock |= nTrigger::dwCAPSLOCKOFF;

		else if ( ! _wcsicmp ( p, L"NumLockOn" ) )
			dwLock |= nTrigger::dwNUMLOCKON;

		else if ( ! _wcsicmp ( p, L"NumLockOff" ) )
			dwLock |= nTrigger::dwNUMLOCKOFF;

		else if ( ! _wcsicmp ( p, L"ScrollLockOn" ) )
			dwLock |= nTrigger::dwSCROLLLOCKON;

		else if ( ! _wcsicmp ( p, L"ScrollLockOff" ) )
			dwLock |= nTrigger::dwSCROLLLOCKOFF;

		//---------------------------------
		//  KEYS
		//---------------------------------

		else if ( WORD wExVK = cKeyboard::pretty_name_to_ex_vk ( p ) )
			aAll.append ( wExVK );
	}

	//---------------------------------
	//  VALIDATE INPUT
	//---------------------------------

	if ( ! validate_init ( &aAll, pRetError ) )
		return false;

	//---------------------------------
	//  GET MAIN KEY
	//---------------------------------

	if ( 0 < aAll.qty() )
		wLastExVK = aAll.pop();

	//---------------------------------
	//  BUILD ARRAYS OF MODS AND MAINS
	//---------------------------------

	for ( unsigned n = 0; n < aAll.qty(); n++ )
	{
		WORD wExVK = aAll[n];

		if ( cKeyboard::is_modifier ( wExVK ) )
			aMod.append ( wExVK );

		else
			aMain.append ( wExVK );
	}

	//---------------------------------
	//  MAKE THE TRIGGER
	//---------------------------------

	if ( ! init ( dwLock, &aMod, &aMain, wLastExVK ) )
	{
		if ( pRetError )
			*pRetError = L"Sorry, an error occurred.";

		return false;
	}

	return true;
}


//----------------------------------------------------------------------------------------------
//  VALIDATE INIT
//----------------------------------------------------------------------------------------------
bool cTrigger :: validate_init ( cArrayW * pAll, cStrW * pRetError )
{

	//---------------------------------
	//  CHECK KEY QTY
	//---------------------------------

	if ( 0 == pAll->qty() )
	{
		if ( pRetError )
			*pRetError = L"You need to include at least one key.";

		return false;
	}

	//---------------------------------
	//  CHECK FOR DUPES
	//---------------------------------

	for ( unsigned j = 0; j < pAll->qty() - 1; j++ )
	{
		for ( unsigned k = j + 1; k < pAll->qty(); k++ )
		{
			if ( (*pAll)[j] == (*pAll)[k] )
			{
				if ( pRetError )
					pRetError->f ( L"You entered %s more than once.\n\nThat will have to be changed.", cKeyboard::ex_vk_to_pretty_name ( (*pAll)[j] ) );
					
				return false;
			}
		}
	}

	//---------------------------------
	//  CHECK FOR BAD MOD COMBOS
	//---------------------------------
	{
		const wchar_t * pSpecific, * pGeneric;

		if ( bad_mods ( &pGeneric, &pSpecific, pAll ) )
		{
			if ( pRetError )
			{
				pRetError->f ( L"You entered both these keys:\n\n"
					  L"%s\n"
					  L"%s\n\n"
					  L"You can't put them both in the same trigger because \n"
					  L"if you include %s, then %s is redundant.",
					  pGeneric,
					  pSpecific,
					  pGeneric,
					  pSpecific );

				return false;
			}
		}
	}

	//---------------------------------
	//  TEMPORARILY DISALLOW BUTTONS
	//---------------------------------

	for ( unsigned q = 0; q < pAll->qty(); q++ )
	{
		if ( cKeyboard::is_mouse_button ( (*pAll)[q] ) )
		{
			if ( pRetError )
				*pRetError = 
						  L"Sorry, you can't use mouse buttons as \n"
				          L"triggers in this version of Mojo.\n\n"
						  L"They will be implemented soon.  ";
			return false;
		}
	}

	//---------------------------------
	//  WARN ABOUT MULTIPLE MAINS
	//---------------------------------

#if 0

	if ( 0 < aMain.qty() )
	{
		int iResult = MessageBox ( hwnd, 
								L"You're using one or more non-modifier keys as \n"
								L"modifiers.\n\n"

								L"Mojo allows you to do this, but the result may \n"
								L"interfere with normal use of the keyboard.  \n\n"

								L"Are you sure you want to do this?",
								g_awAppTitle, 
								MB_YESNO | MB_ICONQUESTION );

		if ( IDNO == iResult )
			return false;
	}
#endif

	//---------------------------------
	//  WARN ABOUT FINAL MODIFIER
	//---------------------------------
#if 0
	if ( cKeyboard::is_modifier ( wLastExVK ) )
	{
		int iResult = MessageBox ( hwnd, 
								L"The last key in your trigger is a modifier.  \n\n"

								L"Mojo allows you to do this, but the result  \n"
								L"may interfere with normal use of the keyboard.  \n\n"

								L"Are you sure you want to do this?",
								g_awAppTitle, 
								MB_YESNO | MB_ICONQUESTION );

		if ( IDNO == iResult )
			return false;
	}
#endif

	return true;
}


//----------------------------------------------------------------------------------------------------------------------
//  INIT
//----------------------------------------------------------------------------------------------------------------------
bool cTrigger :: init ( DWORD dwLock, cArrayW * paMod, cArrayW * paMain, WORD wLastExVK )
{	
	paMain, wLastExVK;

	this->dwModState = 0;

	//---------------------------------
	//  LOCKS
	//---------------------------------

	dwModState |= dwLock;

	//---------------------------------
	//  EXTRA (NON-FINAL) MAINS
	//---------------------------------

	paMain->sort();
	this->aMain = *paMain;

	//---------------------------------
	//  LAST KEY
	//---------------------------------

	this->wLastExVK = wLastExVK;

	//---------------------------------
	//  MODS
	//---------------------------------

	this->dwModState &= ~ ( dwSHIFT | dwLSHIFT | dwRSHIFT | dwALT | dwLALT | dwRALT | dwCTRL | dwLCTRL | dwRCTRL );
	
	for ( unsigned i = 0; i < paMod->qty(); i++ )
	{
		switch ( ex_vk_to_vk ( (*paMod)[i] ) )
		{
		case VK_SHIFT:
			dwModState |= dwSHIFT;
			break;

		case VK_LSHIFT:
			dwModState |= dwLSHIFT;
			break;

		case VK_RSHIFT:
			dwModState |= dwRSHIFT;
			break;

		case VK_MENU:
			dwModState |= dwALT;
			break;

		case VK_LMENU:
			dwModState |= dwLALT;
			break;

		case VK_RMENU:
			dwModState |= dwRALT;
			break;

		case VK_CONTROL:
			dwModState |= dwCTRL;
			break;

		case VK_LCONTROL:
			dwModState |= dwLCTRL;
			break;

		case VK_RCONTROL:
			dwModState |= dwRCTRL;
			break;
		}
	}

	if ( 0 == ( dwModState & ( dwSHIFT | dwLSHIFT | dwRSHIFT ) ) )
		dwModState |= dwNSHIFT;

	if ( 0 == ( dwModState & ( dwALT | dwLALT | dwRALT ) ) )
		dwModState |= dwNALT;

	if ( 0 == ( dwModState & ( dwCTRL | dwLCTRL | dwRCTRL ) ) )
		dwModState |= dwNCTRL;

	return true;
}


//----------------------------------------------------------------------------------------------------------------------
//  TEST MODS
//----------------------------------------------------------------------------------------------------------------------
bool cTrigger :: bad_pair ( const wchar_t ** ppGeneric, const wchar_t ** ppSpecific, WORD wExVK1, WORD wExVK2 )
{
	struct sBadPair
	{
		WORD a;
		WORD b;
		WORD wGenericExVK;
		WORD wSpecificExVK;
	};

	sBadPair aBP [] =
	{
		{ VK_SHIFT, VK_LSHIFT, VK_SHIFT, VK_LSHIFT },
		{ VK_SHIFT, VK_RSHIFT, VK_SHIFT, VK_RSHIFT + 0x100 },
		{ VK_MENU, VK_LMENU, VK_MENU, VK_LMENU },
		{ VK_MENU, VK_RMENU, VK_MENU, VK_RMENU + 0x100 },
		{ VK_CONTROL, VK_LCONTROL, VK_CONTROL, VK_LCONTROL },
		{ VK_CONTROL, VK_RCONTROL, VK_CONTROL, VK_RCONTROL + 0x100 },
	};

	for ( int i = 0; i < sizeof ( aBP ) / sizeof ( sBadPair ); i++ )
	{
		if ( ( aBP[i].a == wExVK1 && aBP[i].b == wExVK2 ) ||
		     ( aBP[i].a == wExVK2 && aBP[i].b == wExVK1 ) )
		{
			*ppGeneric  = cKeyboard::ex_vk_to_pretty_name ( aBP[i].wGenericExVK );
			*ppSpecific = cKeyboard::ex_vk_to_pretty_name ( aBP[i].wSpecificExVK );
			return true;
		}
	}

	return false;
}

//----------------------------------------------------------------------------------------------------------------------
//  BAD MODS
//----------------------------------------------------------------------------------------------------------------------
bool cTrigger :: bad_mods ( const wchar_t ** ppGeneric, const wchar_t ** ppSpecific, cArrayW * pRay )
{
	int iQty = (int) pRay->qty();

	for ( int a = 0; a < iQty - 1; a++ )
	{
		for ( int b = a + 1; b < iQty; b++ )
		{
			if ( bad_pair ( ppGeneric, ppSpecific, (*pRay)[a], (*pRay)[b] ) )
				return true;
		}
	}

	return false;
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