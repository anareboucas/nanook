/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/Data/Xml/Node.cpp $
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
#include "Data/Xml/Node.hpp"
#include "Data/Xml/Document.hpp"
#include "Com/ComException.hpp"
#include "Com/Variant.hpp"
#include "Com/BString.hpp"

using namespace Bolitho;
using namespace Bolitho::Xml;
using namespace Bolitho::Com;
using namespace MSXML2;

//-----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::Xml,Node,Object);
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Node::Node(IXMLDOMNode* pNode)
{
	m_pNode = pNode;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Node::~Node()
{
	if (m_pNode)
		m_pNode->Release();
	m_pNode = 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<Document> Node::GetDocument()
{
	IXMLDOMDocument* pDocument = 0;
	VerifyComResult(m_pNode->get_ownerDocument(&pDocument));
	return new Document(pDocument);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<Node> Node::GetParent()
{
	IXMLDOMNode* pNode = 0;
	HRESULT Result = m_pNode->get_parentNode(&pNode);
	if (FAILED(Result))
		throw ComException(this, Result);
	if (Result == S_FALSE)
		return Ptr<Node>::Null;
	else
		return new Node(pNode);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
String Node::GetName()
{
	BString Name;
	VerifyComResult(m_pNode->get_nodeName(Name));
	return Name.ToString();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
String Node::GetValue()
{
	Variant Value;
	VerifyComResult(m_pNode->get_nodeValue(&Value));
	return Value.ToString();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
String Node::GetNamespacePrefix()
{
	BString NamespacePrefix;
	VerifyComResult(m_pNode->get_prefix(NamespacePrefix));
	return NamespacePrefix.ToString();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Node::SetValue(LPCTSTR V)
{
	Variant Value(V);
	VerifyComResult(m_pNode->put_nodeValue(Value));
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
DWORD Node::GetType()
{
	DOMNodeType Type;
	VerifyComResult(m_pNode->get_nodeType(&Type));
	return (DWORD)Type;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
String Node::GetText()
{
	BString Text;
	VerifyComResult(m_pNode->get_text(Text));
	return Text.ToString();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Node::SetText(LPCTSTR Text)
{
	VerifyComResult(m_pNode->put_text(BString(Text)));
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
String Node::GetXML()
{
	BString XML;
	VerifyComResult(m_pNode->get_xml(XML));
	return XML.ToString();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<Node> Node::AppendChild( Ptr<Node> pNode )
{
	IXMLDOMNode* pReturnNode = 0;
	VerifyComResult(m_pNode->appendChild(pNode->m_pNode, &pReturnNode));
	return new Node(pReturnNode);
}

//#endif
