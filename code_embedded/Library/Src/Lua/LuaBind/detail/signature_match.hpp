/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/Lua/LuaBind/detail/signature_match.hpp $
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


#if !BOOST_PP_IS_ITERATING

#ifndef LUABIND_SIGNATURE_MATCH_HPP_INCLUDED
#define LUABIND_SIGNATURE_MATCH_HPP_INCLUDED

#include "Lua/LuaBind/config.hpp"

#include <boost/config.hpp>
#include <boost/preprocessor/repeat.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum.hpp> 
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/cat.hpp>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/int.hpp>
#include <boost/type_traits.hpp>

#include "Lua/LuaBind/detail/policy.hpp"
#include "Lua/LuaBind/detail/primitives.hpp"
#include "Lua/LuaBind/detail/object_rep.hpp"
#include "Lua/LuaBind/detail/class_rep.hpp"
#include "Lua/LuaBind/detail/most_derived.hpp"

namespace luabind
{

	namespace detail
	{
		template<class A>
		struct constructor_arity_helper
		{
			BOOST_STATIC_CONSTANT(int, value = 1);
		};

		template<>
		struct constructor_arity_helper<luabind::detail::null_type>
		{
			BOOST_STATIC_CONSTANT(int, value = 0);
		};
	}


#define LUABIND_SUM(z, n, _) detail::constructor_arity_helper<A##n >::value + 

	template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(LUABIND_MAX_ARITY, class A, detail::null_type)>
	struct Constructor
	{
		BOOST_STATIC_CONSTANT(int, arity = BOOST_PP_REPEAT(LUABIND_MAX_ARITY, LUABIND_SUM, _) 0);
	};

#undef LUABIND_SUM
}

namespace luabind { namespace detail
{
#define LUABIND_MATCH_DECL(Z, N,_) \
    typedef typename find_conversion_policy< \
        N + 1 \
      , Policies \
    >::type BOOST_PP_CAT(converter_policy, N); \
\
	typedef typename mpl::apply_wrap2< \
		BOOST_PP_CAT(converter_policy, N), BOOST_PP_CAT(A, N), lua_to_cpp \
	>::type BOOST_PP_CAT(converter, N); \
\
	int BOOST_PP_CAT(r, N) = BOOST_PP_CAT(converter, N)::match( \
        L \
      , LUABIND_DECORATE_TYPE(BOOST_PP_CAT(A, N)) \
      , start_index + current_index \
    ); \
\
    current_index += BOOST_PP_CAT(converter_policy, N)::has_arg; \
\
    if (BOOST_PP_CAT(r, N) < 0) return -1; \
	else m += BOOST_PP_CAT(r, N);

	template<int N> struct match_constructor;

	#define BOOST_PP_ITERATION_PARAMS_1 (4, (0, LUABIND_MAX_ARITY, "Lua/LuaBind/detail/signature_match.hpp", 2))
	#include BOOST_PP_ITERATE()

#undef LUABIND_MATCH_DECL

	// this is a function that checks if the lua stack (starting at the given start_index) matches
	// the types in the Constructor type given as 3:rd parameter. It uses the Policies given as
	// 4:th parameter to do the matching. It returns the total number of cast-steps that needs to
	// be taken in order to match the parameters on the lua stack to the given parameter-list. Or,
	// if the parameter doesn't match, it returns -1.
	template<
		BOOST_PP_ENUM_PARAMS(LUABIND_MAX_ARITY, class A)
	  , class Policies
	>
	int match_params(
		lua_State* L
	  , int start_index
	  , const Constructor<BOOST_PP_ENUM_PARAMS(LUABIND_MAX_ARITY, A)>* c
	  , const Policies* p)
	{
		typedef Constructor<BOOST_PP_ENUM_PARAMS(LUABIND_MAX_ARITY, A)> sig_t;
		return match_constructor<sig_t::arity>::apply(
			L, start_index, c, p);
	}

	template<class Sig, int StartIndex, class Policies>
	struct constructor_match
	{
		inline static int apply(lua_State* L)	
		{
			int top = lua_gettop(L) - StartIndex + 1;
			if (top != Sig::arity) return -1;

			return match_params(L, StartIndex, (Sig*)0, (Policies*)0);
		}};

	#define BOOST_PP_ITERATION_PARAMS_1 (4, (0, LUABIND_MAX_ARITY, "Lua/LuaBind/detail/signature_match.hpp", 1))
	#include BOOST_PP_ITERATE()

}}

#endif // LUABIND_SIGNATURE_MATCH_HPP_INCLUDED

#elif BOOST_PP_ITERATION_FLAGS() == 1

#define N BOOST_PP_ITERATION()

	// non-const non-member function this as a pointer
	template<
	    class WrappedClass
	  , class Policies
	  , class R 
		BOOST_PP_COMMA_IF(BOOST_PP_ITERATION()) 
			BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), class A)
	>
	int match(R(*)(
		BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), A))
	  , lua_State* L
	  , WrappedClass*
	  , Policies const*)
	{
		typedef Constructor<BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), A)> ParameterTypes;
		return match_params(
			L, 1, (ParameterTypes*)0, (Policies*)0);
	}

# if (BOOST_PP_ITERATION() < (LUABIND_MAX_ARITY - 1))

	// non-const member function
	template<
		class T
	  , class WrappedClass
	  , class Policies
	  , class R 
		BOOST_PP_COMMA_IF(N)
			BOOST_PP_ENUM_PARAMS(N, class A)
	>
	int match(
		R(T::*)(BOOST_PP_ENUM_PARAMS(N, A))
	  , lua_State* L
	  , WrappedClass*
	  , Policies const*)
	{
		typedef Constructor<
			BOOST_DEDUCED_TYPENAME most_derived<T,WrappedClass>::type&
		    BOOST_PP_ENUM_TRAILING_PARAMS(N, A)
		> params_t;

		return match_params(
			L, 1, (params_t*)0, (Policies*)0);
	}

	// const member function
	template<
		class T
	  , class WrappedClass
	  , class Policies
	  , class R 
			BOOST_PP_COMMA_IF(N)
				BOOST_PP_ENUM_PARAMS(N, class A)
	>
	int match(
		R(T::*)(BOOST_PP_ENUM_PARAMS(N, A)) const
	  , lua_State* L
	  , WrappedClass*
	  , Policies const* policies)
	{
		typedef Constructor<
			BOOST_DEDUCED_TYPENAME most_derived<T,WrappedClass>::type const&
			BOOST_PP_ENUM_TRAILING_PARAMS(N, A)
		> params_t;
		return match_params(
			L, 1, (params_t*)0, (Policies*)0);
	}

# endif

#undef N

#elif BOOST_PP_ITERATION_FLAGS() == 2

#define N BOOST_PP_ITERATION()

	template<>
	struct match_constructor<N>
	{
		template<
			BOOST_PP_ENUM_PARAMS(LUABIND_MAX_ARITY, class A)
		  , class Policies
		>
		static int apply(
			lua_State* L
		  , int start_index
		  , const Constructor<BOOST_PP_ENUM_PARAMS(LUABIND_MAX_ARITY, A)>*
		  , const Policies*)
		{
			int m = 0;
#if N
            int current_index = 0;
#endif
			// Removes unreferenced local variable warning on VC7.
			(void)start_index;
			(void)L;
			
            BOOST_PP_REPEAT(N, LUABIND_MATCH_DECL, _)
			return m;
		}
	};

#undef N

#endif

