/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/Lua/LuaBind/detail/decorate_type.hpp $
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


#ifndef LUABIND_DECORATE_TYPE_HPP_INCLUDED
#define LUABIND_DECORATE_TYPE_HPP_INCLUDED

#include "Lua/LuaBind/config.hpp"
#include "Lua/LuaBind/detail/primitives.hpp"

namespace luabind { namespace detail
{

#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

	template<class T>
	struct decorated_type
	{
		static by_value<T> t;
		static inline by_value<T>& get() { return /*by_value<T>()*/t; }
	};

	template<class T>
	by_value<T> decorated_type<T>::t;

	template<class T>
	struct decorated_type<T*>
	{
		static by_pointer<T> t;
		static inline by_pointer<T>& get() { return /*by_pointer<T>()*/t; }
	};

	template<class T>
	by_pointer<T> decorated_type<T*>::t;

	template<class T>
	struct decorated_type<const T*>
	{
		static by_const_pointer<T> t;
		static inline by_const_pointer<T> get() { return /*by_const_pointer<T>()*/t; }
	};

	template<class T>
	by_const_pointer<T> decorated_type<const T*>::t;

	template<class T>
	struct decorated_type<const T* const>
	{
		static by_const_pointer<T> t;
		static inline by_const_pointer<T>& get() { return /*by_const_pointer<T>()*/t; }
	};

	template<class T>
	by_const_pointer<T> decorated_type<const T* const>::t;

	template<class T>
	struct decorated_type<T&>
	{
		static by_reference<T> t;
		static inline by_reference<T>& get() { return /*by_reference<T>()*/t; }
	};

	template<class T>
	by_reference<T> decorated_type<T&>::t;

	template<class T>
	struct decorated_type<const T&>
	{
		static by_const_reference<T> t;
		static inline by_const_reference<T>& get() { return /*by_const_reference<T>()*/t; }
	};

	template<class T>
	by_const_reference<T> decorated_type<const T&>::t;

	#define LUABIND_DECORATE_TYPE(t) luabind::detail::decorated_type<t>::get()

#else

#include <boost/type_traits/is_array.hpp>

	namespace
	{
		LUABIND_ANONYMOUS_FIX char decorated_type_array[64];
	}

	template<class T>
	struct decorated_type_cref_impl
	{
#if defined(BOOST_MSVC) && BOOST_MSVC == 1200
		template<class U>
		static by_const_reference<U> get(const U&)
		{
			return by_const_reference<U>();
		}
		static T data() { return reinterpret_cast<T>(decorated_type_array); }
#else
	
		static void(*data())(T)
		{ return (void(*)(T))0; }

		template<class U>
		static by_const_reference<U> get(void(*f)(const U&))
		{ return by_const_reference<U>(); }
#endif
	};

	template<class T>
	struct decorated_type_ref_impl
	{
#if defined(BOOST_MSVC) && BOOST_MSVC == 1200
		template<class U>
		static by_reference<U> get(U&)
		{
			return by_reference<U>();
		}
		static T data() { return reinterpret_cast<T>(decorated_type_array); }
#else
		static void(*data())(T)
		{ return (void(*)(T))0; }

		template<class U>
		static by_reference<U> get(void(*)(U&))
		{ return by_reference<U>(); }
#endif
	};

	template<class T>
	struct decorated_type_cptr_impl
	{
#if defined(BOOST_MSVC) && BOOST_MSVC == 1200
		template<class U>
		static by_const_pointer<U> get(const U*)
		{
			return by_const_pointer<U>();
		}
		static T& data() { return reinterpret_cast<T&>(decorated_type_array); }
#else
		static void(*data())(T)
		{ return (void(*)(T))0; }

		template<class U>
		static by_const_pointer<U> get(void(*)(const U*))
		{ return by_const_pointer<U>(); }
#endif
	};

	template<class T>
	struct decorated_type_ptr_impl
	{
#if defined(BOOST_MSVC) && BOOST_MSVC == 1200
		template<class U>
		static by_pointer<U> get(U*)
		{
			return by_pointer<U>();
		}
		static T& data() { return reinterpret_cast<T&>(decorated_type_array); }
#else
		static void(*data())(T)
		{ return (void(*)(T))0; }

		template<class U>
		static by_pointer<U> get(void(*)(U*))
		{ return by_pointer<U>(); }
#endif
	};

	template<class T>
	struct decorated_type_value_impl
	{
#if defined(BOOST_MSVC) && BOOST_MSVC == 1200
		template<class U>
		static by_value<U> get(U&)
		{
			return by_value<U>();
		}
		static T& data() { return reinterpret_cast<T&>(decorated_type_array); }
#else
		static void(*data())(T&)
		{ return (void(*)(T&))0; }

		template<class U>
		static by_value<U> get(void(*)(U&))
		{ return by_value<U>(); }
#endif
	};

	template<>
	struct decorated_type_value_impl<void>
	{
		static by_value<void> get(int)
		{
			return by_value<void>();
		}
		static int data() { return 0; }
	};

	template<class T>
	struct decorated_type_array_impl
	{
		template<class U>
		static by_pointer<U> get(U*)
		{
			return by_pointer<U>();
		}

		template<class U>
		static by_pointer<U> get(void(*)(U))
		{ return by_pointer<U>(); }

		static T& data() { return reinterpret_cast<T&>(decorated_type_array); }
	};

	template<class T>
	struct decorated_type
//		: boost::mpl::if_<boost::is_array<T>
//			,	decorated_type_array_impl<T>
		:		boost::mpl::if_<luabind::detail::is_const_reference<T>
					, decorated_type_cref_impl<T>
					, typename boost::mpl::if_<luabind::detail::is_nonconst_reference<T>
						, decorated_type_ref_impl<T>
						, typename boost::mpl::if_<luabind::detail::is_nonconst_pointer<T>
							, decorated_type_ptr_impl<T>
							, typename boost::mpl::if_<luabind::detail::is_const_pointer<T>
								, decorated_type_cptr_impl<T>
								, decorated_type_value_impl<T>
					>::type
				>::type
			>::type
		>::type
//	>::type
	{
	};

#if defined(BOOST_MSVC) && BOOST_MSVC == 1200
	#define LUABIND_DECORATE_TYPE(t) luabind::detail::decorated_type<t>::get(luabind::detail::decorated_type<t>::data())
#else
//	#define LUABIND_DECORATE_TYPE(t) luabind::detail::decorated_type<t>::get((void(*)(type<t>))0)
	#define LUABIND_DECORATE_TYPE(t) luabind::detail::decorated_type<t>::get(luabind::detail::decorated_type<t>::data())
		//#define LUABIND_DECORATE_TYPE(t) luabind::detail::decorated_type<t>::get(type<t>())
#endif

#endif

}}

#endif // LUABIND_DECORATE_TYPE_HPP_INCLUDED
