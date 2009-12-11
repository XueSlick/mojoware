/***********************************************************************************************************************
/*
/*    cDlgGetTrigger_trigger.cpp / mojo_app
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"

//======================================================================================================================
// DATA
//======================================================================================================================

static const int s_iQtyInRow = 4; // of combo box fields for keys
static const int s_iMargin = 5;
static const int s_iComboDimY = 25;

//======================================================================================================================
// CODE
//======================================================================================================================

//----------------------------------------------------------------------------------------------
//  INIT TRIGGER
//----------------------------------------------------------------------------------------------
bool cDlgGetTrigger :: init_trigger ()
{
	cArrayW aMain, aMod, aAll;
	DWORD dwLock = 0;
	WORD wLastExVK = 0;
	wchar_t awBuf[100];

	if ( IsDlgButtonChecked ( hwnd, ID_CAPSLOCK_ON ) )
		dwLock |= nTrigger::dwCAPSLOCKON;

	else if ( IsDlgButtonChecked ( hwnd, ID_CAPSLOCK_OFF ) )
		dwLock |= nTrigger::dwCAPSLOCKOFF;

	if ( IsDlgButtonChecked ( hwnd, ID_NUMLOCK_ON ) )
		dwLock |= nTrigger::dwNUMLOCKON;

	else if ( IsDlgButtonChecked ( hwnd, ID_NUMLOCK_OFF ) )
		dwLock |= nTrigger::dwNUMLOCKOFF;

	if ( IsDlgButtonChecked ( hwnd, ID_SCROLLLOCK_ON ) )
		dwLock |= nTrigger::dwSCROLLLOCKON;

	else if ( IsDlgButtonChecked ( hwnd, ID_SCROLLLOCK_OFF ) )
		dwLock |= nTrigger::dwSCROLLLOCKOFF;

	//---------------------------------
	//  GET ALL KEYS (NON-BLANK COMBOS)
	//---------------------------------

	for ( unsigned i = 0; i < aCombo.qty(); i++ )
	{
		if ( 0 == ComboBox_GetText ( aCombo[i].hwnd, awBuf, sizeof(awBuf)/sizeof(wchar_t) ) )
			continue;

		WORD wExVK = cKeyboard::pretty_name_to_ex_vk ( awBuf );
		assert ( 0 != wExVK && wExVK < 512 );
		aAll.append ( wExVK );
	}

	//---------------------------------
	//  INITIALIZE INTERMEDIATE DATA
	//---------------------------------

	int iAllQty = (int) aAll.qty();

	if ( 0 == iAllQty )
	{
		MessageBox ( hwnd, L"You need to include at least one key.", g_awAppTitle, MB_OK );
		return false;
	}

	if ( 0 < iAllQty )
		wLastExVK = aAll [ iAllQty - 1 ];

	for ( int n = 0; n < iAllQty - 1; n++ )
	{
		WORD wExVK = aAll[n];

		if ( cKeyboard::is_modifier ( wExVK ) )
			aMod.append ( wExVK );

		else
			aMain.append ( wExVK );
	}

	//---------------------------------
	//  CHECK FOR DUPES
	//---------------------------------

	for ( int j = 0; j < iAllQty - 1; j++ )
	{
		for ( int k = j + 1; k < iAllQty; k++ )
		{
			if ( aAll[j] == aAll[k] )
			{
				cStrW s;
				s.f ( L"You entered %s more than once.\n\nThat will have to be changed.", cKeyboard::ex_vk_to_pretty_name ( aAll[j] ) );
				message_box ( s.cstr() );
				return false;
			}
		}
	}

	//---------------------------------
	//  CHECK FOR BAD MOD COMBOS
	//---------------------------------
	{
		const wchar_t * pSpecific, * pGeneric;

		if ( bad_mods ( &pGeneric, &pSpecific ) )
		{

			cStrW s;
				s.f ( L"You entered both these keys:\n\n"
					  L"%s\n"
					  L"%s\n\n"
					  L"You can't put them both in the same trigger because \n"
					  L"if you include %s, then %s is redundant.",
					  pGeneric,
					  pSpecific,
					  pGeneric,
					  pSpecific );

				message_box ( s.cstr() );

				return false;
		}
	}

	//---------------------------------
	//  TEMPORARILY DISALLOW BUTTONS
	//---------------------------------

	for ( int q = 0; q < iAllQty - 1; q++ )
	{
		if ( cKeyboard::is_mouse_button ( aAll[q] ) )
		{
			message_box ( L"Sorry, you can't use mouse buttons as \n"
				          L"triggers in this version of Mojo.\n\n"
						  L"They will be implemented soon.  " );
			return false;
		}
	}

	//---------------------------------
	//  WARN ABOUT MULTIPLE MAINS
	//---------------------------------

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

	//---------------------------------
	//  WARN ABOUT FINAL MODIFIER
	//---------------------------------

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

	//---------------------------------
	//  MAKE THE TRIGGER
	//---------------------------------

	if ( ! Trigger.init ( dwLock, &aMod, &aMain, wLastExVK ) )
	{
		MessageBox ( hwnd, L"Sorry, an error occurred.", g_awAppTitle, MB_ICONINFORMATION );
		return false;
	}


	//---------------------------------
	//  DISPLAY (TEMP FOR DEBUGGING)
	//---------------------------------
#if 0
	{
		cStrW t;
		Trigger.print ( &t );
		cStrW m;
		m.f ( L"If Freddie implemented me correctly, your hotkey is:\n\n%s", t.cstr() );
		message_box ( m.cstr() );
	}
#endif

	return true;
}


//----------------------------------------------------------------------------------------------
//  DRAW TRIGGER
//----------------------------------------------------------------------------------------------
void cDlgGetTrigger :: draw_trigger ()
{
#if 0
	cStrW s;
	Trigger.print ( &s );
	HWND hCtrl = GetDlgItem ( hwnd, ID_TEXT );
	HDC hdc = GetDC ( hCtrl);
	RECT r;
	GetClientRect ( hCtrl, &r );
	FillRect ( hdc, &r, ( HBRUSH ) GetStockObject ( WHITE_BRUSH ) );
	ReleaseDC ( hCtrl, hdc );
	draw_text ( hCtrl, NULL, text, s.cstr(), true );
#endif
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