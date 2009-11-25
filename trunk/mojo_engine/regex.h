/********************************************************************************************************
/*
/*    regex.h / mojo_engine
/*
/*    This file contains wrappers for calls to functions in Microsoft's regex library
/*    which is part of the new TR1 stuff in Visual C++ 10.0.
/*
/*    If your compiler doesn't support TR1, you can link to the Boost regex library
/*    instead of Microsoft's.  Just change the namespace directive in regex.cpp from 
/*    "std::tr1" to "boost."
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

#pragma once

#include "mojo_engine.h"

namespace mojo {

MOJO_ENGINE_API bool match ( mojo::cStrW  aRet [], int iRetSize, const wchar_t * pHaystack, const wchar_t * pRgx );
MOJO_ENGINE_API bool get_next_match ( cStrW aRet[], int iArrayQty, const wchar_t ** ppNext, const wchar_t * pRegex  );

} // namespace