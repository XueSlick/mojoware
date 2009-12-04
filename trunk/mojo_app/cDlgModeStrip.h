/***********************************************************************************************************************
/*
/*    cDlgModeStrip.h / mojo_app
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#pragma once

#include "cDlg.h"
#include "cLiquidButton.h"

//----------------------------------------------------------------------------------------------------------------------
//  CLASS cDlgAbout
//----------------------------------------------------------------------------------------------------------------------
class cDlgModeStrip : public cDlg
{
public:

	virtual int idd () { return IDD_MODE_STRIP; }
	virtual DialogProc * dialog_proc () { return dialog_proc; }
	static DialogProc dialog_proc;

	int get_height ();

	mojo::cStrW sText;

	void redraw_buttons (); // can be called by parent window

private:

	cLiquidButton ToggleMouseover;
	cLiquidButton ToggleHotkeys;
	cLiquidButton ToggleBroadcast;

	void wm_init ();
	void wm_drawitem ( int iID, DRAWITEMSTRUCT* pDI );
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