/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/Lua/LuaBind/detail/object_funs.hpp $
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

#ifndef LUABIND_OBJECT_PROXY_HPP_INCLUDED
#define LUABIND_OBJECT_PROXY_HPP_INCLUDED

#include <boost/optional.hpp>

#include <luabind/config.hpp>
#include <luabind/detail/policy.hpp>
#include <luabind/error.hpp>
#include <luabind/detail/convert_to_lua.hpp>
#include <luabind/detail/debug.hpp>
#include <luabind/detail/stack_utils.hpp>

#include <boost/mpl/apply_wrap.hpp>

namespace luabind
{

	namespace detail
	{

		namespace mpl = boost::mpl;

		template<class T, class Obj, class Policies>
		inline T object_cast_impl(const Obj& obj, const Policies&)
		{
			if (obj.lua_state() == 0) 
			{
#ifndef LUABIND_NO_EXCEPTIONS
				throw cast_failed(0, LUABIND_TYPEID(T));
#else
				lua_State* L = obj.lua_state();
				cast_failed_callback_fun e = get_cast_failed_callback();
				if (e) e(L, LUABIND_TYPEID(T));

				assert(0 && "object_cast failed. If you want to handle this error use luabind::set_error_callback()");
				std::terminate();
#endif
			}

			LUABIND_CHECK_STACK(obj.lua_state());

			typedef typename detail::find_conversion_policy<0, Policies>::type converter_policy;
			typename mpl::apply_wrap2<converter_policy,T,lua_to_cpp>::type converter;

			obj.pushvalue();

			lua_State* L = obj.lua_state();
			detail::stack_pop p(L, 1);

#ifndef LUABIND_NO_ERROR_CHECKING

			if (converter.match(L, LUABIND_DECORATE_TYPE(T), -1) < 0)
			{
#ifndef LUABIND_NO_EXCEPTIONS
				throw cast_failed(L, LUABIND_TYPEID(T));
#else
				cast_failed_callback_fun e = get_cast_failed_callback();
				if (e) e(L, LUABIND_TYPEID(T));

				assert(0 && "object_cast failed. If you want to handle this error use luabind::set_error_callback()");
				std::terminate();
#endif
			}
#endif

			return converter.apply(L, LUABIND_DECORATE_TYPE(T), -1);
		}

		template<class T, class Obj, class Policies>
		boost::optional<T> object_cast_nothrow_impl(const Obj& obj, const Policies&)
		{
			typedef typename detail::find_conversion_policy<0, Policies>::type converter_policy;
			typename mpl::apply_wrap2<converter_policy,T,lua_to_cpp>::type converter;

			if (obj.lua_state() == 0) return boost::optional<T>();
			LUABIND_CHECK_STACK(obj.lua_state());

			obj.pushvalue();

			lua_State* L = obj.lua_state();
			detail::stack_pop p(L, 1);

#ifndef LUABIND_NO_ERROR_CHECKING

			if (converter.match(L, LUABIND_DECORATE_TYPE(T), -1) < 0)
				return boost::optional<T>();
#endif

			return boost::optional<T>(converter.apply(L, LUABIND_DECORATE_TYPE(T), -1));
		}
	}
	
	template<class T>
	T object_cast(const object& obj)
	{ return detail::object_cast_impl<T>(obj, detail::null_type()); }
	
	template<class T, class Policies>
	T object_cast(const object& obj, const Policies& p)
	{ return detail::object_cast_impl<T>(obj, p); }
	
	template<class T>
	boost::optional<T> object_cast_nothrow(const object& obj)
	{ return detail::object_cast_nothrow_impl<T>(obj, detail::null_type()); }

	template<class T, class Policies>
	boost::optional<T> object_cast_nothrow(const object& obj, const Policies& p)
	{ return detail::object_cast_nothrow_impl<T>(obj, p); }
	

	template<class T>
	T object_cast(const detail::proxy_object& obj)
	{ return detail::object_cast_impl<T>(obj, detail::null_type()); }
	
	template<class T, class Policies>
	T object_cast(const detail::proxy_object& obj, const Policies& p)
	{ return detail::object_cast_impl<T>(obj, p); }
	
	template<class T>
	boost::optional<T> object_cast_nothrow(const detail::proxy_object& obj)
	{ return detail::object_cast_nothrow_impl<T>(obj, detail::null_type()); }
	
	template<class T, class Policies>
	boost::optional<T> object_cast_nothrow(const detail::proxy_object& obj, const Policies& p)
	{ return detail::object_cast_nothrow_impl<T>(obj, p); }

	
	template<class T>
	T object_cast(const detail::proxy_raw_object& obj)
	{ return detail::object_cast_impl<T>(obj, detail::null_type()); }

	template<class T, class Policies>
	T object_cast(const detail::proxy_raw_object& obj, const Policies& p)
	{ return detail::object_cast_impl<T>(obj, p); }

	template<class T>
	boost::optional<T> object_cast_nothrow(const detail::proxy_raw_object& obj)
	{ return detail::object_cast_nothrow_impl<T>(obj, detail::null_type()); }

	template<class T, class Policies>
	boost::optional<T> object_cast_nothrow(const detail::proxy_raw_object& obj, const Policies& p)
	{ return detail::object_cast_nothrow_impl<T>(obj, p); }

	
	template<class T>
	T object_cast(const detail::proxy_array_object& obj)
	{ return detail::object_cast_impl<T>(obj, detail::null_type()); }
	
	template<class T, class Policies>
	T object_cast(const detail::proxy_array_object& obj, const Policies& p)
	{ return detail::object_cast_impl<T>(obj, p); }
	
	template<class T>
	boost::optional<T> object_cast_nothrow(const detail::proxy_array_object& obj)
	{ return detail::object_cast_nothrow_impl<T>(obj, detail::null_type()); }
	
	template<class T, class Policies>
	boost::optional<T> object_cast_nothrow(const detail::proxy_array_object& obj, const Policies& p)
	{ return detail::object_cast_nothrow_impl<T>(obj, p); }




	inline object get_globals(lua_State* L)
	{
		lua_pushvalue(L, LUA_GLOBALSINDEX);
		detail::lua_reference ref;
		ref.set(L);
		return object(L, ref, true/*object::reference()*/);
	}

	inline object get_registry(lua_State* L)
	{
		lua_pushvalue(L, LUA_REGISTRYINDEX);
		detail::lua_reference ref;
		ref.set(L);
		return object(L, ref, true/*object::reference()*/);
	}

	inline object newtable(lua_State* L)
	{
		lua_newtable(L);
		detail::lua_reference ref;
		ref.set(L);
		return object(L, ref, true/*object::reference()*/);
	}
}

/*

struct A
{
};

object f = Class<A>();

A* ptr = object_cast<A*>(f(), adopt(_1));

delete ptr;

*/

#endif // LUABIND_OBJECT_PROXY_HPP_INCLUDED
