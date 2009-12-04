/***********************************************************************************************************************
/*
/*    set_privilege.cpp
/*
/*    This file is not used in the current build.
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"


//======================================================================================================================
//  CODE
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
// SET PRIVILEGE
// Microsoft code from MSDN
//----------------------------------------------------------------------------------------------------------------------
BOOL set_privilege	(
    HANDLE hToken,          // access token handle
    LPCTSTR lpszPrivilege,  // name of privilege to enable/disable
    BOOL bEnablePrivilege   // to enable or disable privilege
    ) 
{
	if ( ! hToken )
		return FALSE;

	TOKEN_PRIVILEGES tp;
	LUID luid;

	if ( !LookupPrivilegeValue( 
        NULL,            // lookup privilege on local system
        lpszPrivilege,   // privilege to lookup 
        &luid ) )        // receives LUID of privilege
	{

#ifdef DEBUG_KILL_MUTEX
			LOG_SYSTEM_ERROR_T ( "LookupPrivilegeValue" );
#endif
		return FALSE; 
	}

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;

	if (bEnablePrivilege)
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	else
		tp.Privileges[0].Attributes = 0;

	// Enable the privilege or disable all privileges.

	if ( !AdjustTokenPrivileges(
		 hToken, 
		 FALSE, 
		 &tp, 
		sizeof(TOKEN_PRIVILEGES), 
		(PTOKEN_PRIVILEGES) NULL, 
		(PDWORD) NULL) )
	{
#ifdef DEBUG_KILL_MUTEX
			LOG_SYSTEM_ERROR_T ( "AdjustTokenPrivileges" );
#endif

		return FALSE; 
	} 

	if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)

	{
#ifdef DEBUG_KILL_MUTEX
		LOG ( "The token does not have the specified privilege.");
#endif
		return FALSE;
	} 

	return TRUE;
}


//----------------------------------------------------------------------------------------------------------------------
// GET THREAD TOKEN
//----------------------------------------------------------------------------------------------------------------------
bool get_thread_token ( HANDLE * phToken)
{
	if(!OpenThreadToken(GetCurrentThread(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, FALSE, phToken))
    {
        if (GetLastError() == ERROR_NO_TOKEN)
        {
            if (!ImpersonateSelf(SecurityImpersonation))
				return false;

            if(!OpenThreadToken(GetCurrentThread(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, FALSE, phToken))
			{
                return false;
            }
		}
     }
	
	return true;
}


/***********************************************************************************************************************
/*
/*    This file is part of Mojo.  For more information, see http://mojoware.org.
/*
/*    You may redistribute and/or modify Mojo under the terms of the GNU General Public License, version 3, as
/*    published by the Free Software Foundation.  You should have received a copy of the license with Mojo.  If you
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