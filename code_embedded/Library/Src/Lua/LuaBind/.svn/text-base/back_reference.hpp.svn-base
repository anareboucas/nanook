/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/Lua/LuaBind/back_reference.hpp $
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


// Copyright (c) 2004 Daniel Wallin and Arvid Norberg

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

#ifndef LUABIND_BACK_REFERENCE_040510_HPP
#define LUABIND_BACK_REFERENCE_040510_HPP

#include "Lua/LuaBind/lua_include.hpp"
#include "Lua/LuaBind/wrapper_base.hpp"
#include "Lua/LuaBind/detail/has_get_pointer.hpp"
#include "Lua/LuaBind/get_pointer.hpp"

#include <boost/type_traits/is_polymorphic.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/mpl/if.hpp>

namespace luabind {

namespace detail
{
  namespace mpl = boost::mpl;
 
  template<class T>
  wrap_base const* get_back_reference_aux0(T const* p, mpl::true_)
  {
      return dynamic_cast<wrap_base const*>(p);
  }

  template<class T>
  wrap_base const* get_back_reference_aux0(T const* p, mpl::false_)
  {
      return 0;
  }

  template<class T>
  wrap_base const* get_back_reference_aux1(T const* p)
  {
      return get_back_reference_aux0(p, boost::is_polymorphic<T>());
  }

  template<class T>
  wrap_base const* get_back_reference_aux2(T const& x, mpl::true_)
  {
      return get_back_reference_aux1(get_pointer(x));
  }

  template<class T>
  wrap_base const* get_back_reference_aux2(T const& x, mpl::false_)
  {
      return get_back_reference_aux1(&x);
  }

  template<class T>
  wrap_base const* get_back_reference(T const& x)
  {
      return detail::get_back_reference_aux2(
          x
        , has_get_pointer<T>()
      );
  }
  
} // namespace detail

template<class T>
bool get_back_reference(lua_State* L, T const& x)
{
#ifndef LUABIND_NO_RTTI
    if (wrap_base const* w = detail::get_back_reference(x))
    {
        detail::wrap_access::ref(*w).get(L);
        return true;
    }
#endif
    return false;
}

template<class T>
bool move_back_reference(lua_State* L, T const& x)
{
#ifndef LUABIND_NO_RTTI
    if (wrap_base* w = const_cast<wrap_base*>(detail::get_back_reference(x)))
    {
        assert(detail::wrap_access::ref(*w).m_strong_ref.is_valid());
        detail::wrap_access::ref(*w).get(L);
        detail::wrap_access::ref(*w).m_strong_ref.reset();
        return true;
    }
#endif
    return false;
}

} // namespace luabind

#endif // LUABIND_BACK_REFERENCE_040510_HPP

