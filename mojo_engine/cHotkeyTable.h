/********************************************************************************************************
/*
/*    cHotkeyTable.h / mojo_engine
/*
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/*    This class converts key codes to names of keys and vice versa.  See cKeyboard.cpp
/*    for an explanation of the data format.
/*
/*******************************************************************************************************/

#pragma once


//=======================================================================================================
// CLASSES
//=======================================================================================================

typedef mojo::tList2<cHotkey> cHotkeyList;

//-------------------------------------------------------------------------------------------------------
//  HOTKEY TABLE
//-------------------------------------------------------------------------------------------------------
class cHotkeyTable
{
public:

	cHotkey * bang ( DWORD dwModState, cKeyboardStateEx * pKB );

protected:

	bool insert ( cHotkey * a );
	cHotkey * get_hotkey ( DWORD dwModState, cKeyboardStateEx * pKB );

	cHotkeyList /* mojo::tList2<cHotkey> */ aList [512];
};


//-------------------------------------------------------------------------------------------------------
//  PREDEFINED HOTKEY TABLE
//-------------------------------------------------------------------------------------------------------
class cPredefinedHotkeyTable : public cHotkeyTable
{
public:


	bool                     insert          ( const wchar_t * pName, mojo::cTrigger * pTrigger );
	bool                     clear           ( const wchar_t * pName );
	bool                     bang            ( DWORD dwModState, cKeyboardStateEx * pKB );

private:

	struct sFuncTableEntry
	{
		const wchar_t * pName;
		fPredefinedHotkeyFunc * pf;
	};

	static bool              toggle_mojo                  ();
	static bool              toggle_window_broadcast      ();
	static bool              toggle_computer_broadcast    ();
	static bool              toggle_mouseover             ();
	static bool              toggle_hotkeys               ();
	static bool              bring_mouseover_cursor_home  ();
	static bool              hide_show_mojo               ();

	fPredefinedHotkeyFunc *  name_to_func                 ( const wchar_t * pName );
	cPredefinedHotkey     *  func_to_hotkey               ( fPredefinedHotkeyFunc * );
	void                     remove                       ( cPredefinedHotkey * a );
	void                     insert                       ( mojo::cTrigger * pTrigger, fPredefinedHotkeyFunc * pfFunc );
	fPredefinedHotkeyFunc  * get_hotkey_func              ( DWORD dwModState, cKeyboardStateEx * pKB );
	static bool              memo                         ( const wchar_t * pName );
	static sFuncTableEntry aFunc [];

};



/********************************************************************************************************
/*
/*    This file is part of Mojo.  For more information, see http://mojoware.org.
/*
/*    You may redistribute and/or modify Mojo under the terms of the GNU General Public License, 
/*    version 3, as published by the Free Software Foundation.  You should have received a copy of the 
/*    license with mojo.  If you did not, go to http://www.gnu.org.
/*     
/*    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
/*    INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A 
/*    PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
/*    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
/*    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
/*    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
/*    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
/*    EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
/*
/********************************************************************************************************/

