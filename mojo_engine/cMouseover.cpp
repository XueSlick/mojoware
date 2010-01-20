/*********************************************************************************************
/*
/*    cMouseover.cpp
/*
/*    begun June 4, 2009
/*
/*********************************************************************************************/

#include "stdafx.h"

using namespace mojo;


#define MOUSEOVER_RELATIVE

static bool bCursor = true;

//=============================================================================================
// CODE
//=============================================================================================

/*   
/*       There are two functions where user input is received and processed:
/*
/*       receive_command() receives input from remote PC
/*       on_mouse_hook() receives input from local PC
*/


//---------------------------------------------------------------------------------------------
//  BRING CURSOR HOME
//---------------------------------------------------------------------------------------------
void cMouseover::bring_cursor_home ()
{
	enter_real_mode();
	
	//-------------------------------
	// CENTER CURSOR
	//-------------------------------

	cPtI ptCenter;
	ptCenter.x = GetSystemMetrics ( SM_CXSCREEN ) / 2;
	ptCenter.y = GetSystemMetrics ( SM_CYSCREEN ) / 2;
	SetCursorPos ( ptCenter.x, ptCenter.y );
}



//---------------------------------------------------------------------------------------------
//  TURN CURSOR OFF
//---------------------------------------------------------------------------------------------
void cMouseover::turn_cursor_off ()
{
	// MoveWindow ( DlgCursorBlind.hwnd, ptCenter.x, ptCenter.y, 1, 1, FALSE );

#if 0
	SetCursorPos ( ptCenter.x, ptCenter.y );
	SetWindowPos ( DlgCursorBlind.hwnd, HWND_TOPMOST, ptCenter.x, ptCenter.y, 1, 1, SWP_NOREDRAW );
	ShowWindow ( DlgCursorBlind.hwnd, SW_RESTORE);
#endif
	SendMessage ( g_hwndApp, mojo::uWM_HIDE_OR_SHOW_CURSOR, 0, 0 );
}


//---------------------------------------------------------------------------------------------
//  TURN CURSOR ON
//---------------------------------------------------------------------------------------------
void cMouseover::turn_cursor_on ()
{
#if 0
	ShowWindow ( DlgCursorBlind.hwnd, SW_HIDE );
#endif
	SendMessage ( g_hwndApp, mojo::uWM_HIDE_OR_SHOW_CURSOR, 1, 0 );
}


//---------------------------------------------------------------------------------------------
//  INJECT MOUSE EVENT SUB
//---------------------------------------------------------------------------------------------
void cMouseover :: inject_mouse_event_sub ( DWORD dwEvent, MSLLHOOKSTRUCT * p )
{

	INPUT i;
	memset ( &i, 0, sizeof(i) );
	i.type = INPUT_MOUSE;
	i.mi.dwFlags = dwEvent;
	// i.mi.dx = p->pt.x;  // these position members take non-pixel units
	// i.mi.dy = p->pt.y;
	i.mi.mouseData = (_int16) HIWORD ( p->mouseData );
	SendInput ( 1, &i, sizeof(i) );
}


//---------------------------------------------------------------------------------------------
//  RECEIVE COMMAND FROM REMOTE
//---------------------------------------------------------------------------------------------
void cMouseover :: receive_command ( const cMessage * pMsg )
{
	sCommand * c = ( sCommand * ) pMsg;

	pMaster = pMsg->pFromMach;

	assert(pMaster);

	switch ( c->wParam )
	{
	case uWM_MOUSEOVER_MASTER_TELLS_SLAVE_IT_HAS_LEFT:
		enter_dormant_mode();
		break;

	case uWM_MOUSEOVER_SLAVE_REPORTS_CURSOR_OFFSCREEN:
		{
			int iRemoteX = c->u.m.pt.x;
			int iRemoteY = c->u.m.pt.y;
			mojo::cPtI ptMega;
			mojo::cMach * pRemoteMach;

			remote_to_mega ( &ptMega.x, &ptMega.y, c->u.m.pt.x, c->u.m.pt.y, pMsg->pFromMach );

			if ( cursor_is_local ( ptMega.x, ptMega.y ) )
			{
				send_mouse_command ( pMsg->pFromMach, uWM_MOUSEOVER_MASTER_TELLS_SLAVE_IT_HAS_LEFT, &c->u.m );
				SetCursorPos ( ptMega.x, ptMega.y );
				enter_real_mode();
			}

			else if ( mega_to_remote ( &pRemoteMach, &iRemoteX, &iRemoteY, ptMega.x, ptMega.y ) )
			{
				if ( g_Pool.get_connect_socket_info_by_ip ( pRemoteMach->dwIP ) )
				{
					if ( pRemoteMach != pMsg->pFromMach )
						send_mouse_command ( pMsg->pFromMach, uWM_MOUSEOVER_MASTER_TELLS_SLAVE_IT_HAS_LEFT, &c->u.m );

					enter_master_mode();
					pSlave = pRemoteMach;

					c->u.m.pt.x = iRemoteX;
					c->u.m.pt.y = iRemoteY;
			
					if ( ! send_mouse_command ( pSlave, uWM_MOUSEOVER_MOVE_ABSOLUTE, &c->u.m ) )
						enter_real_mode();
				}

				else
					enter_real_mode();
			}
		}

		break;
	
	default:

		if ( slave != eMode )
			enter_slave_mode();

		if ( c->eMouseOrKeyboard == sCommand::keyboard )
			inject_keyboard_event ( c->wParam, &c->u.k );

		else if ( c->eMouseOrKeyboard == sCommand::mouse )
		{
			inject_mouse_event ( c->wParam, &c->u.m );
		}

		else
			assert(0);
	}
}


