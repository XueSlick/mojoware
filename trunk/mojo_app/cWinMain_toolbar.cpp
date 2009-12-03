/***********************************************************************************************************************
/*
/*    cWinMain_toolbar.cpp / mojo_app
/* 
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"



//======================================================================================================================
// PROTOTYPES
//======================================================================================================================




//======================================================================================================================
// DATA
//======================================================================================================================




//======================================================================================================================
// CODE
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
//  CREATE TOOLBAR
//----------------------------------------------------------------------------------------------------------------------

HWND cWinMain ::create_toolbar ()
{

#define IDM_NEW 1
#define IDM_OPEN 2
#define IDM_SAVE 3

	assert ( this->hwnd );

	HWND hWndParent = this->hwnd;

    // Define some constants.
    // const int ImageListID = 0;
    // const int numButtons = 3;
    const DWORD buttonStyles = BTNS_AUTOSIZE | BTNS_CHECK ;
    const int iBitmapSize = 18;

	//---------------------------------
	//  CREATE TOOLBAR
	//---------------------------------

    HWND hWndToolbar = CreateWindowEx ( 0,
                                        TOOLBARCLASSNAME,
                                        NULL, 
                                        WS_CHILD | TBSTYLE_WRAPABLE | TBSTYLE_LIST | WS_BORDER,
                                        0, 0, 0, 0,
                                        hWndParent, NULL, g_hInstance, NULL);

    if ( hWndToolbar == NULL)
        return 0;

	//---------------------------------
	//  ADD BITMAP
	//---------------------------------

#if 1
	SendMessage ( hWndToolbar, TB_SETBITMAPSIZE, 0, (LPARAM) MAKELONG ( iBitmapSize, iBitmapSize ) );
	TBADDBITMAP tbab = { g_hInstance, IDB_TOOLBAR };
	LRESULT lResult = SendMessage ( hWndToolbar, TB_ADDBITMAP, 1, (LPARAM) &tbab ); 

	UNREFERENCED_PARAMETER ( lResult );
#endif

	//---------------------------------
	//  INITIALIZE BUTTON INFO
	//---------------------------------

    TBBUTTON tbButtons[] = 
    {
		{ 1, ID_TOGGLE_MOUSEOVER,     TBSTATE_ENABLED, buttonStyles, {0}, 0, (INT_PTR) L"Mouseover is on  " },
		{ 0, ID_TOGGLE_HOTKEYS,       TBSTATE_ENABLED, buttonStyles, {0}, 0, (INT_PTR) L"Hotkeys are off  " },
		{ 1, ID_TOGGLE_BROADCAST,     TBSTATE_ENABLED, buttonStyles, {0}, 0, (INT_PTR) L"Broadcast is on  " },
    };

	const int iQtyButtons = sizeof ( tbButtons ) / sizeof ( TBBUTTON );

    //--------------------------
	// ADD BUTTONS
	//--------------------------

    SendMessage(hWndToolbar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);

    SendMessage(hWndToolbar, TB_ADDBUTTONS, (WPARAM) iQtyButtons, (LPARAM)&tbButtons);

    //-----------------------------
	// SET BUTTON SIZES
	//-----------------------------

#if 0
	unsigned uSize = SendMessage ( hWndToolbar, TB_GETBUTTONSIZE, 0, 0 );
	WORD wDimX = LOWORD ( uSize );
	WORD wDimY = HIWORD ( uSize );
	unsigned uNewSize = MAKELONG ( wDimX, wDimY + 2 );
	SendMessage ( hWndToolbar, TB_SETBUTTONSIZE, 0, LPARAM (&uNewSize) );
#endif

#if 0

	COLORSCHEME cs;
	cs.clrBtnHighlight = RGB ( 0, 255, 0 );
	cs.clrBtnShadow    = RGB ( 240, 0, 25 );
	SendMessage ( hWndToolbar, TB_SETCOLORSCHEME, 0, (LPARAM) &cs );
#endif

    //--------------------------
	// RESIZE AND SHOW
	//--------------------------

    SendMessage ( hWndToolbar, TB_AUTOSIZE, 0, 0 ); 
    ShowWindow  ( hWndToolbar, TRUE );
    return hWndToolbar;
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

