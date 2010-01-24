/***********************************************************************************************************************
/*
/*    cFogWoWTree.h / mojo_app
/*   
/*    Copyright 2010 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/


#pragma once

// #include "cFog.h"
// #include "cFogWoW.h"


//======================================================================================================================
//  CLASS
//======================================================================================================================
class cFogViewItemList;

class cFogWoWTree : public cFogTree
{
public:


protected:
	virtual const sEntry * table () const { return aTable; }
	static sEntry aTable[];
	static cFogTree * create () { return new cFogWoWTree; }

#if 0
	cFogWoW * get_by_hwnd ( HWND hwnd ) const;
	cFogWoW * find_target ( DWORD hMach, HWND hwnd, DWORD dwProcessID ) const;
	cFogWoW * find_target ( mojo::cTarget * a );
#endif

private:

	// cFogViewItemList * create_Fog_view_item_list ();

	friend class cFogMgr;
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