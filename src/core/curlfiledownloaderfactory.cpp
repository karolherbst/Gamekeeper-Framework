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

#include "pch.h"

#include <gamekeeper/core/curlfiledownloaderfactory.h>

#include <gamekeeper/core/curlfiledownloader.h>

GAMEKEEPER_NAMESPACE_START(core)

class CurlFileDownloaderFactory::PImpl
{
public:
	PImpl(std::shared_ptr<LoggerFactory>, std::shared_ptr<PropertyResolver>, std::shared_ptr<UserPaths>);
	std::shared_ptr<LoggerFactory> lf;
	std::shared_ptr<PropertyResolver> pr;
	std::shared_ptr<UserPaths> up;
};

CurlFileDownloaderFactory::PImpl::PImpl(std::shared_ptr<LoggerFactory> _lf, std::shared_ptr<PropertyResolver> _pr, std::shared_ptr<UserPaths> _up)
:	lf(_lf),
	pr(_pr),
	up(_up){}

CurlFileDownloaderFactory::CurlFileDownloaderFactory(std::shared_ptr<LoggerFactory> lf, std::shared_ptr<PropertyResolver> pr, std::shared_ptr<UserPaths> up)
:	data(new CurlFileDownloaderFactory::PImpl(lf, pr, up)){}

CurlFileDownloaderFactory::~CurlFileDownloaderFactory(){}

std::shared_ptr<FileDownloader>
CurlFileDownloaderFactory::create()
{
	return std::make_shared<CurlFileDownloader>(this->data->lf, this->data->pr, this->data->up);
}

GAMEKEEPER_NAMESPACE_END(core)
