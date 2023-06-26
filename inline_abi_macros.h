/*
*   BSD 3-Clause License, see file labled 'LICENSE' for the full License.
*   Copyright (c) 2023, Peter Ferranti
*   All rights reserved.
*/

#ifndef INLINE_ABI_MACROS_H_
#define INLINE_ABI_MACROS_H_
#include <cxxabi.h>

#if _GLIBCXX_USE_CXX11_ABI
	#define ABI_INLINE inline cxx11 __attribute__((abi_tag("cxx11")))
	#define ALWAYS_INLINE inline cxx11 __attribute__((abi_tag("cxx11"), always_inline))
#else
	#define ABI_INLINE inline
	#define ALWAYS_INLINE inline __attribute__((always_inline))
#endif


#endif