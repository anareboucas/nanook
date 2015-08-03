/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/Lua/LuaBind/value_wrapper.hpp $
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


// Copyright (c) 2005 Daniel Wallin and Arvid Norberg

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

#ifndef LUABIND_VALUE_WRAPPER_050419_HPP
#define LUABIND_VALUE_WRAPPER_050419_HPP

#include <boost/mpl/integral_c.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/aux_/msvc_eti_base.hpp>

#ifdef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
# define LUABIND_USE_VALUE_WRAPPER_TAG 
#else
#endif

#ifdef LUABIND_USE_VALUE_WRAPPER_TAG
# include <boost/mpl/identity.hpp>
# include <boost/mpl/eval_if.hpp>
# include <boost/mpl/has_xxx.hpp>
# include <boost/mpl/not.hpp>
# include <boost/mpl/and.hpp>
# include <boost/mpl/or.hpp>
# include <boost/type_traits/is_reference.hpp>
# include <boost/type_traits/is_pointer.hpp>
# include <boost/type_traits/is_array.hpp>
#endif

namespace luabind {

//
// Concept ``ValueWrapper``
//

#ifdef LUABIND_USE_VALUE_WRAPPER_TAG
template<class T>
struct value_wrapper_traits;
    
namespace detail 
{ 

  BOOST_MPL_HAS_XXX_TRAIT_DEF(value_wrapper_tag);

  struct unspecialized_value_wrapper_traits
  {
      typedef boost::mpl::false_ is_specialized;
  };

  template<class T>
  struct value_wrapper_traits_aux
  {
      typedef value_wrapper_traits<typename T::value_wrapper_tag> type;
  };

} // namespace detail
#endif

template<class T>
struct value_wrapper_traits
#ifdef LUABIND_USE_VALUE_WRAPPER_TAG
  : boost::mpl::eval_if<
        boost::mpl::and_<
            boost::mpl::not_<
                boost::mpl::or_<
                    boost::is_reference<T>
                  , boost::is_pointer<T>
                  , boost::is_array<T>
                >
            >
          , detail::has_value_wrapper_tag<T>
        >
      , detail::value_wrapper_traits_aux<T>
      , boost::mpl::identity<detail::unspecialized_value_wrapper_traits>
    >::type
{};
#else
{
    typedef boost::mpl::false_ is_specialized;
};
#endif

template<class T>
struct is_value_wrapper
  : boost::mpl::aux::msvc_eti_base<
        typename value_wrapper_traits<T>::is_specialized
    >::type
{};

} // namespace luabind

#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

# include <boost/type_traits/remove_const.hpp>
# include <boost/type_traits/remove_reference.hpp>

namespace luabind {

template<class T>
struct is_value_wrapper_arg
  : is_value_wrapper<
      typename boost::remove_const<
          typename boost::remove_reference<T>::type
      >::type
    >
{};

} // namespace luabind

#else

# include <luabind/detail/yes_no.hpp>
# include <boost/type_traits/add_reference.hpp>

namespace luabind {

namespace detail
{
  template<class T>
  typename is_value_wrapper<T>::type is_value_wrapper_arg_check(T const*);
  
  yes_t to_yesno(boost::mpl::true_);
  no_t to_yesno(boost::mpl::false_);

  template<class T>
  struct is_value_wrapper_arg_aux
  {
      static typename boost::add_reference<T>::type x;

      BOOST_STATIC_CONSTANT(bool, value = 
          sizeof(to_yesno(is_value_wrapper_arg_check(&x)))
            == sizeof(yes_t)
      );

      typedef boost::mpl::bool_<value> type;
  };

} // namespace detail

template<class T>
struct is_value_wrapper_arg
  : detail::is_value_wrapper_arg_aux<T>::type
{
};

} // namespace luabind

#endif

#endif // LUABIND_VALUE_WRAPPER_050419_HPP

