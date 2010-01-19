/***********************************************************************************************************************
/*
/*    Settings.h / mojo_app
/*
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#pragma once

#include "..\\mojo_engine\\cSettingsBase.h"


//======================================================================================================================
// CLASSES
//======================================================================================================================

extern wchar_t g_awAppTitle [];
extern class mojo::cVersion g_Version;

class cSettings : public mojo::cSettingsBase
{
public:

	cSettings           ( const wchar_t * pSignature = g_awAppTitle, mojo::cVersion * pVersion = &g_Version ) : cSettingsBase ( pSignature, pVersion ) {}

	//---------------------------------------------------
	// PUBLIC DATA
	//---------------------------------------------------

	bool				bLicenseAccepted;
	bool				bConnect;
	bool 				bConnectAutomatically;
	bool				bUseSpecifiedLocalIP;
	unsigned			uSpecifiedLocalIP;
	unsigned			uPort;
	bool				bUseNagle;
	bool                bWindowBroadcastIsOn;
	bool                bComputerBroadcastIsOn;
	bool                bShowIpOnMouseoverSettings;
	bool                bMouseoverDeactivatesScreenSavers;
	bool                bHideRemoteCursor;
	bool                bHotkeysAreOn;
	bool                bMouseoverIsOn;
	unsigned            uFinderBroadcastInterval;
	bool				bRaiseProcessPriority;
	bool				bMaximizeTimerResolution;
	unsigned            uView;
	unsigned			uToonIconWidth;
	unsigned			uToonIconHeight;

	//-----------------------------------
	// DEFINE FOLLOWING IN DERIVED CLASS
	//-----------------------------------

	virtual sEntry * 	table ( int i = 0 );
	virtual int			qty_entries ();
	virtual void		init ();
	static sEntry 		aTable [];
};


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