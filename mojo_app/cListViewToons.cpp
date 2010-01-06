/***********************************************************************************************************************
/*
/*    cListViewToons.cpp / mojo_app
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"

#if 0

using namespace Gdiplus;


//======================================================================================================================
//  CODE
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
//  DO IMAGE LIST
//----------------------------------------------------------------------------------------------------------------------
void cListViewToons :: do_image_list ( cToonList * pToonList )
{
	int iDimX = g_Settings.uToonIconWidth;
	int iDimY = g_Settings.uToonIconHeight;

	HIMAGELIST hIL = ImageList_Create ( iDimX, iDimY, ILC_COLOR32, pToonList->qty(), 1 );

	int iItemIndex = 0;
	for ( cToon * p = pToonList->pHead; p; p = p->pNext, iItemIndex++ )
	{
		if ( ! file_exists ( p->sIconPath.cstr() ) )
		{
			mojo::put_ad_lib_memo ( mojo::cMemo::error, L"Image file not found", L"Missing file for toon:\n%s", p->sIconPath.cstr() );
			cStrW s;
			s.f ( L"Image file not found: %s", p->sIconPath.cstr() );
			LOG ( s.cstr() );
			continue;
		}

		Bitmap bm ( p->sIconPath.cstr() );
		HICON hIcon;
		bm.GetHICON(&hIcon);
		int iIconIndex = ImageList_AddIcon( hIL, hIcon );

		LVITEM lvi;
		ZeroMemory ( &lvi, sizeof(lvi) );
		lvi.mask = LVIF_IMAGE;
		lvi.iItem = iItemIndex;
		lvi.iImage = iIconIndex;
		// lvi.stateMask = LVIS_STATEIMAGEMASK;
		// lvi.state = INDEXTOSTATEIMAGEMASK (iIconIndex + 1);
		ListView_SetItem ( hwnd, &lvi );
	}

	HIMAGELIST hResult = ListView_SetImageList ( hwnd, hIL, LVSIL_NORMAL ); 
	hResult;
}


//----------------------------------------------------------------------------------------------------------------------
//  SET VIEW REPORT
//----------------------------------------------------------------------------------------------------------------------

void cListViewToons :: set_item ( int iItemIndex, cToon * pToon )
{
	pToon;
	iItemIndex;





	int x = 3;
	x++;

	#if 0

	LVITEM lvi;
	ZeroMemory ( &lvi, sizeof(lvi) );
	// int iIconIndex = mach_to_icon_index ( pM );
	// lvi.mask = LVIF_IMAGE | LVIF_STATE; 
	lvi.iItem = iItemIndex;
	// lvi.iImage = iIconIndex;
	// lvi.stateMask = LVIS_STATEIMAGEMASK;
	// lvi.state = INDEXTOSTATEIMAGEMASK (iIconIndex + 1);
	ListView_SetItem ( hwnd, &lvi );
	ListView_SetItemText ( hwnd, iItemIndex, 0, const_cast<LPWSTR>(pM->sName.cstr()) );
	ListView_SetItemText ( hwnd, iItemIndex, 1, const_cast<LPWSTR>(pM->sDottedDec.cstr()) );
	ListView_SetItemText ( hwnd, iItemIndex, 2, const_cast<LPWSTR>(mach_to_connection_state_text ( pM )) );
	#endif
}



//----------------------------------------------------------------------------------------------------------------------
// POPULATE
//----------------------------------------------------------------------------------------------------------------------
void cListViewToons::populate ( cToonList * pToonList )
{
	assert ( pToonList );

	if ( ListView_GetItemCount ( hwnd ) )
		ListView_DeleteAllItems ( hwnd );

	LVITEM lvI;
	ZeroMemory ( &lvI, sizeof(lvI) );
	lvI.mask = LVIF_TEXT |  LVIF_PARAM ; 
	lvI.state = 0; 
	lvI.stateMask = 0; 

	int i = 0;
	for ( cToon * p = pToonList->pHead; p; p = p->pNext, i++ )
	{
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
		ListView_SetItemText ( hwnd, i, 1, const_cast<LPWSTR> ( p->sAccount.cstr () ) );
		ListView_SetItemText ( hwnd, i, 2, const_cast<LPWSTR> ( p->sIconPath.cstr() ) );

		set_item ( i, p );
	}

	do_image_list ( pToonList );

	InvalidateRect ( hwnd, NULL, true );
	UpdateWindow ( hwnd );
}


//----------------------------------------------------------------------------------------------------------------------
//   SET LIST VIEW VIEW
//----------------------------------------------------------------------------------------------------------------------
VOID cListViewToons :: set_view ( DWORD dwView ) 
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
void cListViewToons::init ()
{
	set_view ( LVS_REPORT );

	LVCOLUMN lc;

    memset ( &lc, 0, sizeof(lc) ); 
	lc.mask= LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;    // Type of mask
	lc.pszText = L"Name";                              // First Header
	lc.cx = 250;

	SendMessage ( hwnd, LVM_INSERTCOLUMN, 0, (LPARAM) &lc );

	lc.pszText = L"Account";
	SendMessage ( hwnd, LVM_INSERTCOLUMN, 1, (LPARAM) &lc );

	lc.pszText = L"IconPath";
	lc.cx = 250;
	SendMessage ( hwnd, LVM_INSERTCOLUMN, 2, (LPARAM) &lc );

	set_view ( LVS_REPORT ); // LVS_REPORT
	set_view ( LVS_ICON );
	ShowWindow ( hwnd, SW_NORMAL );
}

//----------------------------------------------------------------------------------------------------------------------
//   HOT ITEM
//   returns handle, i.e., dwSerialNum
//----------------------------------------------------------------------------------------------------------------------
DWORD cListViewToons :: hot_item ()
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
