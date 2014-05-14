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

#include <algorithm>
#include <chrono>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

#include <std_compat/thread>

#include <gamekeeper/core/curlfiledownloader.h>
#include <gamekeeper/core/logger.h>
#include <gamekeeper/core/loggerFactory.h>
#include <gamekeeper/core/loggerStream.h>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>

#include <curl/curl.h>

namespace algo = boost::algorithm;
namespace fs = boost::filesystem;

GAMEKEEPER_NAMESPACE_START(core)

class PRIVATE_API CurlFileDownloadInfo
{
public:
	PRIVATE_API CurlFileDownloadInfo(const FileDownloader::DownloadCallback * _func, uint64_t _maxBufferSize, const fs::path & _path)
	:	maxBufferSize(_maxBufferSize),
		func(_func),
		path(_path){}

	PRIVATE_API uint64_t bytesDownloaded();
	PRIVATE_API bool addData(void * const buffer, uint64_t bytes);
	PRIVATE_API void callback();

private:
	const uint64_t maxBufferSize;
	const FileDownloader::DownloadCallback * func;
	const fs::path path;
	std::vector<gkbyte_t> dataBuffer;
	uint64_t _bytesDownloaded = 0;
};

uint64_t
CurlFileDownloadInfo::bytesDownloaded()
{
	return this->_bytesDownloaded;
}

bool
CurlFileDownloadInfo::addData(void * const buffer, uint64_t bytes)
{
	gkbyte_t * newData = static_cast<gkbyte_t *>(buffer);
	// check amount of downloaded bytes to know if we use a file or buffer stream
	if(this->bytesDownloaded() <= (this->maxBufferSize * 1024))
	{
		// the buffer might be too small for the new data, so check it before
		if((this->dataBuffer.size() + bytes) > (this->maxBufferSize * 1024))
		{
			// first create directories
			if(!fs::exists(this->path.parent_path()))
			{
				fs::create_directories(this->path.parent_path());
			}
			fs::basic_ofstream<gkbyte_t> ofs(this->path, std::ios_base::trunc | std::ios_base::binary);
			ofs.write(this->dataBuffer.data(), this->dataBuffer.size());
			ofs.write(newData, bytes);
			ofs.close();
			// clear internal buffer
			this->dataBuffer.clear();
		}
		else
		{
			this->dataBuffer.insert(this->dataBuffer.end(), newData, &newData[bytes]);
		}
	}
	else
	{
		fs::basic_ofstream<gkbyte_t> ofs(this->path, std::ios_base::app | std::ios_base::binary);
		ofs.write(newData, bytes);
	}
	this->_bytesDownloaded += bytes;
	return true;
}

void
CurlFileDownloadInfo::callback()
{
	if(bytesDownloaded() <= (this->maxBufferSize * 1024))
	{
		namespace bio = boost::iostreams;
		bio::stream<bio::basic_array_source<gkbyte_t>> stream(this->dataBuffer.data(), this->dataBuffer.size());
		stream.peek();
		(*this->func)(stream);
	}
	else
	{
		fs::basic_ifstream<gkbyte_t> ifs(this->path);
		(*this->func)(ifs);
		ifs.close();
		fs::remove(this->path);
	}
}

class PRIVATE_API CURLPrivateData
{
public:
	PRIVATE_API CURLPrivateData(const char * const url, const std::string & userAgent,
	                            std::shared_ptr<PropertyResolver>);
	PRIVATE_API ~CURLPrivateData();

	CURL * handle;
	std::string postData;
	CurlFileDownloadInfo * downloadInfo = nullptr;
	uint16_t resolveFailed = 0;
	uint16_t connectFailed = 0;
};

CURLPrivateData::CURLPrivateData(const char * const url, const std::string & userAgent,
                                 std::shared_ptr<PropertyResolver> pr)
:	handle(curl_easy_init())
{
	curl_easy_setopt(this->handle, CURLOPT_URL, url);
	curl_easy_setopt(this->handle, CURLOPT_NOSIGNAL, 1);
	curl_easy_setopt(this->handle, CURLOPT_USERAGENT, userAgent.c_str());
	curl_easy_setopt(this->handle, CURLOPT_CONNECTTIMEOUT_MS, pr->get<uint16_t>("network.connection.timeout"));
}

