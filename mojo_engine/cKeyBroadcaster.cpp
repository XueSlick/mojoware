/***********************************************************************************************************************
/*
/*    cKeyBroadcaster.cpp
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
//  INJECT KEY EVENT
//----------------------------------------------------------------------------------------------------------------------
void cKeyBroadcaster :: inject_key_event ( const mojo::cTarget * pTarget, WPARAM wParam, KBDLLHOOKSTRUCT * p )
{
	pTarget; wParam; p;

	mojo::put_ad_lib_memo ( cMemo::info, L"Injecting key event", L"Handle: %x: %s", pTarget->hwnd, ((cTarget*)pTarget)->sPath.cstr() );	
}


//----------------------------------------------------------------------------------------------------------------------
//  RECEIVE FROM KEYBOARD HOOK
//----------------------------------------------------------------------------------------------------------------------
void cKeyBroadcaster :: receive_from_keyboard_hook ( WPARAM wParam, KBDLLHOOKSTRUCT * p )
{
	HWND hForeground = GetForegroundWindow ();

	if ( g_TargetMgr.is_broadcast_source ( hForeground ) )
	{
		cMessageBroadcastKeyEvent m ( wParam, p );
		broadcast_to_local_windows ( &m, hForeground );
		cMessenger::broadcast_message ( &m );
	}
}


//----------------------------------------------------------------------------------------------------------------------
//  BROADCAST TO LOCAL WINDOWS
//----------------------------------------------------------------------------------------------------------------------
void cKeyBroadcaster :: broadcast_to_local_windows ( const cMessageBroadcastKeyEvent * pMsg, HWND hExcludeThisWindow )
{
	unsigned uQty = 0;
	cStrW sBody;

	mojo::tList2<mojo::cTarget> * pList = &g_TargetMgr.List;

	pList->lock();
	{
		for ( cTarget * pTarget = pList->pHead; pTarget; pTarget = pTarget->pNext )
		{
			if ( pTarget->hwnd != hExcludeThisWindow )
			{
				cSyringe::send_notify_message ( pTarget->hwnd, pMsg->wParam, &pMsg->kbhs, pTarget->aKeyboardState );

				//--------------------------------------
				// MAKE SCRIB BODY
				//--------------------------------------

				wchar_t t [300];

				if ( uQty )
					swprintf_s ( t, sizeof(t)/sizeof(wchar_t), L"\nWindow handle 0x%X.", pTarget->hwnd );

				else
					swprintf_s ( t, sizeof(t)/sizeof(wchar_t), L"Window handle 0x%X.",   pTarget->hwnd );

				sBody += t;

				uQty ++;
			}
		}
	}
	pList->unlock();

	cStrW sFrom;
	cMessenger::print_from_mach ( &sFrom, pMsg->pFromMach );

	cStrW sEvent;
	cKeyboard::pretty_key_event ( &sEvent, &pMsg->kbhs );

	if ( 0 == uQty )
		sBody = L"No windows were found.";

	put_ad_lib_memo ( cMemo::success, L"Command executed", L"%s\n" L"Broadcast %s to target windows on this PC.\n" L"%s", sFrom.cstr(), sEvent.cstr(), sBody.cstr()   );
}



//----------------------------------------------------------------------------------------------------------------------
//  GET COPY OF ARRAY
//----------------------------------------------------------------------------------------------------------------------
#if 0
void cKeyBroadcaster :: get_broadcast_targets ( mojo::cArrayTarget * pRet )
{
	pRet;
	assert (0);

#if 0 ////////////////////////////////

	List.lock();
	pRet->resize ( List.qty() );

	int i = 0;

	for ( cTarget * p = List.first(); p; p = List.next() )
	{
		(*pRet)[i] = *p;
		i++;
	}

	List.unlock();

#endif /////////////////////////////
}
#endif

//----------------------------------------------------------------------------------------------------------------------
//  HWND IS IN ARRAY
//----------------------------------------------------------------------------------------------------------------------
#if 0
bool cKeyBroadcaster :: hwnd_is_in_array ( cArrayTarget * pRay, HWND hwnd )
{
	for ( unsigned i = 0; i < pRay->qty(); i++ )
	{
		if ( hwnd == (*pRay)[i].hwnd )
			return true;
	}

	return false;
}
#endif

//----------------------------------------------------------------------------------------------------------------------
//  FIND HWND IN LIST
//----------------------------------------------------------------------------------------------------------------------
#if 0
cTarget * cKeyBroadcaster :: find_hwnd_in_list ( HWND hwnd )
{
	for ( cTarget * p = List.pHead; p; p = p->pNext )
	{
		if ( hwnd == p->hwnd )
			return p;
	}

	return NULL;
}
#endif

//----------------------------------------------------------------------------------------------------------------------
//  RECEIVE
//  cFinder calls this to send current list
//----------------------------------------------------------------------------------------------------------------------
#if 0
void cKeyBroadcaster:: receive_from_finder ( cArrayTarget * a )
{
	a;
	assert(0);

#if 0 //////////////////////

	bool bChanged = false;

	List.lock();

	//-------------------------------------------
	//  FOR EACH ITEM IN ARRAY
	//-------------------------------------------

	for ( unsigned i = 0; i < a->qty(); i++ )
	{
		//-------------------------------------------
		//  ADD IT TO LIST IF IT'S NOT IN LIST
		//-------------------------------------------

		if ( ! find_hwnd_in_list ( (*a)[i].hwnd ) )
		{
			cTarget * pNew = new cTarget ( (*a)[i] );
			List.append ( pNew );
			bChanged = true;
		}
	}


	//-------------------------------------------
	//  FOR EACH ITEM IN LIST
	//-------------------------------------------

	cTarget * p = List.pHead;
	while ( p )
	{
		cTarget * pNext = p->pNext;

		//-------------------------------------------
		//  REMOVE IT FROM LIST IF IT'S NOT IN ARRAY
		//-------------------------------------------

		if ( ! hwnd_is_in_array ( a, p->hwnd ) )
		{
			List.rem_del ( p );
			bChanged = true;
		}

		p = pNext;
	}

	List.unlock();

	if ( bChanged )
		cMessenger::tell_app_that_broadcast_targets_changed();

#endif //////////////////////////
}
#endif

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