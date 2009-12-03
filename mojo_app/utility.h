/***********************************************************************************************************************
/*
/*    utility.h / mojo_app
/*
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#pragma once

//======================================================================================================================
//  PROTOTYPES
//======================================================================================================================

void            open_text_file ( const wchar_t * pName );
void 			init_common_controls ();
void 			move_win_to_center ( HWND hwnd );
mojo::cPtI 		get_window_size ( HWND hwnd );
bool 			is_user_admin ();
HFONT 			create_menu_hfont ();
void 			message_box ( const wchar_t * p );
void			message_box ( mojo::cMemo * p );
void			print ( const wchar_t * pTxt, ... ); // prints to monitor
void			print ( mojo::cMemo::_eSeverity, const wchar_t * pTxt, ... ); // prints to monitor
void            show_app_data_file ( const wchar_t * pName );
void            set_menu_item_text ( HWND hwnd, unsigned uID, const wchar_t * pTxt );
bool            file_exists ( const wchar_t * pPathname );



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