CURLPrivateData::~CURLPrivateData()
{
	if(this->downloadInfo != nullptr)
	{
		delete downloadInfo;
	}
	curl_easy_cleanup(this->handle);
}

static uint64_t
curlFileDownloadCallback(void * const buffer, size_t size, size_t nrMem,
                         CURLPrivateData * data)
{
	uint64_t sizeInBytes = size * nrMem;
	CurlFileDownloadInfo * info = data->downloadInfo;
	if(!info->addData(buffer, sizeInBytes))
	{
		return -1;
	}
	return sizeInBytes;
}

static uint64_t
emptyCurlFileDownloadCallback(void * const, size_t size, size_t nrMem, void *)
{
	return size * nrMem;
}

static std::string
buildUserAgentString(const boost::any & configValue)
{
	if(configValue.empty())
	{
		return std::string("GameKeeper/0.1 libcurl/") + curl_version_info(CURLVERSION_NOW)->version;
	}
	return boost::any_cast<std::string>(configValue);
}

const std::unordered_set<std::string>
CurlFileDownloader::supportedProtocols = {"http", "https", "ftp", "ftps", "sftp"};

CurlFileDownloader::CurlFileDownloader(std::shared_ptr<LoggerFactory> loggerFactory,
                                       std::shared_ptr<PropertyResolver> _propertyResolver,
                                       std::shared_ptr<OSPaths> _ospaths)
:	propertyResolver(_propertyResolver),
	ospaths(_ospaths),
	logger(loggerFactory->getComponentLogger("IO.curl")),
	userAgent(buildUserAgentString(_propertyResolver->get("network.user_agent")))
{
	logger << LogLevel::Debug << "init curl with user-agent: " << this->userAgent << endl;
	curl_global_init(CURL_GLOBAL_SSL);
}

CurlFileDownloader::~CurlFileDownloader()
{
	curl_global_cleanup();
}

void
CurlFileDownloader::handleFileDownload(CURLPrivateData & curl, FileDownloader::DownloadCallback * func, const char * const url)
{
	boost::filesystem::path downloadPath = this->resolveDownloadPath(url);
	this->logger << LogLevel::Debug << "try to download file from: " << url << " at: " << downloadPath.string() << endl;
	curl.downloadInfo =
		new CurlFileDownloadInfo(func, this->propertyResolver->get<uint32_t>("network.download.max_buffer_size"),
		                         downloadPath);
	curl_easy_setopt(curl.handle, CURLOPT_WRITEFUNCTION, &curlFileDownloadCallback);
	curl_easy_setopt(curl.handle, CURLOPT_WRITEDATA, &curl);
	this->performCurl(curl);
	curl.downloadInfo->callback();
}

static void
addCookiesToCurl(const HttpFileDownloader::CookieBuket& cookies, CURLPrivateData & curl)
{
	if(!cookies.empty())
	{
		std::ostringstream cookieLineBuilder;
		for (const HttpFileDownloader::Cookie cookie : cookies)
		{
			cookieLineBuilder << cookie.first << '=' << cookie.second << ";";
		}
		std::string cookieLine = cookieLineBuilder.str();
		curl_easy_setopt(curl.handle, CURLOPT_COOKIE, cookieLine.c_str());
	}
}

static HttpFileDownloader::CookieBuket
getCookies(CURLPrivateData & curl)
{
	struct curl_slist * list;
	HttpFileDownloader::CookieBuket result;
	curl_easy_getinfo(curl.handle, CURLINFO_COOKIELIST, &list);

	while(list != nullptr)
	{
		std::vector<std::string> strings;
		boost::split(strings, list->data, boost::is_any_of("\t"));
		result[strings[5]] = strings[6];
		list = list->next;
	}

	curl_slist_free_all(list);
	return result;
}

