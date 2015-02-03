/*
 * GameKeeper Framework
 *
 * Copyright (C) 2013 Karol Herbst <gamekeeper@karolherbst.de>
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

#pragma once

// some headers we want to use nearly everywhere

#ifdef __cplusplus
	#include <cstdint>
#else
	#include <stdbool.h>
	#include <stdint.h>
#endif

// define GCC version
#if defined(__GNUC__) && !defined(__clang__)
  #define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#endif

// some global datatypes
typedef char gkbyte_t;
typedef int64_t gktime64_t;

// for C++ we have to handle visibility and importing exporting
#ifdef __cplusplus
	// dllexport, dllimport and visibility flags for Win32 and Unix
	#if defined(_WIN32) || defined (__CYGWIN__)
		#if defined(BUILDING_DLL)
			#if defined(__GNUC__)
				#define PUBLIC_API __attribute__ ((dllexport))
				#define PUBLIC_INLINE
				#define REVERSE_PUBLIC_API __attribute__ ((dllimport))
			#else
				#define PUBLIC_API __declspec(dllexport)
				#define PUBLIC_INLINE
				#define REVERSE_PUBLIC_API __declspec(dllimport)
			#endif
		#else
			#if defined(__GNUC__)
				#define PUBLIC_API __attribute__ ((dllimport))
				#define PUBLIC_INLINE
				#define REVERSE_PUBLIC_API __attribute__ ((dllexport))
			#else
				#define PUBLIC_API __declspec(dllimport)
				#define PUBLIC_INLINE
				#define REVERSE_PUBLIC_API __declspec(dllexport)
			#endif
		#endif
		#define PRIVATE_API
	#else
		#if defined(__GNUC__) || defined(__clang__)
			#if __GNUC__ >= 4
				#define PUBLIC_API __attribute__ ((visibility ("default")))
				#define PUBLIC_INLINE __attribute__ ((visibility ("default")))
				#define PRIVATE_API  __attribute__ ((visibility ("hidden")))
			#else
				#define PUBLIC_API
				#define PUBLIC_INLINE
				#define PRIVATE_API
			#endif
			#define REVERSE_PUBLIC_API PUBLIC_API
		#endif
	#endif
#else
	#define PUBLIC_API
	#define PRIVATE_API
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
 * namespace gamekeeper
 * {
 * namespace @p name
 * {
 * @endcode
 *
 * @param[in] name of the namespace
 */
#ifdef __cplusplus
	#define GAMEKEEPER_NAMESPACE_START_0() \
	namespace gamekeeper \
	{

	#define GAMEKEEPER_NAMESPACE_START_1(name) \
	namespace gamekeeper \
	{ \
	namespace name \
	{

	#define GAMEKEEPER_NAMESPACE_START_2(name1, name2) \
	namespace gamekeeper \
	{ \
	namespace name1 \
	{ \
	namespace name2 \
	{

	#define GAMEKEEPER_NAMESPACE_START_X(x, name1, name2, FUNC, ...) FUNC
	#define GAMEKEEPER_NAMESPACE_START(...) GAMEKEEPER_NAMESPACE_START_X(,##__VA_ARGS__, GAMEKEEPER_NAMESPACE_START_2(__VA_ARGS__), GAMEKEEPER_NAMESPACE_START_1(__VA_ARGS__), GAMEKEEPER_NAMESPACE_START_0(__VA_ARGS__))
#else
	#define GAMEKEEPER_NAMESPACE_START(...)
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
#ifdef __cplusplus
	#define GAMEKEEPER_NAMESPACE_END_0(...) \
	}

	#define GAMEKEEPER_NAMESPACE_END_1(...) \
	} \
	}

	#define GAMEKEEPER_NAMESPACE_END_2(...) \
	} \
	} \
	}

	#define GAMEKEEPER_NAMESPACE_END_X(x, name1, name2, FUNC, ...) FUNC
	#define GAMEKEEPER_NAMESPACE_END(...) GAMEKEEPER_NAMESPACE_END_X(,##__VA_ARGS__, GAMEKEEPER_NAMESPACE_END_2(__VA_ARGS__), GAMEKEEPER_NAMESPACE_END_1(__VA_ARGS__), GAMEKEEPER_NAMESPACE_END_0(__VA_ARGS__))
#else
	#define GAMEKEEPER_NAMESPACE_END(...)
#endif

// the following stuff is for C++ only
#ifdef __cplusplus
	// declare override if the compiler does not understand it
	#if defined(__clang__)
		#if !__has_feature(cxx_override_control)
			#define override
			#define final
		#endif
	#elif defined(__INTEL_COMPILER)
		// supported since icc-14.0
		#if (__INTEL_COMPILER < 1400)
			#define override
			#define final
		#endif
	#elif defined(GCC_VERSION)
		// supported since gcc-4.7
		#if GCC_VERSION < 40700
			#define override
			#define final
		#endif
	#elif defined(_MSC_VER)
		// supported since Visual Studio 2010
		#if (_MSC_VER < 1600)
			#define override
			#define final
		// MSVC 2010 defines sealed instead of final
		#elif (_MSC_VER < 1700)
			#define final sealed
		#endif
	#endif

	// declare nullptr if the compiler does not understand it
	// we don't need to check clang, because we require 3.1 anyway
	#if defined(__INTEL_COMPILER)
		#if (__INTEL_COMPILER < 1210)
			#include <stddef.h>
			#define nullptr NULL
		#endif
	#elif defined(GCC_VERSION)
		// supported since gcc-4.6
		#if GCC_VERSION < 40600
			#include <stddef.h>
			#define nullptr NULL
		#endif
	// we don't need to check msvc, because we require msvc10 anyway
	#endif

	// declare thread_local if the compiler does not understand it
	#if defined(__clang__)
		#if !__has_feature(cxx_thread_local)
			#define thread_local __thread
		#endif
	#elif defined(GCC_VERSION)
		// supported since gcc-4.8
		#if GCC_VERSION < 40800
			#define thread_local __thread
		#endif
	#elif defined(__INTEL_COMPILER)
		#if defined(_WIN32) || defined (__CYGWIN__)
			#define thread_local __declspec(thread)
		#else
			#define thread_local __thread
		#endif
	#elif defined(_MSC_VER)
		#define thread_local __declspec(thread)
	#endif
#endif

// OS defines
#if defined(_WIN32)
  #define GAMEKEEPER_OS_IS_WINDOWS 1
  #if defined(_WIN64)
    #define GAMEKEEPER_ARCH_IS_64BIT 1
  #else
    #define GAMEKEEPER_ARCH_IS_32BIT 1
  #endif

  #if defined(__MINGW32__)
    #define GAMEKEEPER_WINDOWS_USED_MINGW 1
  #endif
#endif

#if defined(__linux__)
  #define GAMEKEEPER_OS_IS_LINUX 1
#endif

#if defined(__APPLE__) && defined(__MACH__)
  #define GAMEKEEPER_OS_IS_MACOSX 1
#endif

#ifndef interface
	/**
	 * fake type for interfaces.
	 *
	 * Every pure virtual class should be declared with this type instead of class or struct.
	 * Also members, non pure virtual methods, or any protected or private methods may not be used.
	 *
	 * Use the GAMEKEEPER_INTERFACE_* macros to simplify interface declerations.
	 *
	 * @author Karol Herbst
	 * @since 0
	 */
	#define interface struct
#endif

// some global variables
constexpr char GK_TRUE_STRING[] = "true";
constexpr char GK_FALSE_STRING[] = "false";
