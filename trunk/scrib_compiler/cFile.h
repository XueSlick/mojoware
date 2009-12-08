/********************************************************************************************************
/*
/*    cFile
/*
/*    Copyright 2009 Robert Sacks
/*
/*    This file is part of OsBxr.  You may redistribute and/or modify OsBxr under the terms of the GNU 
/*    General Public License, version 3, as published by the Free Software Foundation.  You should have
/*    received a copy of the license with OsBxr.  If you did not, go to http://www.gnu.org.
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

#if 1

#pragma once

#include <stdio.h>


//=======================================================================================================
// CLASSES
//=======================================================================================================

class cFileIn
{
public:

	cFileIn ( const wchar_t * pName );
	~cFileIn();
	FILE * h;
	bool get_line ( mojo::cStrW * s );
	mojo::cStrW sName;
};

class cFileOut
{
public:

	cFileOut ( const wchar_t * pName );
	~cFileOut();
	FILE * h;
	mojo::cStrW sName;
};

#endif