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

//======================================================================================================================
// PROTOTYPES
//======================================================================================================================

//======================================================================================================================
// CODE
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
// WM INIT
//----------------------------------------------------------------------------------------------------------------------
void cDlgPropWoW :: wm_init ()
{
	pWoW = ( cWoW * ) pUserData;

	if ( pWoW )
		SetWindowText ( GetDlgItem ( hwnd, ID_NAME ), pWoW->sName.cstr() );
}


//----------------------------------------------------------------------------------------------------------------------
// DEFAULT PROC
//----------------------------------------------------------------------------------------------------------------------
INT_PTR CALLBACK cDlgPropWoW::dialog_proc (HWND hwnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	cWin * pWin = user_data_to_pWin ( hwnd );
	cDlgPropWoW * pThis = static_cast<cDlgPropWoW*>(pWin);

	switch ( uMessage )
	{

	case WM_INITDIALOG:
		{
			set_user_data ( hwnd, lParam );
			cWin * pWin = lParam_to_pWin ( hwnd, lParam );
			pThis = static_cast<cDlgPropWoW*>(pWin);
			pThis->hwnd = hwnd;
			pThis->wm_init();
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