/***********************************************************************************************************************
/*
/*    click_once.cpp
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"


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
//  CREATE CLICK ONCE SHORTCUT
//----------------------------------------------------------------------------------------------------------------------
void create_click_once_shortcut ()
{
	cStrW sSource = L"\\\\?\\";
	cStrW sDest   = L"\\\\?\\";

	mojo::get_start_menu_directory( &sSource);
	sSource += L"Programs\\Mojoware\\Mojo\\Mojo.appref-ms";

	mojo::get_desktop_directory ( &sDest );
	sDest += L"Mojo.appref-ms";

	if ( 0 == CopyFile ( sSource.cstr(), sDest.cstr(), FALSE ) )
	{
		LOG_SYSTEM_ERROR_T( L"CopyFile" );
		mojo::put_ad_lib_memo ( mojo::cMemo::error, L"Making shortcut", L"Unable to create shortcut for Mojo on desktop" );
	}

	else
		mojo::put_ad_lib_memo ( mojo::cMemo::success, L"Making shortcut", L"Shortcut for Mojo placed on desktop." );
}



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