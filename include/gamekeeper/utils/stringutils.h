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

#include <string>

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
	template <class charT, class traits >
	PUBLIC_INLINE std::basic_string<charT, traits> createFromStream(std::basic_istream<charT, traits> & istream);
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

GAMEKEEPER_NAMESPACE_END(core)

#endif //GAMEKEEPER_UTILS_STRINGUTILS_H
