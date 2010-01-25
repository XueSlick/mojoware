/***********************************************************************************************************************
/*
/*    cDlgCursorBlind.cpp / mojo_app
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"
#include "cDlgCursorBlind.h"


//===============================================================================================
// DATA
//===============================================================================================


//===============================================================================================
// PROTOTYPES
//===============================================================================================


//===============================================================================================
// CODE
//===============================================================================================

//----------------------------------------------------------------------------------------------------------------------
//  MAKE TRANSPARENT CURSOR
//  This hasn't been tested.
//----------------------------------------------------------------------------------------------------------------------
#if 0
HCURSOR make_transparent_cursor ()
{
	int x = GetSystemMetrics ( SM_CXICON );
	int y = GetSystemMetrics ( SM_CYICON );
	BYTE * aAnd = new BYTE [ x * y ];
	BYTE * aXor = new BYTE [ x * y ];
	memset ( aAnd, 0xFF, x*y );
	memset ( aXor, 0x00, x*y );
	HCURSOR hCursor = CreateCursor ( g_hInstance, 0, 0, x, y, aAnd, aXor );
	return hCursor;
}
#endif



//----------------------------------------------------------------------------------------------
// DIALOG PROC
//----------------------------------------------------------------------------------------------
INT_PTR CALLBACK cDlgCursorBlind::dialog_proc (HWND hwnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	cWin * pWin = user_data_to_pWin ( hwnd );
	cDlgCursorBlind * pThis = static_cast<cDlgCursorBlind*>(pWin);
	UNREFERENCED_PARAMETER ( pThis );

	switch ( uMessage )
	{

	case WM_ERASEBKGND:
		return 1;
	}

	return cDlg::dialog_proc ( hwnd, uMessage, wParam, lParam );
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
