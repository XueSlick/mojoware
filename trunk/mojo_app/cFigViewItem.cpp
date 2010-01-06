/***********************************************************************************************************************
/*
/*    cFigViewItem.cpp / mojo_app
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"


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
//  GET ITEM
//----------------------------------------------------------------------------------------------------------------------
cFigViewItem * cFigViewItemList :: get_item_clone ( DWORD hHandle ) const
{
	cFigViewItem * pRet = 0;

	lock();
	{
		for ( cFigViewItem * p = pHead; p; p = p->pNext )
		{
			if ( p->dwSerialNumber == hHandle )
			{
				pRet = p->clone();
				break;
			}
		}
	}
	unlock();

	return pRet;
}



//----------------------------------------------------------------------------------------------------------------------
//   SHOW CONTEXT MENU
//----------------------------------------------------------------------------------------------------------------------
int cFigViewItem :: show_context_menu ( HWND hwnd )
{
	HMENU hMenu =  LoadMenu ( g_hInstance, MAKEINTRESOURCE ( menu() ) );
	HMENU hSubMenu = GetSubMenu ( hMenu, 0 );
	POINT pt;
	GetCursorPos ( &pt );
	return TrackPopupMenuEx ( hSubMenu, TPM_RETURNCMD | TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, hwnd, NULL );
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