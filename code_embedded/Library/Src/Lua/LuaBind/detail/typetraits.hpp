/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/Lua/LuaBind/detail/typetraits.hpp $
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


#ifndef LUABIND_TYPETRAITS_HPP_INCLUDED
#define LUABIND_TYPETRAITS_HPP_INCLUDED

#include "Lua/LuaBind/config.hpp"

#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_reference.hpp>
#include <boost/type_traits/is_const.hpp>

#include "Lua/LuaBind/detail/primitives.hpp"

namespace luabind { namespace detail 
{

#ifdef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

	template<class T>
	struct is_const_type
	{
		typedef typename boost::mpl::if_<boost::is_const<T>
			, yes_t
			, no_t
		>::type type;
	};

	template<bool is_Reference = false>
	struct is_const_reference_helper
	{
		template<class>
		struct apply
		{
			enum
			{
				value = false
			};
		};
	};

	template<class T>
	typename is_const_type<T>::type is_const_reference_tester(T&);
	no_t is_const_reference_tester(...);

	template<>
	struct is_const_reference_helper<true>
	{
		template<class T>
		struct apply
		{
			static T getT();

			enum
			{
				value = sizeof(is_const_reference_tester(getT())) == sizeof(yes_t)
			};
		};
	};

	template<class T>
	struct is_const_reference
		: is_const_reference_helper<boost::is_reference<T>::value>::template apply<T>
	{
		typedef boost::mpl::bool_<value> type;
	};

#else

	template<class T>
	struct is_const_reference
	{
		enum { value = false };
		typedef boost::mpl::bool_<value> type;
	};

	template<class T>
	struct is_const_reference<const T&>
	{
		enum { value = true };
		typedef boost::mpl::bool_<value> type;
	};

#endif


	template<class T>
	struct is_nonconst_reference
	{
		enum
		{
			value = boost::is_reference<T>::value && !is_const_reference<T>::value
		};
		typedef boost::mpl::bool_<value> type;
	};

	template<class A>
	yes_t is_const_pointer_helper(void(*)(const A*));
	no_t is_const_pointer_helper(...);

	template<class T>
	struct is_const_pointer
	{
		enum { value = sizeof(is_const_pointer_helper((void(*)(T))0)) == sizeof(yes_t) };
		typedef boost::mpl::bool_<value> type;
	};

	template<class A>
	yes_t is_nonconst_pointer_helper(void(*)(A*));
	no_t is_nonconst_pointer_helper(...);

	template<class T>
	struct is_nonconst_pointer
	{
		enum { value = sizeof(is_nonconst_pointer_helper((void(*)(T))0)) == sizeof(yes_t) && !is_const_pointer<T>::value };
		typedef boost::mpl::bool_<value> type;
	};
/*
	template<class T>
	struct is_constructable_from_helper
	{
		static yes_t check(const T&);
		static no_t check(...);
	};

	template<class T, class From>
	struct is_constructable_from
	{
		static From getFrom();

		enum
		{
			value = sizeof(is_constructable_from_helper<T>::check(getFrom())) == sizeof(yes_t)
		};
	};

	template<class T>
	struct is_const_member_function_helper
	{
		static no_t test(...);
		template<class R>
		static yes_t test(R(T::*)() const);
		template<class R, class A1>
		static yes_t test(R(T::*)(A1) const);
		template<class R, class A1, class A2>
		static yes_t test(R(T::*)(A1,A2) const);
		template<class R, class A1, class A2, class A3>
		static yes_t test(R(T::*)(A1,A2,A3) const);
	};

	template<class T, class U>
	struct is_const_member_function
	{
		static U getU();

		enum
		{
			value = sizeof(is_const_member_function_helper<T>::test(getU())) == sizeof(yes_t)
		};
	};
*/

	template<int v1, int v2>
	struct max_c
	{
		enum { value = (v1>v2)?v1:v2 };
	};

}}

#endif // LUABIND_TYPETRAITS_HPP_INCLUDED

