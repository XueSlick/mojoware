/***********************************************************************************************************************
/*
/*    cListViewWoWs.cpp / mojo_app
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"

using namespace mojo;

//======================================================================================================================
//  CODE
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
//  SET ITEM
//----------------------------------------------------------------------------------------------------------------------
void cListViewWoWs :: set_item ( cConfigItem * pConfigItem )
{
	cWoW * pItem = reinterpret_cast<cWoW*> ( pConfigItem );

	LVITEM lvI;
	ZeroMemory ( &lvI, sizeof(lvI) );
	lvI.mask = LVIF_TEXT |  LVIF_PARAM | LVIF_IMAGE;
	lvI.iItem = INT_MAX;
	lvI.iImage = pItem->bRunning ? 1 : 0;
	lvI.iSubItem = 0;
	lvI.lParam = (LPARAM) ( pItem->dwSerialNumber );
	lvI.pszText = const_cast<LPWSTR>( pItem->sName.cstr() );
	int iItemIndex = SendMessage ( hwnd, LVM_INSERTITEM, 0, (LPARAM) &lvI );
	ListView_SetItemText ( hwnd, iItemIndex, 1, pItem->bRunning ? L"Yes" : L"No" );
	ListView_SetItemText ( hwnd, iItemIndex, 2, L"Local" );
	ListView_SetItemText ( hwnd, iItemIndex, 3, (LPWSTR) pItem->sPath.cstr() );
}


//----------------------------------------------------------------------------------------------------------------------
//  LOAD OVERLAYS INTO IMAGE LIST
//----------------------------------------------------------------------------------------------------------------------
const int * cListViewWoWs :: default_bitmap_ids ()
{
	static const int aiRet [] = { IDB_WOW_LOGO, IDB_WOW_LOGO_GREEN, 0 };

	return aiRet;
}



//----------------------------------------------------------------------------------------------------------------------
//  GET ICON SIZE
//----------------------------------------------------------------------------------------------------------------------
mojo::cPtI cListViewWoWs :: get_icon_size ()
{
	mojo::cPtI r ( g_Settings.uWoWIconWidth, g_Settings.uWoWIconHeight );
	return r;
}

//----------------------------------------------------------------------------------------------------------------------
// LIST TO POPULATE FROM BY DEFAULT
//----------------------------------------------------------------------------------------------------------------------
void  cListViewWoWs :: create_columns ()
{
	LVCOLUMN lc;

    memset ( &lc, 0, sizeof(lc) ); 
	lc.mask= LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;    // Type of mask
	lc.pszText = L"Name";                              // First Header
	lc.cx = 130;
	SendMessage ( hwnd, LVM_INSERTCOLUMN, 0, (LPARAM) &lc );

	lc.pszText = L"Running";
	lc.cx = 60;
	SendMessage ( hwnd, LVM_INSERTCOLUMN, 1, (LPARAM) &lc );

	lc.pszText = L"Computer";
	lc.cx = 150;
	SendMessage ( hwnd, LVM_INSERTCOLUMN, 2, (LPARAM) &lc );

	lc.pszText = L"Program file";
	lc.cx = 250;
	SendMessage ( hwnd, LVM_INSERTCOLUMN, 3, (LPARAM) &lc );
}


//----------------------------------------------------------------------------------------------------------------------
// LIST TO POPULATE FROM BY DEFAULT
//----------------------------------------------------------------------------------------------------------------------
const cConfigItemList *  cListViewWoWs :: list_to_populate_from_by_default ()
{
	return &g_Config.WoWList;
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
