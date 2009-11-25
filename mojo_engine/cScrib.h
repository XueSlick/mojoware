/********************************************************************************************************
/*
/*    cScrib.h / mojo_engine
/*
/*    See cScribMgr.h for an explanation.
/*
/*    Copyright 2009 Robert Sacks.
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

#pragma once

namespace mojo
{

//-------------------------------------------------------------------------------------------------------
//  CLASS SCRIB
//-------------------------------------------------------------------------------------------------------

#pragma warning ( push )
#pragma warning ( disable : 4275 ) // non dll-interface class 'mojo::cStrW' used as base for dll-interface class 'mojo::cScrib'

class MOJO_ENGINE_API cScrib : public cStrW
{
public:

	bool replace_format_specification_fields ( va_list pArgs );
	cScrib & operator= ( const wchar_t * rh ) { return static_cast<cScrib &> (cStrW::operator= ( rh ) ); }

private:


};

#pragma warning ( pop )



}

