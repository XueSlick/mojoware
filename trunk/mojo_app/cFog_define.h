/***********************************************************************************************************************
/*
/*    cFog_define.h / mojo_app
/*   
/*    Copyright 2010 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#pragma once


//======================================================================================================================
//  DEFINES
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
//  ENTRY
//
//	struct sEntry
//	{
//		const wchar_t *	pwTag;         // Name used in XML tag
//		int				iOffset;       // offset of member from start of the object that contains it
//		fCREATE *       pfCreate;      // 
//	};
//
//  ENTRY ( Tag, 
//
//----------------------------------------------------------------------------------------------------------------------


// #define ENTRY(a,b,c,d) L#a, int ( (BYTE*)&Default.b - (BYTE*)&Default ), &c::Default

// #define ENTRY_TREE(tag,parentclass,childclass) L#tag, false, 0, &childclass::create

#define ENTRY_TREE(   tag, childclass )             L#tag, 0,                                    &childclass::create
#define ENTRY_MEMBER( tag, thisclass, membername )  L#tag, int (offsetof(thisclass,membername)), 0

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