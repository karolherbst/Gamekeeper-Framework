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
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

#include <gamekeeper/core/curlhelper.h>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>

#include <curl/curl.h>

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
	std::string postData;
	CurlFileDownloadInfo * downloadInfo = nullptr;

	PRIVATE_API ~CURLPrivateData()
	{
		if(this->downloadInfo != nullptr)
		{
			delete downloadInfo;
		}
	}
};

CurlHelper::CurlHelper(std::string newUserAgent)
:	userAgent(newUserAgent + " libcurl/" + curl_version_info(CURLVERSION_NOW)->version){
}


static int
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

void
CurlHelper::setUpFileDownload(CURL * curl, FileDownloader::DownloadCallback * func, uint32_t maxBufferSize, const fs::path& path )
{
	curlData[curl]->downloadInfo = new CurlFileDownloadInfo(func, maxBufferSize, path);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &curlFileDownloadCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, curlData[curl]);
}

void
CurlHelper::handleFileDownloadResult(CURL * curl)
{
	curlData[curl]->downloadInfo->callback();
}

int
CurlHelper::emptyCurlFileDownloadCallback(void * const buffer, size_t size, size_t nrMem, void * func)
{
	return size * nrMem;
}

void
CurlHelper::addCookiesToCurl(const HttpFileDownloader::CookieBuket& cookies, CURL * curl)
{
	if(!cookies.empty())
	{
		std::ostringstream cookieLineBuilder;
		for (const HttpFileDownloader::Cookie cookie : cookies)
		{
			cookieLineBuilder << cookie.first << '=' << cookie.second << ";";
		}
		std::string cookieLine = cookieLineBuilder.str();
		curl_easy_setopt(curl, CURLOPT_COOKIE, cookieLine.c_str());
	}
}

HttpFileDownloader::CookieBuket
CurlHelper::getCookies(CURL * curl)
{
	struct curl_slist * list;
	HttpFileDownloader::CookieBuket result;
	curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &list);

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

void
CurlHelper::addFormToCurl(const HttpFileDownloader::Form& form, CURL * curl)
{
	if(!form.empty())
	{
		std::ostringstream cookieLineBuilder;
		for (const HttpFileDownloader::FormField formField : form)
		{
			cookieLineBuilder << formField.first << '=' << formField.second << '&';
		}
		curlData[curl]->postData = cookieLineBuilder.str();
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, curlData[curl]->postData.c_str());
	}
}

CURL *
CurlHelper::createCURL(const char * const url)
{
	CURL * curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_USERAGENT, userAgent.c_str());
	curlData[curl] = new CURLPrivateData();
	return curl;
}

void
CurlHelper::deleteCURL(CURL * curl)
{
	delete curlData[curl];
	curlData.erase(curl);
	curl_easy_cleanup(curl);
}

GAMEKEEPER_NAMESPACE_END(core)
