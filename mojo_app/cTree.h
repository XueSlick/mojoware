/***********************************************************************************************************************
/*
/*    cTree.h / mojo_app
/*
/*    Each node in a tree is an object of this class.   To make a tree of cSomething's, derive cSomething from 
/*    this class.
/*
/*    Rights are used to represent binary lists.   Some particular node is a pointer to the head of the list.  Its
/*    right (if it exists) is the head (first item) of the list.  The right's right is the next item.  Etc.
/*   
/*    Copyright 2009 Robert Sacks.  See end of file for more info.
/*
/**********************************************************************************************************************/


#pragma once

//----------------------------------------------------------------------------------------------------------------------
//  CLASS
//----------------------------------------------------------------------------------------------------------------------
class cTree
{
public:

	cTree () : pParent(0), pRight(0), pLeft(0) {}
	virtual ~cTree ();

	void remove ();

	void insert_left    ( cTree * pNew );
	void append_left    ( cTree * pNew );

	void insert_right   ( cTree * pNew );
	void append_right   ( cTree * pNew );

	size_t count () const;

	virtual cTree * clone_node () const { return new cTree; }
	cTree * clone_tree () const;

	void print ( mojo::cStrW * pRet ) const;

	cTree * head() { return pRight; }   // used with nodes that are acting like linked lists
	cTree * next() { return pRight; }   // used with nodes that are acting like nodes in linked lists

	mojo::cStrW sName;

public:

	cTree * pParent, * pRight, * pLeft;
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