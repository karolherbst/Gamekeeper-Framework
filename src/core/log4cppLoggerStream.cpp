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

#include <gamekeeper/core/log4cppLoggerStream.h>

GAMEKEEPER_NAMESPACE_START(core)

Log4cppLoggerStream::Log4cppLoggerStream(log4cpp::CategoryStream log4cppStream)
:	stream(log4cppStream){}

Log4cppLoggerStream::Log4cppLoggerStream(const Log4cppLoggerStream & l)
:	Log4cppLoggerStream(l.stream){}

LoggerStream&
Log4cppLoggerStream::operator<<(const std::string& str)
{
	this->stream << str;
	return *this;
}

LoggerStream&
Log4cppLoggerStream::flush()
{
	this->stream.flush();
	return *this;
}

GAMEKEEPER_NAMESPACE_END(core)
