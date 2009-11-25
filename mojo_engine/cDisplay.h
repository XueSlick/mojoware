/***********************************************************************************************************************
/*
/*    cDisplay.h / mojo_engine
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/


#pragma once

namespace mojo {


//======================================================================================================================
// CLASSES
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
// CLASS DISPLAY
//----------------------------------------------------------------------------------------------------------------------
struct MOJO_ENGINE_API cDisplay : mojo::cRectI
{
public:

	cDisplay ( const cDisplay & rh );
	cDisplay & operator= ( const cDisplay & );

	cDisplay () :  pNext(0), pPrev(0) {}
	int iIndex;
	cDisplay * pNext;
	cDisplay * pPrev;
};


//----------------------------------------------------------------------------------------------------------------------
// CLASS DISPLAY LIST
//----------------------------------------------------------------------------------------------------------------------
struct MOJO_ENGINE_API cDisplayList : public tList2<cDisplay>
{
public:

	bool		is_point_in				( mojo::cPtI * pt );
	void		set_from_local_hardware ();
	void		calc_mult_virt_rect		( mojo::cRectI * pRet );
	wchar_t *	text					( mojo::cStrW * pRet );
	void		set_from_text			( const wchar_t * p );
};


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