static void
addFormToCurl(const HttpFileDownloader::Form& form, CURLPrivateData & curl)
{
	if(!form.empty())
	{
		std::ostringstream cookieLineBuilder;
		for (const HttpFileDownloader::FormField formField : form)
		{
			cookieLineBuilder << formField.first << '=' << formField.second << '&';
		}
		curl.postData = cookieLineBuilder.str();
		curl_easy_setopt(curl.handle, CURLOPT_POSTFIELDS, curl.postData.c_str());
	}
}

bool
CurlFileDownloader::supportsProtocol(const char * const protocolName)
{
	return supportedProtocols.find(protocolName) != supportedProtocols.end();
}

void
CurlFileDownloader::downloadFile(const char * const url, DownloadCallback callback)
{
	CURLPrivateData curl(url, this->userAgent, this->propertyResolver);
	this->handleFileDownload(curl, &callback, url);
}

void
CurlFileDownloader::downloadFileWithCookies(const char * const url, DownloadCallback callback,
                                            const CookieBuket& cookies)
{
	CURLPrivateData curl(url, this->userAgent, this->propertyResolver);
	addCookiesToCurl(cookies, curl);
	this->handleFileDownload(curl, &callback, url);
}

CurlFileDownloader::CookieBuket
CurlFileDownloader::doPostRequestForCookies(const char * const url, const Form& form)
{
	this->logger << LogLevel::Debug << "try to fetch cookies at: " << url << endl;
	CURLPrivateData curl(url, this->userAgent, this->propertyResolver);
	curl_easy_setopt(curl.handle, CURLOPT_WRITEFUNCTION, &emptyCurlFileDownloadCallback);
	curl_easy_setopt(curl.handle, CURLOPT_COOKIEJAR, nullptr);

	addFormToCurl(form, curl);

	this->performCurl(curl);

	CurlFileDownloader::CookieBuket result = getCookies(curl);

	return result;
}

void
CurlFileDownloader::performCurl(CURLPrivateData & curl, uint32_t timeout)
{
	if(timeout > 0)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(timeout));
	}

	CURLcode code = curl_easy_perform(curl.handle);
	switch (code)
	{
		case CURLE_OK: // 0
			// everything okay
			this->logger << LogLevel::Trace << "CURL returned without errors" << endl;
			break;
		case CURLE_COULDNT_RESOLVE_HOST: // 6
			curl.resolveFailed++;
			if(curl.resolveFailed < this->propertyResolver->get<uint16_t>("network.resolve.retries"))
			{
				this->logger << LogLevel::Warn << "CURL couldn't resolve host, retrying" << endl;
				this->performCurl(curl, this->propertyResolver->get<uint16_t>("network.time_between_retries"));
			}
			else
			{
				this->logger << LogLevel::Error << "CURL couldn't resolve host after " << curl.resolveFailed << " retries" << endl;
			}
			break;
		case CURLE_COULDNT_CONNECT: // 7
			curl.connectFailed++;
			if(curl.connectFailed < this->propertyResolver->get<uint16_t>("network.connection.retries"))
			{
				this->logger << LogLevel::Warn << "CURL couldn't connect to host, retrying" << endl;
				this->performCurl(curl, this->propertyResolver->get<uint16_t>("network.time_between_retries"));
			}
			else
			{
				this->logger << LogLevel::Error << "CURL couldn't connect to host after " << curl.connectFailed << " retries" << endl;
			}
			break;
		default:
			// unhandled error
			this->logger << LogLevel::Fatal << "CURL error \"" << curl_easy_strerror(code) << "\" (" << code <<
				") unhandled, please report a bug" << endl;
			break;
	}
}

boost::filesystem::path
CurlFileDownloader::resolveDownloadPath(const char * const url)
{
	std::string uri = url;
	// frist cut the protocoll
	size_t pos = uri.find("://");
	uri.erase(0, pos + 3);
	// now replace some unsupported characters
	algo::replace_all(uri, ":", "_");
	return this->ospaths->getCacheFile(std::string("downloads/" + uri));
}

GAMEKEEPER_NAMESPACE_END(core)
