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

//----------------------------------------------------------------------------------------------------------------------
//  INIT
//  Called from Get Trigger dialog.
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

	return true;
}


//----------------------------------------------------------------------------------------------------------------------
//  OPERATOR =
//----------------------------------------------------------------------------------------------------------------------
mojo::cTrigger & mojo::cTrigger :: operator= ( const cTrigger & r )
{
	this->dwModState = r.dwModState;
	this->wLastExVK  = r.wLastExVK;
	this->aMain      = r.aMain;
	return *this;
}


//----------------------------------------------------------------------------------------------------------------------
//  PRINT
//----------------------------------------------------------------------------------------------------------------------
const wchar_t * mojo::cTrigger :: print ( cStrW * pRet, bool bIncludeLocks )
{
	//-----------------------------------
	//  LOCKS
	//-----------------------------------

	if ( bIncludeLocks )
	{

		if (  this->dwModState & (  dwNUMLOCKON    | dwNUMLOCKOFF   | 
		                       		dwCAPSLOCKON   | dwCAPSLOCKOFF  | 
							   		dwSCROLLLOCKON | dwSCROLLLOCKOFF  ) )
		{
			if ( this->dwModState & dwNUMLOCKON )
			{
				*pRet += L"NumLockOn";
				*pRet += L" ";
			}

			if ( this->dwModState & dwNUMLOCKOFF )
			{
				*pRet += L"NumLockOff";
				*pRet += L" ";
			}

			if ( this->dwModState & dwCAPSLOCKON )
			{
				*pRet += L"CapsLockOn";
				*pRet += L" ";
			}

			if ( this->dwModState & dwCAPSLOCKOFF )
			{
				*pRet += L"CapsLockOff";
				*pRet += L" ";
			}

			if ( this->dwModState & dwSCROLLLOCKON )
			{
				*pRet += L"ScrollLockOn";
				*pRet += L" ";
			}

			if ( this->dwModState & dwSCROLLLOCKOFF )
			{
				*pRet += L"ScrollLockOff";
				*pRet += L" ";
			}
		}
	}

	//-----------------------------------
	//  MODIFIERS
	//-----------------------------------

	if ( this->dwModState & ( dwLSHIFT | dwRSHIFT | dwSHIFT |
		                      dwLCTRL  | dwRCTRL  | dwCTRL  |
						      dwLALT   | dwRALT   | dwALT     ) )
	{
		if ( this->dwModState & dwLSHIFT )
		{
			*pRet += L"LShift";
			*pRet += L" ";

		}
		if ( this->dwModState & dwRSHIFT )
		{
			*pRet += L"RShift";
			*pRet += L" ";
		}
		if ( this->dwModState & dwSHIFT )
		{
			*pRet += L"Shift";
			*pRet += L" ";
		}
		if ( this->dwModState & dwLCTRL )
		{
			*pRet += L"LCtrl";
			*pRet += L" ";
		}
		if ( this->dwModState & dwRCTRL )
		{
			*pRet += L"RCtrl";
			*pRet += L" ";
		}
		if ( this->dwModState & dwCTRL )
		{
			*pRet += L"Ctrl";
			*pRet += L" ";
		}
		if ( this->dwModState & dwLALT )
		{
			*pRet += L"LAlt";
			*pRet += L" ";
		}
		if ( this->dwModState & dwRALT )
		{
			*pRet += L"RAlt";
			*pRet += L" ";
		}
		if ( this->dwModState & dwALT )
		{
			*pRet += L"Alt";
			*pRet += L" ";
		}
	}

	//-----------------------------------
	//  NON FINAL MAIN KEYS
	//-----------------------------------

	for ( unsigned j = 0; j < aMain.qty(); j++ )
	{
		*pRet += cKeyboard::ex_vk_to_pretty_name ( aMain[j] );
		*pRet += L" ";
	}

	//------------------------------------------
	//  MAIN KEY
	//------------------------------------------

	*pRet += cKeyboard::ex_vk_to_pretty_name ( this->wLastExVK );

	return pRet->cstr();
}


//----------------------------------------------------------------------------------------------------------------------
//  CONSTRUCTOR
//----------------------------------------------------------------------------------------------------------------------
void mojo::cTrigger :: remove_mod_from_modifier_state ( WORD wExVK )
{
	switch ( ex_vk_to_vk ( wExVK ) )
	{
	case VK_LSHIFT:
		dwModState &= ~ ( dwLSHIFT | dwSHIFT ); // we'll put dwSHIFT back if necessary below
		break;

	case VK_RSHIFT:
		dwModState &= ~ ( dwRSHIFT | dwSHIFT );
		break;

	case VK_LMENU:
		dwModState &= ~ ( dwLALT | dwALT );
		break;

	case VK_RMENU:
		dwModState &= ~ ( dwRALT | dwALT );
		break;

	case VK_LCONTROL:
		dwModState &= ~ ( dwLCTRL | dwCTRL );
		break;

	case VK_RCONTROL:
		dwModState &= ~ ( dwRCTRL | dwCTRL );
		break;
	}

	if ( dwModState & ( dwLSHIFT | dwRSHIFT ) )
		dwModState |= dwSHIFT;

	else
		dwModState |= dwNSHIFT;

	if ( dwModState & ( dwLALT | dwRALT ) )
		dwModState |= dwALT;

	else
		dwModState |= dwNALT;

	if ( dwModState & ( dwLCTRL | dwRCTRL ) )
		dwModState |= dwCTRL;

	else
		dwModState |= dwNCTRL;
}


//----------------------------------------------------------------------------------------------------------------------
//  CONSTRUCTOR
//----------------------------------------------------------------------------------------------------------------------
mojo::cTrigger :: cTrigger ( const cKeyState * pKeyState ) : aMain (0)
{
	this->dwModState = pKeyState->mod_state();
	dwModState &= ~ ( dwSHIFT | dwALT | dwCTRL | dwCAPSLOCKON | dwCAPSLOCKOFF | dwNUMLOCKON | dwNUMLOCKOFF | dwSCROLLLOCKON | dwSCROLLLOCKOFF );
	// this->wMainExVK  = cKeyState::is_modifier ( pKeyState->wMostRecentExVK ) ? 0 : pKeyState->wMostRecentExVK;	

	wLastExVK = pKeyState->wMostRecentPressedExVK;

	if ( cKeyboard::is_modifier ( wLastExVK ) )
		remove_mod_from_modifier_state ( wLastExVK );

	dwModState &= ~ ( dwSHIFT | dwALT | dwCTRL );
}


//----------------------------------------------------------------------------------------------------------------------
//  BANG
//  returns true if trigger is triggered
//----------------------------------------------------------------------------------------------------------------------
bool bang ( mojo::cTrigger * pTrigger, DWORD dwKeyboardModState )
{
	DWORD dwTriggerModState = pTrigger->dwModState;

	if ( ( dwTriggerModState & dwKeyboardModState ) == dwTriggerModState )
	{
		//---------------------------------
		// CHECK FOR CHORD PREFIXES
		//---------------------------------
		return true;
	}

	else
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