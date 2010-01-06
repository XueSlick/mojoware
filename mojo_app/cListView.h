/***********************************************************************************************************************
/*
/*    cListView.h / mojo_app
/*
/*    Base class for cListViewToons, cListViewTeams, etc.
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#pragma once

#include "cWin.h"

//----------------------------------------------------------------------------------------------------------------------
// CLASS
//----------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------
// CLASS
//----------------------------------------------------------------------------------------------------------------------
class cListView : public cWin
{

public:

	cListView () : pList(0) {}
	~cListView ();

protected:

	virtual void                     create_columns () = 0;
	virtual void                     do_image_list ( const cFigViewItemList * pList ); // ( cConfigItemList * pList );
	virtual const cFigViewItemList * create_list () = 0;
	virtual void                     set_item ( cFigViewItem * pConfigItem );
	virtual mojo::cPtI               get_icon_size () = 0;
	virtual int                      icon_bitmap_id () = 0;
	virtual const int *              default_bitmap_ids () = 0;

public:

	void                             init ();
	void                             populate (); //  const cFigViewItemList * pList = 0 ); // ( const cConfigItemList * pList = 0 );
	void                             set_view ( DWORD dwView );
	DWORD                            hot_item ();
	void                             toggle_view ();

	const cFigViewItemList         * pList; // a copy; keep it alive until next time we populate

protected:

	HIMAGELIST	                     hImageListLarge;
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