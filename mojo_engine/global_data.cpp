/***********************************************************************************************************************
/*
/*    global_data.cpp / mojo_engine
/*
/*    All of the DLL's global data is defined in this file.
/*
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"
#include "..//mojo_app//global_data_version.h"

using namespace mojo;

//======================================================================================================================
//  GLOBAL DATA
//======================================================================================================================

cVersion			g_Version ( mojo::uVERSION_MAJ, mojo::uVERSION_MIN, mojo::uVERSION_PIP, mojo::uVERSION_BLD );
mojo::cStrW		g_sAppDataDirectory;
mojo::cStrW		g_sAppTitle;
wchar_t				g_awDllTitle [] = L"mojo_engine";
cScribMgr     		g_ScribMgr;

cLog				g_Log ( L"mojo", g_awDllTitle, &g_Version );
cSettings			g_Settings;
HWND 				g_hwndApp;
DWORD 				g_dwHookThreadID;
cInputEventBuffer	g_EventBuffer; // mouse and keyboard events
cMachlist			g_Machlist;
cPool				g_Pool;
HINSTANCE			g_hAppInstance;
cMessenger			g_Messenger;
cMemoBuffer			g_MemoBuffer;
const wchar_t * 	g_apDefaultScribs [];
cFinder			g_Discovery;


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