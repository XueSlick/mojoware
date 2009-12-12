/***********************************************************************************************************************
/*
/*    cDlgGetTrigger_combo.cpp / mojo_app
/*
/*    "Combo" means combo box, the type of Windows control used here to implement entry fields for keys.
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"

//======================================================================================================================
// DATA
//======================================================================================================================

static WNDPROC s_pOldEditProc; // The operating system's default edit control proc.  

//======================================================================================================================
// CODE
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
//  EDIT CONTROL PROC
//  We have to subclass the edit control portion of the combo box conrols because they go berserk if the mouse wheel
//  rotates while they have the focus and the list is closed.  However the mouse wheel is nice to have when the list
//  is open.
//----------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK edit_control_proc ( HWND hwnd, UINT uMessage, WPARAM wParam, LPARAM lParam )
{
	HWND hwndCombo = reinterpret_cast<HWND>( GetWindowLongPtr ( hwnd, GWLP_USERDATA ) );

	switch ( uMessage )
	{
	case WM_MOUSEWHEEL:
		{
			LRESULT result = SendMessage ( hwndCombo, CB_GETDROPPEDSTATE, 0, 0 );

			if ( FALSE == result )
				return 1;
		}
	}

	return CallWindowProc ( s_pOldEditProc, hwnd, uMessage, wParam, lParam);	
}


//----------------------------------------------------------------------------------------------
//  ADD COMBO
//----------------------------------------------------------------------------------------------
void cDlgGetTrigger :: add_combo ()
{

	int iPosY 		        = iInitialComboPosY;
	const int iDimX 		= 116;

	int iComboQty = aCombo.qty();

	// int iInitialQtyOfRows = 1 + ( ( (int)(aCombo.qty()) - 1 ) /  s_iQtyInRow );

	//----------------------------------
	// CREATE IT
	//----------------------------------

	aCombo[iComboQty].hwnd = CreateWindow ( L"COMBOBOX", 
		                     L"", 
							 CBS_DROPDOWN | WS_CHILD | WS_VISIBLE |  WS_VSCROLL | CBS_SORT,
							 iMarginX + ( iComboQty % s_iQtyInRow ) * ( iGutter+iDimX ),
							 iPosY + (iComboQty/s_iQtyInRow)*(iGutter+s_iComboDimY),
							 iDimX,
							 s_iComboDimY,
							 hwnd,
							 NULL,
							 g_hInstance,
							 NULL );

	SetWindowFont ( aCombo[iComboQty].hwnd, g_hMenuFont, FALSE );

	//----------------------------------
	// FILL IT WITH STRINGS
	//----------------------------------

	for ( WORD wExVK = 0; wExVK < 512; wExVK ++ )
	{
		const wchar_t * pName = cKeyboard::ex_vk_to_pretty_name ( wExVK );

		if ( pName )
		{
			int iItemIndex = ComboBox_AddString ( aCombo[iComboQty].hwnd, pName );
			ComboBox_SetItemData ( aCombo[iComboQty].hwnd, iItemIndex, (LPARAM) wExVK );
		}
	}

	//----------------------------------
	// GET EDIT CONTROL HANDLE
	//----------------------------------

	COMBOBOXINFO cbi = { sizeof ( COMBOBOXINFO ) };
	GetComboBoxInfo ( aCombo[iComboQty].hwnd, &cbi );
	aCombo[iComboQty].hEdit = cbi.hwndItem;
	aCombo[iComboQty].iID   = GetDlgCtrlID ( cbi.hwndItem );

	//----------------------------------
	// SUBCLASS EDIT CONTROL AND STORE
	// THE COMBO HWND IN USER DATA
	//----------------------------------

	s_pOldEditProc = (WNDPROC) GetWindowLongPtr ( cbi.hwndItem, GWLP_WNDPROC );
	SetWindowLongPtr ( cbi.hwndItem, GWLP_WNDPROC,  (LONG_PTR) edit_control_proc );
	SetWindowLongPtr ( cbi.hwndItem, GWLP_USERDATA, (LONG_PTR) aCombo[iComboQty].hwnd );

	//----------------------------------
	// SET CURSOR
	//----------------------------------

	HWND hwndCombo = cbi.hwndCombo;
	hwndCombo;
	HWND hwndEdit = cbi.hwndItem;
	hwndEdit;

	if ( 1 == aCombo.qty() )
		PostMessage ( hwnd, WM_NEXTDLGCTL, (WPARAM) aCombo[iComboQty].hwnd, TRUE );

	else
		SendMessage ( hwnd, WM_NEXTDLGCTL, (WPARAM) aCombo[iComboQty].hwnd, TRUE );



	//----------------------------------
	// SET SIZE
	//----------------------------------

	change_size ();
}


//----------------------------------------------------------------------------------------------------------------------
// ON COMBO CHANGED
//----------------------------------------------------------------------------------------------------------------------
void cDlgGetTrigger :: on_combo_changed ( HWND hNewCombo )
{
	wchar_t b1 [100];
	wchar_t b2 [100];

	ComboBox_GetText ( hNewCombo, b1, sizeof(b1)/sizeof(wchar_t) );

	//--------------------------------------
	//  CHECK FOR DUPES
	//--------------------------------------

	if ( 1 < aCombo.qty() )
	{
		for ( unsigned i = 0; i < aCombo.qty() - 1; i++ )
		{
			if ( hNewCombo == aCombo[i].hwnd )
				continue;

			ComboBox_GetText ( aCombo[i].hwnd, b2, sizeof(b2)/sizeof(wchar_t) );

			if ( 0 == wcscmp ( b1, b2 ) )
			{
				ComboBox_SetCurSel ( hNewCombo, -1 );

				cStrW s;
				s.f ( L"You entered %s twice.\n\nThat won't work.", b1 );
				message_box ( s.cstr() );
				SetFocus ( hNewCombo );
				return;
			}
		}
	}

#if 1
	if ( hNewCombo == aCombo[aCombo.qty() - 1].hwnd )
		add_combo();
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