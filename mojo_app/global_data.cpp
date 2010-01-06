/********************************************************************************************************
/*
/*    global_data.cpp / mojo_app
/*
/*    All of the EXE's global data is defined in this file.
/*
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"


//=====================================================================================================================
//  GLOBAL DATA
//======================================================================================================================

cVersion			g_Version ( mojo::uVERSION_MAJ, mojo::uVERSION_MIN, mojo::uVERSION_PIP, mojo::uVERSION_BLD );
cCrashHandler		g_CrashHandler;
cLog				g_Log ( L"mojo", L"mojo", &g_Version );
cGdiPlus			g_GdiPlus;
HINSTANCE			g_hInstance;
HWND				g_hwnd;
wchar_t				g_awAppTitle[] = L"Mojo";
wchar_t				g_awWebsite[] = L"http://mojoware.org";
cWinMain			g_WinMain;
cStrW				g_sAppDataDirectory;
HFONT				g_hMenuFont;
HFONT				g_hCaptionFont;
HFONT				g_hSubheadFont;
HFONT				g_hHeadFont;
HFONT				g_hDialogBoxHeadFont;
NONCLIENTMETRICS    g_NonClientMetrics;
const wchar_t * 	g_apDefaultScribs [];
cSettings			g_Settings ( g_awAppTitle, & g_Version );
cFigMgr				g_FigMgr;

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