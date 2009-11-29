/***********************************************************************************************************************
/*
/*    cMessage.h / mojo_engine
/*
/*    All commands executed by Mojo are put in the form of a cMessage (or one of its derived classes)
/*    regardless of whether they execute locally or remotely.
/*
/*    This rule has been adopted in order to help prevent the evolving program from turning into an
/*    unmanageable mass of spaghetti.
/*
/*    I haven't decided yet whether all commands must pass through cMessenger en route to being
/*    executed, so cMessage plays a vital role in keeping things manageable.
/*
/*    cMessages get sent from one computer to another in a very simple way.  Their memory is treated
/*    as a string of bytes which is shoved into a TCP segment.
/*
/*    When the string of bytes arrives at the destination PC, it is cast to be a cMessage, but the
/*    resulting object isn't complete because it lacks its virtual function table, so be careful about 
/*    calling functions in these circumstances.  The string of bytes contains type info (in the form
/*    of enum eType) which can be used to call the correct virtual function with "::" syntax.
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#pragma once

//======================================================================================================================
//  CLASS
//======================================================================================================================


//----------------------------------------------------------------------------------------------------------------------
//  MESSAGE -- BASE
//----------------------------------------------------------------------------------------------------------------------
class cMessage
{
public:

	const enum eType { none, mouseover, broadcast_key_event };
	cMessage () : Type ( none ), pFromMach(0) {}
	virtual const wchar_t * print ( mojo::cStrW * pRet );
	unsigned uLen;
	mojo::cMach * pFromMach;
	eType Type;
};


//----------------------------------------------------------------------------------------------------------------------
//  MESSAGE -- BROADCAST KEY EVENT
//----------------------------------------------------------------------------------------------------------------------
class cMessageBroadcastKeyEvent : public cMessage
{
public:

	cMessageBroadcastKeyEvent ( WPARAM wParamArg, KBDLLHOOKSTRUCT * pKbhsArg ) : wParam ( wParamArg ), kbhs ( *pKbhsArg ) { Type = broadcast_key_event; uLen = sizeof(cMessageBroadcastKeyEvent); }
	virtual const wchar_t * print ( mojo::cStrW * pRet );
	WPARAM wParam;
	KBDLLHOOKSTRUCT kbhs;
};


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