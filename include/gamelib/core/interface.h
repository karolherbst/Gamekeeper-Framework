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

#ifndef GAMEKEEPER_CORE_INTERFACE_H
#define GAMEKEEPER_CORE_INTERFACE_H 1

#include <gamelib/core/common.h>

#ifndef interface
	/**
	 * fake type for interfaces.
	 * 
	 * Every pure virtual class should be declared with this type instead of class or struct.
	 * Also members, non pure virtual methods, or any protected or private methods may not be used.
	 * 
	 * Use the GAMELIB_INTERFACE_* macros to simplify interface declerations.
	 * 
	 * @author Karol Herbst
	 * @since 0
	 */
	#define interface struct
#endif

/**
 * this method auto creates the destructor for interface types
 * 
 * @author Karol Herbst
 * @since 0
 * @param type the name of the interface
 */
#define GAMELIB_INTERFACE_DESTRUCTOR(type) virtual ~type(){}

/**
 * this method auto creates the pure virtual method given
 * 
 * @author Karol Herbst
 * @since 0
 * @param method the method signature
 */
#define GAMELIB_INTERFACE_METHOD(method) virtual method = 0

/**
 * this method auto creates the optional virtual method given
 * 
 * @author Karol Herbst
 * @since 0
 * @param method the method signature
 */
#define GAMELIB_INTERFACE_METHOD_OPTIONAL(method) virtual method{}

/**
 * this macro shall be used to override interface methods
 * 
 * @author Karol Herbst
 * @since 0
 * @param method the method signature
 */
#define GAMELIB_IMPLEMENTATION_OVERRIDE(method) virtual method override

#endif //GAMEKEEPER_CORE_INTERFACE_H
