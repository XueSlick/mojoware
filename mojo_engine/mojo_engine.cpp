/***********************************************************************************************************************
/*
/*    mojo_engine.cpp / mojo_engine
/*
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/***********************************************************************************************************************/


#include "stdafx.h"
#include <vector>

bool get_ip_addresses ( mojo::cArrayU * pRet );

using namespace mojo;


//----------------------------------------------------------------------------------------------------------------------
//  SET MOUSEOVER LAYOUT
//----------------------------------------------------------------------------------------------------------------------
void mojo::set_mouseover_layout ( cMachlist * pArgList )
{
	g_Machlist.lock();
	{
		for ( cMach * pMach = g_Machlist.pHead; pMach; pMach = pMach->pNext )
		{
			cMach * pArgMach = pArgList->get_by_ip ( pMach->dwIP );

			if ( ! pArgMach )
			{
				assert(0);
				continue;
			}

			pMach->DrawPos = pArgMach->DrawPos;
			pMach->bValidDrawPos = pArgMach->bValidDrawPos;
		}
	}
	g_Machlist.unlock();

	g_Machlist.save_draw_positions_to_file ();
	// SendMessage	( g_hwndApp, uWM_SET_MOUSEOVER_DISPLAY_LIST, 0, 0 ); //g_Mouseover.set_mega_display_list ( &g_Machlist );
	g_Mouseover.set_mega_display_list ( &g_Machlist );

}



//----------------------------------------------------------------------------------------------------------------------
//  LOAD DEFAULT ENGINE SCRIBS
//----------------------------------------------------------------------------------------------------------------------
#if 0
void mojo :: load_default_engine_scribs ()
{
	assert ( g_ScribMgr.load_scribs ( g_apDefaultScribs, false ) );
}
#endif


//----------------------------------------------------------------------------------------------------------------------
//  GET BROADCAST TARGETS
//----------------------------------------------------------------------------------------------------------------------
void mojo :: get_broadcast_targets ( cArrayTarget * pRet )
{
	g_KeyBroadcaster.get_broadcast_targets ( pRet );
}



//----------------------------------------------------------------------------------------------------------------------
//  TEST
//----------------------------------------------------------------------------------------------------------------------
void mojo :: test ()
{
}


//----------------------------------------------------------------------------------------------------------------------
//  GET MACHLIST
//----------------------------------------------------------------------------------------------------------------------
void mojo :: get_machlist ( mojo::cMachlist * pRet )
{
	g_Machlist.lock();
	(*pRet) = g_Machlist;
	g_Machlist.unlock();
}


//----------------------------------------------------------------------------------------------------------------------
//  GET CONNECTION STATUS
//----------------------------------------------------------------------------------------------------------------------
mojo::nConnectionStatus::eConnectionStatus mojo::get_connection_status ( DWORD dwMachHandle )
{
	// g_Machlist.handle_to
	DWORD dwIP = g_Machlist.handle_to_ip ( dwMachHandle );

	

	return (mojo::nConnectionStatus::eConnectionStatus) g_Pool.get_connection_status ( dwIP );
}


//----------------------------------------------------------------------------------------------------------------------
//  IP DWORD TO cStrW
//----------------------------------------------------------------------------------------------------------------------
void mojo :: ip_dword_to_cStrW	( cStrW * pRet, DWORD dw )
{
	wchar_t t [16];
	ip_dword_to_aw ( t, dw );
	*pRet = t;
}


//----------------------------------------------------------------------------------------------------------------------
//  IP WCHAR_T * TO DWORD
//----------------------------------------------------------------------------------------------------------------------
DWORD mojo :: ip_pw_to_dword ( const wchar_t * pAW )
{
	cStrW w = pAW;
	cStrN n ( w );
	return inet_addr ( n.cstr() );
}

//----------------------------------------------------------------------------------------------------------------------
//  GET IP ADDRESSES
//----------------------------------------------------------------------------------------------------------------------
bool mojo :: get_ip_addresses ( mojo::cArrayU * pRet )
{
	return ::get_ip_addresses ( pRet );
}


//----------------------------------------------------------------------------------------------------------------------
//  GET DLL VERSION
//----------------------------------------------------------------------------------------------------------------------
void mojo :: get_engine_version ( cVersion * pRetVersion )
{
	*pRetVersion = g_Version;	
}


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