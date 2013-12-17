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

#ifndef GAMELIB_CORE_HTTPFILEDOWNLOADER_H
#define GAMELIB_CORE_HTTPFILEDOWNLOADER_H 1

#include <gamelib/core/common.h>
#include <gamelib/core/filedownloader.h>
#include <gamelib/core/interface.h>

GAMELIB_NAMESPACE_START(core)

/**
 * @interface HttpFileDownloader httpfiledownloader.h <gamelib/core/httpfiledownloader.h>
 *
 * This interface extends the {@link FileDownloader} interface by Http specific operations
 *
 * @author Karol Herbst
 * @since 0
 */
interface PUBLIC_API HttpFileDownloader : public FileDownloader
{

};

GAMELIB_NAMESPACE_END(core)

#endif //GAMELIB_CORE_HTTPFILEDOWNLOADER_H
