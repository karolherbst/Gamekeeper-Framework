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

#include <gamekeeper/core/curlfiledownloaderfactory.h>

#include <sstream>

#include <curl/curl.h>

#include <gamekeeper/core/curlfiledownloader.h>
#include <gamekeeper/core/userpaths.h>
#include <gamekeeper/core/loggerFactory.h>
#include <gamekeeper/core/propertyresolver.h>

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
:	data(new CurlFileDownloaderFactory::PImpl(lf, pr, up))
{
	curl_global_init(CURL_GLOBAL_SSL);
}

CurlFileDownloaderFactory::~CurlFileDownloaderFactory()
{
	curl_global_cleanup();
}

static std::string
buildUserAgentString(const boost::any & configValue)
{
	if(configValue.empty())
	{
		curl_version_info_data * data = curl_version_info(CURLVERSION_NOW);
		std::stringstream stream;
		stream << "GameKeeper/0.1 libcurl/" << data->version;
		if(data->ssl_version != nullptr)
		{
			stream << ' ' << data->ssl_version;
		}

		if(data->libz_version != nullptr)
		{
			stream << " zlib/" << data->libz_version;
		}
		return stream.str();
	}
	return boost::any_cast<std::string>(configValue);
}

std::shared_ptr<FileDownloader>
CurlFileDownloaderFactory::create()
{
	return std::make_shared<CurlFileDownloader>
	(
		this->data->lf->getComponentLogger("IO.curl"),
		buildUserAgentString(this->data->pr->get("network.user_agent")),
		this->data->up->getCacheFile("downloads/"),
	        this->data->pr->get<uint16_t>("network.connection.timeout"),
		this->data->pr->get<uint16_t>("network.time_between_retries"),
		this->data->pr->get<uint16_t>("network.resolve.retries"),
		this->data->pr->get<uint16_t>("network.connection.retries"),
		this->data->pr->get<uint32_t>("network.download.max_buffer_size")
	);
}

GAMEKEEPER_NAMESPACE_END(core)
