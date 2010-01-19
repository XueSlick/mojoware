/*********************************************************************************************
/*
/*    cMouseover.h
/*
/*    begun June 4, 2009
/*
/*********************************************************************************************/

#pragma once

#include "cMessenger.h"


//=============================================================================================
// CLASSES
//=============================================================================================

//----------------------------------------------------------------------------------------------
// CLASS MOUSEOVER
//----------------------------------------------------------------------------------------------


const unsigned uWM_MOUSEOVER_MOVE_ABSOLUTE                         = WM_APP +  1;
const unsigned uWM_MOUSEOVER_MOVE_RELATIVE                         = WM_APP +  2;
const unsigned uWM_MOUSEOVER_SLAVE_REPORTS_CURSOR_OFFSCREEN        = WM_APP +  3;
const unsigned uWM_MOUSEOVER_MASTER_TELLS_SLAVE_IT_HAS_LEFT        = WM_APP +  4;

class cMouseover
{
public:

	class cMegaDisplay : public mojo::cDisplay
	{
	public:

		mojo::cMach * pMach;
		mojo::cPtI Offset;
		cMegaDisplay * pNext;
		cMegaDisplay * pPrev;
	};

	bool send_keyboard_command_to_slave ( WPARAM wParam, KBDLLHOOKSTRUCT * p );
	bool send_mouse_command				( mojo::cMach * pMach, WPARAM wParam, MSLLHOOKSTRUCT  * p );
	void turn_cursor_on ();
	void turn_cursor_off ();

	void receive_command ( const cMessage * pMsg );

	void enter_real_mode    ();
	void enter_master_mode  ();
	void enter_slave_mode   ();
	void enter_dormant_mode ();

	mojo::tList2<cMegaDisplay> MegaDisplayList;

	void set_mega_display_list ( mojo::cMachlist * pML );

	cMouseover ();

	bool cursor_is_local ( int x, int y );


	bool remote_to_mega ( int * piMegaX, int * piMegaY, int iRemoteX, int iRemoteY, mojo::cMach * pRemote );
	bool mega_to_remote( mojo::cMach ** ppMach, int * piRealX, int * piRealY, int iMegaX, int iMegaY );
	bool on_mouse_hook  ( WPARAM wParam, MSLLHOOKSTRUCT * p );
	bool on_keyboard_hook ( WPARAM wParam, KBDLLHOOKSTRUCT * p );
	// bool on_mouse_move  ( WPARAM wParam, MSLLHOOKSTRUCT * p );  // actually for all events
	bool on_mouse_event ( WPARAM wParam, MSLLHOOKSTRUCT * p );

	// bool is_on () { return bIsOn; }
	mojo::cPtI ptMegaPos;
	mojo::cPtI ptCenter;

	// bool bIsOn;
	enum { real, master, slave, dormant } eMode;
	mojo::cDisplayList LocalDisplayList;
	// cDlgCursorBlind DlgCursorBlind;
	mojo::cMach * get_slave() { return pSlave; }
	void bring_cursor_home ();
private:
	mojo::cMach * pSlave, * pMaster;
	void inject_mouse_event_sub ( DWORD dwEvent, MSLLHOOKSTRUCT * p );
	void inject_mouse_event		( WPARAM wParam, MSLLHOOKSTRUCT * p );
	void inject_keyboard_event	( WPARAM wParam, KBDLLHOOKSTRUCT * p );



	struct sCommand
	{
		cMessage Msg;

		enum { mouse, keyboard } eMouseOrKeyboard;
		WPARAM wParam;
		union
		{
			KBDLLHOOKSTRUCT k;
			MSLLHOOKSTRUCT m;
		} u;
	};
};
