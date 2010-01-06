/***********************************************************************************************************************
/*
/*    cDlgVars.h / mojo_app
/*
/*    This class is used to conveniently create dialogs that change settings.   For each settings dialog there is
/*    an object that derives from this class, for example, cDlgSettingsConnections.  In the derived dialog class
/*    (e.g. in cDlgSettingsConnections) you must include:
/*
/*			(1) an array of sDlgDatum's (one for each control in the dialog that sets a datum);
/*			(2) a call to this class's wm_init when the dialog receives WM_INITDIALOG;
/*			(3) a call to this class's dlg_to_settings() when the user presses "OK."
/*
/*	  This class identifies the data type of each datum from g_Settings.  From the data type, it infers the kind
/*    of control.  For example, it assumes that a boolean variable in g_Settings matches a check box, and that
/*    an int in g_Settings matches an edit control.
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#pragma once

#include "cWin.h"


//======================================================================================================================
//  DLG DATA
//======================================================================================================================

struct sDlgDatum
{
	int 			iID;
	const wchar_t * pName;
	sDlgDatum 		( int iArgID, const wchar_t * pArgName ) : iID ( iArgID ), pName ( pArgName ) {}
};


//======================================================================================================================
//  MAIN CLASS
//======================================================================================================================

class cDlgVars
{
protected:

	void			wm_init 		( HWND h, sDlgDatum * pDlgDataArg );
	void			settings_to_dlg ( HWND h, cSettings * );
	void			dlg_to_settings ( cSettings *, HWND h );

	sDlgDatum * pDlgData; // table of sDlgDatum's
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