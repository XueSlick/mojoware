/***********************************************************************************************************************
/*
/*    cFog.h / mojo_app
/*   
/*    Copyright 2010 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/


#pragma once


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
//  BASE CLASS
//----------------------------------------------------------------------------------------------------------------------
class cFog
{
public:

	cFog () : dwSerialNumber ( ++dwLastSerialNumberAssigned ) {}
	DWORD dwSerialNumber;
	virtual cFog * get_by_handle ( DWORD dwHandle );
	virtual cFog * clone () const { assert(0); return 0; }

protected:

	virtual void set_from_xml ( const wchar_t * pTxt ) = 0;
	friend class cFogMgr;
	friend class cFogTree;

private:
	
	static DWORD dwLastSerialNumberAssigned;
};


//----------------------------------------------------------------------------------------------------------------------
//  FOG TREE
//  A fog that is a node in the underlying tree that can contain members
//----------------------------------------------------------------------------------------------------------------------
class cFogTree : public cFog, public cTree
{
public:

	virtual cFog * get_by_handle ( DWORD dwHandle );


protected:

	typedef cFog * fCREATE ();
	struct sEntry
	{
		const wchar_t *	pwTag;         // Name used in XML tag
		int				iOffset;       // offset of member from start of the object that contains it
		fCREATE *       pfCreate;      // 
	};

	static cFog * create () { return 0; }

	const sEntry * name_to_entry ( const wchar_t * pTxt ) const;

	virtual const sEntry * table () const { return 0; }
	virtual void set_from_xml ( const wchar_t * pTxt );

private:
	
	static DWORD dwLastSerialNumberAssigned;

};


//----------------------------------------------------------------------------------------------------------------------
//  STRING
//----------------------------------------------------------------------------------------------------------------------
class cFogStr : public cFog, public mojo::cStrW
{
public:

	virtual cFog * clone () const { return new cFogStr ( *this ); }

protected:

	virtual void set_from_xml ( const wchar_t * pTxt ) { cStrW::operator= ( pTxt ); }

};



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