/*
 * libgamelib
 *
 * Copyright (C) 2013 Karol Herbst <gamelib@karolherbst.de>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef GAMELIB_CORE_COMMON_H
#define GAMELIB_CORE_COMMON_H 1

// some headers we want to use nearly everywhere

// for C99 integer types
#include <cstdint>

// used for later stuff
#define PRIVATE_TYPE
#define PUBLIC_API
#define PRIVATE_API

#ifdef __cplusplus
#define PUBLIC_API extern "C"
#else
#define PUBLIC_API
#endif

/**
 * used to start a C++ namespace
 * 
 * It should be used in every header file also used in C applications.
 * It guards the namespace decleration with #ifdef __cplusplus check
 * 
 * This macro creates the following lines for C++ applications:
 * 
 * @code{.cpp}
 * namespace gamelib
 * {
 * namespace @p name
 * {
 * @endcode
 * 
 * @param[in] name of the namespace
 */
#define GAMELIB_NAMESPACE_START(name) \
#ifdef __cplusplus \
namespace gamelib \
{ \
namespace name \
{ \
#endif

/**
 * used to end a C++ namespace
 * 
 * It should be used in every header file also used in C applications.
 * It guards the namespace decleration with #ifdef __cplusplus check
 * 
 * This macro creates the following line for C++ applications:
 * 
 * @code{.cpp}
 * }
 * }
 * @endcode
 * 
 * @param[in] name of the namespace
 */
#define GAMELIB_NAMESPACE_END(name) \
#ifdef __cplusplus \
} \
} \
#endif

// declare override if the compiler does not understand it
#if defined(__GNUC__)
	// supported since gcc-4.7
	#if ((GNUC_MAJOR == 4 && GNUC_MINOR < 7) || GNUC_MAJOR < 4)
		#define override
	#endif
#elif defined(__clang__)
	#if !__has_feature(cxx_override_control)
		#define override
	#endif
#elif defined(_MSC_VER)
	// supported since Visual Studio 2010
	#if (_MSC_VER < 1600)
		#define override
	#endif
#endif

#endif //GAMELIB_CORE_COMMON_H
