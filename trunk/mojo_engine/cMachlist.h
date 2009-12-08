/***********************************************************************************************************************
/*
/*    cMachlist.h / mojo_engine
/*
/*    List of cMach's.
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#pragma once


#include "cMach.h"
#include "tList2.h"


//======================================================================================================================
// CLASSES
//======================================================================================================================

namespace mojo
{

//----------------------------------------------------------------------------------------------------------------------
// CLASS MACH
//----------------------------------------------------------------------------------------------------------------------
class MOJO_ENGINE_API cMachlist : public tList2<cMach>
{
public:

	cMachlist () : dwLastSerialNumberAssigned ( DWORD(0) ) {} // zero is local machine

	DWORD handle_to_ip ( DWORD dwHandle );
	bool get_mach ( mojo::cMach * pRet, DWORD dwHandle );
	cMach * get_by_ip_or_add ( DWORD dwIP, const wchar_t * pDisplayList = NULL );

	// void receive_new_mach ( cMach * pNew ); // called by outside thread // { receipts.append ( pNew ); SendMessage ( g_hwnd, uWM_MACHLIST_RECEIPTS, 0, 0 ); } // called by outside thread
	void add_receipts (); // called only by UI thread to transfer new mach's into the list
	void init_and_insert_local_machine ();

	void save_draw_positions_to_file ();
	static bool get_draw_position_from_file ( cMach * pMach );

	cMach * local_machine ();

	cMach * get_by_name			( const wchar_t * pName );
	cMach * get_by_ip			( DWORD dwIP );

private:

	static const wchar_t * file_pathname ( mojo::cStrW * pRet );
	DWORD dwLastSerialNumberAssigned;

	// tList2<cMach> receipts;

};

} // namespace

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

