/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/Lua/LuaBind/detail/get_signature.hpp $
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

#ifndef LUABIND_NO_ERROR_CHECKING

#if !BOOST_PP_IS_ITERATING

#ifndef LUABIND_GET_SIGNATURE_HPP_INCLUDED
#define LUABIND_GET_SIGNATURE_HPP_INCLUDED

#include <boost/config.hpp>
#include <boost/preprocessor/repeat.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum.hpp> 
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/cat.hpp>

#include "Lua/LuaBind/config.hpp"
#include "Lua/LuaBind/detail/signature_match.hpp"


namespace luabind { namespace detail
{

	std::string LUABIND_API get_class_name(lua_State* L, LUABIND_TYPE_INFO i);

	template<class T>
	std::string name_of_type(by_value<T>, lua_State* L, int) { return luabind::detail::get_class_name(L, LUABIND_TYPEID(T)); };
	template<class T>
	std::string name_of_type(by_reference<T>, lua_State* L, int) { return name_of_type(LUABIND_DECORATE_TYPE(T), L, 0L) + "&"; };
	template<class T>
	std::string name_of_type(by_pointer<T>, lua_State* L, int) { return name_of_type(LUABIND_DECORATE_TYPE(T), L, 0L) + "*"; };
	template<class T>
	std::string name_of_type(by_const_reference<T>, lua_State* L, int) { return "const " + name_of_type(LUABIND_DECORATE_TYPE(T), L, 0L) + "&"; };
	template<class T>
	std::string name_of_type(by_const_pointer<T>, lua_State* L, int) { return "const " + name_of_type(LUABIND_DECORATE_TYPE(T), L, 0L) + "*"; };

	inline std::string name_of_type(by_value<luabind::object>, lua_State*, int) { return "object"; };
	inline std::string name_of_type(by_const_reference<luabind::object>, lua_State*, int) { return "object"; };
	inline std::string name_of_type(by_value<bool>, lua_State*, int) { return "boolean"; }
	inline std::string name_of_type(by_value<char>, lua_State*, int) { return "number"; }
	inline std::string name_of_type(by_value<short>, lua_State*, int) { return "number"; }
	inline std::string name_of_type(by_value<int>, lua_State*, int) { return "number"; }
	inline std::string name_of_type(by_value<long>, lua_State*, int) { return "number"; }
	inline std::string name_of_type(by_value<unsigned char>, lua_State*, int) { return "number"; }
	inline std::string name_of_type(by_value<unsigned short>, lua_State*, int) { return "number"; }
	inline std::string name_of_type(by_value<unsigned int>, lua_State*, int) { return "number"; }
	inline std::string name_of_type(by_value<unsigned long>, lua_State*, int) { return "number"; }

	inline std::string name_of_type(by_value<const bool>, lua_State*, int) { return "boolean"; }
	inline std::string name_of_type(by_value<const char>, lua_State*, int) { return "number"; }
	inline std::string name_of_type(by_value<const short>, lua_State*, int) { return "number"; }
	inline std::string name_of_type(by_value<const int>, lua_State*, int) { return "number"; }
	inline std::string name_of_type(by_value<const long>, lua_State*, int) { return "number"; }
	inline std::string name_of_type(by_value<const unsigned char>, lua_State*, int) { return "number"; }
	inline std::string name_of_type(by_value<const unsigned short>, lua_State*, int) { return "number"; }
	inline std::string name_of_type(by_value<const unsigned int>, lua_State*, int) { return "number"; }
	inline std::string name_of_type(by_value<const unsigned long>, lua_State*, int) { return "number"; }

//	template<class T>
//	inline std::string name_of_type(by_value<luabind::functor<T> >, lua_State* L, long) { return "function<" + name_of_type(LUABIND_DECORATE_TYPE(T), L, 0L) + ">"; }

	inline std::string name_of_type(by_value<std::string>, lua_State*, int) { return "string"; }
	inline std::string name_of_type(by_const_pointer<char>, lua_State*, int) { return "string"; }
	inline std::string name_of_type(by_pointer<lua_State>, lua_State*, int) { return "lua_State*"; }
	
	template<class T>
	struct type_name_unless_void
	{
		inline static void apply(std::string& s, lua_State* L, bool first)
		{
			if (!first) s += ", ";
			s += name_of_type(LUABIND_DECORATE_TYPE(T), L, 0L);
		}
	};

	template<>
	struct type_name_unless_void<null_type>
	{
		inline static void apply(std::string&, lua_State*, bool) {}
	};

#define LUABIND_ADD_LUA_TYPE_NAME(z, n, _) type_name_unless_void<BOOST_PP_CAT(A, BOOST_PP_INC(n))>::apply(s, L, false);

