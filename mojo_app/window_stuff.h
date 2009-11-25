/***********************************************************************************************************************
/*
/*   window_stuff.h / mojo_app
/*
/*   This file contains utility functions for moving windows, finding sizes of windows, etc.
/*   Basically these functions wrap Win32 calls for a little more convenience.   The Win32
/*   functions can be used interchangeably with these.
/*
/*   Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#pragma once

#include "..\\mojo_engine\\tRect.h"

//=======================================================================================================
//  PROTOTYPES
//=======================================================================================================

void 			set_dialog_focus ( HWND hwnd, int iItemID );
bool 			client_coord_to_parent_client_coord ( HWND hwnd, mojo::cPtI * pPt );
void 			get_client_offset ( mojo::cPtI * pClientOffset, HWND hwnd );
bool 			is_topmost ( HWND h );
void 			get_window_size ( int * pRetX, int * pRetY, HWND hwnd );
void 			move_win_to_center ( HWND hwnd );
void 			set_main_window_size ( HWND hwnd, int x, int y );
void 			get_window_rect ( mojo::cRectI * pRect, HWND hwnd );
void 			move_window ( HWND hwnd, mojo::cPtI to );
void 			move_window ( HWND hwnd, int x, int y );
mojo::cPtI 		get_client_size ( HWND hwnd );
mojo::cPtI 		get_window_size ( HWND hwnd );
mojo::cPtI 		get_window_position ( HWND hwnd );
mojo::cPtI 		get_screen_size ();
unsigned 		get_show_state ( HWND hwnd );
wchar_t * 		get_window_text ( mojo::cStrW * pRet, HWND hwnd );
bool 			set_child_window_size ( HWND hChild, int iDimX, int iDimY );


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