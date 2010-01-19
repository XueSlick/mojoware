/***********************************************************************************************************************
/*
/*    cDlgPropWow.cpp / mojo_app
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"


//======================================================================================================================
// DATA
//======================================================================================================================

sDlgDatum cDlgFigWoW :: aData [] = 
{
	sDlgDatum ( ID_NAME,                        L"Name"                         ),
	sDlgDatum ( ID_PATH,                        L"Path"                         ),
	sDlgDatum ( 0,                              NULL                            ),
};


//======================================================================================================================
// PROTOTYPES
//======================================================================================================================

//======================================================================================================================
// CODE
//======================================================================================================================



//----------------------------------------------------------------------------------------------------------------------
//  SET TEXT
//----------------------------------------------------------------------------------------------------------------------
void cDlgFigWoW :: set_text ()
{
	set_item_text ( ID_LINK, 						L"DlgFigWoW.Link", L"http://mojoware.org" );
}

//----------------------------------------------------------------------------------------------------------------------
// WM INIT
//----------------------------------------------------------------------------------------------------------------------
void cDlgFigWoW :: wm_init ()
{
	eMode = none;
	pFigWoW = reinterpret_cast<cFigWoW*>(pUserData);
	pUserData = 0;

	const wchar_t * pHead = 0;
	cStrW sHeadInstructions;

	//---------------------------------------------------------
	// CREATE FIG IF NECESSARY
	//---------------------------------------------------------

	if ( 0 == pFigWoW )
	{
		eMode = add;
		pFigWoW = new cFigWoW;
		pFigWoW->bLaunchByMojo = true; // = cFigWoW::launch_by_mojo;
		pFigWoW->hMach = 1; // LOCAL
	}

	//---------------------------------------------------------
	// COMPUTER NAME
	//---------------------------------------------------------

	cStrW sPC;
	cMach mach;
	mojo::get_mach ( &mach, pFigWoW->hMach );
	mach.make_pretty_name ( &sPC );
	SetWindowText ( GetDlgItem ( hwnd, ID_COMPUTER ), sPC.cstr() );

	//------------------------------------------------------
	//  SET MODE
	//------------------------------------------------------

	if ( add == eMode )
	{
		pHead = L"Add a WoW";
		sHeadInstructions.f ( L"%s will launch this WoW on this computer.", g_awAppTitle );
	}

	else if ( ! pFigWoW->bLaunchByMojo ) // FOUND RUNNING
	{
		this->eMode = display;
		pHead = L"View WoW properties";
		sHeadInstructions = L"This WoW's settings can't be changed because it wasn't launched by Mojo.";

		Edit_SetReadOnly ( GetDlgItem ( hwnd, ID_NAME ), TRUE );
		Edit_SetReadOnly ( GetDlgItem ( hwnd, ID_PATH ), TRUE );
		Edit_SetReadOnly ( GetDlgItem ( hwnd, ID_ACCOUNT ), TRUE );
		EnableWindow ( GetDlgItem ( hwnd, ID_PATH_BUTTON ), FALSE );
	}

	else if ( 1 != pFigWoW->hMach ) // LAUNCH ON ANOTHER MACHINE
	{
		this->eMode = display;
		pHead = L"View WoW properties";
		sHeadInstructions.f ( L"This WoW's settings can be changed on the computer where it gets launched." );

		Edit_SetReadOnly ( GetDlgItem ( hwnd, ID_NAME ), TRUE );
		Edit_SetReadOnly ( GetDlgItem ( hwnd, ID_PATH ), TRUE );
		Edit_SetReadOnly ( GetDlgItem ( hwnd, ID_ACCOUNT ), TRUE );
		EnableWindow ( GetDlgItem ( hwnd, ID_PATH_BUTTON ), FALSE );
	}

	else // LAUNCH ON THIS MACHINE
	{
		this->eMode = change;
		pHead = L"Change a WoW";
		sHeadInstructions.f ( L"%s will launch this WoW on this computer.", g_awAppTitle );
	}

	// EnableWindow ( GetDlgItem ( hwnd, ID_COMPUTER ), FALSE );

	HWND hHead = GetDlgItem ( hwnd, ID_HEAD );
	wchar_t b [1000];
	GetWindowText ( hHead, b, sizeof(b)/sizeof(wchar_t ) );
	SetWindowFont ( hHead, g_hDialogBoxHeadFont, TRUE );
	int iResult = SetWindowText ( hHead, pHead );
	GetWindowText ( hHead, b, sizeof(b)/sizeof(wchar_t ) );
	SetWindowText ( GetDlgItem ( hwnd, ID_HEAD_INSTRUCTIONS ), sHeadInstructions.cstr() );

	iResult;

	DWORD dwError = GetLastError();
	dwError;

	VarsFig.wm_init ( hwnd, aData, reinterpret_cast<cFig *>( pFigWoW) );
	set_text();

	InvalidateRect ( hwnd, NULL, TRUE );
	UpdateWindow ( hwnd );
}


//----------------------------------------------------------------------------------------------------------------------
// DEFAULT PROC
//----------------------------------------------------------------------------------------------------------------------
INT_PTR CALLBACK cDlgFigWoW::dialog_proc (HWND hwnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	cWin * pWin = user_data_to_pWin ( hwnd );
	cDlgFigWoW * pThis = static_cast<cDlgFigWoW*>(pWin);

	switch ( uMessage )
	{
	case WM_INITDIALOG:
		{
			set_user_data ( hwnd, lParam );
			cWin * pWin = lParam_to_pWin ( hwnd, lParam );
			pThis = static_cast<cDlgFigWoW*>(pWin);
			pThis->hwnd = hwnd;
			pThis->wm_init();
		}
		break;

	case WM_CTLCOLORSTATIC:

		if ( lParam == (long) GetDlgItem ( hwnd, ID_HEAD ) )
		{
			HDC hdc = (HDC)wParam;
			SetTextColor ( hdc, RGB ( 90, 104, 170 ) );		
			SetBkMode(hdc, TRANSPARENT );
			SetDCBrushColor ( hdc, GetSysColor ( COLOR_BTNFACE ) );
			return (LONG) GetStockObject ( DC_BRUSH );
		}
		break;

	case WM_COMMAND:
		switch ( LOWORD ( wParam ) )
		{
		case ID_PATH_BUTTON:
			{
				wchar_t b [MAX_PATH + 8092];
				GetWindowText ( GetDlgItem ( hwnd, ID_PATH ), b, sizeof(b)/sizeof(wchar_t) );
				cStrW s;
				if ( get_open_file_name ( &s, hwnd, b ) )
					SetWindowText ( GetDlgItem ( hwnd, ID_PATH ), s.cstr() );
			}
			break;

		case ID_CANCEL:
		case IDCANCEL:
			if ( cDlgFigWoW::add == pThis->eMode )
				delete ( cFigWoW* ) pThis->pUserData;
			break;

		case ID_OK:
		case IDOK:
			if ( cDlgFigWoW::add == pThis->eMode )
			{
				g_FigMgr.append_wow ( pThis->pFigWoW );
			}

			if ( cDlgFigWoW::add == pThis->eMode || cDlgFigWoW::change == pThis->eMode )
			{
				pThis->VarsFig.dlg_to_vars();
				g_FigMgr.set_fig ( pThis->VarsFig.pFigOriginal->dwSerialNumber, (cFigWoW*)pThis->VarsFig.pFigCopy );
				PostMessage ( g_hwnd, uWM_WOW_LIST_CHANGED, 0, 0 );
				g_FigMgr.save_to_file();
			}

			if ( cDlgFigWoW::add == pThis->eMode )
			{
				pThis->pFigWoW->sComputerName = L"This computer";
				cTarget t;
				pThis->pFigWoW->set_target ( &t );
				pThis->pFigWoW->dwTargetID = pThis->pFigWoW->dwSerialNumber;
				mojo::set_launch_target ( &t );
			}
			break;
		}
		break;

	case WM_NOTIFY:
		{
			switch( wParam )
			{
			case ID_LINK:

				switch ( ((LPNMHDR)lParam)->code )
				{
					case NM_CLICK:
					case NM_RETURN:
					{
						wchar_t * pURL = PNMLINK(lParam)->item.szUrl;
						ShellExecute ( NULL, L"open", pURL, NULL, NULL, SW_SHOW );
					}
					break;
				}
				break;
			}
        }
		break;

	default:
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