//---------------------------------------------------------------------------------------------
//  INJECT MOUSE EVENT
//---------------------------------------------------------------------------------------------
void cMouseover :: inject_mouse_event ( WPARAM wParam, MSLLHOOKSTRUCT * p )
{
	DWORD dwEvent = 0;

	if ( uWM_MOUSEOVER_MOVE_ABSOLUTE == wParam )
	{
		SetThreadExecutionState ( ES_DISPLAY_REQUIRED | ES_SYSTEM_REQUIRED ); // new aug 4 2009
		SetCursorPos ( p->pt.x, p->pt.y );
		g_EventBuffer.receive ( wParam, p );
		return;
	}

	POINT pt;
	GetCursorPos ( &pt );
	pt.x += p->pt.x;
	pt.y += p->pt.y;

	if ( ! cursor_is_local ( pt.x, pt.y ) )
	{
		p->pt = pt;

#if 0
		g_Monitor.rcv ( sMon ( L"Sending uWM_MOUSEOVER_SLAVE_REPORTS_CURSOR_OFFSCREEN", 0 ),
			sMon ( pMaster->sDottedDec.cstr(), 0, blue ) );
#endif

		send_mouse_command ( pMaster, uWM_MOUSEOVER_SLAVE_REPORTS_CURSOR_OFFSCREEN, p );
		return;
	}

	SetCursorPos ( pt.x, pt.y );
	SetThreadExecutionState ( ES_DISPLAY_REQUIRED | ES_SYSTEM_REQUIRED ); // new aug 4 2009

	switch ( wParam )
	{
	case WM_MOUSEMOVE:
		assert(0);
		break;

	case uWM_MOUSEOVER_MOVE_ABSOLUTE:
		assert(0);
		break;

	case uWM_MOUSEOVER_MOVE_RELATIVE:
		g_EventBuffer.receive ( wParam, p );
		return;
		break;

	case WM_LBUTTONDOWN:
			dwEvent = MOUSEEVENTF_LEFTDOWN; break;

	case WM_LBUTTONUP:
			dwEvent = MOUSEEVENTF_LEFTUP; break;

	case WM_MBUTTONDOWN:
			dwEvent = MOUSEEVENTF_MIDDLEDOWN; break;

	case WM_MBUTTONUP:
			dwEvent = MOUSEEVENTF_MIDDLEUP; break;

	case WM_RBUTTONDOWN:
			dwEvent = MOUSEEVENTF_RIGHTDOWN; break;

	case WM_RBUTTONUP:
			dwEvent = MOUSEEVENTF_RIGHTUP; break;

	case WM_MOUSEWHEEL:
			dwEvent = MOUSEEVENTF_WHEEL; break;

	case WM_XBUTTONDOWN:
			dwEvent = MOUSEEVENTF_XDOWN ; break;

	case WM_XBUTTONUP:
			dwEvent = MOUSEEVENTF_XUP ; break;
			
	default:
			dwEvent = 0;
	}

	if ( dwEvent )
		inject_mouse_event_sub ( dwEvent, p );
}


