/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/Lua/VirtualMachine.cpp $
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

#if defined(USE_LUA)

#include "Lua/VirtualMachine.hpp"
#include "Debug/Trace.hpp"

#include "Lua/LuaLib/lstate.h"

using namespace Bolitho;
using namespace Bolitho::IO;
using namespace Bolitho::Lua;

//---------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::Lua,VirtualMachine,Object)
//---------------------------------------------------------------------------------------------------
VirtualMachine::VirtualMachine()
{
  m_pState = luaL_newstate();
  luaL_openlibs(m_pState);
  m_pState->pVM = this;

  Bind::Open(m_pState);
}
//---------------------------------------------------------------------------------------------------
VirtualMachine::~VirtualMachine()
{
  if (m_pState)
    lua_close( m_pState );
  m_pState = 0;
}
//---------------------------------------------------------------------------------------------------
VOID VirtualMachine::Execute( LPCSTR S )
{
  luaL_loadstring( m_pState, S );
  if (lua_pcall( m_pState, 0, LUA_MULTRET, 0))
  {
    Trace( TraceLevel::ERROR, String::Format("Lua Error: %s", luaL_checkstring( m_pState, -1 )) );
    lua_pop( m_pState, 1 );
  }

}
//---------------------------------------------------------------------------------------------------
VOID VirtualMachine::ExecuteFile( LPCSTR Filename )
{
  luaL_loadfile( m_pState, Filename );
  if (lua_pcall( m_pState, 0, LUA_MULTRET, 0))
  {
    Trace( TraceLevel::ERROR, String::Format("Lua Error: %s", luaL_checkstring( m_pState, -1 )) );
    lua_pop( m_pState, 1 );
  }
}
//---------------------------------------------------------------------------------------------------
VOID VirtualMachine::Bind( Bind::Scope& S )
{
  Bind::Module( m_pState )[S];
}
//---------------------------------------------------------------------------------------------------
VOID VirtualMachine::OnPanic()
{
  Trace( TraceLevel::ERROR, String::Format("Lua Panic: %s", luaL_checkstring( m_pState, -1 )) );
  lua_pop( m_pState, 1 );
}
//---------------------------------------------------------------------------------------------------
VOID VirtualMachine::RunInteractive()
{
  while(TRUE)
  {  
    String Line;
    INT CompileStatus;
    BOOL FirstLine = TRUE;
    BOOL Return = FALSE;

    do
    {
      if (FirstLine)
        Out.Write("> ");
      else
        Out.Write(">> ");
      
      FirstLine = FALSE;
      Line += In.ReadLine();
    
      if (!Line.IsEmpty())
      {
        if (Line.Compare("exit", FALSE) == 0)
          return; 
        if (Line.BeginsWith("="))
        {
          Line = String("return ") + Line.Substr(1);
          Return = TRUE;
        }
      }

      CompileStatus = luaL_loadbuffer(m_pState, Line, Line.Length(), "StdIn");

      if (CompileStatus == LUA_ERRSYNTAX)
      {
        String Error = luaL_checkstring( m_pState, -1 );
      
        if (!Error.EndsWith("'<eof>'", FALSE))
        {
          Trace( TraceLevel::ERROR, String::Format("Syntax Error: %s", Error.ConstStr() ) );
          lua_pop( m_pState, 1 );
          break;
        }
      }
    }
    while(CompileStatus != 0);

    if (CompileStatus == 0)
    {

      if (lua_pcall( m_pState, 0, Return ? 1 : 0, 0 ))
      {
        Trace( TraceLevel::ERROR, String::Format("Lua Runtime Error: %s", luaL_checkstring( m_pState, -1 )) );
        lua_pop( m_pState, 1 );
      }

      if (lua_gettop(m_pState) > 0)
      {
        lua_getglobal(m_pState, "print");
        lua_insert(m_pState, 1);
        lua_pcall(m_pState, lua_gettop(m_pState)-1, 0, 0);
      }
      lua_settop(m_pState, 0);

    }

  }

}
//---------------------------------------------------------------------------------------------------


#endif
