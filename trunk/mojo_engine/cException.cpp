/********************************************************************************************************
/*
/*    cException.cpp / mojo_engine
/*
/*    Copyright 2009 Robert Sacks
/*
/*    This file is part of Mojo.  You may redistribute and/or modify Mojo under the terms of the GNU 
/*    General Public License, version 3, as published by the Free Software Foundation.  You should have
/*    received a copy of the license with mojo.  If you did not, go to http://www.gnu.org.
/*
/*    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR 
/*    IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND 
/*    FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
/*    CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
/*    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
/*    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER 
/*    IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT 
/*    OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
/*
/********************************************************************************************************/

#include "stdafx.h"


using namespace mojo;
using namespace std;

//=======================================================================================================
//  CODE
//=======================================================================================================

//-------------------------------------------------------------------------------------------------------
//  CONSTRUCTOR
//  loads from multimap and replaces format specification fields
//-------------------------------------------------------------------------------------------------------
cException :: cException ( const wchar_t * pKeyArg, va_list pArgs ) : cMemo ( error, pKeyArg, pArgs )
{

}


//----------------------------------------------------------------------------------------------------------------------
//  CONSTRUCTOR
//----------------------------------------------------------------------------------------------------------------------
cException :: cException ( const wchar_t * pKey, ... )
{
	va_list args;
  	va_start ( args, pKey );
	eSeverity = error;
	load_from_multimap ( pKey, args );
	eSeverity = error;
}

