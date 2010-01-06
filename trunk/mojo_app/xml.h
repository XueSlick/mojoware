/***********************************************************************************************************************
/*
/*    xml.h / mojo_app
/*
/*    These functions implement a tiny subset of XML for cFig files.  The resulting files aren't valid XML
/*    because they lack a root element.
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#pragma once

//----------------------------------------------------------------------------------------------------------------------
//   XML standard terms.  In the following example:
//
//   <toon flag = "yeppers">jerry</toon>
//
//   the whole thing is an element.
//   <toon ...> is a tag
//   toon is a name
//   flag = "yeppers" is an attribute
//   flag is an attribute name
//   "yeppers" is an attribute value
//   jerry is content
//
//----------------------------------------------------------------------------------------------------------------------

//======================================================================================================================
//  PROTOTYPES
//======================================================================================================================

bool xml_name_to_content  ( mojo::cStrW * pRet, const wchar_t ** ppHaystack, const wchar_t * pName );
bool xml_get_next_element ( mojo::cStrW * pRetName, mojo::cStrW * pRetAttribute, mojo::cStrW * pRetContent, const wchar_t ** ppHaystack );
void xml_write_start_tag  ( mojo::cStrW * pRet, const wchar_t * pName );
void xml_write_end_tag    ( mojo::cStrW * pRet, const wchar_t * pName );


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

