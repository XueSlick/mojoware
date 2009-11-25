/********************************************************************************************************
/*
/*    stdafx.h / mojo_app
/*
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#pragma once

//=======================================================================================================
//  INCLUDES
//=======================================================================================================

//---------------------------------
//  DEFINES
//---------------------------------


//---------------------------------
//  TARGET VERSION
//---------------------------------

#include "targetver.h"

//---------------------------------
//  MEMORY DEBUG (in this order)
//---------------------------------

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

//---------------------------------
//  WINDOWS
//---------------------------------

#include <windows.h>
#include <shellapi.h>
#include <tchar.h>
#include <commctrl.h>
#include <Dbghelp.h>
#include <windowsx.h>
#include <gdiplus.h>


//---------------------------------
//  C RUNTIME LIBRARY
//---------------------------------

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

//---------------------------------
//  MOJO ENGINE
//---------------------------------

#include "..\\mojo_engine\\mojo_engine.h"

//---------------------------------
//  THIS PROJECT
//---------------------------------

#include "cCrashHandler.h"
#include "resource.h"
#include "utility.h"
#include "rich_edit.h"
#include "window_stuff.h"
#include "global_data_version.h"
#include "cSettings.h"
#include "balloon.h"

//---------------------------------
//  THIS PROJECT'S WINDOW CLASSES
//---------------------------------

// general

#include "cWin.h"
#include "cWinLabel.h"
#include "cDlg.h"
#include "cDlgVars.h"

// specific

#include "cWinMain.h"
#include "cDlgMessageBox.h"
#include "cDlgMonitor.h"
#include "cDlgAbout.h"
#include "cDlgLicense.h"
#include "cDlgSettingsConnection.h"
#include "cDlgSettingsConnectionAdvanced.h"

//---------------------------------
//  GLOBAL DATA
//---------------------------------

#include "global_data.h" // keep this last


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