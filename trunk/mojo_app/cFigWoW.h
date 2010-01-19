/***********************************************************************************************************************
/*
/*    cFigWoW.h / mojo_app
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/


#pragma once

#include "cFigViewItem.h"

//======================================================================================================================
//  CLASS
//======================================================================================================================

class cFigWoW : public cFigViewItem
{
public:

	enum etOrigin { launch_by_mojo, found };

	cFigWoW () : hwnd (0), bLaunchByMojo(false), bRunning(false), pNext(0), pPrev(0), dwProcessID(0), hMach(0), dwIP(0), dwTargetID(0) {}
	cFigWoW( const cFigWoW & r );
	cFigWoW & operator= ( const cFig & r );
	virtual int menu () { return ID_WOW_MENU; }
	virtual cFigWoW * clone () const; // DELETE RETURN VALUE AFTER USING IT
	void set_target ( mojo::cTarget * t );

	//------------------------------------
	// DATA
	//------------------------------------

	mojo::cStrW sName;
	mojo::cStrW sPath;
	mojo::cStrW sComputerName;
	mojo::cStrW sDottedDec;
	DWORD dwIP;
	DWORD hMach;
	HWND hwnd;
	DWORD dwTargetID;
	DWORD dwProcessID;
	bool bLaunchByMojo;
	bool bRunning;

	cFigWoW * pNext, * pPrev;

	//------------------------------------
	// cFIG OVERRIDES
	//------------------------------------

	virtual const sEntry * table () const { return aTable; }
	static sEntry aTable[];
	static const cFigWoW Default;
	virtual void set_from_xml ( void * pvDest, const wchar_t * pTxt ) const; // override only if class is a "terminal" node
	virtual void handle_context_menu ( int iID );
private:

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