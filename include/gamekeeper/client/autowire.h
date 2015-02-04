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

#include <gamekeeper/core/common.h>

#include <gamekeeper/client/hypodermic.h>

GAMEKEEPER_NAMESPACE_START(client)

/**
 * @class HypodermicUtil hypodermic.h <gamekeeper/client/hypodermic.h>
 *
 * class to optiain a hypodermic container instance
 *
 * Just use it as if it were a shared_ptr with only the operator-> and operator* methods
 *
 * @author Karol Herbst
 * @since 0
 *
 * @param T the type of the autowired variable
 */
template <typename T>
class PUBLIC_INLINE Autowire
{
public:
	PUBLIC_INLINE Autowire();
	PUBLIC_INLINE T & operator*() const noexcept;
	PUBLIC_INLINE T * operator->() const noexcept;
	PUBLIC_INLINE operator const std::shared_ptr<T> &() const noexcept;
	PUBLIC_INLINE operator std::shared_ptr<T> &() noexcept;
	PUBLIC_INLINE T * get() const noexcept;
private:
	std::shared_ptr<T> ptr;
};

template <typename T>
Autowire<T>::Autowire()
    :	ptr(HypodermicUtil::getContainer().resolve<T>()){}

template <typename T>
T &
Autowire<T>::operator*() const noexcept
{
	return this->ptr.operator*();
}

template <typename T>
T *
Autowire<T>::operator->() const noexcept
{
	return this->ptr.operator->();
}

template <typename T>
Autowire<T>::operator const std::shared_ptr<T> &() const noexcept
{
	return this->ptr;
}

template <typename T>
Autowire<T>::operator std::shared_ptr<T> &() noexcept
{
	return this->ptr;
}

template <typename T>
T *
Autowire<T>::get() const noexcept
{
	return this->ptr.get();
}

GAMEKEEPER_NAMESPACE_END(client)
