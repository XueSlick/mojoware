/***********************************************************************************************************************
/*
/*    cMonitor.h / mojo_engine
/*
/*    This class was used in HotkeyNet 2. In Mojo it has been replaced by the cScrib/cMemo system.
/*
/*    This class allows object to report error status and error message.  Can be called in two ways:
/*
/*	  1.  Call it directly with print().  This puts text directly into rich edit control.  This
/*	  should only be used when it's certain that other threads will not print text at the same
/*    time.  If they do, lines of text can appear out of order.
/*
/*    2.  Call it indirectly with rcv()  This puts text into circular buffer, then sends
/*    message to UI thread to display it.  The advantage of second way is that several lines
/*	  can be clustered together, guaranteeing that they will be printed adjacent to each other.
/*    However this guarantee breaks is some thread is using method 1.  It only works if all
/*    threads use method 2.
/*
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#pragma once

#include "tCircBuf.h"
#include "tStr.h"


//======================================================================================================================
// DATA
//======================================================================================================================



//======================================================================================================================
// PROTOTYPES
//======================================================================================================================



//======================================================================================================================
// CODE
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
//  
//----------------------------------------------------------------------------------------------------------------------

//======================================================================================================================
// PRELIMS
//======================================================================================================================

#if 0
enum eWeight { regular, bold };

class cColor
{
public:
	cColor () {}
	cColor ( COLORREF arg, int i ) : colorref ( arg ) { UNREFERENCED_PARAMETER(i); } // keep the extra integer argument; it ensures that mistakes are caught by compiler
	                                                     // if int/color args are transposed in calls to sMon() or print().  It has no other purpose.
	COLORREF colorref;
};

static const cColor red		( RGB ( 0xCC, 0x00, 0x33 ), 0 );
static const cColor blue	( RGB ( 0x00, 0x00, 0xDD ), 0 );
static const cColor green	( RGB ( 0x00, 0x99, 0x00 ), 0 );
static const cColor black	( RGB ( 0x00, 0x00, 0x00 ), 0 );
static const cColor purple	( RGB ( 150, 0, 190 ), 0 );


struct sMon // USED BY CALLERS AS ARGUMENT
{
	sMon() {}
	sMon ( const wchar_t * pTxtArg, int iIndentArg=0, cColor ColorArg=black, eWeight eWeightArg=regular ) : pTxt(pTxtArg), color(ColorArg), iIndent(iIndentArg), weight(eWeightArg) {}
	const wchar_t * pTxt;
	cColor color;
	eWeight weight;
	int iIndent;
};


struct sMonSubEntry // USED AS ARRAY MEMBER IN CIRCULAR BUFFER ENTRY
{
	sMonSubEntry	() {};
	sMonSubEntry	( sMon & r ) : sTxt(r.pTxt), color(r.color),weight(r.weight), iIndent(r.iIndent) {}
	sMonSubEntry &	operator= ( sMon & r ) { sTxt=r.pTxt; color=r.color; weight=r.weight; iIndent=r.iIndent; return *this; }
	mojo::cStrW	sTxt;
	cColor			color;
	eWeight			weight;
	int				iIndent;
};


struct sMonEntry // CIRCULAR BUFFER ENTRY
{
	sMonSubEntry	a [5];
};


//======================================================================================================================
// CLASS
//======================================================================================================================

class cMonitor
{
public:

	cMonitor		() : b ( 20 ) {}

	static void		clear ();
	void			rcv ( sMon & e0, sMon & e1=NullArg,  sMon & e2=NullArg,  sMon & e3=NullArg,  sMon & e4=NullArg );
	void			display ();
	static void		print ( const wchar_t * p, int iIndentLevel=0, cColor color = black, eWeight weight = regular );
	static void print_error ( const wchar_t * pError, const wchar_t * pSource = 0, const wchar_t * pHint = 0 );

private:

	static sMon		NullArg;
	mojo::tCircBuf<sMonEntry> b;
};

#endif

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








