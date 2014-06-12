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

#ifndef GAMEKEEPER_CORE_CURLFILEDOWNLOADERFACTORY_H
#define GAMEKEEPER_CORE_CURLFILEDOWNLOADERFACTORY_H 1

#include <gamekeeper/core/common.h>

#include <gamekeeper/core/filedownloaderfactory.h>

GAMEKEEPER_NAMESPACE_START(core)

interface Logger;
interface LoggerFactory;
interface PropertyResolver;
interface UserPaths;

class PUBLIC_API CurlFileDownloaderFactory : public FileDownloaderFactory
{
public:
	PUBLIC_API CurlFileDownloaderFactory(std::shared_ptr<LoggerFactory>, std::shared_ptr<PropertyResolver>, std::shared_ptr<UserPaths>);
	PRIVATE_API virtual ~CurlFileDownloaderFactory();
	PRIVATE_API virtual std::shared_ptr<FileDownloader> create() override;
private:
	class PRIVATE_API PImpl;

	std::unique_ptr<CurlFileDownloaderFactory::PImpl> data;
};

GAMEKEEPER_NAMESPACE_END(core)

#endif //GAMEKEEPER_CORE_CURLFILEDOWNLOADERFACTORY_H
