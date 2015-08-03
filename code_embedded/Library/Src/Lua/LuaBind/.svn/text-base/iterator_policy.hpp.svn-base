/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/Lua/LuaBind/iterator_policy.hpp $
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


#ifndef LUABIND_ITERATOR_POLICY_HPP_INCLUDED
#define LUABIND_ITERATOR_POLICY_HPP_INCLUDED

#include <luabind/config.hpp>
#include <luabind/detail/policy.hpp>
#include <luabind/detail/implicit_cast.hpp>
#include <luabind/detail/convert_to_lua.hpp>
#include <boost/mpl/bool.hpp>

namespace luabind { namespace detail 
{
	template<class Iter>
	struct iterator_state
	{
		typedef iterator_state<Iter> self_t;

		static int step(lua_State* L)
		{
			self_t& state = *static_cast<self_t*>(lua_touserdata(L, lua_upvalueindex(1)));

			if (state.start == state.end)
			{
				lua_pushnil(L);
			}
			else
			{
				convert_to_lua(L, *state.start);
				++state.start;
			}

			return 1;
		}

		iterator_state(const Iter& s, const Iter& e)
			: start(s)
			, end(e)
		{}

		Iter start;
		Iter end;
	};

	struct iterator_converter
	{
		typedef boost::mpl::bool_<false> is_value_converter;
		typedef iterator_converter type;

		template<class T>
		void apply(lua_State* L, const T& c)
		{
			typedef typename T::const_iterator iter_t;
			typedef iterator_state<iter_t> state_t;

			// note that this should be destructed, for now.. just hope that iterator
			// is a pod
			void* iter = lua_newuserdata(L, sizeof(state_t));
			new (iter) state_t(c.begin(), c.end());
			lua_pushcclosure(L, state_t::step, 1);
		}

		template<class T>
		void apply(lua_State* L, T& c)
		{
			typedef typename T::iterator iter_t;
			typedef iterator_state<iter_t> state_t;

			// note that this should be destructed, for now.. just hope that iterator
			// is a pod
			void* iter = lua_newuserdata(L, sizeof(state_t));
			new (iter) state_t(c.begin(), c.end());
			lua_pushcclosure(L, state_t::step, 1);
		}
	};

	struct iterator_policy : conversion_policy<0>
	{
		static void precall(lua_State*, const index_map&) {}
		static void postcall(lua_State*, const index_map&) {}

		template<class T, class Direction>
		struct apply
		{
			typedef iterator_converter type;
		};
	};

}}

namespace luabind
{
	namespace
	{
		LUABIND_ANONYMOUS_FIX detail::policy_cons<detail::iterator_policy, detail::null_type> return_stl_iterator;
	}
}

#endif // LUABIND_ITERATOR_POLICY_HPP_INCLUDED

