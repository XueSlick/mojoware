/***********************************************************************************************************************
/*
/*    SettingsBase.h / mojo_engine
/*
/*    This is the base class for persistent settings.  This class holds settings during runtime, saves them to files,
/*    and loads them when the program restarts.
/*
/*    Don't include members called Version or sSignature in derived classes.  Those two entries
/*    are supplied and handled by the base class.
/*
/*    Define the four following members in your derived class:
/*
/*          aTable
/*          the three virtual functions
/*
/*    See cSettings for an example of how to use this class.
/*
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#pragma once

//======================================================================================================================
// CLASSES
//======================================================================================================================



namespace mojo
{
class cFileOut;
class cFileIn;
#pragma warning ( push )
#pragma warning ( disable : 4251 ) // class 'mojo::cVersion' needs to have dll-interface to be used by clients of class 'mojo::cSettingsBase'

//----------------------------------------------------------------------------------------------------------------------
//  CLASS SETTINGS BASE
//----------------------------------------------------------------------------------------------------------------------
class MOJO_ENGINE_API cSettingsBase
{
public:
	cSettingsBase       ( const wchar_t * pSignature, mojo::cVersion * pVersion );
	enum				_eType { novalue=0, boolean, integer, uint, version, string, rect_i };
	void				save_to_file ();
	bool				load_from_file();
	void				set_pathname ( const wchar_t * pArg ) { sPathname = pArg; }
	void				restore_defaults();

	mojo::cVersion		LoadedVersion;         // the version that created the loaded settings file
	mojo::cVersion		CurrentVersion;        // the version that will be saved on next save
	mojo::cStrW         sSignature;            // typically name of program; used for first data member in file
	mojo::cStrW         sLoadedSignature;

	bool 				get_val_from_name ( void ** ppRetVal, _eType * pRetType, const wchar_t * pName ); // used by cDlgVars
	bool				set_val_from_name ( const wchar_t * pName, const void * pNewVal, _eType eType = novalue );
	bool				set_val_from_name ( const wchar_t * pName, bool bNewVal ); // used by cDlgVars

protected:

	struct sEntry
	{
		int				offset; // offset of address of item from start of struct
		_eType			type;   
		const wchar_t *	name;   
	};

	void				write_item ( mojo::cFileOut * pFile, const wchar_t * pName, const wchar_t * pValue );
	void *				entry_to_item ( sEntry * e ) { return (void *) ( (BYTE*)this + e->offset ); }
	sEntry * 			name_to_entry ( const wchar_t * pName );
	mojo::cStrW         sPathname; // fully qualified name of file

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
