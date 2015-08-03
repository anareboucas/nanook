/***************************************************************************************************
 * File:         $URL: http://obtuse.cs.jhu.edu/Projects/Library/Source/Branches/NASABot/Src/Lua/LuaBind/detail/operator_id.hpp $
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


#ifndef LUABIND_OPERATOR_ID_HPP_INCLUDED
#define LUABIND_OPERATOR_ID_HPP_INCLUDED

#include "Lua/LuaBind/config.hpp"

namespace luabind { namespace detail {

	enum operator_id
	{
		op_add = 0,
		op_sub,
		op_mul,
		op_div,
		op_pow,
		op_lt,
		op_le,
		op_eq,
		op_call,
		op_unm,
		op_tostring,
        op_concat,

		number_of_operators
	};

	struct op_add_tag {};
	struct op_sub_tag {};
	struct op_mul_tag {};
	struct op_div_tag {};
	struct op_pow_tag {};
	struct op_lt_tag {};
	struct op_le_tag {};
	struct op_eq_tag {};
	struct op_call_tag {};
	struct op_unm_tag {};
	struct op_tostring_tag {};
    struct op_concat_tag {};

	inline const char* get_operator_name(int i)
	{
		static const char* a[number_of_operators] = {
            "__add", "__sub", "__mul", "__div", "__pow", 
            "__lt", "__le", "__eq", "__call", "__unm", 
            "__tostring", "__concat" };
		return a[i];
	}

	inline const char* get_operator_symbol(int i)
	{
		static const char* a[number_of_operators] = {
            "+", "-", "*", "/", "^", "<", 
            "<=", "==", "()", "- (unary)", 
            "tostring", ".." };
		return a[i];
	}

	inline bool is_unary(int i)
	{
		// the reason why unary minus is not considered a unary operator here is
		// that it always is given two parameters, where the second parameter always
		// is nil.
		return i == op_tostring;
	}


}}

#endif // LUABIND_OPERATOR_ID_HPP_INCLUDED
