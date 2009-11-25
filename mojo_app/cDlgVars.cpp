/***********************************************************************************************************************
/*
/*    cDlgVars.cpp / mojo_app
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"

//----------------------------------------------------------------------------------------------------------------------
//  DLG TO SETTINGS
//----------------------------------------------------------------------------------------------------------------------
void cDlgVars :: dlg_to_settings ( cSettings * pSettings, HWND hwnd )
{
	//------------------------------------------
	//  FOR EACH DATUM IN THE DLG
	//------------------------------------------

	for ( sDlgDatum * p = this->pDlgData; p->pName; p++ )
	{
		assert ( p->pName );

		//------------------------------------------
		//  GET DATA FROM SETTINGS
		//------------------------------------------

		cSettingsBase::_eType eType;
		void * pVal;
		if ( ! pSettings->get_val_from_name ( &pVal, &eType, p->pName ) )
			continue;
		assert ( pVal );

		//------------------------------------------
		//  GET CTRL HANDLE
		//------------------------------------------

		HWND hCtrl = GetDlgItem ( hwnd, p->iID );
		assert ( hCtrl );
		if ( ! hCtrl )
			continue;

		//------------------------------------------
		//  SET VARIABLE IN SETTINGS
		//------------------------------------------

		switch ( eType )
		{
		case cSettingsBase::boolean:
			(*(bool*)pVal) = IsDlgButtonChecked ( hwnd, p->iID ) ? true : false;
			break;

		case cSettingsBase::uint:
			{
				wchar_t t[100];
				Edit_GetText ( hCtrl, t, sizeof(t)/sizeof(wchar_t) );
				int x = wcstoul ( t, NULL, 10 );
				(*(unsigned*)pVal) = x;
			}
 			break;
		}
	}	
}


//----------------------------------------------------------------------------------------------------------------------
//  SETTINGS TO DLG
//----------------------------------------------------------------------------------------------------------------------
void cDlgVars :: settings_to_dlg ( HWND hwnd, cSettings * pSettings )
{
	//------------------------------------------
	//  FOR EACH DATUM IN THE DLG
	//------------------------------------------

	for ( sDlgDatum * p = this->pDlgData; p->pName; p++ )
	{
		assert ( p->pName );

		//------------------------------------------
		//  GET DATA FROM SETTINGS
		//------------------------------------------

		cSettingsBase::_eType eType;
		void * pVal;
		if ( ! pSettings->get_val_from_name ( &pVal, &eType, p->pName ) )
			continue;
		assert ( pVal );

		//------------------------------------------
		//  GET CTRL HANDLE
		//------------------------------------------

		HWND hCtrl = GetDlgItem ( hwnd, p->iID );
		assert ( hCtrl );
		if ( ! hCtrl )
			continue;

		//------------------------------------------
		//  SET CTRL
		//------------------------------------------

		switch ( eType )
		{
		case cSettingsBase::boolean:
			{
				bool bVal = *(bool*)pVal;
				BOOL iVal = bVal ? TRUE : FALSE;
				CheckDlgButton ( hwnd, p->iID, iVal );
			}
			break;

		case cSettingsBase::uint:
			{
				unsigned uVal = *(unsigned*)pVal;
				wchar_t t [100];
				_ultow_s ( uVal, t, sizeof(t)/sizeof(wchar_t), 10 );
				Edit_SetText ( hCtrl, t );
			}
			break;
		}
	}
}



//----------------------------------------------------------------------------------------------------------------------
// WM INIT
//----------------------------------------------------------------------------------------------------------------------
void cDlgVars :: wm_init ( HWND hwnd, sDlgDatum * pDlgDataArgs )
{
	assert ( pDlgDataArgs );
	pDlgData = pDlgDataArgs;
	settings_to_dlg ( hwnd, &g_Settings );
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
void cDlgVars :: update_vars ()
{
	for ( sDlgDatum * d = DlgData.pHead; d; d = d->pNext )
	{
		d->update_var ();
	}
}


//----------------------------------------------------------------------------------------------------------------------
// UPDATE DLG
//----------------------------------------------------------------------------------------------------------------------
void cDlgVars :: update_dlg ()
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