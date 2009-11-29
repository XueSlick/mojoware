/***********************************************************************************************************************
/*
/*    SettingsBase.cpp / mojo_engine
/*
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"
#include "cSettingsBase.h"

using namespace mojo;


//======================================================================================================================
// DEFINES
//======================================================================================================================

#define ENTRY(a,b,c) int ( (BYTE*)&DefaultSettings.a - (BYTE*)&DefaultSettings ),   	\
					 b,																\
					 c	// DON'T ERASE THIS LINE


//======================================================================================================================
// DATA
//======================================================================================================================

//======================================================================================================================
// CODE
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
//  CONSTRUCTOR
//----------------------------------------------------------------------------------------------------------------------
cSettingsBase :: cSettingsBase ( const wchar_t * pSignature, mojo::cVersion * pVersion )
{
	CurrentVersion = *pVersion;
	sSignature = pSignature;
}


//----------------------------------------------------------------------------------------------------------------------
//  SET VALUE FROM NAME
//  Used by cDlgVars
//----------------------------------------------------------------------------------------------------------------------
bool cSettingsBase :: set_val_from_name ( const wchar_t * pName, const void * pNewVal, _eType eType )
{
	//------------------------------------------
	// CHECK PARMS
	//------------------------------------------

	assert ( pName );
	assert ( pNewVal );

	//------------------------------------------
	// GET ENTRY
	//------------------------------------------

	sEntry * pEntry = name_to_entry ( pName );
	assert ( pEntry );
	if ( ! pEntry )
		return false;

	//------------------------------------------
	// IF CALLER SPECIFIED TYPE, CHECK TYPES
	//------------------------------------------

	if ( eType != novalue && pEntry->type != eType )
	{
		assert(0);
		return false;
	}

	//------------------------------------------
	// GET ADDRESS OF VARIABLE IN SETTINGS
	//------------------------------------------

	void * pStorage = entry_to_item ( pEntry );
	assert ( pStorage );

	//------------------------------------------
	// SET THE VALUE
	//------------------------------------------

	switch ( pEntry->type )
	{
	case boolean:
		(*(bool*)pStorage) = (*(bool*)pNewVal);
		break;

	case uint:
		(*(unsigned*)pStorage) = (*(unsigned*)pNewVal);
		break;

	default:
		assert(0);
	}

	return true;
}


//----------------------------------------------------------------------------------------------------------------------
//  GET VAL FROM NAME
//  Used by cDlgVars
//----------------------------------------------------------------------------------------------------------------------
bool cSettingsBase :: get_val_from_name ( void ** ppRetVal, _eType * pRetType, const wchar_t * pName ) // used by cDlgVars
{
	assert ( pName );
	sEntry * pEntry = name_to_entry ( pName );
	assert ( pEntry );
	if ( ! pEntry )
		return false;
	*pRetType = pEntry->type;
	void * pVal = entry_to_item ( pEntry );
	assert ( pVal );
	if ( pVal )
	{
		*ppRetVal = pVal;
		return true;
	}

	else
	{
		*ppRetVal = NULL;
		return false;
	}
}


//---------------------------------------------------------------------------------------------
// NAME TO ENTRY
//---------------------------------------------------------------------------------------------
cSettingsBase::sEntry * cSettingsBase :: name_to_entry ( const wchar_t * pNameArg )
{
	assert ( pNameArg );

	for ( int i = 0; i < qty_entries(); i++ )
	{
		const wchar_t * pName = table(i)->name;
		pName++;
		pName--;

		if ( 0 == wcscmp ( pNameArg, table(i)->name ) )
			return table(i);
	}

	assert(0);
	return NULL;
}


//---------------------------------------------------------------------------------------------
// SAVE TO FILE
//---------------------------------------------------------------------------------------------
void cSettingsBase :: save_to_file ()
{
	assert ( sPathname.len() );

	cFileOut f ( sPathname.cstr() );
	FILE * h = f.h;

	if ( 0 == h )
		return;

	//-----------------------------------------
	//  SAVE VERSION AND SIGNATURE
	//-----------------------------------------
	{
		write_item ( &f, L"sSignature", sSignature.cstr() );
		cStrW t;
		CurrentVersion.get_text ( &t );
		write_item ( &f, L"Version", t.cstr() );
	}

	//-----------------------------------------
	//  SAVE THE DERIVED CLASS'S STUFF
	//-----------------------------------------

	for ( int i = 0; i < qty_entries(); i++ )
	{
		sEntry * e = table(i);

		switch ( e->type )
		{
		case uint:
			{
				unsigned * p = (unsigned*) entry_to_item(e);
				wchar_t t [500];
				wsprintf ( t, L"%u", *p );
				write_item ( &f, e->name, t );
			}
			break;

		case boolean:
			{
				bool * p = (bool*) entry_to_item(e);
				write_item ( &f, e->name, *p ? L"true" : L"false" );
			}
			break;

		case string:
			write_item ( &f, e->name, ((cStrW*) entry_to_item(e))->cstr() );
			break;

		case version:
			{
				cStrW t;
				::cVersion * p = (::cVersion*) entry_to_item (e);
				p->get_text ( &t );
				write_item ( &f, e->name, t.cstr() );
			}
			break;

		case rect_i:
			{
				wchar_t t [500];
				cRectI * p = (cRectI*) entry_to_item(e);
				wsprintf ( t, L"%d %d %d %d", p->x, p->y, p->dx, p->dy );
				write_item ( &f, e->name, t );
			}
		}
	}
}


//---------------------------------------------------------------------------------------------
// LOAD FROM FILE
//---------------------------------------------------------------------------------------------
bool cSettingsBase :: load_from_file ()
{
	sLoadedSignature.erase();
	LoadedVersion = cVersion ( 0, 0, 0, 0 );
	init();

	assert ( sPathname.len() );

	cFileIn f ( sPathname.cstr()  );
	FILE * h = f.h;

	if ( 0 == h )
		return false;

	cStrW sLine;
	cStrW aRay[2];

	for ( ; f.get_line ( &sLine ); sLine.erase() )
	{
		match ( aRay, 2, sLine.cstr(), L"(\\S+) = (.+)" );
		const wchar_t * pKey   = aRay[0].cstr();
		const wchar_t * pValue = aRay[1].cstr();

		//------------------------------------------
		//  GET THE TWO BASE CLASS ITEMS FIRST
		//------------------------------------------

		if ( 0 == wcscmp ( L"sSignature", pKey ) )
		{
			sLoadedSignature = pValue;
			continue;
		}

		else if ( 0 == wcscmp ( L"Version", pKey ) )
		{
			LoadedVersion.set_from_text ( pValue );
			continue;
		}

		//------------------------------------------
		//  GET THE DERIVED CLASS (TABLE) ITEMS
		//------------------------------------------

		sEntry * e = name_to_entry ( aRay[0].cstr() );

		if ( ! e )
		{
			#ifdef _DEBUG
				assert(e);
			#endif

			continue;
		}

		switch ( e->type )
		{
		case uint:
			{
				wchar_t * pwStop;
				unsigned *pR = (unsigned*) entry_to_item(e);
				*pR = wcstoul ( pValue, &pwStop, 10 ); 
			}

			break;

		case boolean:
			{
				bool * pB = (bool*) entry_to_item(e);
				if ( 0 == wcscmp ( L"true", pValue ) )
					*pB = true;
				else
					*pB = false;
			}
			break;

		case string:
			*((cStrW*) entry_to_item (e)) = aRay[1];
			break;

		case version:
			assert(0); // REPLACED BY BASE CLASS nov 28 2009 // LoadedVersion.set_from_text ( aRay[1].cstr() );
			break;

		case rect_i:
			{
				cStrW aRay[4];
				::match ( aRay, 4, L"(\\d+) (\\d+) (\\d+) (\\d+)", pValue );
				cRectI *pR = (cRectI*) entry_to_item(e);
				pR->x  = _wtoi ( aRay[0].cstr() );
				pR->y  = _wtoi ( aRay[1].cstr() );
				pR->dx = _wtoi ( aRay[2].cstr() );
				pR->dy = _wtoi ( aRay[3].cstr() );
			}

			break;
		}
	}

	return true;
}


//---------------------------------------------------------------------------------------------
//  DEFAULT VALUES
//---------------------------------------------------------------------------------------------
void cSettingsBase :: restore_defaults ()
{
	init();
}


//---------------------------------------------------------------------------------------------
//  WRITE ITEM
//---------------------------------------------------------------------------------------------
void cSettingsBase::write_item ( cFileOut * pFile, const wchar_t * pName, const wchar_t * pValue )
{
	assert ( pFile );
	assert ( pName );
	assert ( pValue );
	fwprintf ( pFile->h, L"%s = %s\n", pName, pValue );
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