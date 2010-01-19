/********************************************************************************************************
/*
/*    utility.cpp / mojo_app
/*
/*    Copyright 2009 Robert Sacks
/*
/*    This file is part of Mojo.  You may redistribute and/or modify Mojo under the terms of the GNU 
/*    General Public License, version 3, as published by the Free Software Foundation.  You should have
/*    received a copy of the license with mojo.  If you did not, go to http://www.gnu.org.
/*
/*    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR 
/*    IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND 
/*    FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
/*    CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
/*    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
/*    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER 
/*    IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT 
/*    OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
/*
/********************************************************************************************************/


#include "stdafx.h"
#include <commctrl.h>

using namespace mojo;



//----------------------------------------------------------------------------------------------------------------------
//  FILE EXISTS
//----------------------------------------------------------------------------------------------------------------------
bool file_exists ( const wchar_t * pPathname )
{
	if ( INVALID_FILE_ATTRIBUTES == GetFileAttributesW ( pPathname ) )
		return false;

	else
		return true;
}



//----------------------------------------------------------------------------------------------------------------------
//  SET MENU ITEM TEXT
//----------------------------------------------------------------------------------------------------------------------
void set_menu_item_text ( HWND hwnd, unsigned uID, const wchar_t * pTxt )
{
	HMENU hMenu = GetMenu ( hwnd );
	MENUITEMINFO mii;
	memset ( &mii, 0, sizeof ( mii ) );
	mii.cbSize = sizeof(mii);
	mii.fMask = MIIM_STRING;
	mii.dwTypeData = (LPWSTR) pTxt;
	SetMenuItemInfo ( hMenu, uID, FALSE, &mii );
}


//----------------------------------------------------------------------------------------------------------------------
//  SHOW APP DATA FILE
//----------------------------------------------------------------------------------------------------------------------
void show_app_data_file ( const wchar_t * pName )
{
	cStrW s;
	mojo::get_our_local_app_data_directory ( &s, g_awAppTitle );
	s += pName;
	open_text_file ( s.cstr() );
}


//----------------------------------------------------------------------------------------------------------------------
//  SHOW TEXT FILE
//----------------------------------------------------------------------------------------------------------------------
void open_text_file ( const wchar_t * pName )
{
	HINSTANCE result = ShellExecute ( NULL, L"open", pName,  NULL, NULL, SW_SHOW );

	if ( 32 < (int) result )
		return;

	switch ( (int) result )
	{
	case ERROR_FILE_NOT_FOUND:
		mojo::put_ad_lib_memo ( cMemo::error,  L"Cannot open file", L"File not found.\n" L"%s", pName );
		break;

	case ERROR_PATH_NOT_FOUND:
		mojo::put_ad_lib_memo ( cMemo::error,  L"Cannot open file", L"Path not found.\n" L"%s", pName );
		break;

	case SE_ERR_ACCESSDENIED:
		mojo::put_ad_lib_memo ( cMemo::error,  L"Cannot open file", L"The operating system denied access to the file.\n" L"%s", pName );
		break;

	case SE_ERR_ASSOCINCOMPLETE:
		mojo::put_ad_lib_memo ( cMemo::error,  L"Cannot open file", L"File name association is incomplete or invalid.\n" L"%s", pName );
		break;

	default:
		mojo::put_ad_lib_memo ( cMemo::error,  L"Cannot open file", L"Operating system function ShellExecute cannot open the file.\n" L"%s", pName );
		break;
	}
}




//-------------------------------------------------------------------------------------------------------
//  GET CURSOR MESSAGE POS
//  From Raymond Chen's blog
//-------------------------------------------------------------------------------------------------------
#if 0
void get_client_cursor_message_pos ( POINT * pt, HWND hwnd )
{
	DWORD dwPos = GetMessagePos();
	(*pt) = { GET_X_LPARAM(dwPos), GET_Y_LPARAM(dwPos) };
	ScreenToClient ( hwnd, pt );
}
#endif


//-------------------------------------------------------------------------------------------------------
//  GET DEFAULT GUI FONT FACE
//-------------------------------------------------------------------------------------------------------
void get_default_gui_font_face ( cStrW * pRet )
{
	HFONT hFont = (HFONT) GetStockObject(DEFAULT_GUI_FONT);
	HDC hdc = GetDC ( g_hwnd );
	HFONT hOldFont = (HFONT) SelectObject ( hdc, (HGDIOBJ) hFont );
	wchar_t t[1000];
	GetTextFace ( hdc, sizeof(t)/sizeof(wchar_t)-1, t );
	SelectObject ( hdc, (HGDIOBJ) hOldFont );
	ReleaseDC ( g_hwnd, hdc );
	*pRet = t;
}


