/***********************************************************************************************************************
/*
/*    cDlgSetTrigger.h / mojo_app
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#pragma once

#include "cDlg.h"

//----------------------------------------------------------------------------------------------------------------------
//  CLASS
//
//  Pass a pointer to a trigger to this dialog box.   The dialog box takes input from user, tries to initialize the
//  trigger with that data, and returns true or false depending on whether the trigger returns with valid data.
//----------------------------------------------------------------------------------------------------------------------
class cDlgSetTrigger : public cDlgModal
{
public:

	INT_PTR make_dlg ( mojo::cTrigger * pTrigger ) { return cDlgModal :: make_dlg ( (void*) pTrigger ); }
	virtual int idd () { return IDD_GET_TRIGGER; }
	virtual DialogProc * dialog_proc () { return dialog_proc; }
	virtual void set_text ();
	static  DialogProc dialog_proc;

	void wm_init ();
	void on_clear ();
	void set_state ( int iID );
	void wm_key_event ( WORD wExVK );

	class cComboWin : public cWin
	{
	public:

		HWND hwndEdit;
		HWND hwndCombo;
		HWND hwndList;
		int iID;
		WNDPROC pOldEditProc; // The edit control needs to be subclassed to avoid incredibly annoying mousewheel behavior
	};

	cComboWin * get_combo_from_edit_hwnd  ( HWND hwnd );
	cComboWin * get_combo_from_combo_hwnd ( HWND hwnd );
	void on_combo_changed ( HWND hNewCombo );

private:

	void clear_combo ( HWND hwndCombo );

	bool bad_mods ( const wchar_t ** ppGeneric, const wchar_t ** pSppecific );
	unsigned get_all_keys ( mojo::cArrayW * pRet );
	bool bad_pair ( const wchar_t ** ppGeneric, const wchar_t ** ppSpecific, WORD wExVK1, WORD wExVK2 );

	cWinLabel PressedLabel;
	cWin      Pressed;
	cWin      Released;

	cWinLabel LockLabel;
	cWin      CapsLockOn;
	cWin      CapsLockOff;
	cWin      NumLockOn;
	cWin      NumLockOff;
	cWin      ScrollLockOn;
	cWin      ScrollLockOff;
	cWin      Link;

	cWin      Clear, OK, Cancel;

	static const int s_iQtyInRow;
	static const int s_iComboDimY;
	static const int iMarginX;
	static const int iMarginY;
	static const int iGutter;

	void change_size ();
	void add_combo ();

	bool init_trigger ();

	int iInitialWindowHeight;
	int iInitialComboPosY;

	tArray<cComboWin> aCombo;
	mojo::cTrigger Trigger;
};


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