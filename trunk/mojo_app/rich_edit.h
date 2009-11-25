/***********************************************************************************************************************
/*
/*    rich_edit.h / mojo_app
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/


#pragma once



//======================================================================================================================
//  PROTOTYPES
//======================================================================================================================

void rich_edit_append_line 			( HWND hwnd, const wchar_t * pText );
void rich_edit_set_color 			( HWND hwnd, COLORREF color, bool bSelection = true );
void rich_edit_set_bold 			( HWND hwnd, bool bSelection = true );
void rich_edit_set_weight_regular 	( HWND hwnd, bool bSelection = true );
void rich_edit_set_indent 			( HWND hwnd, int iQty );
void rich_edit_set_face				( HWND hwnd, const wchar_t * pFace, bool bSelection = true );
void rich_edit_set_height			( HWND hwnd, int iHeight, bool bSelection = true );
bool rich_edit_last_char_is_visible ( HWND h );


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