//---------------------------------------------------------------------------------------------
//  SEND COMMAND TO SLAVE
//---------------------------------------------------------------------------------------------
bool cMouseover :: send_mouse_command ( mojo::cMach * pMach, WPARAM wParam, MSLLHOOKSTRUCT * p )
{
	if ( ! pMach )
		return false;

	if ( wParam == WM_MOUSEMOVE )
		assert(0);

	sCommand s;

	s.Msg.Type = cMessage::mouseover;
	s.Msg.uLen = sizeof(sCommand);

	s.wParam = wParam;
	s.eMouseOrKeyboard = sCommand::mouse;
	s.u.m = *p;

	return g_Pool.send ( pMach->dwIP, (char*)&s, sizeof(s) );
}


//---------------------------------------------------------------------------------------------
//  ON MOUSE HOOK
//---------------------------------------------------------------------------------------------
bool cMouseover :: on_mouse_hook ( WPARAM wParam, MSLLHOOKSTRUCT * p )
{
	mojo::cMach * pRemoteMach;
	int iRemoteX, iRemoteY;

	//-----------------------------------
	//  IF MOUSEOVER IS OFF, MAKE SURE
	//  WE'RE IN REAL MODE
	//-----------------------------------
	if ( ! g_Settings.bMouseoverIsOn )
	{
		if ( real != eMode )
		   enter_real_mode();

		return true;
	}

	//-----------------------------------
	// DORMANT?
	//-----------------------------------
	if ( dormant == eMode )
	{
		enter_real_mode();
	}

	//-----------------------------------
	// REAL MODE?
	//-----------------------------------
	else if ( real == eMode )
	{
		//-----------------------------------
		// SWITCH TO MASTER MODE?
		//-----------------------------------
		if ( mega_to_remote ( &pRemoteMach, &iRemoteX, &iRemoteY, p->pt.x, p->pt.y ) )
		{
			if ( g_Pool.get_connect_socket_info_by_ip ( pRemoteMach->dwIP ) )
			{
				enter_master_mode();
				pSlave = pRemoteMach;

				p->pt.x = iRemoteX;
				p->pt.y = iRemoteY;
			
				if ( ! send_mouse_command ( pSlave, uWM_MOUSEOVER_MOVE_ABSOLUTE, p ) )
					enter_real_mode();

				return false;
			}
		}

		else
			return true;
	}

	//-----------------------------------
	// SLAVE MODE?
	//-----------------------------------
	else if ( slave == eMode )
	{
		enter_real_mode();
		return true;
	}

	//-----------------------------------
	// MASTER MODE?
	//-----------------------------------
	else if ( master == eMode )
	{
		//---------------------------------------
		//  CALC RELATIVE MOTION
		//---------------------------------------
		mojo::cPtI ptDelta = p->pt;
		ptDelta -= ptCenter;
		p->pt.x = ptDelta.x;
		p->pt.y = ptDelta.y;

		//-----------------------------------
		// SEND MOVEMENT COMMAND
		//-----------------------------------
		if ( WM_MOUSEMOVE == wParam )
		{
			if ( ! send_mouse_command ( pSlave, uWM_MOUSEOVER_MOVE_RELATIVE, p ) )
				enter_real_mode();
		}

		else
		{
			if ( ! send_mouse_command ( pSlave, wParam, p ) )
				enter_real_mode();
		}

		return false;
	}

	return true;
}


//---------------------------------------------------------------------------------------------
//  ENTER REAL MODE
//---------------------------------------------------------------------------------------------
void cMouseover :: enter_real_mode ()
{
	eMode = real;
	pSlave = 0;
	turn_cursor_on();

#if 0
	cMonitor::print ( L"Entering real mode." );
#endif
}


//---------------------------------------------------------------------------------------------
//  ENTER DORMANT MODE
//  dormant means "remote cursor is hidden."
//---------------------------------------------------------------------------------------------
void cMouseover :: enter_dormant_mode ()
{
	if ( g_Settings.bHideRemoteCursor )
	{
		eMode = dormant;
		pSlave = pMaster = 0;
		turn_cursor_off();

		#if 0
			cMonitor::print ( L"Entering dormant mode." );
		#endif
	}

	else
		enter_real_mode();
}


//---------------------------------------------------------------------------------------------
//  ENTER MASTER MODE
//---------------------------------------------------------------------------------------------
void cMouseover :: enter_master_mode ()
{
	eMode = master;

#if 0
	cMonitor::print ( L"Entering master mode." );
#endif

#if 0
	MoveWindow ( DlgCursorBlind.hwnd, ptCenter.x, ptCenter.y, 1, 1, FALSE );
	ShowWindow ( DlgCursorBlind.hwnd, SW_RESTORE);
	SetCursorPos ( ptCenter.x, ptCenter.y );
#endif

	turn_cursor_off();
}


