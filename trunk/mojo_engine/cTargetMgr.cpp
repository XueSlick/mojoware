/***********************************************************************************************************************
/*
/*    cTargetMgr.cpp
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"

using namespace mojo;


//======================================================================================================================
//  DATA
//======================================================================================================================


//======================================================================================================================
//  PROTOTYPES
//======================================================================================================================


//======================================================================================================================
//  CODE
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
//  REMOVE LAUNCH TARGET
//----------------------------------------------------------------------------------------------------------------------
void cTargetMgr :: remove_launch_target ( DWORD dwID )
{
	cTarget * pFound = 0;

	//-------------------------------------------------
	// GET TARGET
	//-------------------------------------------------

	for ( cTarget * t = List.pHead; t; t = t->pNext )
	{
		if ( 1 == t->hMach && dwID == t->dwID )
		{
			pFound = t;
			break;
		}
	}

	if ( pFound )
	{
		List.rem_del ( pFound );
		cMessenger::tell_app_that_targets_changed();
	}
}


//----------------------------------------------------------------------------------------------------------------------
//  SET LAUNCH TARGET
//  Called by application.  Doesn't launch the target.  Merely sets it.
//  If the target isn't already on the list, it gets added.
//----------------------------------------------------------------------------------------------------------------------
void cTargetMgr :: set_launch_target ( mojo::cTarget * p )
{
	cTarget * pFound = 0;

	//-------------------------------------------------
	// CHECK WHETHER THIS TARGET IS ALREADY ON LIST
	//-------------------------------------------------

	for ( cTarget * t = List.pHead; t; t = t->pNext )
	{
		if ( 1 == t->hMach && p->dwID == t->dwID )
		{
			pFound = t;
			break;
		}
	}

	//-------------------------------------------------
	// DO IT
	//-------------------------------------------------

	if ( ! pFound )
	{
		cTarget * pNew = new cTarget ( *p );
		pNew->bLaunchByMojo = true;
		pNew->hMach = 1; // local
		List.append ( pNew );
	}

	else
		*pFound = *p;

	cMessenger::tell_app_that_targets_changed();
}


//----------------------------------------------------------------------------------------------------------------------
//  UPDATE LIST FROM REMOTE FINDS
//  The argument is a target array containing targets that were found running on the PC represesnted by hMach.
//----------------------------------------------------------------------------------------------------------------------
void cTargetMgr :: update_list_from_remote_finds ( DWORD hMach, cArrayTarget * a )
{
	bool bChanged = false;

	List.lock();

	//-------------------------------------------
	//  FOR EACH ITEM IN ARRAY
	//-------------------------------------------

	for ( unsigned i = 0; i < a->qty(); i++ )
	{
		//-------------------------------------------
		//  CHECK WHETHER IT'S IN LIST
		//-------------------------------------------

		cTarget * pTarget = find_target_in_list ( &(*a)[i] ); // ( hMach, (*a)[i].hwnd, (*a)[i].dwProcessID );

		if ( ! pTarget )
		{
			cTarget * pNew = new cTarget ( (*a)[i] );
			// pNew->hMachHandle = hMach;
			List.append ( pNew );
			bChanged = true;
		}
	}

	//-------------------------------------------
	//  FOR EACH TARGET IN LIST FROM THE REMOTE
	//  MACHINE
	//-------------------------------------------

	cTarget * p = List.pHead;
	while ( p )
	{
		cTarget * pNext = p->pNext;

		if ( hMach == p->hMach ) // THAT REMOTE MACH ONLY
		{
			//-------------------------------------------
			//  REMOVE IT FROM LIST IF IT'S NOT IN ARRAY
			//-------------------------------------------

			// if ( ! target_is_in_array ( a, hMach, p->hwnd, p->dwProcessID ) )
			if ( ! a->find_target ( p ) )
			{
				List.rem_del ( p );
				bChanged = true;
			}
		}

		p = pNext;
	}

	List.unlock();

	if ( bChanged )
		cMessenger::tell_app_that_targets_changed();

	put_ad_lib_memo ( cMemo::info, L"cTargetMgr", L"Qty after receive remote: %d", List.qty() );

}


//----------------------------------------------------------------------------------------------------------------------
//  RECEIVE REMOTE TARGETS
//  These are all running on the remote.
//----------------------------------------------------------------------------------------------------------------------
void cTargetMgr :: receive_remote_targets ( const cMessage * pMsgArg )
{
	const cMessageArrayTarget * pMAT = reinterpret_cast<const cMessageArrayTarget*>(pMsgArg);

	const BYTE * py = (const BYTE*)pMAT + sizeof ( cMessageArrayTarget );
	const wchar_t * pw = (const wchar_t*)py;

	cArrayTarget at ( pMAT->iQty );

	//----------------------------------------------
	//  FOR EACH ITEM IN REMOTE'S ARRAY
	//----------------------------------------------

	for ( int i = 0; i < pMAT->iQty; i++ )
	{
		pw += at[i].deserialize ( pw );
		at[i].hMach = pMsgArg->pFromMach->dwSerialNumber;
	}

	update_list_from_remote_finds ( pMAT->pFromMach->dwSerialNumber, &at );
}


//----------------------------------------------------------------------------------------------------------------------
//  BROADCAST FINDS
//----------------------------------------------------------------------------------------------------------------------
void cTargetMgr :: broadcast_targets ( cArrayTarget * p )
{
	cMessageArrayTarget * m = cMessageArrayTarget::create ( p );

	g_Messenger.broadcast_message ( m );

	delete m;
}


//----------------------------------------------------------------------------------------------------------------------
//  IS BROADCAST SOURCE
//----------------------------------------------------------------------------------------------------------------------
bool cTargetMgr :: is_broadcast_source ( HWND hwnd )
{
	if ( find_hwnd_in_list ( hwnd ) )
		return true;

	else
		return false;
}


//----------------------------------------------------------------------------------------------------------------------
//  GET COPY OF TARGETS
//----------------------------------------------------------------------------------------------------------------------
void cTargetMgr :: get_copy_of_targets ( mojo::cArrayTarget * pRet )
{
	List.lock();
	pRet->resize ( List.qty() );

	int i = 0;

	for ( cTarget * p = List.first(); p; p = List.next() )
	{
		(*pRet)[i] = *p;
		i++;
	}

	List.unlock();
}

//----------------------------------------------------------------------------------------------------------------------
//  GET COPY OF LOCAL TARGETS
//----------------------------------------------------------------------------------------------------------------------
void cTargetMgr :: get_copy_of_local_targets ( mojo::cArrayTarget * pRet )
{
	List.lock();
	pRet->resize ( List.qty() );

	int i = 0;

	for ( cTarget * p = List.first(); p; p = List.next() )
	{
		if ( 1 == p->hMach )
		{
			(*pRet)[i] = *p;
			i++;
		}
	}

	List.unlock();
}


//----------------------------------------------------------------------------------------------------------------------
//  FIND TARGETS
//  Called by cFinder every five seconds.  Runs in cFinder's low-priority thread.
//  Looks to see if target programs are running and if so, collects their hwnds.
//----------------------------------------------------------------------------------------------------------------------
void cTargetMgr :: find_targets ()
{
	find_wow ();
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