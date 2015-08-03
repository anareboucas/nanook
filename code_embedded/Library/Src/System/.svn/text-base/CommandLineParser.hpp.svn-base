/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/System/CommandLineParser.hpp $
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




#pragma once

namespace Bolitho
{
  namespace System
  {
    
    class LIBRARY_API CommandLineParser : public Object
    {
      DeclareRuntimeType();
    public:
      BOOL Parse( LPCTSTR CmdLineString );
      BOOL Validate();

      VOID DefineFlag( LPCTSTR Name, LPCTSTR ConfigurationName, LPCTSTR Description );
      VOID DefineKeyedValue( LPCTSTR Name, LPCTSTR ConfigurationName, LPCTSTR Description, BOOL Required, LPCTSTR DefaultValue = 0 );
      VOID DefineValue( LPCTSTR Name, LPCTSTR Description, BOOL Required );
      VOID DefineValueList( LPCTSTR Name, LPCTSTR Description, UINT MinimumRequired = 0, UINT MaximumItems = MAXUINT );

      String Usage( LPCTSTR ApplicationName );
      String ActualUsage();

			template<class T>
			T GetValue( LPCTSTR Name ) CONST;
      template<class T>
      Array<T> GetValues( LPCTSTR Name ) CONST;

      BOOL IsPresent( LPCTSTR Name ) CONST;
    private:

      class Flag
      {
      public:
        Flag() {}
        Flag( LPCTSTR Name, LPCTSTR Description, LPCTSTR ConfigurationName ) : m_Name(Name), m_Description(Description), m_ConfigurationName(ConfigurationName), m_Present(FALSE) {}
        Flag( CONST Flag& F ) : m_Name(F.m_Name), m_Description(F.m_Description), m_Present(F.m_Present) {}

        String m_Name;
        String m_ConfigurationName;
        String m_Description;
        BOOL m_Present;
      };

      class KeyedValue
      {
      public:
        KeyedValue() {}
        KeyedValue( LPCTSTR Name, LPCTSTR Description, LPCTSTR ConfigurationName, BOOL Required, LPCTSTR Default ) : m_Name(Name), m_Description(Description), m_ConfigurationName(ConfigurationName), m_Present(FALSE), m_Required(Required), m_Value(Default) {}
        KeyedValue( CONST KeyedValue& KV ) : m_Name(KV.m_Name), m_Description(KV.m_Description), m_Present(KV.m_Present), m_Required(KV.m_Required), m_Value(KV.m_Value) {}

        String m_Name;
        String m_ConfigurationName;
        String m_Description;
        String m_Value;
        BOOL m_Required;
        BOOL m_Present;
      };

      class ValueList
      {
      public:
        ValueList() {}
        ValueList( LPCTSTR Name, LPCTSTR Description, UINT RequiredItems, UINT MaximumItems ) : m_Name(Name), m_Description(Description), m_RequiredItems(RequiredItems), m_MaximumItems(MaximumItems) {}
        ValueList( CONST ValueList& V ) : m_Name(V.m_Name), m_Description(V.m_Description), m_Values(V.m_Values), m_RequiredItems(V.m_RequiredItems), m_MaximumItems(V.m_MaximumItems) {}
        
        Array<String> m_Values;
        String m_Name;
        String m_Description;
        UINT m_RequiredItems;
        UINT m_MaximumItems;        
      };

      Array<Flag> m_Flags;
      Array<KeyedValue> m_KeyedValues;
      Array<ValueList> m_Values;

    };

		#include "CommandLineParserImpl.hpp"

  }
}