//---------------------------------------------------------------------------------------------
//  ENTER SLAVE MODE
//---------------------------------------------------------------------------------------------
void cMouseover :: enter_slave_mode ()
{
	eMode = slave;
	pSlave = 0;

	turn_cursor_on();

#if 0
	cMonitor::print ( L"Entering slave mode." );
#endif
}


//---------------------------------------------------------------------------------------------
//  SEND KEYBOARD COMMAND TO SLAVE
//---------------------------------------------------------------------------------------------
bool cMouseover :: send_keyboard_command_to_slave ( WPARAM wParam, KBDLLHOOKSTRUCT * p )
{
	if ( ! pSlave )
		return false;

	sCommand s;

	s.Msg.Type = cMessage::mouseover;
	s.Msg.uLen = sizeof(s);

	s.wParam = wParam;
	s.eMouseOrKeyboard = sCommand::keyboard;
	s.u.k = *p;

	return g_Pool.send ( pSlave->dwIP, (char*)&s, sizeof(s) );
}


//---------------------------------------------------------------------------------------------
//  INJECT KEYBOARD EVENT
//---------------------------------------------------------------------------------------------
void cMouseover :: inject_keyboard_event ( WPARAM wParam, KBDLLHOOKSTRUCT * p )
{
	SetThreadExecutionState ( ES_DISPLAY_REQUIRED | ES_SYSTEM_REQUIRED ); // new aug 4 2009

	INPUT i;
	memset ( &i, 0, sizeof(i) );

	i.type = INPUT_KEYBOARD;

	// i.ki.dwFlags |= KEYEVENTF_SCANCODE;
	// i.ki.dwFlags = KEYEVENTF_SCANCODE; // june 22

	if ( wParam == WM_KEYUP || wParam == WM_SYSKEYUP )
		i.ki.dwFlags |= KEYEVENTF_KEYUP;

	i.ki.wVk = (WORD)p->vkCode;

	i.ki.wScan = (WORD)p->scanCode; // june 22
	// i.ki.wScan = MapVirtualKey ( p->vkCode, MAPVK_VK_TO_VSC );
	// i.ki.wScan = (WORD)p->scanCode;

	if ( p->flags & LLKHF_EXTENDED )
	{
		// if ( i.ki.wVk != 144 )  // numlock is special case -- carried over from HKN1
		{
			// i.ki.wScan |= 0xE000;
			i.ki.wScan |= 0xE000; // june 22
			i.ki.dwFlags |= KEYEVENTF_EXTENDEDKEY;
		}
	}

	SendInput ( 1, &i, sizeof(i) );
	// DWORD dwError = GetLastError();
}


//---------------------------------------------------------------------------------------------
//  ON KEYBOARD HOOK
//  Returns value that should be passed to keyboard hook, i.e., if it sends the keystroke
//  to a remote PC, it returns false
//---------------------------------------------------------------------------------------------
bool cMouseover :: on_keyboard_hook ( WPARAM wParam, KBDLLHOOKSTRUCT * p )
{
	//-----------------------------------
	//  IF MOUSEOVER IS OFF, MAKE SURE
	//  WE'RE IN REAL MODE
	//-----------------------------------
	if ( ! g_Settings.bMouseoverIsOn )
	{
		if ( real != eMode )
		   enter_real_mode();

		return true;
	}

	//-----------------------------------
	//  SLAVE MODE?
	//-----------------------------------
	if ( slave == eMode )
	{
		enter_real_mode();
		return true;
	}

	//-----------------------------------
	//  REAL MODE?
	//-----------------------------------
	else if ( real == eMode )
		return true;

	//-----------------------------------
	//  MASTER MODE AND MOUSEOVER IS ON
	//-----------------------------------
	else if ( master == eMode )
	{
		send_keyboard_command_to_slave ( wParam, p );
		return false;
	}

	else
		return true;
}


//---------------------------------------------------------------------------------------------
//  REMOTE TO MEGA
//---------------------------------------------------------------------------------------------
bool cMouseover :: remote_to_mega ( int * piMegaX, int * piMegaY, int iRemoteX, int iRemoteY, mojo::cMach * pRemote )
{
	for ( cMegaDisplay * d = MegaDisplayList.first(); d; d = MegaDisplayList.next(d) )
	{
		if ( d->pMach == pRemote )
		{
			*piMegaX = iRemoteX + d->Offset.x;
			*piMegaY = iRemoteY + d->Offset.y;
			return true;
		}
	}

	return false;
}


