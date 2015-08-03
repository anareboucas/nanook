/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/Lua/LuaBind/detail/overload_rep_base.hpp $
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

#ifndef LUABIND_OVERLOAD_REP_BASE_HPP_INCLUDED
#define LUABIND_OVERLOAD_REP_BASE_HPP_INCLUDED

#include "Lua/LuaBind/config.hpp"

#include <boost/function/function1.hpp>

struct lua_State;

namespace luabind { namespace detail
{
	// this class represents a specific overload of a member-function.
	struct overload_rep_base
	{
#if !defined(NDEBUG) && !defined(LUABIND_NO_ERROR_CHECKING)
		overload_rep_base(): m_get_signature_fun(0), m_match_fun(0), m_arity(-1) {}
#else
		overload_rep_base(): m_match_fun(0), m_arity(-1) {}
#endif

		typedef boost::function1<int, lua_State*> match_fun_t;
		typedef void(*get_sig_ptr)(lua_State*, std::string&);

		inline int match(lua_State* L, int num_params) const
		{
			if (num_params != m_arity) return -1;
			return m_match_fun(L);
		}

		inline void set_match_fun(match_fun_t const& fn) 
		{
			m_match_fun = fn;
		}

#ifndef LUABIND_NO_ERROR_CHECKING
		inline void get_signature(lua_State* L, std::string& s) const 
		{ 
			m_get_signature_fun(L, s); 
		}

		inline void set_sig_fun(get_sig_ptr f) 
		{ 
			m_get_signature_fun = f; 
		}
#endif

	protected:

#ifndef LUABIND_NO_ERROR_CHECKING
		get_sig_ptr m_get_signature_fun;
#endif

//		match_ptr m_match_fun;
		match_fun_t m_match_fun;
		int m_arity;
	};

}} // namespace luabind::detail

#endif // LUABIND_OVERLOAD_REP_BASE_HPP_INCLUDED
