/********************************************************************************************************
/*
/*	  utility_private.h / mojo_engine
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



//======================================================================================================================
//  PROTOTYPES
//======================================================================================================================

bool replace_format_specification_fields ( wchar_t * pRet, int iSize,  const wchar_t * pIn, va_list pVA );
void log_and_put_memo ( const wchar_t * pFile, int iLine, mojo::cMemo::_eSeverity, const wchar_t * pTxt, ... );