/***********************************************************************************************************************
/*
/*    cDlgGetTrigger_test_mods.cpp / mojo_app
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"

//======================================================================================================================
// DATA
//======================================================================================================================

//======================================================================================================================
// CODE
//======================================================================================================================


//----------------------------------------------------------------------------------------------------------------------
//  GET ALL KEYS
//----------------------------------------------------------------------------------------------------------------------
unsigned cDlgGetTrigger :: get_all_keys ( cArrayW * pRet )
{
	wchar_t awBuf [500];

	for ( unsigned i = 0; i < aCombo.qty(); i++ )
	{
		if ( 0 == ComboBox_GetText ( aCombo[i].hwnd, awBuf, sizeof(awBuf)/sizeof(wchar_t) ) )
			continue;

		WORD wExVK = cKeyboard::pretty_name_to_ex_vk ( awBuf );
		assert ( 0 != wExVK && wExVK < 512 );
		pRet->append ( wExVK );
	}

	return pRet->qty();
}


//----------------------------------------------------------------------------------------------------------------------
//  TEST MODS
//----------------------------------------------------------------------------------------------------------------------
bool cDlgGetTrigger :: bad_pair ( const wchar_t ** ppGeneric, const wchar_t ** ppSpecific, WORD wExVK1, WORD wExVK2 )
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
//  TEST MODS
//----------------------------------------------------------------------------------------------------------------------
bool cDlgGetTrigger :: bad_mods ( const wchar_t ** ppGeneric, const wchar_t ** ppSpecific )
{
	cArrayW aAll;
	get_all_keys ( &aAll );

	int iQty = (int) aAll.qty();

	for ( int a = 0; a < iQty - 1; a++ )
	{
		for ( int b = a + 1; b < iQty; b++ )
		{
			if ( bad_pair ( ppGeneric, ppSpecific, aAll[a], aAll[b] ) )
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
/*    published by the Free Software Foundation.  You should have received a copy of the license with mojo.  If you
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