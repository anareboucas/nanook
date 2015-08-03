/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/System/Application.cpp $
 * Author:       $Author: OBTUSE\matthew $
 * Revision:     $Rev: 1406 $
 * Last Updated: $Date: 2008-01-26 02:07:19 -0500 (Sat, 26 Jan 2008) $
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

#include "System/Application.hpp"
#include "IO/StdIO.hpp"
#include "IO/Path.hpp"
#include "Text/StringBuilder.hpp"
#include "Debug/Trace.hpp"
#include "System/Configuration.hpp"
#include "IO/DebugStream.hpp"

#include <stdio.h>

using namespace Bolitho;
using namespace Bolitho::IO;
using namespace Bolitho::System;

//-----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::System,Application,Object);
//-----------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL Application::ParseConfiguration()
{
  if (File::Exists(m_MachineConfigFile))
    Configuration::LoadConfigFile(m_MachineConfigFile);
  if (File::Exists(m_UserConfigFile))
    Configuration::LoadConfigFile(m_UserConfigFile);
  Configuration::LoadCommandLine(GetCommandLine());
  
  return TRUE;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
#if defined(PLATFORM_WINDOWS)
Application* Application::m_pApplication = 0;
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Application::Application() : Thread(0), Process(0)
{
  DuplicateHandle( ::GetCurrentProcess(), ::GetCurrentThread(), ::GetCurrentProcess(), &m_hThread, GENERIC_READ | GENERIC_WRITE, FALSE, 0 );
  DuplicateHandle( ::GetCurrentProcess(), ::GetCurrentProcess(), ::GetCurrentProcess(), &m_hProcess, GENERIC_READ | GENERIC_WRITE, FALSE, 0 );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Application::InitApplication()
{
  #if defined(USE_COM)
  CoInitialize(NULL);
  #endif

  SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER) &UnhandledSEHException);

  #if defined(DEBUG)
  Trace.AddSink(TextStream(new DebugStream()));
  #endif

  #if defined(LIBRARYAPI_STATIC)
  g_pLibrary = new BaseLibrary();
  g_pLibrary->OnInit();
  #endif

  DWORD ModuleCount = 0;
  EnumProcessModules(::GetCurrentProcess(), 0, 0, &ModuleCount);
  HMODULE* pModules = new HMODULE[ModuleCount];
  EnumProcessModules(::GetCurrentProcess(), pModules, ModuleCount, &ModuleCount);
  
  for (SIZE_T i=0; i<ModuleCount; i++)
  {
    if (!m_HandleToObjectMap.Contains(pModules[i]))
    {
      Module* pModule = new Module();
      pModule->InitModule(pModules[i]);
      pModule->GetModuleInformation();
    }
  }

  for (Map<HMODULE,Module*>::Iterator i = m_HandleToObjectMap.Begin(); i != m_HandleToObjectMap.End(); i++)
  {
    Pair<HMODULE,Module*> P = *i;
    if (IsDerivedFrom(P.Second, &Library::RuntimeType))
    {
      Library* pLib = (Library*)P.Second;
      if (!pLib->InitLibrary(this))
        throw InvalidOperationException(this, TEXT("A Library failed to Initialize") );
    }
    P.Second->GetModuleInformation();
  }

  if (m_ProductCompany.Length() > 0)
  {
    m_UserSettingsKey = System::RegistryKey::CurrentUser.TryCreateSubkey( String::Format(TEXT("SOFTWARE\\%s\\%s\\"), m_ProductCompany.ConstStr(), m_ProductName.ConstStr() ), KEY_ALL_ACCESS );
    m_MachineSettingsKey = System::RegistryKey::LocalMachine.TryCreateSubkey( String::Format(TEXT("SOFTWARE\\%s\\%s\\"), m_ProductCompany.ConstStr(), m_ProductName.ConstStr() ), KEY_ALL_ACCESS );
  }
  
  m_UserConfigFile = Path::Basename(m_ImageName) + ".Config";
  m_MachineConfigFile = Path::Basename(m_ImageName) + ".Config";
  
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<Library> Application::LoadLibrary( LPCTSTR FileName ) CONST
{
  HINSTANCE hInstance = ::LoadLibrary( FileName );
  
  if (hInstance == 0)
    SystemException::ThrowError(this);
  
  Module* pModule = 0;
  if (Module::m_HandleToObjectMap.Get(hInstance, pModule))
    return pModule;

  pModule = new Library(hInstance);
  pModule->GetModuleInformation();

  return pModule;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL Application::OnInit()
{
  return TRUE;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Application::OnExit()
{
  #if defined(USE_COM)
  CoUninitialize();
  #endif

  return;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Application::OnException(Exception& E)
{
  Trace0Enter("");

  StringBuilder Message;

  Message.AppendLine(TEXT("UNHANDLED EXCEPTION"));
  Message.AppendFormatLine(TEXT("Exception Type: %s::%s"), E.GetType()->Namespace(), E.GetType()->Name() );
  Message.AppendFormatLine(TEXT("Description: %s"), E.Description().ConstStr() );

  Message.AppendLine(TEXT("\nStack Trace:"));
  
  for( SIZE_T i=0; i<E.StackTrace().Length(); i++ )
  {
#if defined(UNICODE)
    if (E.StackTrace()[i].Symbol().SourceFile() != 0)
      Message.AppendFormatLine(TEXT("0x%p %S() at %S:%d"), E.StackTrace()[i].InstructionPtr(), E.StackTrace()[i].Symbol().Name(), E.StackTrace()[i].Symbol().SourceFile(), E.StackTrace()[i].Symbol().SourceLine() );
    else
      Message.AppendFormatLine(TEXT("0x%p %S()"), E.StackTrace()[i].InstructionPtr(), E.StackTrace()[i].Symbol().Name() );
#else
    if (E.StackTrace()[i].Symbol().SourceFile() != 0)
      Message.AppendFormatLine(TEXT("0x%p %s()\r\n  at %s:%d"), E.StackTrace()[i].InstructionPtr(), E.StackTrace()[i].Symbol().Name(), Path::Filename(E.StackTrace()[i].Symbol().SourceFile()).ConstStr(), E.StackTrace()[i].Symbol().SourceLine() );
    else
      Message.AppendFormatLine(TEXT("0x%p %s()"), E.StackTrace()[i].InstructionPtr(), E.StackTrace()[i].Symbol().Name() );
#endif
  }

	Trace(TraceLevel::ERROR, this, Message.ToString() );

	Debug::GenerateCoreDump();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
LONG Application::UnhandledSEHException( PEXCEPTION_POINTERS Ptrs )
{
  Debug::StackFrame Context[32];
  SIZE_T ContextLength = Debug::GenerateStackTrace( &Context[0], 32, Ptrs->ContextRecord);
  
  switch (Ptrs->ExceptionRecord->ExceptionCode)
  {
  case EXCEPTION_ACCESS_VIOLATION:
    throw AccessViolationException( &Context[0], ContextLength, (PVOID)Ptrs->ExceptionRecord->ExceptionInformation[1], (UINT)Ptrs->ExceptionRecord->ExceptionInformation[0] );
  case EXCEPTION_STACK_OVERFLOW:
    throw StackOverflowException( &Context[0], ContextLength );
  case EXCEPTION_INT_DIVIDE_BY_ZERO:
  case EXCEPTION_INT_OVERFLOW:
    throw IntegerMathException( &Context[0], ContextLength, Ptrs->ExceptionRecord->ExceptionCode );
  case EXCEPTION_FLT_DENORMAL_OPERAND:
  case EXCEPTION_FLT_DIVIDE_BY_ZERO:
  case EXCEPTION_FLT_INEXACT_RESULT:
  case EXCEPTION_FLT_INVALID_OPERATION:
  case EXCEPTION_FLT_OVERFLOW:
  case EXCEPTION_FLT_STACK_CHECK:
  case EXCEPTION_FLT_UNDERFLOW:
    throw FloatingPointMathException( &Context[0], ContextLength, Ptrs->ExceptionRecord->ExceptionCode );
  default:
    StdErr.WriteLine(TEXT("*** UNHANDLED SEH EXCEPTION ***"));
    StdErr.WriteLine(TEXT("ExceptionCode: %p"), Ptrs->ExceptionRecord->ExceptionCode);
		return EXCEPTION_CONTINUE_SEARCH;
  };

  return EXCEPTION_EXECUTE_HANDLER;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
String Application::GetCommandLine()
{
  return ::GetCommandLine();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
Array<String> Application::GetCommandLineTokens()
{
  String CmdLineString = ::GetCommandLine();
  Array<String> Tokens;
  String CurrentToken;

  SIZE_T i = 0;
  enum { WHITESPACE, TEXT, QUOTEDTEXT } State = WHITESPACE;

  while (CmdLineString(i) != 0)
  {
    switch(State)
    {
    case WHITESPACE:
      if (CmdLineString(i) == '\"')
      {
        State = QUOTEDTEXT;
        break;
      }
      else if (!String::IsWhitespace(CmdLineString(i)))
      {
        State = TEXT;
        CurrentToken += CmdLineString(i);
        break;
      }
      break;
    case QUOTEDTEXT:
      if (CmdLineString(i) == '\\' && CmdLineString(i+1) == '\"')
      {
        CurrentToken += '\"';
        i++;
        break;
      }
      else if (CmdLineString(i) == '\"')
      {
        if (String::IsWhitespace(CmdLineString(i+1)))
        {
          State = WHITESPACE;
          Tokens.Add(CurrentToken);
          CurrentToken = String::Empty;
          break;
        }
        else
          State = TEXT;
      }
      CurrentToken += CmdLineString(i);
      break;
    case TEXT:
      if (String::IsWhitespace(CmdLineString(i)))
      {
        State = WHITESPACE;
        Tokens.Add(CurrentToken);
        CurrentToken = String::Empty;
        break;
      }
      else if (CmdLineString(i) == '\"')
      {
        State = QUOTEDTEXT;
        break;
      }
      CurrentToken += CmdLineString(i);
      break;
      
    };

    i++;
  };

  if (State == TEXT || State == QUOTEDTEXT)
    Tokens.Add(CurrentToken);

  return Tokens;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL Application::ParseCommandLine()
{
  String CommandLine = GetCommandLine();
  if (!m_CmdLine.Parse(CommandLine))
  {
    StdOut << m_CmdLine.Usage( IO::Path::Filename(m_ImageName) );
    return FALSE;
  }
  return TRUE;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
String Application::GetEnvironment(LPCTSTR Name)
{
  DWORD Length = GetEnvironmentVariable( Name, 0, 0 );
  String S(Length);
  GetEnvironmentVariable( Name, S.Str(), Length );
  return S;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Application::SetEnvironment(LPCTSTR Name, LPCTSTR Value)
{
  SetEnvironmentVariable( Name, Value );
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Application::Exit( UINT ExitCode )
{
  ExitProcess(ExitCode);
}
#endif
//----------------------------------------------------------------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------------------------------------------------------------
#if defined(PLATFORM_LINUX)
Application::Application() : Thread(0), Process(0)
{
  m_hProcess = getpid();
  m_hThread = pthread_self();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Application::InitApplication()
{
  StdIn = StandardStream::GetStandardIn();
  StdOut = StandardStream::GetStandardOut();
  StdErr = StandardStream::GetStandardError();

  signal( SIGSEGV, &Application::UnhandledSignal );
  signal( SIGKILL, &Application::UnhandledSignal );
  signal( SIGFPE, &Application::UnhandledSignal );
  signal( SIGABRT, &Application::UnhandledSignal );
  signal( SIGILL, &Application::UnhandledSignal );
  signal( SIGQUIT, &Application::UnhandledSignal );
  signal( SIGSEGV, &Application::UnhandledSignal );
  
  CHAR Str0[PATH_MAX];
  CHAR Str1[PATH_MAX];
  
  ZeroMemory( Str0, PATH_MAX );
  ZeroMemory( Str1, PATH_MAX );
  
  sprintf( Str0, "/proc/%i/exe", getpid() );
  readlink( Str0, Str1, PATH_MAX );
  
  m_ImageName = Str1;
  
#if defined(ARCH_AMD64) && defined(DEBUG)
  m_ImageConfiguration = "amd64 Debug";
#elif defined(ARCH_AMD64) && defined(RELEASE)
  m_ImageConfiguration = "amd64 Release";
#elif defined(ARCH_I386) && defined(DEBUG)
  m_ImageConfiguration = "amd64 Debug";
#elif defined(ARCH_I386) && defined(RELEASE)
  m_ImageConfiguration = "i386 Release";
#else	
  m_ImageConfiguration = "Unknown";
#endif
  
  m_ProductName = IO::Path::Filename(m_ImageName); 

  m_UserConfigFile = Path::Basename(m_ImageName) + ".Config";
  m_MachineConfigFile = Path::Basename(m_ImageName) + ".Config";
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<Library> Application::LoadLibrary( LPCTSTR FileName ) CONST
{
  return Ptr<Library>::Null;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL Application::OnInit()
{
  
  String CommandLine = GetCommandLine();
  if (!m_CmdLine.Parse(CommandLine))
    {
      StdOut << m_CmdLine.Usage( IO::Path::Filename(m_ImageName) );
      return FALSE;
    }
  
  return TRUE;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Application::OnExit()
{
  return;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Application::OnException(Exception& E)
{
  StdErr.WriteLine(TEXT("\e[41;1;37m") );
  StdErr.WriteLine(TEXT("*** UNHANDLED EXCEPTION ***"));
  StdErr.WriteLine(TEXT("Exception Type: %s::%s"), E.GetType()->Namespace(), E.GetType()->Name() );
  StdErr.WriteLine(TEXT("Description: %s"), E.Description().ConstStr() );

  StdErr.WriteLine(TEXT("\nStack Trace:"));
  
  for( SIZE_T i=0; i<E.StackTrace().Length(); i++ )
  {
    if (E.StackTrace()[i].Symbol().SourceFile() != 0)
      StdErr.WriteLine(TEXT("%p %s()\r\n  at %s:%d"), E.StackTrace()[i].InstructionPtr(), E.StackTrace()[i].Symbol().Name(), Path::Filename(E.StackTrace()[i].Symbol().SourceFile()).ConstStr(), E.StackTrace()[i].Symbol().SourceLine() );
    else if (E.StackTrace()[i].Symbol().Name() != 0)
      StdErr.WriteLine(TEXT("%p %s()"), E.StackTrace()[i].InstructionPtr(), E.StackTrace()[i].Symbol().Name() );
		else
      StdErr.WriteLine(TEXT("%p"), E.StackTrace()[i].InstructionPtr() );
  }
  
  StdErr.WriteLine(TEXT("\e[m"));
  
  Debug::GenerateCoreDump();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
extern CHAR* g_CommandLine;
String Application::GetCommandLine()
{
  return g_CommandLine;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL Application::ParseCommandLine()
{
  String CommandLine = GetCommandLine();
  if (!m_CmdLine.Parse(CommandLine))
  {
    StdOut << m_CmdLine.Usage( IO::Path::Filename(m_ImageName) );
    return FALSE;
  }
	return TRUE;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
String Application::GetEnvironment( LPCTSTR Name )
{
  return "";
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Application::SetEnvironment( LPCTSTR Name, LPCTSTR Value )
{
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Application::Exit( UINT ExitCode )
{
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
VOID Application::UnhandledSignal( INT Signal )
{
	switch (Signal)
	{
		case SIGSEGV:
			throw SegmentationFaultException();
	}
}
#endif
//----------------------------------------------------------------------------------------------------------------------------------------------------
