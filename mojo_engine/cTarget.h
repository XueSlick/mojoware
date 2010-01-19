/***********************************************************************************************************************
/*
/*    cTarget.h / mojo_engine
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#pragma once

//======================================================================================================================
//  CLASS
//======================================================================================================================
namespace mojo
{

class MOJO_ENGINE_API cTarget
{
public:

	void serialize ( mojo::cStrW * pRet ) const;
	size_t deserialize ( const wchar_t * p ); // returns increment of p

	cTarget ();
	cTarget ( const cTarget & p ); 
	cTarget & operator= ( const cTarget & r );
	DWORD hMach;
	HWND hwnd;
	DWORD dwProcessID;
	mojo::cStrW sName;
	mojo::cStrW sPath;
	bool bLaunchByMojo;
	DWORD dwID; // NOT USED BY ENGINE; ASSIGNED BY APP
	bool bIsRunning;
	cTarget * pPrev;
	cTarget * pNext;

	// temp 5 jan 2010
	BYTE aKeyboardState [256];
};

} // namespace



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