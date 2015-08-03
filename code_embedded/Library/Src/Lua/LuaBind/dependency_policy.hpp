/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/Lua/LuaBind/dependency_policy.hpp $
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


#ifndef LUABIND_DEPENDENCY_POLICY_HPP_INCLUDED
#define LUABIND_DEPENDENCY_POLICY_HPP_INCLUDED

#include "Lua/LuaBind/config.hpp"
#include "Lua/LuaBind/detail/policy.hpp"

namespace luabind { namespace detail 
{
	// makes A dependent on B, meaning B will outlive A.
	// internally A stores a reference to B
	template<int A, int B>
	struct dependency_policy
	{
		static void postcall(lua_State* L, const index_map& indices)
		{
			int nurse_index = indices[A];
			int patient = indices[B];

			object_rep* nurse = static_cast<object_rep*>(lua_touserdata(L, nurse_index));
			assert((nurse != 0) && "internal error, please report"); // internal error

			nurse->add_dependency(L, patient);
		}
	};

}}

#if defined (BOOST_MSVC) && (BOOST_MSVC <= 1200)

namespace luabind
{
	// most absurd workaround of all time?
	namespace detail
	{
		template<int N>
		struct size_char_array
		{
			char storage[N + 2];
		};

		template<int N>
		size_char_array<N> deduce_size(LUABIND_PLACEHOLDER_ARG(N));

		template<class T>
		struct get_index_workaround
		{
			static T t;
			BOOST_STATIC_CONSTANT(int, value = sizeof(deduce_size(t)) - 2);
		};
	}

	template<class A, class B>
	detail::policy_cons<detail::dependency_policy<detail::get_index_workaround<A>::value
		, detail::get_index_workaround<B>::value>, detail::null_type> dependency(A,B)
	{
		return detail::policy_cons<detail::dependency_policy<
			detail::get_index_workaround<A>::value, detail::get_index_workaround<B>::value>
			, detail::null_type>();
	}

	template<class A>
	detail::policy_cons<detail::dependency_policy<0
		, detail::get_index_workaround<A>::value>, detail::null_type>
	return_internal_reference(A)
	{
		return detail::policy_cons<detail::dependency_policy<0
			, detail::get_index_workaround<A>::value>, detail::null_type>();
	}
}

#else

namespace luabind
{
	template<int A, int B>
	detail::policy_cons<detail::dependency_policy<A, B>, detail::null_type>
	dependency(LUABIND_PLACEHOLDER_ARG(A), LUABIND_PLACEHOLDER_ARG(B))
	{
		return detail::policy_cons<detail::dependency_policy<A, B>, detail::null_type>();
	}

	template<int A>
	detail::policy_cons<detail::dependency_policy<0, A>, detail::null_type>
	return_internal_reference(LUABIND_PLACEHOLDER_ARG(A))
	{
		return detail::policy_cons<detail::dependency_policy<0, A>, detail::null_type>();
	}
}

#endif

#endif // LUABIND_DEPENDENCY_POLICY_HPP_INCLUDED

