/***********************************************************************************************************************
/*
/*    cFig_define.h / mojo_app
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#pragma once


//======================================================================================================================
//  DEFINES
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
//  ENTRY
//
//  Used to initialize sEntry.   The arguments mean, in order:
//
//  1.  Name of XML tag
//
//  2.  Name of variable used for storage
//
//  3.  Name of class used to load and save.
//
//  If (2) is omitted, an offset of 0 is passed
//
//----------------------------------------------------------------------------------------------------------------------

#define ENTRY_MEMBERLESS(a,c) L#a, 0, &c::Default

#define ENTRY_MEMBER(a,b,c) L#a, int ( (BYTE*)&Default.b - (BYTE*)&Default ), &c::Default

// ENTRY(a,b,c) L#a, (offsetof(typeid(*this).name(),b), &c::Default


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