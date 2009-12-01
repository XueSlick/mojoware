/***********************************************************************************************************************
/*
/*    cDlgMonitor.h / mojo_app
/*
/*    Wrapper for the "monitor dialog" which is one of the dialog boxes that fills the main window.  The monitor
/*    dialog has three sections:  input events, connections, and a scrolling stream of status messages.
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#pragma once

#include "cDlg.h"
#include "cListViewConnections.h"

//----------------------------------------------------------------------------------------------------------------------
//  CLASS DIALOG EVENT MONITOR
//----------------------------------------------------------------------------------------------------------------------
class cDlgMonitor : public cDlg
{
	friend class cWinMain;



private:

	bool set_region ();

	cWin InputEvents;
	cWin InputEventsHead;
	cWin Memos;
	cWin MemosHead;
	cWin MemosHeadT;
	cWin MemosHeadR;
	cWin MemosHeadB;
	cWin Clear;
	cWin ConnectionsHead;
	cWin RedX;
	cListViewConnections Connections;

	COLORREF get_memo_color ( cMemo * pM );
	virtual int idd () { return IDD_MONITOR; }
	virtual DialogProc * dialog_proc () { return dialog_proc; }
	static DialogProc dialog_proc;
	void show_memo ();
	void wm_paint ();
	void wm_initdialog ();
	void draw_head ( cWin * pHead );
	virtual void set_text();
	void draw_red_X ();
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