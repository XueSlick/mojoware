/***********************************************************************************************************************
/*
/*    cTargetMgr.h
/*
/*    Launches target programs and keeps track of them.
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#pragma once


//----------------------------------------------------------------------------------------------------------------------
//   CLASS TARGET MANAGER
//----------------------------------------------------------------------------------------------------------------------
class cTargetMgr
{
public:

	void update_list_from_remote_finds   ( DWORD hMach, mojo::cArrayTarget * a );
	void find_targets                    (); // called by cFinder periodically
	void receive_remote_targets          ( const cMessage * pMsg );
	void get_copy_of_targets             ( mojo::cArrayTarget * pRet );
	void get_copy_of_local_targets       ( mojo::cArrayTarget * pRet );
	bool is_broadcast_source             ( HWND hwnd );
	static bool hwnd_is_in_array         ( mojo::cArrayTarget * pRay, HWND hwnd );
	static bool target_is_in_array       ( mojo::cArrayTarget * pRay, DWORD hMach, HWND hwnd, DWORD dwProcessID );
	void set_launch_target               ( mojo::cTarget * p );
	void remove_launch_target            ( DWORD dwID );


private:

	void broadcast_targets		         ( mojo::cArrayTarget * p ); // called by itself from cFinder's thread
	void receive_local_finds             ( mojo::cArrayTarget * p ); // called by itself from another thread

	bool find_wow ();
	mojo::cTarget * find_hwnd_in_list    ( HWND hwnd );
	mojo::cTarget * find_target_in_list  ( DWORD hMach, HWND hwnd, DWORD dwProcessID );

	mojo::cTarget * find_target_in_list  ( mojo::cTarget * a );
	mojo::cTarget * find_target_in_array ( mojo::cArrayTarget * pRay, mojo::cTarget * a );

	mojo::tList2<mojo::cTarget> List;

	friend class cKeyBroadcaster;
};


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