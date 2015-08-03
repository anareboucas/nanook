/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/Lua/LuaBind/detail/call_member.hpp $
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

#ifndef LUABIND_CALL_MEMBER_HPP_INCLUDED
#define LUABIND_CALL_MEMBER_HPP_INCLUDED

#include "Lua/LuaBind/config.hpp"
#include "Lua/LuaBind/detail/convert_to_lua.hpp"
#include "Lua/LuaBind/detail/pcall.hpp"
#include "Lua/LuaBind/error.hpp"
#include "Lua/LuaBind/detail/stack_utils.hpp"
#include "Lua/LuaBind/object.hpp" // TODO: REMOVE DEPENDENCY

#include <boost/tuple/tuple.hpp>

#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/facilities/expand.hpp>

#include <boost/mpl/apply_wrap.hpp>

namespace luabind
{
	namespace detail
	{

		namespace mpl = boost::mpl;

		// if the proxy_member_caller returns non-void
			template<class Ret, class Tuple>
			class proxy_member_caller
			{
//			friend class luabind::object;
			public:

				proxy_member_caller(lua_State* L_, const Tuple args)
					: L(L_)
					, m_args(args)
					, m_called(false)
				{
				}

				proxy_member_caller(const proxy_member_caller& rhs)
					: L(rhs.L)
					, m_args(rhs.m_args)
					, m_called(rhs.m_called)
				{
					rhs.m_called = true;
				}

				~proxy_member_caller()
				{
					if (m_called) return;

					m_called = true;

					// don't count the function and Self-reference
					// since those will be popped by pcall
					int top = lua_gettop(L) - 2;

					// pcall will pop the function and Self reference
					// and all the parameters

					push_args_from_tuple<1>::apply(L, m_args);
					if (pcall(L, boost::tuples::length<Tuple>::value + 1, 0))
					{
						assert(lua_gettop(L) == top + 1);
#ifndef LUABIND_NO_EXCEPTIONS
						throw luabind::error(L);
#else
						error_callback_fun e = get_error_callback();
						if (e) e(L);
	
						assert(0 && "the lua function threw an error and exceptions are disabled."
								"If you want to handle this error use luabind::set_error_callback()");
						std::terminate();
#endif
					}
					// pops the return values from the function
					stack_pop pop(L, lua_gettop(L) - top);
				}

				operator Ret()
				{
					typename mpl::apply_wrap2<default_policy,Ret,lua_to_cpp>::type converter;

					m_called = true;

					// don't count the function and Self-reference
					// since those will be popped by pcall
					int top = lua_gettop(L) - 2;

					// pcall will pop the function and Self reference
					// and all the parameters
					push_args_from_tuple<1>::apply(L, m_args);
					if (pcall(L, boost::tuples::length<Tuple>::value + 1, 1))
					{
						assert(lua_gettop(L) == top + 1);
#ifndef LUABIND_NO_EXCEPTIONS
						throw luabind::error(L); 
#else
						error_callback_fun e = get_error_callback();
						if (e) e(L);
	
						assert(0 && "the lua function threw an error and exceptions are disabled."
							"If you want to handle this error use luabind::set_error_callback()");
						std::terminate();
#endif
					}

					// pops the return values from the function
					stack_pop pop(L, lua_gettop(L) - top);

#ifndef LUABIND_NO_ERROR_CHECKING

					if (converter.match(L, LUABIND_DECORATE_TYPE(Ret), -1) < 0)
					{
						assert(lua_gettop(L) == top + 1);
#ifndef LUABIND_NO_EXCEPTIONS
						throw cast_failed(L, LUABIND_TYPEID(Ret));
#else
						cast_failed_callback_fun e = get_cast_failed_callback();
						if (e) e(L, LUABIND_TYPEID(Ret));

						assert(0 && "the lua function's return value could not be converted."
							"If you want to handle this error use luabind::set_error_callback()");
						std::terminate();
#endif
					}
#endif
					return converter.apply(L, LUABIND_DECORATE_TYPE(Ret), -1);
				}

				template<class Policies>
				Ret operator[](const Policies& p)
				{
					typedef typename find_conversion_policy<0, Policies>::type converter_policy;
					typename mpl::apply_wrap2<converter_policy,Ret,lua_to_cpp>::type converter;

					m_called = true;

					// don't count the function and Self-reference
					// since those will be popped by pcall
					int top = lua_gettop(L) - 2;

					// pcall will pop the function and Self reference
					// and all the parameters

					detail::push_args_from_tuple<1>::apply(L, m_args, p);
					if (pcall(L, boost::tuples::length<Tuple>::value + 1, 1))
					{
						assert(lua_gettop(L) == top + 1);
#ifndef LUABIND_NO_EXCEPTIONS
						throw error(L);
#else
						error_callback_fun e = get_error_callback();
						if (e) e(L);
	
						assert(0 && "the lua function threw an error and exceptions are disabled."
							"If you want to handle this error use luabind::set_error_callback()");
						std::terminate();
#endif
					}

					// pops the return values from the function
					stack_pop pop(L, lua_gettop(L) - top);

#ifndef LUABIND_NO_ERROR_CHECKING

					if (converter.match(L, LUABIND_DECORATE_TYPE(Ret), -1) < 0)
					{
						assert(lua_gettop(L) == top + 1);
#ifndef LUABIND_NO_EXCEPTIONS
						throw cast_failed(L, LUABIND_TYPEID(Ret));
#else
						cast_failed_callback_fun e = get_cast_failed_callback();
						if (e) e(L, LUABIND_TYPEID(Ret));

						assert(0 && "the lua function's return value could not be converted."
							"If you want to handle this error use luabind::set_error_callback()");
						std::terminate();
#endif
					}
#endif
					return converter.apply(L, LUABIND_DECORATE_TYPE(Ret), -1);
				}

