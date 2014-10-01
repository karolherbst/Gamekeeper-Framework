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

#ifndef GAMEKEEPER_MODEL_CONVERTER_GAMESTRINGCONVERTER_H
#define GAMEKEEPER_MODEL_CONVERTER_GAMESTRINGCONVERTER_H 1

#include <gamekeeper/core/common.h>

#include <memory>
#include <unordered_map>

#include <gamekeeper/model/game.h>

GAMEKEEPER_NAMESPACE_START(model)

class PUBLIC_API GameStringConverter
{
public:
	static std::unique_ptr<Game> convert(const std::unordered_map<std::string,std::string>&);
	static std::unordered_map<std::string,std::string> convert(const Game &);
	static std::unordered_map<std::string,std::string> convert(const Game * const);
};

GAMEKEEPER_NAMESPACE_END(model)

#endif //GAMEKEEPER_MODEL_CONVERTER_GAMESTRINGCONVERTER_H
