/***********************************************************************************************************************
/*
/*    cListView.cpp / mojo_app
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"

using namespace Gdiplus;


//======================================================================================================================
//  CODE
//======================================================================================================================


//----------------------------------------------------------------------------------------------------------------------
//  TOGGLE VIEW
//----------------------------------------------------------------------------------------------------------------------
void cListView :: toggle_view ()
{
    LONG_PTR dwStyle = GetWindowLongPtr ( hwnd, GWL_STYLE); 
	DWORD dwOldView = dwStyle & LVS_TYPEMASK;
	DWORD dwNewView = dwOldView == LVS_REPORT ? LVS_ICON : LVS_REPORT;
    SetWindowLongPtr ( hwnd, GWL_STYLE, ( dwStyle & ~LVS_TYPEMASK ) | dwNewView ); 
}


//----------------------------------------------------------------------------------------------------------------------
//   INIT
//----------------------------------------------------------------------------------------------------------------------
void cListView :: init ()
{


	set_view ( LVS_REPORT );

	LVCOLUMN lc;

    memset ( &lc, 0, sizeof(lc) ); 
	lc.mask= LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;    // Type of mask
	lc.pszText = L"Name";                              // First Header
	lc.cx = 250;

	SendMessage ( hwnd, LVM_INSERTCOLUMN, 0, (LPARAM) &lc );

	lc.pszText = L"IconPath";
	lc.cx = 250;
	SendMessage ( hwnd, LVM_INSERTCOLUMN, 1, (LPARAM) &lc );

	set_view ( LVS_REPORT ); // LVS_REPORT
	set_view ( LVS_ICON );
	ShowWindow ( hwnd, SW_NORMAL );
}


//----------------------------------------------------------------------------------------------------------------------
//  DO IMAGE LIST
//----------------------------------------------------------------------------------------------------------------------
void cListView :: do_image_list ( cConfigItemList * pList )
{
	int iDimX = g_Settings.uWoWIconWidth;
	int iDimY = g_Settings.uWoWIconHeight;

	HIMAGELIST hIL = ImageList_Create ( iDimX, iDimY, ILC_COLOR32, pList->qty(), 1 );

	int iItemIndex = 0;
	for ( cConfigItem * p = pList->pHead; p; p = p->pNext, iItemIndex++ )
	{
		bool bLoadedFromFile = false;
		int iImageIndex = -1;

		//-------------------------------
		// TRY TO LOAD IMAGE FROM FILE
		//-------------------------------

		if ( p->sIconPath.len() )
		{
			if ( ! file_exists ( p->sIconPath.cstr() ) )
			{
				mojo::put_ad_lib_memo ( mojo::cMemo::error, L"Image file not found", L"%s", p->sIconPath.cstr() );
				cStrW s;
				s.f ( L"Image file not found: %s", p->sIconPath.cstr() );
				LOG ( s.cstr() );
			}

			else
			{
				HICON hIcon;
				Bitmap bm ( p->sIconPath.cstr() );
				if ( ! ( Ok == bm.GetLastStatus() ) )
					LOG_V ( L"Bitmap constructor failed with %s.", p->sIconPath.cstr() );

				else
				{
					bm.GetHICON(&hIcon);
					if ( Ok !=  bm.GetLastStatus() )
						LOG_V ( L"Bitmap::GetHICON() failed with %s", p->sIconPath.cstr() );

					else
					{
						iImageIndex = ImageList_AddIcon( hIL, hIcon );
						bLoadedFromFile = true;
					}
				}

				if ( ! bLoadedFromFile)
					mojo::put_ad_lib_memo ( mojo::cMemo::error, L"Unable to display image file", L"%s", p->sIconPath.cstr() );
			}
		}

		if ( ! bLoadedFromFile )
		{
			HANDLE hBitmap = LoadImage ( g_hInstance, MAKEINTRESOURCE ( IDB_WOW_LOGO ), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION );
			iImageIndex = ImageList_Add ( hIL, (HBITMAP) hBitmap, NULL );
		}

		LVITEM lvi;
		ZeroMemory ( &lvi, sizeof(lvi) );
		lvi.mask = LVIF_IMAGE;
		lvi.iItem = iItemIndex;
		lvi.iImage = iImageIndex;
		ListView_SetItem ( hwnd, &lvi );
	}

	HIMAGELIST hResult = ListView_SetImageList ( hwnd, hIL, LVSIL_NORMAL ); 
	hResult;
}


//----------------------------------------------------------------------------------------------------------------------
//  SET ITEM
//----------------------------------------------------------------------------------------------------------------------
void cListView :: set_item ( int iItemIndex, cConfigItem * pConfigItem )
{
	LVITEM lvI;
	ZeroMemory ( &lvI, sizeof(lvI) );
	lvI.mask = LVIF_TEXT |  LVIF_PARAM ; 
	lvI.state = 0; 
	lvI.stateMask = 0; 

	SetLastError(0);
	lvI.iItem = iItemIndex;
	lvI.iImage = 0;
	lvI.iSubItem = 0;
	lvI.lParam = (LPARAM) ( pConfigItem->dwSerialNumber );
	lvI.pszText = const_cast<LPWSTR>( pConfigItem->sName.cstr() );
	lvI.mask = LVIF_TEXT |  LVIF_PARAM | LVIF_STATE ; 
	lvI.state = 1<<12;
	SendMessage ( hwnd, LVM_INSERTITEM, 0, (LPARAM) &lvI );
	lvI.iSubItem = 1;
	ListView_SetItemText ( hwnd, iItemIndex, 2, const_cast<LPWSTR> ( pConfigItem->sIconPath.cstr() ) );
}


//----------------------------------------------------------------------------------------------------------------------
// POPULATE
//----------------------------------------------------------------------------------------------------------------------
void cListView :: populate ( cConfigItemList * pListArg )
{
	assert ( pListArg );
	this->pList = pListArg;

	if ( ListView_GetItemCount ( hwnd ) )
		ListView_DeleteAllItems ( hwnd );

	if ( 0 == pListArg->qty() )
		return;

	cConfigItemList LocalList ( *pListArg );

	int i = 0;
	for ( cConfigItem * p = LocalList.pHead; p; p = p->pNext, i++ )
		set_item ( i, p );

	do_image_list ( &LocalList );

	InvalidateRect ( hwnd, NULL, true );
	UpdateWindow ( hwnd );
}


//----------------------------------------------------------------------------------------------------------------------
//   SET LIST VIEW VIEW
//----------------------------------------------------------------------------------------------------------------------
VOID cListView :: set_view ( DWORD dwView ) 
{ 
    LONG_PTR dwStyle = GetWindowLongPtr ( hwnd, GWL_STYLE); 
    
	if ( ( dwStyle & LVS_TYPEMASK ) != (int) dwView ) 
        SetWindowLongPtr ( hwnd, GWL_STYLE, ( dwStyle & ~LVS_TYPEMASK ) | dwView ); 
} 


//----------------------------------------------------------------------------------------------------------------------
//   HOT ITEM
//   returns handle, i.e., dwSerialNum
//----------------------------------------------------------------------------------------------------------------------
DWORD cListView :: hot_item ()
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
