/*
 * GameKeeper Framework
 *
 * Copyright (C) 2013 - 2014 Karol Herbst <gamekeeper@karolherbst.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Linking against the GameKeeper Framework statically or dynamically with
 * other modules is making a combined work based on the GameKeeper Framework.
 * Thus, the terms and conditions of the GNU General Public License cover the
 * whole combination.
 *
 * In addition, as a special exception, the copyright holders of the GameKeeper
 * Framework give you permission to copy and distribute a work based on the
 * GameKeeper Framework in object code or executable form without the
 * requirements to accompany it with the source code, provided that you meet
 * all of these conditions:
 *
 *   a) The dervied work links with the GameKeeper Framework solely through
 *      symbols defined in any file in the /include directory and its
 *      subdirectories.
 *
 *   b) The dervied work does not link against symbols of the GameKeeper
 *      Framework outside the gamekeeper::client, gamekeeper::core and
 *      gamekeeper::model C++ namespaces.
 *
 * You may copy and distribute such a system
 * following the terms of the GNU GPL for the GameKeeper Framework and the licenses
 * of the other code concerned, provided that you include the source code of that other
 * code when and as the GNU GPL requires distribution of source code.
 *
 * Note that people who make modified versions of the GameKeeper Framework are
 * not obligated to grant this special exception for their modified versions;
 * it is their choice whether to do so.  The GNU General Public License gives
 * permission to release a modified version without this exception; this
 * exception also makes it possible to release a modified version which
 * carries forward this exception.
 */

#ifndef GAMEKEEPER_CLIENT_GAME_H
#define GAMEKEEPER_CLIENT_GAME_H 1

#include <gamekeeper/core/common.h>

#include <gamekeeper/model/game.h>

GAMEKEEPER_NAMESPACE_START(client)

interface PUBLIC_API Game : public model::Game
{
};

GAMEKEEPER_NAMESPACE_END(client)

#endif //GAMEKEEPER_CLIENT_GAME_H
