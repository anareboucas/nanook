/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/Lua/LuaBind/class_registry.cpp $
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


// Copyright (c) 2004 Daniel Wallin

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

#include "Lua/LuaBind/luabind.hpp"
#include "Lua/LuaBind/detail/class_registry.hpp"
#include "Lua/LuaBind/detail/class_rep.hpp"
#include "Lua/LuaBind/detail/operator_id.hpp"

namespace luabind { namespace detail {

    namespace {

        void add_operator_to_metatable(lua_State* L, int op_index)
        {
            lua_pushstring(L, get_operator_name(op_index));
            lua_pushstring(L, get_operator_name(op_index));
            lua_pushboolean(L, op_index == op_unm);
            lua_pushcclosure(L, &class_rep::operator_dispatcher, 2);
            lua_settable(L, -3);
        }

        int create_cpp_class_metatable(lua_State* L)
        {
            lua_newtable(L);

            // mark the table with our (hopefully) unique tag
            // that says that the user data that has this
            // metatable is a class_rep
            lua_pushstring(L, "__luabind_classrep");
            lua_pushboolean(L, 1);
            lua_rawset(L, -3);

            lua_pushstring(L, "__gc");
            lua_pushcclosure(
                L
              , &garbage_collector_s<
                    detail::class_rep
                >::apply
                , 0);

            lua_rawset(L, -3);

            lua_pushstring(L, "__call");
            lua_pushcclosure(L, &class_rep::constructor_dispatcher, 0);
            lua_rawset(L, -3);

            lua_pushstring(L, "__index");
            lua_pushcclosure(L, &class_rep::static_class_gettable, 0);
            lua_rawset(L, -3);

            lua_pushstring(L, "__newindex");
            lua_pushcclosure(L, &class_rep::lua_settable_dispatcher, 0);
            lua_rawset(L, -3);

            return detail::ref(L);
        }

        int create_cpp_instance_metatable(lua_State* L)
        {
            lua_newtable(L);

            // just indicate that this really is a class and not just 
            // any user data
            lua_pushstring(L, "__luabind_class");
            lua_pushboolean(L, 1);
            lua_rawset(L, -3);

            // __index and __newindex will simply be references to the 
            // class_rep which in turn has it's own metamethods for __index
            // and __newindex
            lua_pushstring(L, "__index");
            lua_pushcclosure(L, &class_rep::gettable_dispatcher, 0);
            lua_rawset(L, -3);

            lua_pushstring(L, "__newindex");
            lua_pushcclosure(L, &class_rep::settable_dispatcher, 0);
            lua_rawset(L, -3);

            lua_pushstring(L, "__gc");

            lua_pushcclosure(L, detail::object_rep::garbage_collector, 0);
            lua_rawset(L, -3);

            lua_pushstring(L, "__gettable");
            lua_pushcclosure(L, &class_rep::static_class_gettable, 0);
            lua_rawset(L, -3);

            for (int i = 0; i < number_of_operators; ++i) 
                add_operator_to_metatable(L, i);

            // store a reference to the instance-metatable in our class_rep
            assert((lua_type(L, -1) == LUA_TTABLE) 
                && "internal error, please report");

            return detail::ref(L);
        }

        int create_lua_class_metatable(lua_State* L)
        {
            lua_newtable(L);

            lua_pushstring(L, "__luabind_classrep");
            lua_pushboolean(L, 1);
            lua_rawset(L, -3);

            lua_pushstring(L, "__gc");
            lua_pushcclosure(
                L
              , &detail::garbage_collector_s<
                    detail::class_rep
                >::apply
                , 0);

            lua_rawset(L, -3);

            lua_pushstring(L, "__newindex");
            lua_pushcclosure(L, &class_rep::lua_settable_dispatcher, 0);
            lua_rawset(L, -3);

            lua_pushstring(L, "__call");
            lua_pushcclosure(L, &class_rep::construct_lua_class_callback, 0);
            lua_rawset(L, -3);

            lua_pushstring(L, "__index");
            lua_pushcclosure(L, &class_rep::static_class_gettable, 0);
            lua_rawset(L, -3);

            return detail::ref(L);
        }

        int create_lua_instance_metatable(lua_State* L)
        {
            lua_newtable(L);

            // just indicate that this really is a class and not just 
            // any user data
            lua_pushstring(L, "__luabind_class");
            lua_pushboolean(L, 1);
            lua_rawset(L, -3);

            lua_pushstring(L, "__index");
            lua_pushcclosure(L, &class_rep::lua_class_gettable, 0);
            lua_rawset(L, -3);

            lua_pushstring(L, "__newindex");
            lua_pushcclosure(L, &class_rep::lua_class_settable, 0);
            lua_rawset(L, -3);

            lua_pushstring(L, "__gc");
            lua_pushcclosure(L, detail::object_rep::garbage_collector, 0);
            lua_rawset(L, -3);

            for (int i = 0; i < number_of_operators; ++i) 
                add_operator_to_metatable(L, i);

            // store a reference to the instance-metatable in our class_rep
            return detail::ref(L);
        }

        int create_lua_function_metatable(lua_State* L)
        {
            lua_newtable(L);

            lua_pushstring(L, "__gc");
            lua_pushcclosure(
                L 
              , detail::garbage_collector_s<
                    detail::free_functions::function_rep
                >::apply
              , 0);

            lua_rawset(L, -3);

            return detail::ref(L);
        }

    } // namespace unnamed

    class class_rep;

    class_registry::class_registry(lua_State* L)
        : m_cpp_instance_metatable(create_cpp_instance_metatable(L))
        , m_cpp_class_metatable(create_cpp_class_metatable(L))
        , m_lua_instance_metatable(create_lua_instance_metatable(L))
        , m_lua_class_metatable(create_lua_class_metatable(L))
        , m_lua_function_metatable(create_lua_function_metatable(L))
    {
    }

    class_registry* class_registry::get_registry(lua_State* L)
    {

#ifdef LUABIND_NOT_THREADSAFE

        // if we don't have to be thread safe, we can keep a
        // chache of the class_registry pointer without the
        // need of a mutex
        static lua_State* cache_key = 0;
        static class_registry* registry_cache = 0;
        if (cache_key == L) return registry_cache;

#endif

        lua_pushstring(L, "__luabind_classes");
        lua_gettable(L, LUA_REGISTRYINDEX);
        class_registry* p = static_cast<class_registry*>(lua_touserdata(L, -1));
        lua_pop(L, 1);

#ifdef LUABIND_NOT_THREADSAFE

        cache_key = L;
        registry_cache = p;

#endif

        return p;
    }

    void class_registry::add_class(LUABIND_TYPE_INFO info, class_rep* crep)
    {
        // class is already registered
        assert((m_classes.find(info) == m_classes.end()) 
            && "you are trying to register a class twice");
        m_classes[info] = crep;
    }

    class_rep* class_registry::find_class(LUABIND_TYPE_INFO info) const
    {
        std::map<LUABIND_TYPE_INFO, class_rep*, cmp>::const_iterator i(
            m_classes.find(info));

        if (i == m_classes.end()) return 0; // the type is not registered
        return i->second;
    }

}} // namespace luabind::detail

