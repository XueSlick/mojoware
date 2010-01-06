/***********************************************************************************************************************
/*
/*    cDlgVarsFig.cpp / mojo_app
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"


//----------------------------------------------------------------------------------------------------------------------
//  DLG TO VARS
//----------------------------------------------------------------------------------------------------------------------
void cDlgVarsFig :: dlg_to_vars ()
{
	//------------------------------------------
	//  FOR EACH DATUM IN THE DLG
	//------------------------------------------

	for ( sDlgDatum * pDatum = this->pData; pDatum->pName; pDatum++ )
	{
		assert ( pDatum->pName );

		//------------------------------------------
		//  GET FIG ITEM
		//------------------------------------------

		const cFig :: sEntry * pEntry = this->pFigCopy -> name_to_entry ( pDatum->pName );

		if ( ! pEntry )
		{
			assert(0);
			continue;
		}

		pEntry->pDefaultFig->ctrl_to_fig ( (void*) ((BYTE*) pFigCopy + pEntry->iOffset ), GetDlgItem ( hwnd, pDatum->iID ) );
	}
}


//----------------------------------------------------------------------------------------------------------------------
//  SETTINGS TO DLG
//----------------------------------------------------------------------------------------------------------------------
void cDlgVarsFig :: vars_to_dlg ()
{
	//------------------------------------------
	//  FOR EACH DATUM IN THE DLG
	//------------------------------------------

	for ( sDlgDatum * pDatum = this->pData; pDatum->pName; pDatum++ )
	{
		assert ( pDatum->pName );

		//------------------------------------------
		//  GET FIG ITEM
		//------------------------------------------

		const cFig :: sEntry * pEntry = this->pFigCopy -> name_to_entry ( pDatum->pName );

		if ( ! pEntry )
		{
			assert(0);
			continue;
		}

		pEntry->pDefaultFig->fig_to_ctrl( GetDlgItem ( hwnd, pDatum->iID ), (void*) ((BYTE*) pFigCopy + pEntry->iOffset ) );
	}
}


//----------------------------------------------------------------------------------------------------------------------
// WM INIT
//----------------------------------------------------------------------------------------------------------------------
void cDlgVarsFig :: wm_init ( HWND hwndArg, sDlgDatum * pDataArg, cFig * pFig )
{
	assert ( pDataArg );
	pData = pDataArg;

	if ( pFigCopy )
	{
		delete pFigCopy;
		pFigCopy = 0;
		pFigOriginal = 0;
	}

	pFigCopy = pFig->clone();
	pFigOriginal = pFig;

	hwnd = hwndArg;
	vars_to_dlg();
}


//----------------------------------------------------------------------------------------------------------------------
//  DESTRUCTOR
//----------------------------------------------------------------------------------------------------------------------
cDlgVarsFig :: ~cDlgVarsFig ()
{
	if ( pFigCopy )
	{
		delete pFigCopy;
		pFigCopy = 0;
		pFigOriginal = 0;
	}
}


#if 0
//----------------------------------------------------------------------------------------------
// UPDATE VAR -- bool
//----------------------------------------------------------------------------------------------
void sDlgDatumBool::update_var ( HWND hParent, cSettings * pSettingsArg )
{
	pSettings = pSettingsArg;
	
	if ( IsDlgButtonChecked ( hParent, iID ) )
		*pVar = true;

	else
		*pVar = false;
}


//----------------------------------------------------------------------------------------------
// UPDATE CTRL -- bool
//----------------------------------------------------------------------------------------------
void sDlgDatumBool::update_ctrl ( HWND hParent, cSettings * pSettingsArg )
{
	pSettings - pSettingsArg;

	if ( *pVar )
		CheckDlgButton ( hParent, iID, TRUE );

	else
		CheckDlgButton ( hParent, iID, FALSE );
}





//----------------------------------------------------------------------------------------------------------------------
// UPDATE VARS
//----------------------------------------------------------------------------------------------------------------------
void cDlgVarsFig :: update_vars ()
{
	for ( sDlgDatum * d = DlgData.pHead; d; d = d->pNext )
	{
		d->update_var ();
	}
}


//----------------------------------------------------------------------------------------------------------------------
// UPDATE DLG
//----------------------------------------------------------------------------------------------------------------------
void cDlgVarsFig :: update_dlg ()
{
	for ( sDlgDatum * d = DlgData.pHead; d; d = d->pNext )
	{
		d->update_ctrl ();
	}
}


#if 0
//----------------------------------------------------------------------------------------------
// UPDATE VAR -- INT
//----------------------------------------------------------------------------------------------
void sDlgDatumInt::update_var ( HWND hParent, void * pVar, int iID )
{
	HWND hCtrl = GetDlgItem ( hParent, iID );
	wchar_t b[100];
	Edit_GetText ( hCtrl, b, sizeof(b)-1 );
	*(int*)pVar = wcstoul ( b, NULL, 10 );
}


//----------------------------------------------------------------------------------------------
// UPDATE CTRL -- INT
//----------------------------------------------------------------------------------------------
void sDlgDatumInt::update_ctrl ( HWND hParent, int ID, void * pVar )
{
	HWND hCtrl = GetDlgItem ( hParent, iID );
	wchar_t b[200];
	int iVal = *(int*)pVar;
	_itow_s ( iVal, b, sizeof(b)/sizeof(wchar_t), 10 );
	Edit_SetText ( hCtrl, b );
}
#endif

#if 0

//----------------------------------------------------------------------------------------------
// UPDATE VAR -- UINT
//----------------------------------------------------------------------------------------------
void sDlgDatumUnsigned::update_var ()
{
	HWND hCtrl = GetDlgItem ( hParent, iID );
	wchar_t b[100];
	Edit_GetText ( hCtrl, b, sizeof(b)-1 );
	*(unsigned*)pVar = wcstoul ( b, NULL, 10 );
}

//----------------------------------------------------------------------------------------------
// UPDATE CTRL -- UINT
//----------------------------------------------------------------------------------------------
void sDlgDatumUnsigned::update_ctrl ()
{
	HWND hCtrl = GetDlgItem ( hParent, iID );
	wchar_t b[200];
	unsigned uVal = *(unsigned*)pVar;
	_ultow_s ( uVal, b, sizeof(b)/sizeof(wchar_t), 10 );
	Edit_SetText ( hCtrl, b );
}

#endif

#endif


/***********************************************************************************************************************
/*
/*    This file is part of Mojo.  For more information, see http://mojoware.org.
/*
/*    You may redistribute and/or modify Mojo under the terms of the GNU General Public License, version 3, as
/*    published by the Free Software Foundation.  You should have received a copy of the license with mojo.  If you
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