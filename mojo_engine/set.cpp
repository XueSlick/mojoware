/***********************************************************************************************************************
/*
/*    set.cpp / mojo_engine
/*
/*    Whenever the application changes one of the engine's settings during run-time, the change
/*    is made through the overridden choke-point function set() in this file.
/*
/*    This gives the engine a chance to alter its behavior after every setting change with the function
/*    on_setting_changed();
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/


#include "stdafx.h"

using namespace mojo;

//======================================================================================================================
//  CODE
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
//  ON SETTING CHANGED
//  This gets called every time a setting changes.
//----------------------------------------------------------------------------------------------------------------------
bool on_setting_changed ( const wchar_t * pName )
{
	//-------------------------------
	// RAISE PROCESS PRIORITY
	//-------------------------------

	if ( 0 == wcscmp ( L"bRaiseProcessPriority", pName ) )
	{
		if ( g_Settings.bRaiseProcessPriority )
		{
			if ( ! SetPriorityClass ( GetCurrentProcess(), HIGH_PRIORITY_CLASS ) )
			{
				LOG_SYSTEM_ERROR_T ( L"SetPriorityClass" );
				put_ad_lib_memo ( cMemo::error, L"Change setting", L"Unable to raise process priority." );
			}

			else
				put_ad_lib_memo ( cMemo::success, L"Change setting", L"Process priority has been raised." );
		}

		else
		{
			if ( ! SetPriorityClass ( GetCurrentProcess(), NORMAL_PRIORITY_CLASS ) )
			{
				LOG_SYSTEM_ERROR_T ( L"SetPriorityClass" );
				put_ad_lib_memo ( cMemo::error, L"Change setting", L"Unable to set process priority." );
			}

			else
				put_ad_lib_memo ( cMemo::success, L"Change setting", L"Process priority has been set to normal." );
		}
	}

	return true;
}


//----------------------------------------------------------------------------------------------------------------------
//  SET (BOOL)
//----------------------------------------------------------------------------------------------------------------------
bool mojo :: set ( const wchar_t * pName, bool bNewVal )
{
	if ( ! g_Settings.set_val_from_name ( pName, &bNewVal, cSettingsBase::boolean ) )
		return false;

	return on_setting_changed ( pName );
}

//----------------------------------------------------------------------------------------------------------------------
//  SET (UINT)
//----------------------------------------------------------------------------------------------------------------------
bool mojo :: set ( const wchar_t * pName, UINT uNewVal )
{
	if ( ! g_Settings.set_val_from_name ( pName, &uNewVal, cSettingsBase::uint ) )
		return false;

	return on_setting_changed ( pName );
}

// namespace


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