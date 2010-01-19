/***********************************************************************************************************************
/*
/*    cArrayTarget.cpp
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"




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
//  FIND TARGET
//----------------------------------------------------------------------------------------------------------------------
mojo::cTarget * mojo::cArrayTarget :: find_target ( mojo::cTarget * a )
{
	for ( unsigned i = 0; i < qty(); i++ )
	{
		mojo::cTarget * p = &(*this)[i];

#if 0
		if ( a->hMach == 1 && a->bLaunchByMojo == true )
		{
			if ( p->dwID == a->dwID )
				return p;
		}
#endif

		if ( a->bLaunchByMojo == true )
		{
			if ( p->hMach == a->hMach && p->dwID == a->dwID )
				return p;
		}

		else
		{
			if ( a->hwnd == p->hwnd && a->hMach == p->hMach && a->dwProcessID == p->dwProcessID )
				return p;
		}
	}

	return NULL;
}


//----------------------------------------------------------------------------------------------------------------------
//  FIND TARGET
//----------------------------------------------------------------------------------------------------------------------
mojo::cTarget * mojo::cArrayTarget :: find_target ( DWORD hMach, HWND hwnd, DWORD dwProcessID )
{
	for ( unsigned i = 0; i < qty(); i++ )
	{
		if ( hMach == (*this)[i].hMach && hwnd == (*this)[i].hwnd && dwProcessID == (*this)[i].dwProcessID )
			return &(*this)[i];
	}

	return false;
}


//----------------------------------------------------------------------------------------------------------------------
//  FIND HWND
//----------------------------------------------------------------------------------------------------------------------
mojo::cTarget * mojo::cArrayTarget :: find_hwnd ( HWND hwnd )
{
	for ( unsigned i = 0; i < qty(); i++ )
	{
		if ( hwnd == (*this)[i].hwnd )
			return &(*this)[i];
	}

	return false;
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
