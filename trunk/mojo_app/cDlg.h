/***********************************************************************************************************************
/*
/*    cDlg.h / mojo_app
/*
/*    Base class for wrappers for dialogs.
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#pragma once

#include "cWin.h"

//======================================================================================================================
//  TYPEDEFS
//======================================================================================================================

typedef INT_PTR CALLBACK DialogProc (HWND hDlg, UINT uMessage, WPARAM wParam, LPARAM lParam);


//======================================================================================================================
//  MAIN CLASSES
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
//  DLG NON-MODAL
//----------------------------------------------------------------------------------------------------------------------
class cDlg : public cWin  // non modal
{
public:

	static cWin *	lParam_to_pWin 	( HWND hwnd, LPARAM lParam );
	static void		set_user_data 	( HWND hDlg, LPARAM lParam );
	HWND 			make_dlg 		();

	virtual DialogProc * dialog_proc () { return dialog_proc; }
	static 	DialogProc dialog_proc;
	virtual int idd () { return 0; }

	enum _eType { text, note, subhead, head, caption }; // determines displayed text font, size, color

	static int draw_text ( HDC hdc,   RECT *pR, _eType eType, const wchar_t * pTxt, bool bDraw, DWORD dwStyles = 0 );
	static int draw_text ( HWND hwnd, RECT *pR, _eType eType, const wchar_t * pTxt, bool bDraw, DWORD dwStyles = 0 );
};


//----------------------------------------------------------------------------------------------------------------------
//  DLG MODAL
//----------------------------------------------------------------------------------------------------------------------
class cDlgModal : public cDlg
{
public:
	
	INT_PTR make_dlg ( void * p = 0 );
	virtual DialogProc * dialog_proc () { return dialog_proc; }
	static DialogProc dialog_proc;
	virtual int idd () { return 0; }
};


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