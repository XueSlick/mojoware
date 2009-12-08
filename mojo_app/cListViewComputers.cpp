/***********************************************************************************************************************
/*
/*    cListViewPC.cpp / mojo_app
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"


//======================================================================================================================
//  CODE
//======================================================================================================================


//----------------------------------------------------------------------------------------------------------------------
//   HOT MACH
//   returns handle, i.e., dwSerialNum
//----------------------------------------------------------------------------------------------------------------------
DWORD cListViewComputers :: hot_mach ()
{
	int iItem = ListView_GetHotItem ( hwnd );

	if ( -1 == iItem )
		return (DWORD) -1;

	LVITEM item;
	item.mask = LVIF_PARAM;
	item.iItem = iItem;

	if ( FALSE == ListView_GetItem ( hwnd, &item ) )
	{
		LOG_SYSTEM_ERROR_T ( L"ListView_GetItem" );
		return (DWORD) -1;
	}

	return item.lParam;
}


//----------------------------------------------------------------------------------------------------------------------
//  SET VIEW REPORT
//----------------------------------------------------------------------------------------------------------------------
void cListViewComputers :: set_item ( int iItemIndex, cMach * pM)
{
	LVITEM lvi;
	ZeroMemory ( &lvi, sizeof(lvi) );
	int iIconIndex = mach_to_icon_index ( pM );
	lvi.mask = LVIF_IMAGE | LVIF_STATE; 
	lvi.iItem = iItemIndex;
	lvi.iImage = iIconIndex;
	lvi.stateMask = LVIS_STATEIMAGEMASK;
	lvi.state = INDEXTOSTATEIMAGEMASK (iIconIndex + 1);
	ListView_SetItem ( hwnd, &lvi );
	ListView_SetItemText ( hwnd, iItemIndex, 0, const_cast<LPWSTR>(pM->sName.cstr()) );
	ListView_SetItemText ( hwnd, iItemIndex, 1, const_cast<LPWSTR>(pM->sDottedDec.cstr()) );
	ListView_SetItemText ( hwnd, iItemIndex, 2, const_cast<LPWSTR>(mach_to_connection_state_text ( pM )) );
}


//----------------------------------------------------------------------------------------------------------------------
// PC LIST VIEW :: MACH TO ICON INDEX
//----------------------------------------------------------------------------------------------------------------------
int cListViewComputers::mach_to_icon_index ( cMach * p )
{
	if ( ! p )
		return 0;

	if ( p->bThisPC )
		return 2;

	switch ( mojo::get_connection_status ( p->dwSerialNumber ) )
	{
	case mojo::nConnectionStatus::client:
	case mojo::nConnectionStatus::server:
		return 1;

	case mojo::nConnectionStatus::both:
		return 2;

	default:
		return 0;
	}
}


//----------------------------------------------------------------------------------------------------------------------
//  MACH TO CONNECTION STATE TEXT
//----------------------------------------------------------------------------------------------------------------------
const wchar_t * cListViewComputers :: mach_to_connection_state_text ( cMach * p )
{
	if ( ! p )
		return 0;

	if ( p->bThisPC )
		return L"Full connection";

	switch ( mojo::get_connection_status ( p->dwSerialNumber ) )
	{
	case mojo::nConnectionStatus::client:
		return L"Partial connection (C)";

	case mojo::nConnectionStatus::server:
		return L"Partial connection (A)";

	case mojo::nConnectionStatus::both:
		return L"Full connection";

	default:
		return L"Not connected";
	}
}


//----------------------------------------------------------------------------------------------------------------------
//  INSERT IMAGE LISTS
//----------------------------------------------------------------------------------------------------------------------
void cListViewComputers :: insert_image_lists ( HWND hwnd )
{ 
	HANDLE hStateBlue = 0;
	HANDLE hStateGold = 0;
	HANDLE hStateGray = 0;

	//-----------------------------------
	// INSERT IMAGES IN ORDER
	// 0 == GRAY == NO CONNECTIONS
	// 1 == GOLD == ONE-WAY CONNECTION
	// 2 == BLUE == TWO-WAY CONNECTION
	//-----------------------------------

	HANDLE hDeskStraightGray = LoadImage ( g_hInstance, MAKEINTRESOURCE ( IDI_COMPUTER_GRAY ), 
	 	                      IMAGE_ICON, 85, 73, LR_CREATEDIBSECTION );

	HANDLE hDeskStraightGold = LoadImage ( g_hInstance, MAKEINTRESOURCE ( IDI_COMPUTER_GOLD), // returns 0 on fail
	 	                      IMAGE_ICON, 85, 73, LR_CREATEDIBSECTION | LR_LOADTRANSPARENT );

	HANDLE hDeskStraightBlue = LoadImage ( g_hInstance, MAKEINTRESOURCE ( IDI_COMPUTER_BLUE ), 
	 	                      IMAGE_ICON, 85, 73, LR_CREATEDIBSECTION );


	hStateGray = LoadImage ( g_hInstance, MAKEINTRESOURCE ( IDI_CONNECTION_STATE_GRAY ), 
	 	                      IMAGE_ICON, 16,24, LR_CREATEDIBSECTION );

	hStateGold = LoadImage ( g_hInstance, MAKEINTRESOURCE ( IDI_CONNECTION_STATE_GOLD ), 
	 	                      IMAGE_ICON, 16,24, LR_CREATEDIBSECTION );

	hStateBlue = LoadImage ( g_hInstance, MAKEINTRESOURCE ( IDI_CONNECTION_STATE_BLUE ), 
	 	                      IMAGE_ICON, 16,24, LR_CREATEDIBSECTION );

    hImageListLarge = ImageList_Create ( 85, 73, ILC_MASK | ILC_COLORDDB | ILC_COLOR32, 3, 0 );
	HRESULT iResult = ImageList_AddIcon ( hImageListLarge, (HICON)hDeskStraightGray );
	iResult = ImageList_AddIcon ( hImageListLarge, (HICON)hDeskStraightGold );
	iResult = ImageList_AddIcon ( hImageListLarge, (HICON)hDeskStraightBlue );

	hImageListState = ImageList_Create ( 16, 24, ILC_MASK | ILC_COLORDDB | ILC_COLOR32, 3, 0 );
	iResult = ImageList_AddIcon ( hImageListState, (HICON)hStateGray);
	iResult = ImageList_AddIcon ( hImageListState, (HICON)hStateGold );
	iResult = ImageList_AddIcon ( hImageListState, (HICON)hStateBlue );

	HIMAGELIST hResult = ListView_SetImageList ( hwnd, hImageListLarge, LVSIL_NORMAL ); 
	ListView_SetExtendedListViewStyle ( hwnd, LVS_EX_BORDERSELECT | LVS_EX_DOUBLEBUFFER | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP );
	hResult = ListView_SetImageList ( hwnd, hImageListState, LVSIL_STATE );
	HWND hHeader = ListView_GetHeader ( hwnd );
	RECT rect;
	GetWindowRect ( hHeader, & rect );
	MoveWindow ( hHeader, rect.left, rect.top, 200, 100, TRUE );
	ShowWindow ( hHeader, SW_NORMAL );
}


//----------------------------------------------------------------------------------------------------------------------
// POPULATE
//----------------------------------------------------------------------------------------------------------------------
void cListViewComputers::populate ( cMachlist * pMachlist )
{
	assert ( 0 < iQtyColumns );
	assert ( pMachlist );

	if ( ListView_GetItemCount ( hwnd ) )
		ListView_DeleteAllItems ( hwnd );

	LVITEM lvI;
	ZeroMemory ( &lvI, sizeof(lvI) );
	lvI.mask = LVIF_TEXT |  LVIF_PARAM ; 
	lvI.state = 0; 
	lvI.stateMask = 0; 

	int i = 0;
	for ( mojo::cMach * p = pMachlist->pHead; p; p = p->pNext, i++ )
	{
		// if ( p->bExclude )
		//	continue;

		SetLastError(0);
		lvI.iItem = i;
		lvI.iImage = 0;
		lvI.iSubItem = 0;
		lvI.lParam = (LPARAM) ( p->dwSerialNumber );
		lvI.pszText = const_cast<LPWSTR>( p->sName.cstr() );
		lvI.mask = LVIF_TEXT |  LVIF_PARAM | LVIF_STATE ; 
		lvI.state = 1<<12;
		SendMessage ( hwnd, LVM_INSERTITEM, 0, (LPARAM) &lvI );
		lvI.iSubItem = 1;

		if ( 2 <= iQtyColumns )
			ListView_SetItemText ( hwnd, i, 1, L"This is an IP" );

		if ( 3 <= iQtyColumns )
			ListView_SetItemText ( hwnd, i, 2, L"This is a status" );

		if ( 4 <= iQtyColumns )
			ListView_SetItemText ( hwnd, i, 2, L"Screen size" );

		set_item ( i, p );
	}

	InvalidateRect ( hwnd, NULL, true );
	UpdateWindow ( hwnd );
}


//----------------------------------------------------------------------------------------------------------------------
//   SET LIST VIEW VIEW
//----------------------------------------------------------------------------------------------------------------------
VOID cListViewComputers :: set_view ( DWORD dwView ) 
{ 
	//-----------------------------------------
	// IF REQUESTED STYLE IS DIFFERENT FROM
	// EXISTING ONE, CHANGE IT
	//-----------------------------------------

    LONG_PTR dwStyle = GetWindowLongPtr ( hwnd, GWL_STYLE); 
    
	if ( ( dwStyle & LVS_TYPEMASK ) != (int) dwView ) 
    {
        SetWindowLongPtr ( hwnd, GWL_STYLE, ( dwStyle & ~LVS_TYPEMASK ) | dwView ); 
    }

    //----------------------------------------------
	// TURN STATE IMAGES ON OR OFF
	//----------------------------------------------

	if ( LVS_REPORT == dwView )
		ListView_SetImageList ( hwnd, hImageListState, LVSIL_STATE );

	else
		ListView_SetImageList ( hwnd, 0, LVSIL_STATE );
} 


//----------------------------------------------------------------------------------------------------------------------
//   INIT
//----------------------------------------------------------------------------------------------------------------------
void cListViewComputers::init ()
{
	assert ( 0 < iQtyColumns );
	set_view ( LVS_REPORT ); // LVS_REPORT
	// INSERT COLUMNS
	// AT LEAST ONE MUST BE INSERTED FOR THE CONTROL TO BE VISIBLE

	LVCOLUMN lc;

    memset ( &lc, 0, sizeof(lc) ); 
	lc.mask= LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;    // Type of mask
	lc.pszText = L"ComputerX name";                     // First Header
	lc.cx = 150;

	SendMessage ( hwnd, LVM_INSERTCOLUMN, 0, (LPARAM) &lc );

	if ( 2 <= iQtyColumns )
	{
		lc.pszText = L"IP address";
		SendMessage ( hwnd, LVM_INSERTCOLUMN, 1, (LPARAM) &lc );
	}

	if ( 3 <= iQtyColumns )
	{
		lc.pszText = L"Status";
		lc.cx = 250;
		SendMessage ( hwnd, LVM_INSERTCOLUMN, 2, (LPARAM) &lc );
	}

	insert_image_lists ( hwnd );
	set_view ( LVS_REPORT ); // LVS_REPORT
	set_view ( LVS_ICON ); // LVS_REPORT
	ShowWindow ( hwnd, SW_NORMAL );
}


//----------------------------------------------------------------------------------------------------------------------
//  CONSTRUCTOR
//----------------------------------------------------------------------------------------------------------------------
cListViewComputers :: cListViewComputers () : iQtyColumns(3), bStateImages(true)
{
}


//----------------------------------------------------------------------------------------------------------------------
//   GET SELECT LIST
//----------------------------------------------------------------------------------------------------------------------
#if 0
int cListViewComputers :: get_selected ( tListNI<cMach *> * pRet )
{
	// To obtain the items being activated, the receiving application should use the 
	// LVM_GETSELECTEDCOUNT message to retrieve the number of items that are selected 
	// and then send the LVM_GETNEXTITEM message with LVNI_SELECTED until all of the 
	// items have been retrieved. 

	// int iQty = (int) ListView_GetSelectedCount ( hwnd );

	WPARAM wpStart = (WPARAM) -1;
	int iQty = 0;
	int iIndex = 0;

	for (;;)
	{
		iIndex = (int) SendMessage ( hwnd, LVM_GETNEXTITEM, wpStart, LVNI_SELECTED );

		if ( -1 == iIndex )
			break;

		wpStart = iIndex;
		iQty++;
		cMach ** p = new cMach *;
		*p = item_index_to_mach ( iIndex );
		pRet->insert ( p );
	}

	return iQty;
}
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
