/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/Data/Xml/ParseError.cpp $
 * Author:       $Author: OBTUSE\matthew $
 * Revision:     $Rev: 1168 $
 * Last Updated: $Date: 2007-09-29 11:47:34 -0400 (Sat, 29 Sep 2007) $
 * 
 * 
 * Copyright (c) 2004-2007, Matthew G Bolitho
 * All rights reserved.
 *
 *
 * Microsoft Reference License (Ms-RL)
 *   
 * This license governs use of the accompanying software. If you use the software, you accept 
 * this license. If you do not accept the license, do not use the software.
 *
 * 1. Definitions
 *  - The terms "reproduce," "reproduction" and "distribution" have the same meaning here as under 
 *    U.S. copyright law.
 *  - "You" means the licensee of the software.
 *  - "Your company" means the company you worked for when you downloaded the software.
 *  - "Reference use" means use of the software within your company as a reference, in read only 
 *    form, for the sole purposes of debugging your products, maintaining your products, or 
 *    enhancing the interoperability of your products with the software, and specifically excludes 
 *    the right to distribute the software outside of your company.
 *  - "Licensed patents" means any Licensor patent claims which read directly on the software as 
 *    distributed by the Licensor under this license.
 * 
 * 2. Grant of Rights
 *  (A) Copyright Grant- Subject to the terms of this license, the Licensor grants you a non-transferable, 
 *      non-exclusive, worldwide, royalty-free copyright license to reproduce the software for reference use.
 *  (B) Patent Grant- Subject to the terms of this license, the Licensor grants you a non-transferable,
 *      non-exclusive, worldwide, royalty-free patent license under licensed patents for reference use.
 * 
 * 3. Limitations
 *  (A) No Trademark License - This license does not grant you any rights to use the Licensor's name
 *      logo, or trademarks.
 *  (B) If you begin patent litigation against the Licensor over patents that you think may apply 
 *      to the software (including a cross-claim or counterclaim in a lawsuit), your license to the 
 *      software ends automatically.
 *  (C) The software is licensed "as-is." You bear the risk of using it. The Licensor gives no express 
 *      warranties, guarantees or conditions. You may have additional consumer rights under your local 
 *      laws which this license cannot change. To the extent permitted under your local laws, the 
 *      Licensor excludes the implied warranties of merchantability, fitness for a particular purpose 
 *      and non-infringement.
 *
 ***************************************************************************************************/




#include "CodeLibrary.hpp"

//#if defined(USE_XML)
#include "Data/Xml/Document.hpp"
#include "Com/ComException.hpp"
#include "Com/Variant.hpp"
#include "Com/BString.hpp"

using namespace Bolitho;
using namespace Bolitho::Xml;
using namespace Bolitho::Com;
using namespace MSXML2;

//-----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::Xml,ParseError,Object);
//-----------------------------------------------------------------------------------------------------------------------------------------------------
ParseError::ParseError(IXMLDOMParseError* pParseError)
{
	m_pParseError = pParseError;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
ParseError::~ParseError()
{
	if (m_pParseError)
		m_pParseError->Release();
	m_pParseError = 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
LONG ParseError::GetErrorCode()
{
	LONG ErrorCode = 0;
	VerifyComResult(m_pParseError->get_errorCode(&ErrorCode));
	return ErrorCode;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
LONG ParseError::GetFilePosition()
{
	LONG FilePosition = 0;
	VerifyComResult(m_pParseError->get_filepos(&FilePosition));
	return FilePosition;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
LONG ParseError::GetLine()
{
	LONG Line = 0;
	VerifyComResult(m_pParseError->get_line(&Line));
	return Line;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
LONG ParseError::GetLinePosition()
{
	LONG LinePosition = 0;
	VerifyComResult(m_pParseError->get_linepos(&LinePosition));
	return LinePosition;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
String ParseError::GetSourceText()
{
	BString SourceText;
	m_pParseError->get_srcText(SourceText);
	return SourceText.ToString();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
String ParseError::GetReason()
{
	BString Reason;
	m_pParseError->get_reason(Reason);
	return Reason.ToString();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
String ParseError::GetURL()
{
	BString URL;
	m_pParseError->get_url(URL);
	return URL.ToString();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
//#endif
