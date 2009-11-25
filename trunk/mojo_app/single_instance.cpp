/***********************************************************************************************************************
/*
/*    single_instance.cpp / mojo_app
/*
/*    This code ensures that only one instance of this program is running.  It's adapted from an article by 
/*    Joseph M. Newcomer at http://www.codeproject.com/cpp/avoidmultinstance.asp
/*
/*    INSTRUCTIONS:
/*
/*	  1.  Make a uuid with uuidgen.exe in the bin directory of the SDK.
/*
/*	  2.  Paste the uuid into the RegisterWindowMessage call below.
/*
/*	  3.  Paste the uuid into the mutex definition below.
/*
/*	  4.  Adjust the message return value in the callback below to reflect
/*		  dialog box app or window ap.
/*
/*	  5.  Handle uWM_ARE_YOU_ME in the main window proc.  We return
/*		  the same uWM_ARE_YOU_ME.
/*
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"
#include "process.h"

//======================================================================================================================
// DATA
//======================================================================================================================

// GUID obtained for mojo nov 5 2009
unsigned uWM_ARE_YOU_ME = ::RegisterWindowMessage ( L"UWM_ARE_YOU_ME-{bf4acab0-e885-47c9-a326-3128ea7b16a5}");


//======================================================================================================================
//  PROTOTYPES
//======================================================================================================================

static BOOL CALLBACK callback ( HWND hWnd, LPARAM lParam );


//======================================================================================================================
//  CODE
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
//  CALLBACK
//----------------------------------------------------------------------------------------------------------------------
BOOL CALLBACK callback ( HWND hWnd, LPARAM lParam )
{
    DWORD_PTR result;
    LRESULT ok = ::SendMessageTimeout ( hWnd,
                                        uWM_ARE_YOU_ME,
                                        0, 0, 
                                        SMTO_BLOCK | SMTO_ABORTIFHUNG,
                                        200,
                                        &result);
    if ( 0 == ok )
		return TRUE; // ignore this and continue

    if ( result == uWM_ARE_YOU_ME )  // this doesn't work with dialog box proc's because
	                                 // dialog box proc's only return BOOLS
	// if ( result == TRUE )         // use <--- this instead with dialog box programs
    {
        HWND * target = (HWND *)lParam;  // FOUND IT
        *target = hWnd;
        return FALSE;                    // stop search
    }

    return TRUE; // continue search
} 


//----------------------------------------------------------------------------------------------------------------------
// PREVIOUS INSTANCE IS RUNNING
//----------------------------------------------------------------------------------------------------------------------
bool previous_instance_is_running ()
{
    bool bAlreadyRunning;

	// GUID obtained for mojo nov 5 2009
    HANDLE hMutexOneInstance = ::CreateMutex ( NULL, FALSE, L"mojo{bf4acab0-e885-47c9-a326-3128ea7b16a5}" );

	UNREFERENCED_PARAMETER ( hMutexOneInstance );

          // what changes for the alternative solutions
          // is the UID in the above call
          // which will be replaced by a call on
          // createExclusionName

    bAlreadyRunning = ( ::GetLastError() == ERROR_ALREADY_EXISTS || 
                        ::GetLastError() == ERROR_ACCESS_DENIED);

    // The call fails with ERROR_ACCESS_DENIED if the Mutex was 
    // created in a different users session because of passing
    // NULL for the SECURITY_ATTRIBUTES on Mutex creation);

    if ( bAlreadyRunning )
    { 
	    HWND hOther = NULL;
        EnumWindows ( callback, (LPARAM) &hOther );

        if ( hOther != NULL )
        { 
			// previous instance is running so make it
			// visible and bring to foreground

			ShowWindow ( hOther, SW_RESTORE );
			ShowWindow ( hOther, SW_NORMAL );
			SetForegroundWindow ( hOther );		
        } 

        return true;
    } 

    return false;
} 


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