//-------------------------------------------------------------------------------------------------------
//  PRINT
//  Prints message in main monitor window.
//-------------------------------------------------------------------------------------------------------
void print ( const wchar_t * pTxt, ... )
{
	UNREFERENCED_PARAMETER ( pTxt );
#if 0
	va_list args;
	va_start ( args, pTxt );
	cMemo m;
	m.eSeverity = cMemo::info;
	m.head = L"Print";
	m.body.f ( pTxt , args );
	put_memo ( &m );
#endif
}


//-------------------------------------------------------------------------------------------------------
//  PRINT
//  Prints message in main monitor window.
//-------------------------------------------------------------------------------------------------------
void print ( cMemo::_eSeverity eSevArg, const wchar_t * pTxt, ... )
{
	UNREFERENCED_PARAMETER ( pTxt );
	UNREFERENCED_PARAMETER ( eSevArg );
#if 0
	va_list args;
	va_start ( args, pTxt );
	cMemo m;
	m.eSeverity = eSevArg;
	m.head = L"Print";
	m.body.f ( pTxt , args );
	put_memo ( &m );
#endif
}


//-------------------------------------------------------------------------------------------------------
// MESSAGE BOX
//-------------------------------------------------------------------------------------------------------
void message_box ( cMemo * m )
{
	cStrW s;

	if ( wcslen ( m->head() ) )
		s = m->head();

	if ( wcslen ( m->body() ) )
	{
		s += L"\n\n";
		s += m->body();
	}

	if ( 0 == s.len() )
		s = L"An error has occurred that prevents this message \n"
		    L"from being displayed properly.";

	s += L"\n\n\nerror code: ";
	s += wcslen( m->key() ) ? m->key() : L"NULL";

	message_box ( s.cstr() );
}

//-------------------------------------------------------------------------------------------------------
// MESSAGE BOX
//-------------------------------------------------------------------------------------------------------
void message_box ( const wchar_t * p )
{
	MessageBox ( g_hwnd, p, g_awAppTitle, MB_OK | MB_ICONINFORMATION );

#if 0
	cDlgMessageBox d;
	d.make_dlg ( (void*) p );
#endif
}


//-------------------------------------------------------------------------------------------------------
// GET MENU FONT
//-------------------------------------------------------------------------------------------------------
HFONT create_menu_hfont ()
{
	NONCLIENTMETRICS ncm;
	memset( & ncm, 0, sizeof(ncm) );
	ncm.cbSize = sizeof ( ncm );
	SystemParametersInfo ( SPI_GETNONCLIENTMETRICS, sizeof(ncm), &ncm, 0 );
	
	return ( CreateFontIndirect ( &ncm.lfMenuFont ) );
}


//-------------------------------------------------------------------------------------------------------
//   IS USER ADMIN
//   Microsoft code
//-------------------------------------------------------------------------------------------------------
bool is_user_admin ()
{
	/* 
	Routine Description: This routine returns TRUE if the caller's
	process is a member of the Administrators local group. Caller is NOT
	expected to be impersonating anyone and is expected to be able to
	open its own process and process token. 
	Arguments: None. 
	Return Value: 
	TRUE - Caller has Administrators local group. 
	FALSE - Caller does not have Administrators local group. --
	*/ 

	BOOL b;
	SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
	PSID AdministratorsGroup; 

	b = AllocateAndInitializeSid (
		&NtAuthority,
		2,
		SECURITY_BUILTIN_DOMAIN_RID,
		DOMAIN_ALIAS_RID_ADMINS,
		0, 0, 0, 0, 0, 0,
		&AdministratorsGroup ); 

	if ( b ) 
	{
		if ( ! CheckTokenMembership ( NULL, AdministratorsGroup, &b ) ) 
		{
			 b = FALSE;
		} 
		FreeSid ( AdministratorsGroup ); 
	}

	return  ( b ? true : false );
}


//-------------------------------------------------------------------------------------------------------
// INIT COMMON CONTROLS
//-------------------------------------------------------------------------------------------------------
void init_common_controls ()
{
	INITCOMMONCONTROLSEX sex;

	ZeroMemory ( &sex, sizeof(sex) );
	sex.dwSize = sizeof(sex);
	sex.dwICC = ICC_LISTVIEW_CLASSES | ICC_BAR_CLASSES | ICC_STANDARD_CLASSES;
	InitCommonControlsEx ( &sex );
}
