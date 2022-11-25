/*
*   BSD 3-Clause License, see file labled 'LICENSE' for the full License.
*   Copyright (c) 2022, Peter Ferranti
*   All rights reserved.
*/

#ifndef INLINE_ABI_MACROS_H_
#define INLINE_ABI_MACROS_H_
#include <cxxabi.h>

#if _GLIBCXX_USE_CXX11_ABI
	#define ABI_INLINE inline cxx11 __attribute__((abi_tag("cxx11")))
#else
	#define ABI_INLINE inline 
#endif

#define ALWAYS_INLINE ABI_INLINE __attribute__((always_inline))

#endif