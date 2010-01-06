/***********************************************************************************************************************
/*
/*    cDlgView.h / mojo_app
/*
/*    Abstract base class for cDlgToons, cDlgTeams, etc.
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#pragma once

#include "cDlg.h"
#include "cListView.h"


//----------------------------------------------------------------------------------------------------------------------
//  CLASS
//----------------------------------------------------------------------------------------------------------------------
class cDlgView : public cDlg
{
public:

	void populate () { pListView->populate(); }
	virtual int idd () { return IDD_VIEW; }
	cDlgView ( cListView * pLV ) : pListView (pLV) {}
	virtual DialogProc * dialog_proc () { return dialog_proc; }
	static DialogProc dialog_proc;

protected:



private:
	void draw_strip ( HDC hdc, const wchar_t * pText );
	virtual cConfigItemList * item_list () = 0;
	void toggle_view ();
	void wm_paint ();
	void wm_init ();

	cListView * pListView;
	cWin ToggleView;
	cWin Add;
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