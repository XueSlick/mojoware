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
//  GET COPY OF ARRAY
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