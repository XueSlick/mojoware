/***********************************************************************************************************************
/*
/*    cFigMgr.cpp / mojo_app
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"

//======================================================================================================================
//  DATA
//======================================================================================================================

DWORD cFigMgr :: dwLastSerialNumberAssigned = 0;


//======================================================================================================================
//  PROTOTYPES
//======================================================================================================================


//======================================================================================================================
//  CODE
//======================================================================================================================

//---------------------------------------------
//  SET ENGINE LAUNCH TARGETS
//---------------------------------------------
void cFigMgr :: set_engine_launch_targets ()
{
	cFigWoWTree * pFWT = Root.get_wow_tree ();

	for ( cTree * p = pFWT->pRight; p; p = p->pRight )
	{
		cFigWoW * w = reinterpret_cast<cFigWoW*>(p);

		cTarget t;

		w->set_target ( &t );
		w->dwTargetID = w->dwSerialNumber;

		mojo::set_launch_target ( &t );
	}
}


//----------------------------------------------------------------------------------------------------------------------
//  RECEIVE (TARGETS)
//  Receives updated target list from engine.  Called by GUI thread when engine notifies app that target list
//  has changed.
//----------------------------------------------------------------------------------------------------------------------
void cFigMgr :: receive ( mojo::cArrayTarget * pAT )
{
	// 2 x 2 matrix
	//  found or launch_by_mojo
	//  local or remote

	bool bChanged = false;

	cFigWoWTree * const pRoot = Root.get_wow_tree ();
	assert ( pRoot);

#if 0
	put_ad_lib_memo ( cMemo::info, L"cFigMgr::receive", L"Qty in array: %d", pAT->qty() );
#endif

	//-------------------------------------------------
	//  FOR EACH ITEM IN ARRAY, ADD OR UPDATE
	//  CORRESPONDING ITEM IN TREE
	//-------------------------------------------------

	// WoWList.lock();
	{
		for ( unsigned u = 0; u < pAT->qty(); u++ )
		{
			cTarget * pT = &(*pAT)[u];

			cFigWoW * pWoW = pRoot->find_target ( pT );

			if ( pWoW )
			{
				if ( pT->bIsRunning && ! pWoW->bRunning )
				{
					pWoW->bRunning = true;
					bChanged = true;
				}

				else if ( ! pT->bIsRunning && pWoW->bRunning )
				{
					pWoW->bRunning = false;
					bChanged = true;
				}

				continue;
			}

			else
			{
				cFigWoW * pNew = new cFigWoW;
				pNew->hwnd = (*pAT)[u].hwnd;
				pNew->sName = (*pAT)[u].sName;
				pNew->sPath = (*pAT)[u].sPath;
				pNew->bRunning = (*pAT)[u].bIsRunning;
				pNew->bLaunchByMojo = (*pAT)[u].bLaunchByMojo;
				pNew->dwProcessID = (*pAT)[u].dwProcessID;
				pNew->hMach = (*pAT)[u].hMach;
				pNew->dwTargetID = (*pAT)[u].dwID;

				if ( 1 == pNew->hMach )
					pNew->sComputerName = L"Local";

				else
				{
					mojo::cMach m;
					mojo::get_mach ( &m, pNew->hMach );
					pNew->sComputerName = m.sName;
					pNew->dwIP = m.dwIP;
					pNew->sDottedDec = m.sDottedDec;
				}

				pRoot->append_right ( pNew );

				bChanged = true;
			}
		}
	}

	//-------------------------------------------
	//  FOR EACH ITEM IN TREE, CHECK TO SEE
	//  IF IT'S NOT IN ARRAY AND DELETE OR
	//  CHANGE TREE AS NECESSARY
	//-------------------------------------------

	cTree * pTree = pRoot->pRight; 
	while ( pTree )
	{
		cTree * pNextTree = pTree->pRight;

		cFigWoW * pWoW = static_cast<cFigWoW*>(pTree);

		cTarget t;
		pWoW->set_target ( &t );
		cTarget * pTarget = pAT->find_target ( &t );

		if ( ! pTarget )
		{
			if ( false == pWoW->bLaunchByMojo )
			{
				pTree->remove();
				delete pTree;
				pTree = 0;
				bChanged = true;
			}

			else if ( pWoW->bRunning )
			{
				pWoW->bRunning = false;
				bChanged = true;
			}
		}

		pTree = pNextTree;
	}

	// WoWList.unlock();
}


//----------------------------------------------------------------------------------------------------------------------
//  CREATE WOW LIST
//  Caller should delete list after using it.
//----------------------------------------------------------------------------------------------------------------------
cFigViewItemList * cFigMgr :: create_wow_list ()
{
	cFigWoWTree * pTree = Root.get_wow_tree ();

	if ( ! pTree )
		return 0;

	cFigViewItemList * pList = new cFigViewItemList;

	for ( cTree * p = pTree->pRight; p; p = p->pRight )
	{
		pList->append ( (reinterpret_cast<cFigWoW*>(p))->clone() );
	}

	return pList;
}

//----------------------------------------------------------------------------------------------------------------------
//  DELETE FIG
//----------------------------------------------------------------------------------------------------------------------
bool cFigMgr :: delete_fig ( DWORD dwHandle )
{
	cFig * p = Root.get_by_handle ( dwHandle );

	if ( ! p )
		return false;

	else
	{
		p->remove();
		delete p;
	}

	return true;
}


//----------------------------------------------------------------------------------------------------------------------
//  SET FIG
//----------------------------------------------------------------------------------------------------------------------
bool cFigMgr :: set_fig ( DWORD dwHandle, const cFig * pArg )
{
	cFig * p = Root.get_by_handle ( dwHandle );

	if ( ! p )
		return false;

	else
		*p = *pArg;

	return true;
}


//----------------------------------------------------------------------------------------------------------------------
//  PATHNAME
//----------------------------------------------------------------------------------------------------------------------
const wchar_t * cFigMgr :: pathname ( cStrW * pRet )
{
	assert ( g_awAppTitle );
	pRet->erase();
	mojo::get_our_local_app_data_directory ( pRet, g_awAppTitle );
	*pRet += g_awAppTitle;
	*pRet += L".fig.xml";
	return pRet->cstr();
}


//----------------------------------------------------------------------------------------------------------------------
//  LOAD
//----------------------------------------------------------------------------------------------------------------------
void cFigMgr :: load_from_file ( cFig * pFig )
{
	cStrW sPathname;
	pathname ( &sPathname );

	cFileIn f ( sPathname.cstr() );

	if ( 0 ==  f.h )
		return;

	cStrW sTxt ( 10000 );

	f.get_whole_thing_without_line_breaks ( &sTxt );

	pFig->set_from_xml ( pFig, sTxt.cstr() );

	PostMessage ( g_hwnd, uWM_TOON_LIST_CHANGED, 0, 0 );
	PostMessage ( g_hwnd, uWM_WOW_LIST_CHANGED, 0, 0 );
}


//----------------------------------------------------------------------------------------------------------------------
//  SAVE
//----------------------------------------------------------------------------------------------------------------------
void cFigMgr :: save_to_file ()
{

	cStrW sPathname;
	pathname ( &sPathname );

	FILE * h;

	if ( 0 != _wfopen_s ( & h, sPathname.cstr(), L"wt, ccs=UTF-8" ) )
	{
		LOG_V ( L"Unable to open configuration file for writing: %s.", sPathname.cstr() );
		return;
	}

	cStrW s (10000);

	Root.write_to_xml ( &s, &Root, 0 );

	const wchar_t * pTxt = s.cstr();
	int iLen = s.len() * 2;
	pTxt;
	iLen;

	// fwprintf ( h, L"Test string" );
	fwrite ( pTxt, iLen, 1, h );
	fclose(h);

}


//----------------------------------------------------------------------------------------------------------------------
//  SET FROM XML
//----------------------------------------------------------------------------------------------------------------------
#if 0
void cFig :: set_from_xml ( const wchar_t * pTxt )
{
	const wchar_t * pNext = pTxt;

	cStrW sName;
	cStrW sAttribute;
	cStrW sContent;

	while ( xml_get_next_element ( &sName, &sAttribute, &sContent, &pNext ) )
	{
		cFig * p = name_to_item ( sName.cstr() );

		if ( ! p)
		{
			#ifdef _DEBUG
				assert(p);
			#endif

			continue;
		}

		p->set_from_xml ( sContent.cstr() );
	}

#if 0
		if ( sName == L"Toon" )
		{
			cToon * p = new cToon;
			p->init_from_xml ( sContent.cstr() );
			p->dwSerialNumber = ++this->dwLastSerialNumberAssigned;
			this->ToonList.append ( p );
		}

		else if ( sName == L"WoW" )
		{
			cWoW * p = new cWoW;
			p->init_from_xml ( sContent.cstr() );
			p->eOrigin = cWoW::launched_by_mojo;
			append ( &WoWList, p );
		}
#endif

	PostMessage ( g_hwnd, uWM_TOON_LIST_CHANGED, 0, 0 );
}
#endif

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