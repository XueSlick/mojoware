/********************************************************************************************************
/*
/*    hooks_public.h / mojo_hooks
/*
/*    The functions on this page should be called only by mojo_engine.  Do not call these functions
/*    from your application.
/*
/*    Your application can control hooks with functions that are exported by mojo_engine.
/*
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/*******************************************************************************************************/

#pragma once

#include <windows.h>

#ifndef MOJO_HOOKS_API
#ifdef MOJO_HOOKS_EXPORTS
#define MOJO_HOOKS_API __declspec(dllexport)
#else
#define MOJO_HOOKS_API __declspec(dllimport)
#endif
#endif

//=======================================================================================================
//  TYPEDEFS
//=======================================================================================================

typedef bool fKEYBOARDHOOKSERVICEROUTINE	( WPARAM, KBDLLHOOKSTRUCT * p ); 
typedef bool fMOUSEHOOKSERVICEROUTINE		( WPARAM, MSLLHOOKSTRUCT  * p );

namespace mojo_hooks
{

//=======================================================================================================
//  DATA
//=======================================================================================================

const UINT uWM_INSTALL_KEYBOARD_HOOK 	= WM_USER + 0;
const UINT uWM_UNINSTALL_KEYBOARD_HOOK	= WM_USER + 1;
const UINT uWM_INSTALL_MOUSE_HOOK 		= WM_USER + 2;
const UINT uWM_UNINSTALL_MOUSE_HOOK 	= WM_USER + 3;


//=======================================================================================================
//  PROTOTYPES
//=======================================================================================================

MOJO_HOOKS_API DWORD start_thread               ( HWND hwndApp, fKEYBOARDHOOKSERVICEROUTINE * pfKB, fMOUSEHOOKSERVICEROUTINE * pfM );
MOJO_HOOKS_API bool  install_keyboard_hook      ( fKEYBOARDHOOKSERVICEROUTINE * );
MOJO_HOOKS_API bool  uninstall_keyboard_hook    ();
MOJO_HOOKS_API bool  install_mouse_hook         ( fMOUSEHOOKSERVICEROUTINE * );
MOJO_HOOKS_API bool  uninstall_keyboard_hook    ();
MOJO_HOOKS_API void  get_version   			( unsigned *pMaj, unsigned *pMin, unsigned *pPip, unsigned * pBld );
} // namespace