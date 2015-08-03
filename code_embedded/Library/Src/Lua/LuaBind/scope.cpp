/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/Lua/LuaBind/scope.cpp $
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

#include "Lua/LuaBind/lua_include.hpp"

#include "Lua/LuaBind/Scope.hpp"
#include "Lua/LuaBind/detail/debug.hpp"
#include "Lua/LuaBind/detail/stack_utils.hpp"
#include <cassert>

namespace luabind { namespace detail {

    registration::registration()
        : m_next(0)
    {
    }

    registration::~registration()
    {
        delete m_next;
    }

    } // namespace detail
    
    Scope::Scope()
        : m_chain(0)
    {
    }
    
    Scope::Scope(std::auto_ptr<detail::registration> reg)
        : m_chain(reg.release())
    {
    }

    Scope::Scope(Scope const& other)
        : m_chain(other.m_chain)
    {
        const_cast<Scope&>(other).m_chain = 0;
    }

    Scope::~Scope()
    {
        delete m_chain;
    }
    
    Scope& Scope::operator,(Scope s)
    {
        if (!m_chain) 
        {
            m_chain = s.m_chain;
            s.m_chain = 0;
            return *this;
        }
        
        for (detail::registration* c = m_chain;; c = c->m_next)
        {
            if (!c->m_next)
            {
                c->m_next = s.m_chain;
                s.m_chain = 0;
                break;
            }
        }

        return *this;
    }

    void Scope::register_(lua_State* L) const
    {
        for (detail::registration* r = m_chain; r != 0; r = r->m_next)
        {
			LUABIND_CHECK_STACK(L);
            r->register_(L);
        }
    }

} // namespace luabind

namespace luabind {

    namespace {

        struct lua_pop_stack
        {
            lua_pop_stack(lua_State* L)
                : m_state(L)
            {
            }

            ~lua_pop_stack()
            {
                lua_pop(m_state, 1);
            }

            lua_State* m_state;
        };

    } // namespace unnamed
    
    module_::module_(lua_State* L, char const* name = 0)
        : m_state(L)
        , m_name(name)
    {
    }

    void module_::operator[](Scope s)
    {
        if (m_name)
        {
            lua_pushstring(m_state, m_name);
            lua_gettable(m_state, LUA_GLOBALSINDEX);

            if (!lua_istable(m_state, -1))
            {
                lua_pop(m_state, 1);

                lua_newtable(m_state);
                lua_pushstring(m_state, m_name);
                lua_pushvalue(m_state, -2);
                lua_settable(m_state, LUA_GLOBALSINDEX);
            }
        }
        else
        {
            lua_pushvalue(m_state, LUA_GLOBALSINDEX);
        }

        lua_pop_stack guard(m_state);

        s.register_(m_state);
    }

    struct Namespace::registration_ : detail::registration
    {
        registration_(char const* name)
            : m_name(name)
        {
        }

        void register_(lua_State* L) const
        {
			LUABIND_CHECK_STACK(L);
            assert(lua_gettop(L) >= 1);

            lua_pushstring(L, m_name);
            lua_gettable(L, -2);

			detail::stack_pop p(L, 1); // pops the table on exit

            if (!lua_istable(L, -1))
            {
                lua_pop(L, 1);

                lua_newtable(L);
                lua_pushstring(L, m_name);
                lua_pushvalue(L, -2);
                lua_settable(L, -4);
            }

            m_Scope.register_(L);
        }

        char const* m_name;
        Scope m_Scope;
    };

    Namespace::Namespace(char const* name)
        : Scope(std::auto_ptr<detail::registration>(
              m_registration = new registration_(name)))
    {
    }

    Namespace& Namespace::operator[](Scope s)
    {
        m_registration->m_Scope.operator,(s);        
        return *this;
    }

} // namespace luabind

