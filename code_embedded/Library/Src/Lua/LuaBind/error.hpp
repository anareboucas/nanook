/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/Lua/LuaBind/error.hpp $
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


// Copyright (c) 2003 Daniel Wallin and Arvid Norberg

// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
// ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
// SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
// ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
// OR OTHER DEALINGS IN THE SOFTWARE.

#ifndef LUABIND_ERROR_HPP_INCLUDED
#define LUABIND_ERROR_HPP_INCLUDED

#include "Lua/LuaBind/prefix.hpp"

#include <exception>

#include "Lua/LuaBind/config.hpp"

struct lua_State;

namespace luabind
{

#ifndef LUABIND_NO_EXCEPTIONS

	// this exception usually means that the lua function you called
	// from C++ failed with an error code. You will have to
	// read the error code from the top of the lua stack
	// the reason why this exception class doesn't contain
	// the message itself is that std::string's copy Constructor
	// may throw, if the copy Constructor of an exception that is
	// being thrown throws another exception, terminate will be called
	// and the entire application is killed.
	class error : public std::exception
	{
	public:
		error(lua_State* L): m_L(L) {}
		lua_State* state() const throw() { return m_L; }
		virtual const char* what() const throw()
		{
			return "lua runtime error";
		}
	private:
		lua_State* m_L;
	};

	// if an object_cast<>() fails, this is thrown
	// it is also thrown if the return value of
	// a lua function cannot be converted
	class cast_failed : public std::exception
	{
	public:
		cast_failed(lua_State* L, LUABIND_TYPE_INFO i): m_L(L), m_info(i) {}
		lua_State* state() const throw() { return m_L; }
		LUABIND_TYPE_INFO info() const throw() { return m_info; }
		virtual const char* what() const throw() { return "unable to make cast"; }
	private:
		lua_State* m_L;
		LUABIND_TYPE_INFO m_info;
	};

#else

	typedef void(*error_callback_fun)(lua_State*);
	typedef void(*cast_failed_callback_fun)(lua_State*, LUABIND_TYPE_INFO);

	void set_error_callback(error_callback_fun e);
	void set_cast_failed_callback(cast_failed_callback_fun c);
	error_callback_fun get_error_callback();
	cast_failed_callback_fun get_cast_failed_callback();

#endif

	typedef int(*pcall_callback_fun)(lua_State*);
	void set_pcall_callback(pcall_callback_fun e);
	pcall_callback_fun get_pcall_callback();

}

#endif // LUABIND_ERROR_HPP_INCLUDED

