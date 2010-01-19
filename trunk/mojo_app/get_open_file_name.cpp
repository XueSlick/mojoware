/***********************************************************************************************************************
/*
/*    get_open_file_name.cpp / mojo_app
/*   
/*    Copyright 2010 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/

#include "stdafx.h"

#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x600
#endif

#include <shobjidl.h>

//======================================================================================================================
//  PROTOTYPES
//======================================================================================================================

bool get_open_file_name_vista ( cStrW * pRet, HWND hwnd, const wchar_t * pDefaultFilename );
bool get_open_file_name_pre_vista ( cStrW * pRet, HWND hwnd, const wchar_t * pDefaultFilename );
bool get_open_file_name_pre_vista ( cStrW * pRet, HWND hwnd, wchar_t * pwDefaultPath, const wchar_t * pwDefaultFilename, wchar_t * pwDlgTitle );

//======================================================================================================================
//  CODE
//======================================================================================================================

//----------------------------------------------------------------------------------------------------------------------
//  GET OPEN FILE NAME
//----------------------------------------------------------------------------------------------------------------------
bool get_open_file_name ( cStrW * pRet, HWND hwnd, const wchar_t * pDefaultFilename )
{
	return get_open_file_name_pre_vista ( pRet, hwnd, pDefaultFilename );
}


//----------------------------------------------------------------------------------------------------------------------
//  GET OPEN FILE NAME PRE VISTA
//----------------------------------------------------------------------------------------------------------------------
bool get_open_file_name_pre_vista ( cStrW * pRet, HWND hwnd, const wchar_t * pDefaultFilename )
{
	return get_open_file_name_pre_vista ( pRet, hwnd, 0, pDefaultFilename, g_awAppTitle );
}

//----------------------------------------------------------------------------------------------------------------------
//  GET OPEN FILE NAME PRE VISTA
//----------------------------------------------------------------------------------------------------------------------
bool get_open_file_name_pre_vista ( cStrW * pRet, HWND hwnd, wchar_t * pwDefaultPath, const wchar_t * pwDefaultFilename, wchar_t * pwDlgTitle )
{
	bool retval = false;

	wchar_t awFileTitle [MAX_PATH+8092] = {0}; // on return without path
	wchar_t awFileName  [MAX_PATH+8092] = {0}; // on return includes path

	if ( pwDefaultFilename )
		wcscpy_s ( awFileName, sizeof(awFileName)/sizeof(wchar_t), pwDefaultFilename );

	OPENFILENAMEW o;
	::memset ( &o, 0, sizeof(o) );
	o.lStructSize = sizeof (o);
	o.hwndOwner = hwnd;
	o.lpstrFile = &awFileName[0]; // on return includes path
	o.nMaxFile = sizeof(awFileName)/sizeof(wchar_t);
	o.lpstrFileTitle = awFileTitle; // without path
	o.nMaxFileTitle = sizeof(awFileTitle)/sizeof(wchar_t);
	o.lpstrTitle = pwDlgTitle;
	o.lpstrDefExt = L"txt";
	o.lpstrInitialDir = pwDefaultPath;

	if ( 0 == GetOpenFileNameW ( &o) ) // FAILED
	{
		DWORD dwError = GetLastError();
		dwError;
		pRet->erase();
		retval = false;
	}

	else
	{
		*pRet = awFileName;
		retval = true;
	}

	return retval;
}


//----------------------------------------------------------------------------------------------------------------------
//  GET OPEN FILE NAME VISTA
//----------------------------------------------------------------------------------------------------------------------
bool get_open_file_name_vista ( cStrW * pRet, HWND hwnd )
{

    IFileDialog *pfd;
    
    // CoCreate the dialog object.
    HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, 
                                  NULL, 
                                  CLSCTX_INPROC_SERVER, 
                                  IID_PPV_ARGS(&pfd));
    
	DWORD dwOptions;
	hr = pfd->GetOptions(&dwOptions);

	if (SUCCEEDED(hr))
	{
    	hr = pfd->SetOptions(dwOptions | FOS_FORCEFILESYSTEM);
	}

	if (SUCCEEDED(hr))
	{
   	 IShellItem *psiResult;
    
    	hr = pfd->Show(hwnd);
    	if (SUCCEEDED(hr))
    	{
        	hr = pfd->GetResult(&psiResult);
        	if (SUCCEEDED(hr))
        	{
            	WCHAR *pszPath;
            
           		hr = psiResult->GetDisplayName(SIGDN_FILESYSPATH, &pszPath);
            	if (SUCCEEDED(hr))
				{
					*pRet = pszPath;
                	CoTaskMemFree(pszPath);
            	}
            	psiResult->Release();
       	 	}
		}
	}

	return SUCCEEDED(hr) ? true : false;
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