//---------------------------------------------------------------------------------------------
//  MEGA TO REMOTE
//  returns true if the mega point is inside a remote
//---------------------------------------------------------------------------------------------
bool cMouseover :: mega_to_remote ( mojo::cMach ** ppMach, int * piRealX, int * piRealY, int iMegaX, int iMegaY )
{
	mojo::cPtI pt ( iMegaX, iMegaY );

	for ( cMegaDisplay * d = MegaDisplayList.first(); d; d = MegaDisplayList.next(d) )
	{
		if ( is_point_in_rect ( static_cast<mojo::cRectI*>(d), &pt ) )
		{
#if 0
			// if the remote isn't connected, return false
			// ON SECOND THOUGHT, DO THIS AT HIGHER LEVEL
			if ( ! g_Pool.get_connect_socket_info_by_ip ( d->pMach->dwIP ) )
				return false;
#endif

			*ppMach = d->pMach;
			*piRealX = iMegaX - d->Offset.x;
			*piRealY = iMegaY - d->Offset.y;
			return true;
		}
	}

	return false;
}


//---------------------------------------------------------------------------------------------
//  SET MEGA DISPLAY LIST
//  only remote displays are included
//---------------------------------------------------------------------------------------------
void cMouseover :: 	set_mega_display_list ( mojo::cMachlist * pML )
{
	ptCenter.x = GetSystemMetrics ( SM_CXSCREEN ) / 2;
	ptCenter.y = GetSystemMetrics ( SM_CYSCREEN ) / 2;

	mojo::cPtI LocalDrawPos = pML->local_machine()->DrawPos; // upper left corner of virtual rectangle

	//---------------------------------------
	// Adjust for fact that primary monitor 
	// on local machine (the monitor whose
	// origin is 0,0 in Windows coordinates) 
	// may not be in upper left of
	// mult_virt_rect
	//---------------------------------------
	cRectI rMultVirtRect;
	pML->local_machine()->DisplayList.calc_mult_virt_rect ( & rMultVirtRect );
	LocalDrawPos.x -= ( rMultVirtRect.x / 8 );
	LocalDrawPos.y -= ( rMultVirtRect.y / 8 );

	MegaDisplayList.lock();

	MegaDisplayList.rem_del_all();

	for ( cMach * m = pML->first(); m; m = pML->next(m) )
	{
		if ( ! m->bValidDrawPos )
			continue;

		if ( ! m->bThisPC )
		{
			cPtI ptOffset;
			ptOffset.x =  8 * ( m->DrawPos.x - LocalDrawPos.x);
			ptOffset.y =  8 * ( m->DrawPos.y - LocalDrawPos.y);

			cRectI rMultVirtRect;
			m->DisplayList.calc_mult_virt_rect ( & rMultVirtRect );
			ptOffset.x -= rMultVirtRect.x;
			ptOffset.y -= rMultVirtRect.y;

			for ( cDisplay * d = m->DisplayList.first(); d; d = m->DisplayList.next(d) )
			{
				cMegaDisplay * md = new cMegaDisplay;
				md->Offset = ptOffset;

				md->x		= d->x + ptOffset.x;
				md->y		= d->y + ptOffset.y;
				md->dx		= d->dx;
				md->dy		= d->dy;
				md->pMach	= m;
				MegaDisplayList.append ( md );
			}
		}
	}

	MegaDisplayList.unlock();
}


//---------------------------------------------------------------------------------------------
// CURSOR IS LOCAL
//---------------------------------------------------------------------------------------------
bool cMouseover :: cursor_is_local ( int x, int y )
{
	// return LocalDisplayList.is_point_in ( &cPtI ( x, y ) ); // changed jul 8 2009

	POINT p;
	p.x = x;
	p.y = y;
	return ( NULL != MonitorFromPoint ( p, MONITOR_DEFAULTTONULL ) );
}


//---------------------------------------------------------------------------------------------
// CONSTRUCTOR
//---------------------------------------------------------------------------------------------
cMouseover::cMouseover () : eMode ( real ),  pSlave(0)
{
	LocalDisplayList.set_from_local_hardware();

#if 0
	DlgCursorBlind.make_dlg();
	ShowWindow ( DlgCursorBlind.hwnd, SW_HIDE );
	SetParent ( DlgCursorBlind.hwnd, g_hwnd );
#endif
}

