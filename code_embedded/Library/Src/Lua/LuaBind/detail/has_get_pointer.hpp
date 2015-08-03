/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/Lua/LuaBind/detail/has_get_pointer.hpp $
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


// Copyright (c) 2005 Daniel Wallin

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

#ifndef LUABIND_HAS_GET_POINTER_051022_HPP
# define LUABIND_HAS_GET_POINTER_051022_HPP

# include <boost/type_traits/add_reference.hpp>

# ifndef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
#  include <memory>
# endif

namespace luabind { namespace detail { 

namespace has_get_pointer_
{

  struct any 
  { 
      template<class T> any(T const&);
  };

  struct no_overload_tag 
  {};

  typedef char (&yes)[1];
  typedef char (&no)[2];

  no_overload_tag operator,(no_overload_tag, int);

//
// On compilers with ADL, we need these generic overloads in this
// namespace as well as in luabind::. Otherwise get_pointer(any)
// will be found before them.
//
# ifndef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP

  template<class T>
  T* get_pointer(T*);

  template<class T>
  T* get_pointer(std::auto_ptr<T> const&);

# endif

//
// On compilers that doesn't support ADL, the overload below has to
// live in luabind::.
//
# ifdef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
}} // namespace detail::has_get_pointer_
# endif

detail::has_get_pointer_::no_overload_tag 
  get_pointer(detail::has_get_pointer_::any);

# ifdef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
namespace detail { namespace has_get_pointer_ 
{
# endif

  template<class T>
  yes check(T const&);
  no check(no_overload_tag);

  template<class T>
  struct impl
  {
      static typename boost::add_reference<T>::type x;

      BOOST_STATIC_CONSTANT(bool,
          value = sizeof(has_get_pointer_::check( (get_pointer(x),0) )) == 1
      );

      typedef boost::mpl::bool_<value> type;
  };

} // namespace has_get_pointer_

template<class T>
struct has_get_pointer
  : has_get_pointer_::impl<T>::type
{};

}} // namespace luabind::detail

#endif // LUABIND_HAS_GET_POINTER_051022_HPP

