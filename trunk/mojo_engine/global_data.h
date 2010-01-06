/********************************************************************************************************
/*
/*    global_data.h / mojo_engine
/*
/*    All of the DLL's global data is declared in this file.
/*
/*    Copyright 2009 Robert Sacks
/*
/*    This file is part of Mojo.  You may redistribute and/or modify Mojo under the terms of the GNU 
/*    General Public License, version 3, as published by the Free Software Foundation.  You should have
/*    received a copy of the license with mojo.  If you did not, go to http://www.gnu.org.
/*
/*    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR 
/*    IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND 
/*    FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
/*    CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
/*    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
/*    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER 
/*    IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT 
/*    OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
/*
/********************************************************************************************************/


//=======================================================================================================
// DEFINES
//=======================================================================================================

#define                     awENGINE_TITLE L"mojo_engine"


//=======================================================================================================
// DATA
//=======================================================================================================

extern mojo::cVersion		   g_Version;
extern mojo::cLog			   g_Log;
extern mojo::cStrW			   g_sAppTitle;
extern mojo::cStrW			   g_sAppDataDirectory;
extern const wchar_t * const   g_pwEngineTitle;
extern mojo::cScribMgr         g_ScribMgr;
extern cSettings			   g_Settings;
extern HWND					   g_hwndApp;
extern DWORD 				   g_dwHookThreadID;
extern cInputEventBuffer	   g_EventBuffer;
extern mojo::cMachlist		   g_Machlist;
extern cPool				   g_Pool;
extern HINSTANCE			   g_hAppInstance;
extern cMessenger			   g_Messenger;
extern cMemoBuffer			   g_MemoBuffer;
extern const wchar_t * 		   g_apDefaultScribs [];
extern cFinder                 g_Finder;
extern cKeyBroadcaster         g_KeyBroadcaster;
extern cMouseover              g_Mouseover;
extern cKeyState			   g_KeyState;
extern cTargetMgr              g_TargetMgr;