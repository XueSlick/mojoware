/***********************************************************************************************************************
/*
/*    cDlgTest.cpp / mojo_app
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"



//======================================================================================================================
// DATA
//======================================================================================================================


//======================================================================================================================
// PROTOTYPES
//======================================================================================================================

//======================================================================================================================
// CODE
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
// DO IT
//----------------------------------------------------------------------------------------------------------------------
void cDlgTest :: do_it ()
{
	HWND hWoW = FindWindow ( L"GxWindowClassD3d", NULL );

	if ( ! hWoW )
		return;

	BOOL iResult;
	WPARAM wParam = GetDlgItemInt ( hwnd, ID_UNICODE_CODE_POINT, &iResult, FALSE );



	WORD wScanCode = 0;
	bool bExtended = false;
	bool bAltDown = false;
	bool bPrevKeyState = false; // true == down 
	bool bUp = false;

	LPARAM lParam = 1; // repeat count

	lParam |= ( wScanCode << 16 );
	lParam |= ( bExtended  ? ( 1 << 24 ) : 0 ); // extended
	lParam |= ( bAltDown  ? ( 1 << 29 ) : 0 ); // context code
	lParam |= ( bPrevKeyState ? ( 1 << 30 ) : 0 ); // previous key state; our keyboard hook handler inserted this bit
	lParam |= ( bUp ? ( 1 << 31 ) : 0 ); // transition state

	SendMessage ( hWoW, WM_KEYDOWN, wParam, lParam );

	lParam = 0; // repeat count
	bUp = true;
	bPrevKeyState = true;

	lParam |= ( wScanCode << 16 );
	lParam |= ( bExtended  ? ( 1 << 24 ) : 0 ); // extended
	lParam |= ( bAltDown  ? ( 1 << 29 ) : 0 ); // context code
	lParam |= ( bPrevKeyState ? ( 1 << 30 ) : 0 ); // previous key state; our keyboard hook handler inserted this bit
	lParam |= ( bUp ? ( 1 << 31 ) : 0 ); // transition state

	SendMessage ( hWoW, WM_KEYUP, wParam, lParam );
}


//----------------------------------------------------------------------------------------------------------------------
// DEFAULT PROC
//----------------------------------------------------------------------------------------------------------------------
INT_PTR CALLBACK cDlgTest::dialog_proc (HWND hwnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	cWin * pWin = user_data_to_pWin ( hwnd );
	cDlgTest * pThis = static_cast<cDlgTest*>(pWin);

	switch ( uMessage )
	{
	case WM_COMMAND:
		switch ( LOWORD ( wParam ) )
		{
		case ID_DO_IT:
			{
				pThis->do_it();
			}
			break;
		}
		break;


	case WM_INITDIALOG:
		{
			set_user_data ( hwnd, lParam );
			cWin * pWin = lParam_to_pWin ( hwnd, lParam );
			pThis = static_cast<cDlgTest*>(pWin);
			pThis->hwnd = hwnd;
		}
		break;
	}

	return cDlgModal::dialog_proc ( hwnd, uMessage, wParam, lParam );
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