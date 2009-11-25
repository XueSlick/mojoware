/***********************************************************************************************************************
/*
/* 	  global_data.h / mojo_app
/*
/*    All of the EXE's global data is declared in this file. 
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/


//======================================================================================================================
// DATA
//======================================================================================================================

extern cCrashHandler		g_CrashHandler;
extern cLog					g_Log;
extern HINSTANCE 			g_hInstance;
extern HWND					g_hwnd;
extern wchar_t 				g_awAppTitle [];
extern wchar_t				g_awWebsite[];
extern mojo::cVersion		g_Version;
extern cWinMain				g_WinMain;
extern mojo::cStrW			g_sAppDataDirectory;
extern HFONT				g_hMenuFont;
extern HFONT				g_hCaptionFont;
extern HFONT				g_hSubheadFont;
extern HFONT				g_hHeadFont;
extern NONCLIENTMETRICS		g_NonClientMetrics;
extern const wchar_t * 		g_apDefaultScribs [];
extern cSettings			g_Settings;

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