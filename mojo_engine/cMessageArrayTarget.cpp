/***********************************************************************************************************************
/*
/*    cMessageArrayTarget.cpp / mojo_engine
/*   
/*    Copyright 2010 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"

using namespace mojo;


//======================================================================================================================
//  DATA
//======================================================================================================================


//======================================================================================================================
//  PROTOTYPES
//======================================================================================================================


//======================================================================================================================
//  CODE
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
//  CREATE
//----------------------------------------------------------------------------------------------------------------------
cMessageArrayTarget * cMessageArrayTarget :: create ( mojo::cArrayTarget * p )
{
	cStrW s;

	int iQty = 0;

	for ( unsigned u = 0; u < p->qty(); u++ )
	{
		iQty++;
		cTarget * t = &(*p)[u];
		t->serialize(&s);
	}

	unsigned uLen = sizeof(cMessageArrayTarget) + s.len() * sizeof(wchar_t);

	BYTE * pBuf = new BYTE [ uLen ];

	cMessageArrayTarget * pMAT = reinterpret_cast < cMessageArrayTarget * > ( pBuf );

	pMAT->Type = array_target;
	pMAT->uLen = uLen;
	pMAT->iQty = iQty;

	memcpy ( pBuf + sizeof(cMessageArrayTarget), s.cstr(), s.len() * sizeof(wchar_t) );

	return pMAT;
}


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