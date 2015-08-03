/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/Lua/LuaBind/detail/constructor.hpp $
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

#ifndef LUABIND_CONSTRUCTOR_HPP_INCLUDED
#define LUABIND_CONSTRUCTOR_HPP_INCLUDED

#include <boost/config.hpp>
#include <boost/preprocessor/iterate.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repeat.hpp>
#include <boost/preprocessor/comma_if.hpp>

#include <boost/mpl/apply_wrap.hpp>

#include "Lua/LuaBind/config.hpp"
#include "Lua/LuaBind/wrapper_base.hpp"
#include "Lua/LuaBind/detail/policy.hpp"
#include "Lua/LuaBind/detail/signature_match.hpp"
#include "Lua/LuaBind/detail/call_member.hpp"
#include "Lua/LuaBind/wrapper_base.hpp"
#include "Lua/LuaBind/weak_ref.hpp"

namespace luabind { namespace detail
{
	template<int N>
	struct constructor_helper;

	namespace mpl = boost::mpl;
	
	template<int N>
	struct wrapped_constructor_helper;
	
	#define BOOST_PP_ITERATION_PARAMS_1 (4, (0, LUABIND_MAX_ARITY, "Lua/LuaBind/detail/Constructor.hpp", 1))
	#include BOOST_PP_ITERATE()

	template<class T, class Policies, class ConstructorSig>
	struct construct_class
	{
		inline static void* apply(lua_State* L, weak_ref const& ref)
		{
			typedef constructor_helper<ConstructorSig::arity> helper;
			return helper::execute(
                L
			  , ref
              , (T*)0
              , (ConstructorSig*)0
              , (Policies*)0
            );
		}
	};

	template<class T, class W, class Policies, class ConstructorSig>
	struct construct_wrapped_class
	{
		inline static void* apply(lua_State* L, weak_ref const& ref)
		{
			typedef wrapped_constructor_helper<ConstructorSig::arity> helper;
			return helper::execute(
                L
              , ref
              , (T*)0
              , (W*)0
              , (ConstructorSig*)0
              , (Policies*)0
            );
		}
	};

}}

#endif // LUABIND_CONSTRUCTOR_HPP_INCLUDED


#elif BOOST_PP_ITERATION_FLAGS() == 1


#define LUABIND_DECL(z, n, text) \
	typedef typename find_conversion_policy<n+1,Policies>::type BOOST_PP_CAT(converter_policy,n); \
\
	typename mpl::apply_wrap2< \
		BOOST_PP_CAT(converter_policy,n), BOOST_PP_CAT(A,n), lua_to_cpp \
	>::type BOOST_PP_CAT(c,n);

#define LUABIND_PARAM(z,n,text) \
	BOOST_PP_CAT(c,n).apply(L, LUABIND_DECORATE_TYPE(BOOST_PP_CAT(A,n)), n + 2)

	template<>
	struct constructor_helper<BOOST_PP_ITERATION()>
	{
        template<class T, class Policies, BOOST_PP_ENUM_PARAMS(LUABIND_MAX_ARITY, class A)>
        static T* execute(
            lua_State* L
          , weak_ref const&
          , T*
          , Constructor<BOOST_PP_ENUM_PARAMS(LUABIND_MAX_ARITY,A)>*
          , Policies*)
        {
            BOOST_PP_REPEAT(BOOST_PP_ITERATION(), LUABIND_DECL, _)
            return new T(BOOST_PP_ENUM(BOOST_PP_ITERATION(), LUABIND_PARAM, _));
        }
/*
		template<class T>
		struct apply
		{
			template<class Policies, BOOST_PP_ENUM_PARAMS(LUABIND_MAX_ARITY, class A)>
			static T* call(lua_State* L, const Constructor<BOOST_PP_ENUM_PARAMS(LUABIND_MAX_ARITY,A)>*, const Policies*)
			{
				// L is used, but the metrowerks compiler warns about this before expanding the macros
				L = L;
				BOOST_PP_REPEAT(BOOST_PP_ITERATION(), LUABIND_DECL, _)
				return new T(BOOST_PP_ENUM(BOOST_PP_ITERATION(), LUABIND_PARAM, _));
			}
		};*/
	};

	template<>
	struct wrapped_constructor_helper<BOOST_PP_ITERATION()>
	{
        template<class T, class W, class Policies, BOOST_PP_ENUM_PARAMS(LUABIND_MAX_ARITY, class A)>
        static T* execute(
            lua_State* L
          , weak_ref const& ref
          , T*
          , W*
          , Constructor<BOOST_PP_ENUM_PARAMS(LUABIND_MAX_ARITY,A)>*
          , Policies*)
        {
            BOOST_PP_REPEAT(BOOST_PP_ITERATION(), LUABIND_DECL, _)
            W* result = new W(BOOST_PP_ENUM(BOOST_PP_ITERATION(), LUABIND_PARAM, _));
            static_cast<weak_ref&>(detail::wrap_access::ref(*result)) = ref;
            return result;
        }
/*
        template<class T>
		struct apply
		{
			template<class Policies, BOOST_PP_ENUM_PARAMS(LUABIND_MAX_ARITY, class A)>
			static T* call(lua_State* L, weak_ref const& ref, const Constructor<BOOST_PP_ENUM_PARAMS(LUABIND_MAX_ARITY,A)>*, const Policies*)
			{
				BOOST_PP_REPEAT(BOOST_PP_ITERATION(), LUABIND_DECL, _)
				T* o = new T(BOOST_PP_ENUM(BOOST_PP_ITERATION(), LUABIND_PARAM, _));
				static_cast<weak_ref&>(detail::wrap_access::ref(*o)) = ref;
				return o;
			}
		};*/
	};


#undef LUABIND_PARAM
#undef LUABIND_DECL

#endif // LUABIND_CONSTRUCTOR_HPP_INCLUDED

