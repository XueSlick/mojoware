/***********************************************************************************************************************
/*
/*    cInputEventBuffer.h / mojo_engine
/*
/*    When the keyboard hook and mouse hook receive info from the OS, they copy the info to the circular
/*    buffer contained inside this class using the receive() functions.   At the same time, the hook thread 
/*    sends a message to the exe telling it that the info is ready.   The exe can then (if it wants) read 
/*    the info from the circular buffer using its own thread and the API function mojo::get_input_event.
/*    The data is made thread safe by a critical section built into template tCircBuf.
/*
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#pragma once


//=======================================================================================================
// PRELIMS
//=======================================================================================================


//=======================================================================================================
// CLASS
//=======================================================================================================

//-------------------------------------------------------------------------------------------------------
//  CLASS EVENT BUFFER
//-------------------------------------------------------------------------------------------------------
class cInputEventBuffer
{
public:

	cInputEventBuffer ();;

	void set_hwnd ( HWND hArg );

	void receive   ( WPARAM wParam, KBDLLHOOKSTRUCT * p );
	void receive   ( WPARAM wParam, MSLLHOOKSTRUCT * p );

private:

	mojo::tCircBuf<mojo::cInputEvent> Buf;

	friend bool mojo::get_input_event ( cInputEvent * );

	HWND hwnd; // output window
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