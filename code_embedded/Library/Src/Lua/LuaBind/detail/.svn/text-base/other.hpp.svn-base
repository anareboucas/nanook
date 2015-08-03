/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/Lua/LuaBind/detail/other.hpp $
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


#ifndef LUABIND_OTHER_HPP_INCLUDED
#define LUABIND_OTHER_HPP_INCLUDED

// header derived from source code found in Boost.Python

// Copyright David Abrahams 2002. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#include "config.hpp"
#include <boost/config.hpp>

namespace luabind
{
	template<class T>
	struct other
	{
		typedef T type;
	};
}

#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
namespace luabind { namespace detail
{
	template<typename T>
	class unwrap_other
	{
	public:
			typedef T type;
	};

	template<typename T>
	class unwrap_other<other<T> >
	{
	public:
			typedef T type;
	};
}} // namespace luabind::detail

# else // no partial specialization

#include <boost/type.hpp>

namespace luabind { namespace detail
{
	typedef char (&yes_other_t)[1];
	typedef char (&no_other_t)[2];

	no_other_t is_other_test(...);

	template<typename T>
	yes_other_t is_other_test(type_< other<T> >);

	template<bool wrapped>
	struct other_unwrapper
	{
			template <class T>
			struct apply
			{
					typedef T type;
			};
	};

	template<>
	struct other_unwrapper<true>
	{
			template <class T>
			struct apply
			{
					typedef typename T::type type;
			};
	};

	template<typename T>
	class is_other
	{
	 public:
			BOOST_STATIC_CONSTANT(
					bool, value = (
							sizeof(detail::is_other_test(type_<T>()))
							== sizeof(detail::yes_other_t)));
	};

	template <typename T>
	class unwrap_other
			: public detail::other_unwrapper<
			is_other<T>::value
	>::template apply<T>
	{};

}} // namespace luabind::detail
#endif // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

#endif // LUABIND_OTHER_HPP_INCLUDED
