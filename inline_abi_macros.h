/*
*   BSD 3-Clause License, see file labled 'LICENSE' for the full License.
*   Copyright (c) 2024, Peter Ferranti
*   All rights reserved.
*/

#ifndef INLINE_ABI_MACROS_H_
#define INLINE_ABI_MACROS_H_

#if _GLIBCXX_USE_CXX11_ABI
	#include <cxxabi.h>
	#define ABI_INLINE inline __attribute__((abi_tag("cxx11")))
	#define AINLINE inline __attribute__((abi_tag("cxx11"), always_inline))
#else
	#define ABI_INLINE inline
	#define AINLINE inline __attribute__((always_inline))
#endif


#endif