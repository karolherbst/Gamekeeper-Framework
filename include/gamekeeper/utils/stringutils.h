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

#ifndef GAMEKEEPER_UTILS_STRINGUTILS_H
#define GAMEKEEPER_UTILS_STRINGUTILS_H 1

#include <gamekeeper/core/common.h>

#include <cstdint>
#include <string>

#include <boost/lexical_cast.hpp>

namespace std
{
	template <class charT, class traits >
	class basic_istream;

	template <class charT, class traits >
	class istreambuf_iterator;
}

GAMEKEEPER_NAMESPACE_START(utils)

/**
 * This namespace collect util functions for handling/creation/modifying std::string objects
 *
 * @author Karol Herbst
 * @since 0
 */
namespace String
{
	template <class T>
	struct Is8BitInt : std::integral_constant<bool,
		std::is_same<T, int8_t>::value || std::is_same<T, uint8_t>::value>{};

	template <class T>
	struct Is16BitOrHigherInt : std::integral_constant<bool,
		std::is_arithmetic<T>::value && !std::is_same<T, char>::value && !Is8BitInt<T>::value>{};

	template <class charT, class traits >
	PUBLIC_INLINE std::basic_string<charT, traits> createFromStream(std::basic_istream<charT, traits> & istream);

	template <class T>
	PUBLIC_INLINE
	typename std::enable_if<std::is_constructible<std::string, T>::value, const T &>::type
	toString(const T &);

	template <class T>
	PUBLIC_INLINE
	typename std::enable_if<Is8BitInt<T>::value, std::string>::type
	toString(const T &);

	template <class T>
	PUBLIC_INLINE
	typename std::enable_if<Is16BitOrHigherInt<T>::value, std::string>::type
	toString(const T &);

	template <class T>
	PUBLIC_INLINE
	typename std::enable_if<std::is_enum<T>::value, std::string>::type
	toString(const T &);

	template <class T>
	PUBLIC_INLINE
	typename std::enable_if<std::is_same<T, char>::value, std::string>::type
	toString(const T &);

	template <class T>
	PUBLIC_INLINE
	typename std::enable_if<std::is_pointer<T>::value,
	                        typename std::enable_if<!std::is_same<typename std::remove_cv<typename std::remove_pointer<T>::type>::type,
	                                                              char>::value, std::string>::type>::type
	toString(const T &);
}

template <class charT, class traits>
std::basic_string<charT, traits>
String::createFromStream(std::basic_istream<charT, traits> & is)
{
	if(is.peek() == traits::eof())
	{
		return std::basic_string<charT, traits>();
	}
	return std::basic_string<charT, traits>(std::istreambuf_iterator<charT>(is), std::istreambuf_iterator<charT>());
}

template <class T>
typename std::enable_if<std::is_constructible<std::string, T>::value, const T &>::type
String::toString(const T & t)
{
	return t;
}

template <class T>
PUBLIC_INLINE
typename std::enable_if<String::Is8BitInt<T>::value, std::string>::type
String::toString(const T &t)
{
	// lexical_cast treats (u)int8_t as char
	#if defined(_GLIBCXX_HAVE_BROKEN_VSWPRINTF)
		// if _GLIBCXX_HAVE_BROKEN_VSWPRINTF is defined we can't use std::to_string
		return boost::lexical_cast<std::string>(static_cast<int16_t>(t));
	#else
		return std::to_string(t);
	#endif
}

template <class T>
typename std::enable_if<String::Is16BitOrHigherInt<T>::value, std::string>::type
String::toString(const T & t)
{
	return boost::lexical_cast<std::string>(t);
}

template <class T>
typename std::enable_if<std::is_enum<T>::value, std::string>::type
String::toString(const T & t)
{
	// just refer to the implementation of the underlying_type
	typedef typename std::underlying_type<T>::type UT;
	return String::toString<UT>(static_cast<UT>(t));
}

template <class T>
typename std::enable_if<std::is_same<T, char>::value, std::string>::type
String::toString(const T & t)
{
	return std::string(1, t);
}

template <class T>
typename std::enable_if<std::is_pointer<T>::value,
                        typename std::enable_if<!std::is_same<typename std::remove_cv<typename std::remove_pointer<T>::type>::type,
                                                              char>::value, std::string>::type>::type
String::toString(const T & t)
{
	return boost::lexical_cast<std::string>(t);
}

GAMEKEEPER_NAMESPACE_END(core)

#endif //GAMEKEEPER_UTILS_STRINGUTILS_H
