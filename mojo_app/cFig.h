/***********************************************************************************************************************
/*
/*    cFig.h / mojo_app
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
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
class cFig : public cTree
{
public:

	cFig ();
	DWORD dwSerialNumber;
	virtual void set_from_xml ( void * pvDest, const wchar_t * pTxt ) const; // override only if class is a "terminal" node
	virtual void write_to_xml ( mojo::cStrW * pRet, void * pObject, const wchar_t * pTagName ) const;
	virtual void fig_to_ctrl ( HWND hwnd, void * pVar ) const { hwnd; pVar; assert(0); } // override only if class is a "terminal" node
	virtual void ctrl_to_fig ( void * pVar, HWND hwnd ) const { pVar; hwnd; assert(0); } // override only if class is a "terminal" node
	virtual cFig * clone () const { assert(0); return 0; }
	virtual cFig & operator= ( const cFig & ) { assert(0); return *this; }
	cFig * get_by_typeid ( const char * pRawName );

protected:
	
	struct sEntry
	{
		const wchar_t *	pwTag;         // Name used in XML tag
		int				iOffset;       // offset of address of storage variable from start of object
		const cFig *    pDefaultFig;   // a const object of appropriate class that's used to call static funcs
	};

	const sEntry * name_to_entry ( const wchar_t * pTxt ) const;
	virtual const sEntry * table () const { return 0; }

private:

	cFig * get_by_handle ( DWORD dwHandle );

	friend class cDlgVarsFig;
	friend class cFigMgr;
};



//----------------------------------------------------------------------------------------------------------------------
//  UNSIGNED
//----------------------------------------------------------------------------------------------------------------------
class cFigUnsigned : public cFig
{
public:

	virtual void set_from_xml ( void * pDest, const wchar_t * pTxt ) const; // override only if class is a "terminal" node
	static const cFigUnsigned Default;
};


//----------------------------------------------------------------------------------------------------------------------
//  STRING
//----------------------------------------------------------------------------------------------------------------------
class cFigStr : public cFig
{
public:
	virtual void fig_to_ctrl ( HWND hwnd, void * pVar ) const; // override only if class is a "terminal" node
	virtual void ctrl_to_fig ( void * pVar, HWND hwnd ) const; // override only if class is a "terminal" node
	virtual void set_from_xml ( void * pDest, const wchar_t * pTxt ) const; // override only if class is a "terminal" node
	virtual void write_to_xml ( mojo::cStrW * pRet, void * pObject, const wchar_t * pTagName ) const;
	static const cFigStr Default;
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