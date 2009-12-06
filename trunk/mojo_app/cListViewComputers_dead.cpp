/*************************************************************************************************
/*
/*   cListViewPC_dead.cpp / mojo_app
/* 
/*   Copyright 2009 Robert Sacks
/*   
/*************************************************************************************************/

#if 0

#include "stdafx.h"


//======================================================================================================================
// PROTOTYPES
//======================================================================================================================


//======================================================================================================================
//  CODE
//======================================================================================================================




//------------------------------------------------------------------------------------------------
// PC LIST VIEW :: TOGGLE VIEW
//------------------------------------------------------------------------------------------------
#if 0
void cListViewComputers :: toggle_view () 
{
	static DWORD iIndex = 0;
	iIndex++;
	iIndex %= 2;
	DWORD dwView;

	switch ( iIndex )
	{
	case 0:  dwView = LVS_ICON;		break;
	case 1:  dwView = LVS_REPORT;	break;
	default: dwView = 0; assert(0); break;
	}

	if ( LVS_ICON == dwView )
	{
		ListView_SetImageList ( hwnd, 0, LVSIL_STATE );
		SetWindowText ( GetDlgItem ( GetParent ( hwnd ), ID_TOGGLE_VIEW ), L"Details" ) ;
	}

	else if ( LVS_REPORT == dwView )
	{
		if ( bStateImages )
			ListView_SetImageList ( hwnd, hImageListState, LVSIL_STATE );

		SetWindowText ( GetDlgItem ( GetParent ( hwnd ), ID_TOGGLE_VIEW ), L"Big picture" ) ;
	}

	set_list_view_view ( hwnd, dwView  );
}
#endif

//------------------------------------------------------------------------------------------------
// PC LIST VIEW :: MACH TO LIST VIEW ITEM
// returns index
//------------------------------------------------------------------------------------------------
#if 0
int cListViewComputers::mach_to_item_index ( cMach * pM )
{
	LVFINDINFO lvf;
	ZeroMemory ( &lvf, sizeof(lvf) );
	lvf.lParam = (LPARAM) pM;
	lvf.flags = LVFI_PARAM;

	int iResult = ListView_FindItem ( hwnd, -1, &lvf );
	return iResult;
}
#endif

//------------------------------------------------------------------------------------------------
// PC LIST VIEW :: SET_ICON
//------------------------------------------------------------------------------------------------
#if 0
void cListViewComputers :: set_icon ( cMach * pM )
{
	int iIconIndex = mach_to_icon_index ( pM );
	int iItemIndex = mach_to_item_index ( pM );

	LVITEM lvi;
	ZeroMemory ( &lvi, sizeof(lvi) );
	lvi.mask = LVIF_IMAGE ;
	lvi.iImage = iIconIndex;
	lvi.iItem = iItemIndex;

	ListView_SetItem ( hwnd, &lvi );
}
#endif


//------------------------------------------------------------------------------------------------
// QTY
//------------------------------------------------------------------------------------------------
#if 0
int cListViewComputers :: qty ()
{
	return ListView_GetItemCount ( hwnd );
}
#endif

//------------------------------------------------------------------------------------------------
// SET STATES TO ZERO
//------------------------------------------------------------------------------------------------
#if 0
void cListViewComputers :: set_all_states_to_zero ()
{
	ListView_SetItemState ( hwnd, -1, 0, LVIS_STATEIMAGEMASK );
}
#endif

// ---------------------------------------------------------------------------------------------
//   HOT INDEX
//   returns -1 if there isn't one
// ---------------------------------------------------------------------------------------------
#if 0
int cListViewComputers :: hot_index ()
{
	return ListView_GetHotItem ( hwnd );
}
#endif

// ---------------------------------------------------------------------------------------------
//   HOT MACH
// ---------------------------------------------------------------------------------------------
#if 0
cMach * cListViewComputers :: hot_mach ()
{
	int iIndex = hot_index();
	cMach * pM = item_index_to_mach ( iIndex );
	return pM;
}
#endif

//------------------------------------------------------------------------------------------------
// MACH TO SCREEN RES
//------------------------------------------------------------------------------------------------
#if 0
wchar_t * mach_to_screen_res ( wchar_t * b, int iSize, cMach * pM )
{
b, iSize, pM;
#if 0 // mojo port
	// wsprintf ( b, L"%d x %d", pM->ScreenRes.x, pM->ScreenRes.y );
	cRectI v;
	pM->calc_mult_virt_rect ( &v );
	wsprintf ( b, L"%d x %d", v.dx, v.dy );
#endif
	return b;
}
#endif


//------------------------------------------------------------------------------------------------
// ITEM INDEX TO MACH
//------------------------------------------------------------------------------------------------
#if 0
cMach * cListViewComputers :: item_index_to_mach ( int iIndex )
{
	LVITEM lvi;
	ZeroMemory ( &lvi, sizeof(lvi) );
	lvi.iItem = iIndex;
	lvi.mask = LVIF_PARAM;
	ListView_GetItem ( hwnd, &lvi );
 	return ( cMach * ) lvi.lParam;
}
#endif





//------------------------------------------------------------------------------------------------
// SET STATES FROM MACH LIST
//------------------------------------------------------------------------------------------------
#if 0
void cListViewComputers :: set_all_states_from_mach_list ( cMachlist * pMachList )
{
	LVITEM lvi;
	ZeroMemory ( &lvi, sizeof(lvi) );

	// wchar_t b[200];

	for ( cMach * pM = pMachList->pHead; pM; pM = pM->pNext )
	{
	#if 0 // mojo port
		if ( pM->bExclude )
			continue;
	#endif

		int iIconIndex;

		iIconIndex = mach_to_icon_index ( pM );

		lvi.mask = LVIF_IMAGE | LVIF_STATE; 
		int iItemIndex = mach_to_item_index ( pM );
		lvi.iItem = iItemIndex;
		lvi.iImage = iIconIndex;
		lvi.stateMask = LVIS_STATEIMAGEMASK;
		lvi.state = INDEXTOSTATEIMAGEMASK (iIconIndex + 1);
		ListView_SetItem ( hwnd, &lvi );

		if ( 1 <= iQtyColumns )
			ListView_SetItemText ( hwnd, iItemIndex, 0, const_cast<LPWSTR>(pM->sName.cstr()) );

		if ( 2 <= iQtyColumns )
			ListView_SetItemText ( hwnd, iItemIndex, 1, const_cast<LPWSTR>(pM->sDottedDec.cstr()) );

		if ( 3 <= iQtyColumns )
			ListView_SetItemText ( hwnd, iItemIndex, 2, L"Connection state" );
#if 0
		if ( 4 <= iQtyColumns )
			ListView_SetItemText ( hwnd, iItemIndex, 3, mach_to_screen_res ( b, sizeof(b)/sizeof(wchar_t), pM ) );
#endif
	}
}
#endif

#endif
