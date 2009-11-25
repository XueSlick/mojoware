/*************************************************************************************************
/*
/*   cListViewConnections.h / mojo_app
/*
/*   Wrapper for the listview control that shows connections.
/*   
/*   started May 23, 2009
/*   
/*************************************************************************************************/

#pragma once

#include "cWin.h"
class mojo::cMachlist;
class mojo::cMach;

//------------------------------------------------------------------------------------------------
//  CLASS cDlg
//------------------------------------------------------------------------------------------------
class cListViewConnections : public cWin
{
public:

	void init ();
	void populate ( mojo::cMachlist * p );
	DWORD hot_mach ();
};
