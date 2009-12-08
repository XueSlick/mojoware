/***********************************************************************************************************************
/*
/*    cMach.h / mojo_engine
/*
/*    This class represents a machine, i.e., a computer to which we're actually or potentially connected.
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#pragma once

struct sSocketInfo;
#include "tPt.h"
#include "tRect.h"
#include "tList.h"
#include "cDisplay.h"

namespace mojo
{

//----------------------------------------------------------------------------------------------------------------------
// CLASS MACH
///----------------------------------------------------------------------------------------------------------------------

#pragma warning ( push )
#pragma warning ( disable : 4251 ) // class 'mojo::tPt<C>' needs to have dll-interface to be used by clients of class 'mojo::cMach'

class MOJO_ENGINE_API cMach
{
public:

	cMach () : pPrev(0), pNext(0), dwSerialNumber(DWORD(-1)), dwIP(0), bThisPC (false), DrawPos(-1,-1), bValidDrawPos(false) { InitializeCriticalSectionAndSpinCount(&this->cs, 4000 ); }
	cMach ( const cMach & rh );
	~cMach () { DeleteCriticalSection ( & cs ); }

	cMach & operator= ( const cMach & rh );

   void lock	() { EnterCriticalSection ( &cs ); }
   void unlock	() { LeaveCriticalSection ( &cs ); }

   //------------------------------------
   //  DATA
   //------------------------------------

   DWORD dwSerialNumber;

	bool bThisPC;
	mojo::cPtI DrawPos;			// for mouseover screen
	cDisplayList DisplayList;	// for mouseover screen
	bool bValidDrawPos;
	DWORD dwIP;
	mojo::cStrW sDottedDec;
	mojo::cStrW sName;

	cMach * pNext;
	cMach * pPrev;

private:
	CRITICAL_SECTION cs;
};

#pragma warning ( pop )

} // namespace


/***********************************************************************************************************************
/*
/*    This file is part of Mojo.  For more information, see http://mojoware.org.
/*
/*    You may redistribute and/or modify Mojo under the terms of the GNU General Public License, 
/*    version 3, as published by the Free Software Foundation.  You should have received a copy of the 
/*    license with mojo.  If you did not, go to http://www.gnu.org.
/*     
/*    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
/*    INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A 
/*    PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
/*    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
/*    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
/*    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
/*    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
/*    EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
/*
/***********************************************************************************************************************/

