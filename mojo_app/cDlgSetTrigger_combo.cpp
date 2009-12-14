/***********************************************************************************************************************
/*
/*    cDlgSetTrigger_combo.cpp / mojo_app
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
//  CLEAR COMBO
//----------------------------------------------------------------------------------------------------------------------
void cDlgSetTrigger :: clear_combo ( HWND hwndCombo )
{

	ComboBox_SetText ( hwndCombo, L"" );
	ComboBox_SetCurSel ( hwndCombo, -1 );
}


//----------------------------------------------------------------------------------------------------------------------
//  GET COMBO FROM HWND
//----------------------------------------------------------------------------------------------------------------------
cDlgSetTrigger::cComboWin * cDlgSetTrigger :: get_combo_from_edit_hwnd ( HWND hwnd )
{
	for ( unsigned i = 0; i < aCombo.qty(); i++ )
	{
		if ( aCombo[i].hwndEdit == hwnd )
			return &aCombo[i];
	}

	return NULL;
}


//----------------------------------------------------------------------------------------------------------------------
//  GET COMBO FROM HWND
//----------------------------------------------------------------------------------------------------------------------
cDlgSetTrigger::cComboWin * cDlgSetTrigger :: get_combo_from_combo_hwnd ( HWND hwnd )
{
	for ( unsigned i = 0; i < aCombo.qty(); i++ )
	{
		if ( aCombo[i].hwnd == hwnd )
			return &aCombo[i];
	}

	return NULL;
}

//----------------------------------------------------------------------------------------------------------------------
//  EDIT CONTROL PROC
//  We have to subclass the edit control portion of the combo box conrols because (1) they go berserk if the mouse 
//  wheel rotates while they have the focus and the list is closed and (2) it's a convenient way to enter mouse
//  buttons.
//----------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK edit_control_proc ( HWND hwnd, UINT uMessage, WPARAM wParam, LPARAM lParam )
{
	cDlgSetTrigger * pDlg = reinterpret_cast<cDlgSetTrigger*>( GetWindowLongPtr ( hwnd, GWLP_USERDATA ) );
	cDlgSetTrigger::cComboWin * pCombo = pDlg->get_combo_from_edit_hwnd ( hwnd );
	HWND hwndCombo = pCombo->hwnd;

	switch ( uMessage )
	{
	case WM_LBUTTONDBLCLK:
		{
				int iIndex = ComboBox_FindStringExact ( hwndCombo, 0, L"LButton" );
				ComboBox_SetCurSel ( hwndCombo, iIndex );
				pDlg->on_combo_changed ( hwndCombo );
		}
		break;

	case WM_RBUTTONDOWN:
		{
				int iIndex = ComboBox_FindStringExact ( hwndCombo, 0, L"RButton" );
				ComboBox_SetCurSel ( hwndCombo, iIndex );
				pDlg->on_combo_changed ( hwndCombo );
				return TRUE;
		}
		break;

	case WM_MBUTTONDOWN:
		{
				int iIndex = ComboBox_FindStringExact ( hwndCombo, 0, L"MButton" );
				ComboBox_SetCurSel ( hwndCombo, iIndex );
				pDlg->on_combo_changed ( hwndCombo );
				return TRUE;
		}
		break;

	case WM_XBUTTONDOWN:
		{
			int iIndex;

			if ( XBUTTON1 == HIWORD ( wParam ) )
				iIndex = ComboBox_FindStringExact ( hwndCombo, 0, L"Button4" );
			else
				iIndex = ComboBox_FindStringExact ( hwndCombo, 0, L"Button5" );

				ComboBox_SetCurSel ( hwndCombo, iIndex );
				pDlg->on_combo_changed ( hwndCombo );
				return TRUE;
		}
		break;

	case WM_MOUSEWHEEL:
		{
			LRESULT result = SendMessage ( hwndCombo, CB_GETDROPPEDSTATE, 0, 0 );

			if ( FALSE == result )
			{
				//---------------------------------------------------
				// TEMPORARILY ALLOW USER TO ENTER MOUSEWHEEL
				// BY DIRECT ACTION.  THIS IS PROBABLY A BAD IDEA
				// BUT LET'S GIVE TESTERS A CHANCE TO COMMENT.
				//---------------------------------------------------
#if 1
				int iDelta = GET_WHEEL_DELTA_WPARAM ( wParam );

				int iIndex;

				if ( 0 < iDelta )
					iIndex = ComboBox_FindStringExact ( hwndCombo, 0, L"WheelForward" );

				else
					iIndex = ComboBox_FindStringExact ( hwndCombo, 0, L"WheelBackward" );

				ComboBox_SetCurSel ( hwndCombo, iIndex );
				pDlg->on_combo_changed ( hwndCombo );
#endif
				return 1;
			}
		}
		break;
	}

	return CallWindowProc ( s_pOldEditProc, hwnd, uMessage, wParam, lParam);	
}


//----------------------------------------------------------------------------------------------
//  ADD COMBO
//----------------------------------------------------------------------------------------------
void cDlgSetTrigger :: add_combo ()
{

	int iPosY 		        = iInitialComboPosY;
	const int iDimX 		= 116;

	int iComboQty = aCombo.qty();

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
	// INSERT "CLEAR ENTRY"
	//----------------------------------

	SendMessage ( aCombo[iComboQty].hwnd, CB_INSERTSTRING, 0, (LPARAM) L"(Clear entry)" );

	//----------------------------------
	// GET EDIT CONTROL HANDLE
	//----------------------------------

	COMBOBOXINFO cbi = { sizeof ( COMBOBOXINFO ) };
	GetComboBoxInfo ( aCombo[iComboQty].hwnd, &cbi );
	aCombo[iComboQty].hwndEdit = cbi.hwndItem;
	aCombo[iComboQty].hwndCombo = cbi.hwndCombo;
	aCombo[iComboQty].hwndList = cbi.hwndList;
	aCombo[iComboQty].iID   = GetDlgCtrlID ( cbi.hwndItem );

	//----------------------------------
	// SUBCLASS EDIT CONTROL AND STORE
	// OUR ADDRESS IN USER DATA
	//----------------------------------

	s_pOldEditProc = (WNDPROC) GetWindowLongPtr ( cbi.hwndItem, GWLP_WNDPROC );
	SetWindowLongPtr ( cbi.hwndItem, GWLP_WNDPROC,  (LONG_PTR) edit_control_proc );
	SetWindowLongPtr ( cbi.hwndItem, GWLP_USERDATA, (LONG_PTR) this );

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
void cDlgSetTrigger :: on_combo_changed ( HWND hNewCombo )
{
	wchar_t awBuf1 [100], awBuf2 [100];

	int iItemIndex = ComboBox_GetCurSel ( hNewCombo );

	//--------------------------------------
	//  CHECK FOR "(CLEAR ENTRY)"
	//--------------------------------------

	if ( 0 == iItemIndex )
	{
		clear_combo ( hNewCombo );
		return;
	}

	//-----------------------------------------
	//  CHECK FOR DUPES
	//  For some reason, this doesn't work
	//  if you try to compare selection
	//  indices.  You have to compare text.
	//-----------------------------------------

	ComboBox_GetText ( hNewCombo, awBuf1, sizeof(awBuf1)/sizeof(wchar_t) );

	if ( 1 < aCombo.qty() )
	{
		for ( unsigned i = 0; i < aCombo.qty() - 1; i++ )
		{
			if ( hNewCombo == aCombo[i].hwnd )
				continue;

			ComboBox_GetText ( aCombo[i].hwnd, awBuf2, sizeof(awBuf2)/sizeof(wchar_t) );

			if ( 0 == wcscmp ( awBuf1, awBuf2 ) )
			{
				clear_combo ( hNewCombo );

				cStrW s;
				s.f ( L"You entered %s twice.\n\nThat won't work.", awBuf1 );
				MessageBox ( hwnd, s.cstr(), L"Mojo", MB_OK | MB_ICONINFORMATION );

				clear_combo ( hNewCombo );
				return;
			}
		}
	}

	if ( hNewCombo == aCombo[aCombo.qty() - 1].hwnd )
		add_combo();
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