	#define BOOST_PP_ITERATION_PARAMS_1 (4, (0, LUABIND_MAX_ARITY, "Lua/LuaBind/detail/get_signature.hpp", 1))
	#include BOOST_PP_ITERATE()

	template<class F>
	struct get_member_signature
	{
		static inline void apply(lua_State* L, std::string& s)
		{
			get_member_signature_impl(static_cast<F>(0), L, s);
		}
	};

	template<class F>
	struct get_free_function_signature
	{
		static inline void apply(lua_State* L, std::string& s)
		{
			get_free_function_signature_impl(static_cast<F>(0), L, s);
		}
	};


	template<class Sig>
	struct get_signature
	{
		static inline void apply(lua_State* L, std::string& s)
		{
			get_signature_impl(static_cast<const Sig*>(0), L, s);
		}
	};

	template<BOOST_PP_ENUM_PARAMS(LUABIND_MAX_ARITY, class A)>
	inline void get_signature_impl(const Constructor<BOOST_PP_ENUM_PARAMS(LUABIND_MAX_ARITY, A)>*, lua_State* L, std::string& s)
	{
		s += "(";
		type_name_unless_void<A0>::apply(s, L, true);
		BOOST_PP_REPEAT(BOOST_PP_DEC(LUABIND_MAX_ARITY), LUABIND_ADD_LUA_TYPE_NAME, _)
		s += ")";
	}

#undef LUABIND_ADD_LUA_TYPE_NAME

	template<class T>
	struct get_setter_signature
	{
		static void apply(lua_State* L, std::string& s)
		{
			s += "(";
			s += name_of_type(LUABIND_DECORATE_TYPE(T), L, 0L);
			s += ")";
		}
	};

}}

#endif // LUABIND_GET_SIGNATURE_HPP_INCLUDED

#elif BOOST_PP_ITERATION_FLAGS() == 1

	// member functions
	template<class T, class C BOOST_PP_COMMA_IF(BOOST_PP_ITERATION()) BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), class A)>
	inline void get_member_signature_impl(T(C::*)(BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), A)), lua_State* L, std::string& s)
	{
		s += "(";
#if BOOST_PP_ITERATION() > 0
		s += name_of_type(LUABIND_DECORATE_TYPE(A0), L, 0L);
		BOOST_PP_REPEAT(BOOST_PP_DEC(BOOST_PP_ITERATION()), LUABIND_ADD_LUA_TYPE_NAME, _)
#endif
		s += ")";
	}

	template<class T, class C BOOST_PP_COMMA_IF(BOOST_PP_ITERATION()) BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), class A)>
	inline void get_member_signature_impl(T(C::*)(BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), A)) const, lua_State* L, std::string& s)
	{
		(void)L;
		s += "(";
#if BOOST_PP_ITERATION() > 0
		s += name_of_type(LUABIND_DECORATE_TYPE(A0), L, 0L);
		BOOST_PP_REPEAT(BOOST_PP_DEC(BOOST_PP_ITERATION()), LUABIND_ADD_LUA_TYPE_NAME, _)
#endif
		s += ") const";
	}

 	// const C& obj
	template<class T BOOST_PP_COMMA_IF(BOOST_PP_ITERATION()) BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), class A)>
	inline void get_member_signature_impl(T(*f)(BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), A)), lua_State* L, std::string& s)
	{
		s += "(";
#if BOOST_PP_ITERATION() > 0
		s += name_of_type(LUABIND_DECORATE_TYPE(A0), L, 0L);
		BOOST_PP_REPEAT(BOOST_PP_DEC(BOOST_PP_ITERATION()), LUABIND_ADD_LUA_TYPE_NAME, _)
#endif
		s += ")";
	}

	// free functions
	template<class T BOOST_PP_COMMA_IF(BOOST_PP_ITERATION()) BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), class A)>
	inline void get_free_function_signature_impl(T(*f)(BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), A)), lua_State* L, std::string& s)
	{
		(void)f;
		s += "(";
#if BOOST_PP_ITERATION() > 0
		s += name_of_type(LUABIND_DECORATE_TYPE(A0), L, 0L);
		BOOST_PP_REPEAT(BOOST_PP_DEC(BOOST_PP_ITERATION()), LUABIND_ADD_LUA_TYPE_NAME, _)
#endif
		s += ")";
	}

#endif

#endif // LUABIND_NO_ERROR_CHECKING
