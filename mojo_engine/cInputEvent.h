/********************************************************************************************************
/*
/*    cInputEvent.h / mojo_engine
/*
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/*******************************************************************************************************/

#pragma once

//=======================================================================================================
// CLASS
//=======================================================================================================

//-------------------------------------------------------------------------------------------------------
//  CLASS INPUT EVENT
//-------------------------------------------------------------------------------------------------------

namespace mojo
{
#pragma warning ( push )
#pragma warning ( disable : 4251 ) // class 'mojo::cStrW' needs to have dll-interface to be used by clients of struct 'mojo::cInputEvent'


class MOJO_ENGINE_API cInputEvent
{
public:

	enum { keyboard, mouse } eType;
	WPARAM wParam;
	union { KBDLLHOOKSTRUCT k; MSLLHOOKSTRUCT m; } u;

	const wchar_t * print ( cStrW * pRet );

	static void load_scribs ();

private:
	static void load_scribs_sub ( cStrW * pRet, const wchar_t * pKey );
	// SCRIBS:

	static cStrW sMouseMotion;
	static cStrW sScrnPosition;
	static cStrW sWinPosition;
	static cStrW sKeyName;
	static cStrW sEvent;
	static cStrW sVirtualKey;
	static cStrW sScanCode;
	static cStrW sExtended;
};

#pragma warning ( pop )

} // namespace

/********************************************************************************************************
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
/********************************************************************************************************/

