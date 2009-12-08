// xml.h


#pragma once


//-----------------------------------------------------------------
//   xml lingo:
// 
//   In the following example
//
//        <toon = "yeppers">jerry</toon>
//
//   the whole thing is an element.
//   toon is a name
//   yeppers is an attribute
//   jerry is content
//


bool xml_name_to_content ( mojo::cStrW * pRet, const wchar_t ** ppHaystack, const wchar_t * pName );
bool xml_get_next_element ( mojo::cStrW * pRetName, mojo::cStrW * pRetAttribute, mojo::cStrW * pRetContent, const wchar_t ** ppHaystack );


////////////////////////////////////////////////

#if 0

/*********************************************************************************
/*
/*   xml.h
/*
/*   started 12/23/2005
/*
/*********************************************************************************/

#pragma once

/************************************************************************************/
/*** PROTOTYPES *********************************************************************/
/************************************************************************************/

bool xml_key_to_val_sub    ( cStrW * pValue,   const wchar_t *    pcSearchMe,  const wchar_t * pcKey );
bool xml_key_to_val_iter   ( cStrW * pValue,   const wchar_t **   ppcSearchMe, const wchar_t * pcKey );
bool xml_key_to_val_nested ( cStrW * pValue,   const wchar_t *    pSearchMe,   const wchar_t * pcKey );
bool xml_key_to_val        ( cStrW * pValue,   const wchar_t *    pSearchMe,   const wchar_t * pcKey );


void xml_encode ( cStrW * arg );
} 

#endif