			private:

				lua_State* L;
				Tuple m_args;
				mutable bool m_called;

			};

		// if the proxy_member_caller returns void
			template<class Tuple>
			class proxy_member_void_caller
			{
			friend class luabind::object;
			public:

				proxy_member_void_caller(lua_State* L_, const Tuple args)
					: L(L_)
					, m_args(args)
					, m_called(false)
				{
				}

				proxy_member_void_caller(const proxy_member_void_caller& rhs)
					: L(rhs.L)
					, m_args(rhs.m_args)
					, m_called(rhs.m_called)
				{
					rhs.m_called = true;
				}

				~proxy_member_void_caller()
				{
					if (m_called) return;

					m_called = true;

					// don't count the function and Self-reference
					// since those will be popped by pcall
					int top = lua_gettop(L) - 2;

					// pcall will pop the function and Self reference
					// and all the parameters

					push_args_from_tuple<1>::apply(L, m_args);
					if (pcall(L, boost::tuples::length<Tuple>::value + 1, 0))
					{
						assert(lua_gettop(L) == top + 1);
#ifndef LUABIND_NO_EXCEPTIONS
						throw luabind::error(L);
#else
						error_callback_fun e = get_error_callback();
						if (e) e(L);
	
						assert(0 && "the lua function threw an error and exceptions are disabled."
							"If you want to handle this error use luabind::set_error_callback()");
						std::terminate();
#endif
					}
					// pops the return values from the function
					stack_pop pop(L, lua_gettop(L) - top);
				}

				template<class Policies>
				void operator[](const Policies& p)
				{
					m_called = true;

					// don't count the function and Self-reference
					// since those will be popped by pcall
					int top = lua_gettop(L) - 2;

					// pcall will pop the function and Self reference
					// and all the parameters

					detail::push_args_from_tuple<1>::apply(L, m_args, p);
					if (pcall(L, boost::tuples::length<Tuple>::value + 1, 0))
					{
						assert(lua_gettop(L) == top + 1);
#ifndef LUABIND_NO_EXCEPTIONS
						throw error(L);
#else
						error_callback_fun e = get_error_callback();
						if (e) e(L);
	
						assert(0 && "the lua function threw an error and exceptions are disabled."
							"If you want to handle this error use luabind::set_error_callback()");
						std::terminate();
#endif
					}
					// pops the return values from the function
					stack_pop pop(L, lua_gettop(L) - top);
				}

			private:
				lua_State* L;
				Tuple m_args;
				mutable bool m_called;

			};

	} // detail

	#define BOOST_PP_ITERATION_PARAMS_1 (4, (0, LUABIND_MAX_ARITY, "Lua/LuaBind/detail/call_member.hpp", 1))
	#include BOOST_PP_ITERATE()

}

#endif // LUABIND_CALL_MEMBER_HPP_INCLUDED

#elif BOOST_PP_ITERATION_FLAGS() == 1

#define LUABIND_TUPLE_PARAMS(z, n, data) const A##n *
#define LUABIND_OPERATOR_PARAMS(z, n, data) const A##n & a##n

	template<class R BOOST_PP_COMMA_IF(BOOST_PP_ITERATION()) BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), class A)>
	typename boost::mpl::if_<boost::is_void<R>
			, luabind::detail::proxy_member_void_caller<boost::tuples::tuple<BOOST_PP_ENUM(BOOST_PP_ITERATION(), LUABIND_TUPLE_PARAMS, _)> >
			, luabind::detail::proxy_member_caller<R, boost::tuples::tuple<BOOST_PP_ENUM(BOOST_PP_ITERATION(), LUABIND_TUPLE_PARAMS, _)> > >::type
	call_member(object const& obj, const char* name BOOST_PP_COMMA_IF(BOOST_PP_ITERATION()) BOOST_PP_ENUM(BOOST_PP_ITERATION(), LUABIND_OPERATOR_PARAMS, _))
	{
		typedef boost::tuples::tuple<BOOST_PP_ENUM(BOOST_PP_ITERATION(), LUABIND_TUPLE_PARAMS, _)> tuple_t;
#if BOOST_PP_ITERATION() == 0
		tuple_t args;
#else
		tuple_t args(BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), &a));
#endif

		typedef typename boost::mpl::if_<boost::is_void<R>
			, luabind::detail::proxy_member_void_caller<boost::tuples::tuple<BOOST_PP_ENUM(BOOST_PP_ITERATION(), LUABIND_TUPLE_PARAMS, _)> >
			, luabind::detail::proxy_member_caller<R, boost::tuples::tuple<BOOST_PP_ENUM(BOOST_PP_ITERATION(), LUABIND_TUPLE_PARAMS, _)> > >::type proxy_type;

		// this will be cleaned up by the proxy object
		// once the call has been made

		// get the function
		obj.push(obj.interpreter());
		lua_pushstring(obj.interpreter(), name);
		lua_gettable(obj.interpreter(), -2);
		// duplicate the Self-object
		lua_pushvalue(obj.interpreter(), -2);
		// remove the bottom Self-object
		lua_remove(obj.interpreter(), -3);

		// now the function and Self objects
		// are on the stack. These will both
		// be popped by pcall
		return proxy_type(obj.interpreter(), args);
	}

#undef LUABIND_OPERATOR_PARAMS
#undef LUABIND_TUPLE_PARAMS

#endif

