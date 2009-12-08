/***********************************************************************************************************************
/*
/*    cDlgComputers.cpp / mojo_app
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"



//======================================================================================================================
// DATA
//======================================================================================================================

const int iButtonHeight = 27;
const int iMargin = 9;

//======================================================================================================================
// PROTOTYPES
//======================================================================================================================

//======================================================================================================================
// CODE
//======================================================================================================================



//----------------------------------------------------------------------------------------------------------------------
// WM INIT
//----------------------------------------------------------------------------------------------------------------------
void cDlgComputers::wm_init ()
{
	const int iMargin = 9;
	const int iLeftMargin = 0;

#if 0
	const int iButtonWidth = 80;
	const int iButtonHeight = 21;

	List.hwnd = GetDlgItem ( hwnd, ID_LIST );
	register_child ( &List,
							  nAnchor::right,		0,		-iMargin,
							  nAnchor::top,			0,		iMargin,
							  nAnchor::right,		0,		-(iMargin + iButtonWidth),
							  nAnchor::top,		    0,      iMargin + iButtonHeight );

	Pictures.hwnd = GetDlgItem ( hwnd, ID_PICTURES );
	register_child ( &Pictures,
							  nAnchor::right,		0,		-(iMargin*2 + iButtonWidth*2 ),
							  nAnchor::top,			0,		iMargin,
							  nAnchor::right,		0,		-(iMargin*2 + iButtonWidth),
							  nAnchor::top,		    0,      iMargin + iButtonHeight );
#endif

	ListView.hwnd = GetDlgItem ( hwnd, ID_LIST_VIEW );
	register_child ( &ListView,

							  nAnchor::left,		0,		iLeftMargin,
							  nAnchor::top,			0,		iMargin,
							  nAnchor::right,		0,		-iMargin,
							  nAnchor::bottom,		0,      -iMargin );

	ListView.init();

	cMachlist ml;
	mojo::get_machlist ( &ml );
	ListView.populate ( &ml );
}


//----------------------------------------------------------------------------------------------------------------------
// DEFAULT PROC
//----------------------------------------------------------------------------------------------------------------------
INT_PTR CALLBACK cDlgComputers::dialog_proc (HWND hwnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	cWin * pWin = user_data_to_pWin ( hwnd );
	cDlgComputers * pThis = static_cast<cDlgComputers*>(pWin);

	switch ( uMessage )
	{
	case uWM_MACHLIST_CHANGED:
		{
			cMachlist ml;
			mojo::get_machlist ( &ml );
			pThis->ListView.populate ( &ml );
		}
		break;

	case WM_INITDIALOG:
		{
			set_user_data ( hwnd, lParam );
			cWin * pWin = lParam_to_pWin ( hwnd, lParam );
			pThis = static_cast<cDlgComputers*>(pWin);
			pThis->hwnd = hwnd;
			pThis->wm_init ();
		}
		break;

	case WM_NOTIFY:
		if ( pThis )
		{
			NMHDR * pN = (NMHDR*)lParam;

			if ( pN->code == NM_RCLICK )
			{
				DWORD dwHandle = pThis->ListView.hot_mach();

				if ( (DWORD)-1 != dwHandle )
				{
					cMach m;
					if ( mojo::get_mach ( &m, dwHandle ) )
					{
						cStrW s;
						s.f ( L"You right-clicked a computer.\n\nHandle = %d.\nName = %s\n", dwHandle, m.sName.cstr() );
						message_box ( s.cstr() );

						// do_pc_context_menu ( hwnd, pM );
					}
				}
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