/********************************************************************************************************
/*
/*    SettingsBase.h / mojo_engine
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



//======================================================================================================================
// CLASSES
//======================================================================================================================

class cFileIn;
class cFileOut;

namespace mojo
{

#pragma warning ( push )
#pragma warning ( disable : 4251 ) // class 'mojo::cVersion' needs to have dll-interface to be used by clients of class 'mojo::cSettingsBase'

//----------------------------------------------------------------------------------------------------------------------
//  CLASS SETTINGS BASE
//----------------------------------------------------------------------------------------------------------------------
class MOJO_ENGINE_API cSettingsBase
{
public:
	enum				_eType { novalue=0, boolean, integer, uint, version, string, rect_i };
	void				save_to_file ();
	bool				load_from_file();
	void				set_pathname ( const wchar_t * pArg ) { sPathname = pArg; }
	void				restore_defaults();
	mojo::cVersion		PrevVersion; // the version that created the loaded settings file

	bool 				get_val_from_name ( void ** ppRetVal, _eType * pRetType, const wchar_t * pName ); // used by cDlgVars
	bool				set_val_from_name ( const wchar_t * pName, const void * pNewVal, _eType eType = novalue );
	// bool				set_val_from_name ( const wchar_t * pName, bool bNewVal ); // used by cDlgVars
	// functions for setting various types

	bool				set_val_from_name ( const wchar_t * pName, bool bNewVal ); // used by cDlgVars

protected:


	struct sEntry
	{
		int				offset; // offset of address of item from start of struct
		_eType			type;   // type
		const wchar_t *	name;   // name
	};

	void				write_item ( cFileOut * pFile, const wchar_t * pName, const wchar_t * pValue );
	void *				entry_to_item ( sEntry * e ) { return (void *) ( (BYTE*)this + e->offset ); }
	sEntry * 			name_to_entry ( const wchar_t * pName );
	mojo::cStrW		sPathname; // fully qualified name of file

	//-----------------------------------
	// DEFINE FOLLOWING IN DERIVED CLASS
	//-----------------------------------

	virtual sEntry * 	table ( int i = 0 ) = 0;
	virtual int			qty_entries () = 0;
	virtual void		init () = 0;
	
#if 0
	static sEntry		aTable[]; // define in derived class
#endif 

};

# pragma warning ( pop )

} // namespace