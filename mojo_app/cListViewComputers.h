/***********************************************************************************************************************
/*
/*    cListViewPC.h / mojo_app
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#pragma once

#include "cWin.h"

//----------------------------------------------------------------------------------------------------------------------
// CLASS CTRL LIST VIEW PC
//----------------------------------------------------------------------------------------------------------------------
class cListViewComputers : public cWin
{
public:

	cListViewComputers ();
	void               init ();
	void               populate ( cMachlist * pMachList );
	void               set_item ( int iIndex, cMach * pMach );
	int                mach_to_icon_index ( cMach * pM );
	void               insert_image_lists ( HWND hwnd );
	void               set_view ( DWORD dwView );
	const wchar_t *    mach_to_connection_state_text ( cMach * p );
	DWORD              hot_mach ();

private:



	HIMAGELIST	hImageListLarge;
	HIMAGELIST	hImageListState;

	int iQtyColumns;
	bool bStateImages;
};

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