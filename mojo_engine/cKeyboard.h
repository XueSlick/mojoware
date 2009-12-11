/********************************************************************************************************
/*
/*    cKeyboard.h / mojo_engine
/*
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/*    This class converts key codes to names of keys and vice versa.  See cKeyboard.cpp
/*    for an explanation of the data format.
/*
/*******************************************************************************************************/

#pragma once

namespace mojo
{

//=======================================================================================================
// CLASSES
//=======================================================================================================

class MOJO_ENGINE_API cKeyboard
{
private:

	struct sExVK
	{
		wchar_t * pwName;
		wchar_t * pwPrettyName;
		WORD	wExVK;
	};

public:

	static const wchar_t *    pretty_key_event ( mojo::cStrW * pRet, const KBDLLHOOKSTRUCT * pHS );
	static const wchar_t *    ex_vk_to_name ( WORD dwExVK );
	static const wchar_t *    ex_vk_to_pretty_name ( WORD dwExVK );
	static WORD               name_to_ex_vk ( const wchar_t * pName );
	static WORD               pretty_name_to_ex_vk ( const wchar_t * pName );
	static bool               is_modifier ( WORD wExVK );
	static bool               is_mouse_button ( WORD wExVK );
private:

	static                    sExVK aExVK [];
	bool                      verify_table ();
};

} // namespace

/********************************************************************************************************
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
/********************************************************************************************************/

