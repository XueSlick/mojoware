/***********************************************************************************************************************
/*
/*    rich_edit.cpp / mojo_app
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"
#include <richedit.h>

using namespace mojo;

//======================================================================================================================
//  DATA
//======================================================================================================================

//======================================================================================================================
//  PROTOTYPES
//======================================================================================================================

//======================================================================================================================
//  CODE
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
//  LAST CHAR IS VISIBLE 
//  To make this work reliably, end the text with a linefeed.
//----------------------------------------------------------------------------------------------------------------------
bool rich_edit_last_char_is_visible ( HWND h )
{
	RECT r;
	SendMessage ( h, EM_GETRECT, 0, (LPARAM ) &r );

	POINTL pt;
	pt.x = r.right-1;
	pt.y = r.bottom-1;
	
	int iLastVisibleCharIndex 		= SendMessage ( h, EM_CHARFROMPOS, 		0, (LPARAM) &pt );
	int iLastVisibleCharLineIndex 	= SendMessage ( h, EM_EXLINEFROMCHAR, 	0, (LPARAM) iLastVisibleCharIndex );
	int iLineCount 					= SendMessage ( h, EM_GETLINECOUNT, 	0, 0 );

	if ( iLineCount <= iLastVisibleCharLineIndex + 1 )
		return true;

	else
		return false;
}

//----------------------------------------------------------------------------------------------------------------------
// SET INDENT
//----------------------------------------------------------------------------------------------------------------------
void rich_edit_set_indent ( HWND hwnd, int iQty )
{
	UNREFERENCED_PARAMETER ( iQty );

	PARAFORMAT2 pf;
	memset ( &pf, 0, sizeof (pf) );
	pf.cbSize = sizeof (pf);
	pf.dwMask = PFM_STARTINDENT;
	pf.dxStartIndent = iQty;

	int iResult;

	iResult = SendMessage ( hwnd, EM_SETPARAFORMAT, 0, (LPARAM) &pf );

}


//----------------------------------------------------------------------------------------------------------------------
// SET HEIGHT
// In rich edits' native units which is (I think, not sure) twips or 1/1440th of an inch
// assumes the cursor is at the insertion point, i.e., end of text
// so the new setting applies to whatever is entered in the future
//----------------------------------------------------------------------------------------------------------------------
void rich_edit_set_height ( HWND hwnd, int iHeight, bool bSelection )
{
	CHARFORMAT2 cf;
	memset ( &cf, 0, sizeof(cf) );
	cf.cbSize = sizeof(cf);
	cf.dwMask = CFM_SIZE;
	cf.yHeight = iHeight;

	int iResult;

	if ( bSelection )
		iResult = SendMessage ( hwnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) &cf );

	else
		iResult = SendMessage ( hwnd, EM_SETCHARFORMAT, SCF_ALL, (LPARAM) &cf );
}



//----------------------------------------------------------------------------------------------------------------------
// SET FACE
// assumes the cursor is at the insertion point, i.e., end of text
// so the new setting applies to whatever is entered in the future
//----------------------------------------------------------------------------------------------------------------------
void rich_edit_set_face ( HWND hwnd, const wchar_t * pFace, bool bSelection )
{
	CHARFORMAT2 cf;
	memset ( &cf, 0, sizeof(cf) );
	cf.cbSize = sizeof(cf);
	cf.dwMask = CFM_FACE;
	wcscpy_s ( cf.szFaceName, sizeof(cf.szFaceName)/sizeof(wchar_t), pFace );

	int iResult;

	if ( bSelection )
		iResult = SendMessage ( hwnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) &cf );

	else
		iResult = SendMessage ( hwnd, EM_SETCHARFORMAT, SCF_ALL, (LPARAM) &cf );
}


//----------------------------------------------------------------------------------------------------------------------
// SET WEIGHT REGULAR
// assumes the cursor is at the insertion point, i.e., end of text
// so the new setting applies to whatever is entered in the future
//----------------------------------------------------------------------------------------------------------------------
void rich_edit_set_weight_regular ( HWND hwnd, bool bSelection )
{
	CHARFORMAT cf;
	memset ( &cf, 0, sizeof(cf) );
	cf.cbSize = sizeof(cf);
	cf.dwMask = CFM_BOLD;

	int iResult;

	if ( bSelection )
		iResult = SendMessage ( hwnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) &cf );

	else
		iResult = SendMessage ( hwnd, EM_SETCHARFORMAT, SCF_ALL, (LPARAM) &cf );
}


//----------------------------------------------------------------------------------------------------------------------
// SET BOLD
// assumes the cursor is at the insertion point, i.e., end of text
// so the new setting applies to whatever is entered in the future
//----------------------------------------------------------------------------------------------------------------------
void rich_edit_set_bold ( HWND hwnd, bool bSelection )
{
	CHARFORMAT cf;
	memset ( &cf, 0, sizeof(cf) );
	cf.cbSize		= sizeof(cf);
	cf.dwMask		= CFM_BOLD;
	cf.dwEffects	= CFE_BOLD;

	int iResult;

	if ( bSelection )
		iResult = SendMessage ( hwnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) &cf );

	else
		iResult = SendMessage ( hwnd, EM_SETCHARFORMAT, SCF_ALL, (LPARAM) &cf );
}


//----------------------------------------------------------------------------------------------------------------------
// SET COLOR
// assumes the cursor is at the insertion point, i.e., end of text
// so the new setting applies to whatever is entered in the future
//----------------------------------------------------------------------------------------------------------------------
void rich_edit_set_color ( HWND hwnd, COLORREF color, bool bSelection )
{
	CHARFORMAT cf;
	memset ( &cf, 0, sizeof(cf) );
	cf.cbSize = sizeof(cf);
	cf.dwMask = CFM_COLOR;
	cf.crTextColor = color;

	int iResult;

	if ( bSelection )
		iResult = SendMessage ( hwnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) &cf );

	else
		iResult = SendMessage ( hwnd, EM_SETCHARFORMAT, SCF_ALL, (LPARAM) &cf );
}


//----------------------------------------------------------------------------------------------------------------------
//  RICH EDIT APPEND LINE
//----------------------------------------------------------------------------------------------------------------------
void rich_edit_append_line ( HWND hwnd, const wchar_t * pText )
{
	if ( 0 == pText )
		return;

	//-----------------------------------------------
	// DELETE SOME LINES WHEN THERE ARE MORE
	// THAN 1000
	//-----------------------------------------------

	if ( 1000 < Edit_GetLineCount(hwnd) )
	{
		int iEnd = Edit_LineIndex ( hwnd, 200 ); // delete 200 lines
		Edit_SetSel ( hwnd, 0, iEnd );
		Edit_ReplaceSel ( hwnd, "" );
	}

	//------------------------------------------------
	//  GET TEXT READY
	//------------------------------------------------

	cStrW s = pText;
	s += L"\r";

	//------------------------------------------------
	//  APPEND TEXT
	//------------------------------------------------

	Edit_SetSel ( hwnd, (WPARAM) -1, -1 );
	Edit_ReplaceSel ( hwnd, s.cstr() );

	//------------------------------------------------
	//  SCROLL TO BOTTOM
	//------------------------------------------------

    SendMessage ( hwnd, WM_VSCROLL, SB_BOTTOM, 0 );
}


/***********************************************************************************************************************
/*
/*    This file is part of Mojo.  For more information, see http://mojoware.org.
/*
/*    You may redistribute and/or modify Mojo under the terms of the GNU General Public License, 
/*    version 3, as published by the Free Software Foundation.  You should have received a copy of the 
/*    license with mojo.  If you did not, go to http://www.gnu.org.
/*     
/*    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
/*    INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A 
/*    PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
/*    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
/*    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
/*    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
/*    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
/*    EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
/*
/***********************************************************************************************************************/