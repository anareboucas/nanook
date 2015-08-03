/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/Lua/LuaBind/detail/object_rep.hpp $
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


#ifndef LUABIND_OBJECT_REP_HPP_INCLUDED
#define LUABIND_OBJECT_REP_HPP_INCLUDED

#include "Lua/LuaBind/config.hpp"
#include "Lua/LuaBind/detail/ref.hpp"

namespace luabind { namespace detail
{
	class class_rep;

	void finalize(lua_State* L, class_rep* crep);

	// this class is allocated inside lua for each pointer.
	// it contains the actual c++ object-pointer.
	// it also tells if it is const or not.
	class LUABIND_API object_rep
	{
	public:
		enum { constant = 1, owner = 2, lua_class = 4, call_super = 8 };

		// dest is a function that is called to delete the c++ object this struct holds
		object_rep(void* obj, class_rep* crep, int flags, void(*dest)(void*));
		object_rep(class_rep* crep, int flags, detail::lua_reference const& table_ref);
		~object_rep();

		void* ptr() const { return m_object; }

		void* ptr(int pointer_offset) const
		{
			return reinterpret_cast<char*>(m_object) + pointer_offset;
		}

		const class_rep* crep() const { return m_classrep; }
		class_rep* crep() { return m_classrep; }
		int flags() const { return m_flags; }
		void set_flags(int flags) { m_flags = flags; }

		detail::lua_reference& get_lua_table() { return m_lua_table_ref; }
		detail::lua_reference const& get_lua_table() const { return m_lua_table_ref; }

		void remove_ownership();
		void set_destructor(void(*ptr)(void*));

		void set_object(void* p) { m_object = p; }

		void add_dependency(lua_State* L, int index);

		static int garbage_collector(lua_State* L);

	private:

		void* m_object; // pointer to the c++ object or holder / if lua class, this is a pointer the the instance of the
									// c++ base or 0.
		class_rep* m_classrep; // the class information about this object's type
		int m_flags;
		detail::lua_reference m_lua_table_ref; // reference to lua table if this is a lua class
		void(*m_destructor)(void*); // this could be in class_rep? it can't: see intrusive_ptr
		int m_dependency_cnt; // counts dependencies
		detail::lua_reference m_dependency_ref; // reference to lua table holding dependency references

		// ======== the new way, separate object_rep from the holder
//		instance_holder* m_instance;
	};

	template<class T>
	struct delete_s
	{
		static void apply(void* ptr)
		{
			delete static_cast<T*>(ptr);
		}
	};

	template<class T>
	struct destruct_only_s
	{
		static void apply(void* ptr)
		{
			// Removes unreferenced formal parameter warning on VC7.
			(void)ptr;
#ifndef NDEBUG
			int completeness_check[sizeof(T)];
			(void)completeness_check;
#endif
			static_cast<T*>(ptr)->~T();
		}
	};


	inline object_rep* is_class_object(lua_State* L, int index)
	{
		object_rep* obj = static_cast<detail::object_rep*>(lua_touserdata(L, index));
		if (!obj) return 0;
		if (lua_getmetatable(L, index) == 0) return 0;

		lua_pushstring(L, "__luabind_class");
		lua_gettable(L, -2);
		bool confirmation = lua_toboolean(L, -1) != 0;
		lua_pop(L, 2);
		if (!confirmation) return 0;
		return obj;

	}

}}

#endif // LUABIND_OBJECT_REP_HPP_INCLUDED

