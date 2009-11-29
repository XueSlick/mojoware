/***********************************************************************************************************************
/*
/*    cListViewConnections.cpp / mojo_app
/*
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/



#include "stdafx.h"
#include "cListViewConnections.h"
#include <commctrl.h>


//======================================================================================================================
// CODE
//======================================================================================================================


//----------------------------------------------------------------------------------------------------------------------
//   HOT MACH
//----------------------------------------------------------------------------------------------------------------------
DWORD cListViewConnections::hot_mach ()
{
	int iIndex = ListView_GetHotItem ( hwnd );
	LVITEM lvi;
	ZeroMemory ( &lvi, sizeof(lvi) );
	lvi.iItem = iIndex;
	lvi.mask = LVIF_PARAM;
	ListView_GetItem ( hwnd, &lvi );

 	return lvi.lParam;
}


//----------------------------------------------------------------------------------------------------------------------
// POPULATE
//----------------------------------------------------------------------------------------------------------------------
void cListViewConnections::populate ( mojo::cMachlist * pMachlist )
{
	int iQtyColumns = 3;
	assert ( 0 < iQtyColumns );
	assert ( pMachlist);

	cStrW sTemp;

	if ( ListView_GetItemCount ( hwnd ) )
		ListView_DeleteAllItems ( hwnd );

	int iQtyM = pMachlist->qty(); 

	if ( ! iQtyM )
		return;

	LVITEM lvI;
	ZeroMemory ( &lvI, sizeof(lvI) );
	lvI.mask = LVIF_TEXT |  LVIF_PARAM ; // | LVIF_STATE ; 
	lvI.state = 0; 
	lvI.stateMask = 0; 

	int i = -1;

	for ( cMach * p = pMachlist->pHead; p; p = pMachlist->next ( p ) )
	{
		if ( p->bThisPC )
			continue;
		i++;

		SetLastError(0);
		lvI.iItem = i;
		lvI.iImage = 0;
		lvI.iSubItem = 0;
		lvI.lParam = p->dwSerialNumber;

		lvI.mask = LVIF_TEXT |  LVIF_PARAM | LVIF_STATE ; 
		lvI.state = 1<<12;
		lvI.pszText = (wchar_t *) p->sName.cstr();

		// lvI.pszText = p->sName.cstr();
			
		SendMessage ( hwnd, LVM_INSERTITEM, 0, (LPARAM) &lvI );


		if ( 2 <= iQtyColumns )
		{
			assert ( p->sDottedDec.len() );
			ListView_SetItemText ( hwnd, i, 1, (wchar_t*) p->sDottedDec.cstr() );
		}

		if ( 3 <= iQtyColumns )
		{
			sTemp.erase();

			switch ( mojo::get_connection_status ( p->dwSerialNumber ) )
			{
			case mojo::nConnectionStatus::client:
				sTemp = L"C";
				break;

			case mojo::nConnectionStatus::server:
				sTemp = L"A";
				break;

			case mojo::nConnectionStatus::both:
				sTemp = L"AC";
				break;

			default:
				sTemp = L" ";
			}

			ListView_SetItemText ( hwnd, i, 2, (wchar_t*) sTemp.cstr() );
		}

		if ( 4 <= iQtyColumns )
			ListView_SetItemText ( hwnd, i,3, L"Screen size" );
	}

	InvalidateRect ( hwnd, NULL, true );
	UpdateWindow ( hwnd );
}


//----------------------------------------------------------------------------------------------------------------------
// INIT
//----------------------------------------------------------------------------------------------------------------------
void cListViewConnections::init ()
{
	DWORD dwStyles = /* LVS_EX_ONECLICKACTIVATE | */ /* | LVS_EX_TRACKSELECT  | */  LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;

	ListView_SetExtendedListViewStyleEx ( hwnd, dwStyles, dwStyles  );

	// INSERT COLUMNS
	// AT LEAST ONE MUST BE INSERTED FOR THE CONTROL TO BE VISIBLE

	int iQtyColumns = 3;

	LVCOLUMN lc;

    memset ( &lc, 0, sizeof(lc) ); 
	lc.mask= LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;    // Type of mask
	lc.pszText = L"Computer name";                     // First Header
	lc.cx = 100;

	SendMessage ( hwnd, LVM_INSERTCOLUMN, 0, (LPARAM) &lc );

	if ( 2 <= iQtyColumns )
	{
		lc.pszText = L"IP address";
		lc.cx = 100;
		SendMessage ( hwnd, LVM_INSERTCOLUMN, 1, (LPARAM) &lc );
	}

	if ( 3 <= iQtyColumns )  // C or S, INDICATES SOCKET TYPE (SERVER OR CLIENT)
	{
		lc.pszText = L"";
		lc.cx = 30;
		SendMessage ( hwnd, LVM_INSERTCOLUMN, 2, (LPARAM) &lc );
	}

	if ( 4 <= iQtyColumns )
	{
		lc.pszText = L"Screen Size"; // sockets
		lc.cx = 150;
		SendMessage ( hwnd, LVM_INSERTCOLUMN, 3, (LPARAM) &lc );
	}

	ShowWindow ( hwnd, SW_NORMAL );
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