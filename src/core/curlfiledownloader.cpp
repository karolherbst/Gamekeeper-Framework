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

#include <gamekeeper/core/curlfiledownloader.h>

#include <vector>

#include <std_compat/thread>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>

#include <curl/curl.h>

#include <gamekeeper/core/logger.h>
#include <gamekeeper/core/loggerStream.h>
#include <gamekeeper/utils/stringutils.h>

namespace balgo = boost::algorithm;
namespace bfs = boost::filesystem;
namespace bio = boost::iostreams;

GAMEKEEPER_NAMESPACE_START(core)

class PRIVATE_API CurlFileDownloadInfo
{
public:
	PRIVATE_API CurlFileDownloadInfo(const FileDownloader::DownloadCallback &, uint64_t maxBufferSize, const bfs::path & cacheFilePath);
	PRIVATE_API uint64_t bytesDownloaded();
	PRIVATE_API bool addData(void * const buffer, uint64_t bytes);
	PRIVATE_API void callback();

private:
	const uint64_t maxBufferSize;
	const FileDownloader::DownloadCallback & func;
	const bfs::path path;
	std::vector<gkbyte_t> dataBuffer;
	uint64_t _bytesDownloaded = 0;
};

CurlFileDownloadInfo::CurlFileDownloadInfo(const FileDownloader::DownloadCallback & _func, uint64_t mbs, const bfs::path & cacheFilePath)
:	maxBufferSize(mbs),
	func(_func),
	path(cacheFilePath){}

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
			if(!bfs::exists(this->path.parent_path()))
			{
				bfs::create_directories(this->path.parent_path());
			}
			bfs::basic_ofstream<gkbyte_t> ofs(this->path, std::ios_base::trunc | std::ios_base::binary);
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
		bfs::basic_ofstream<gkbyte_t> ofs(this->path, std::ios_base::app | std::ios_base::binary);
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
		bio::stream<bio::basic_array_source<gkbyte_t>> stream(this->dataBuffer.data(), this->dataBuffer.size());
		stream.peek();
		this->func(stream);
	}
	else
	{
		bfs::basic_ifstream<gkbyte_t> ifs(this->path);
		this->func(ifs);
		ifs.close();
		bfs::remove(this->path);
	}
}

class CurlFileDownloader::PImpl
{
public:
	PImpl(Logger &, const std::string & userAgent, const bfs::path & cacheDir, uint16_t connectionTimeout, uint16_t retryPause,
	      uint16_t maxResolveRetries, uint16_t maxConnectRetries, uint32_t maxBufferSize);
	~PImpl();

	void performCurl(uint16_t timeout = 0, uint16_t resolveFailed = 0, uint16_t connectFailed = 0);
	template <typename T>
	void setOpt(CURLoption option, const T &);
	void handleCurlError(CURLcode code);
	boost::filesystem::path resolveDownloadPath(const std::string & url);

	CURL * handle;
	Logger & logger;

	bfs::path cacheDir;
	uint32_t retryPause;
	uint16_t maxResolveRetries;
	uint16_t maxConnectRetries;
	uint32_t maxBufferSize;
};

static int
curlDebugCallback(CURL *, curl_infotype type, char * data, size_t length, Logger * loggerPtr)
{
	if(type == CURLINFO_TEXT || type == CURLINFO_HEADER_IN || type == CURLINFO_HEADER_OUT)
	{
		Logger & logger = *loggerPtr;
		std::string line(data, length - 1);
		logger << LogLevel::Debug << line << endl;
	}
	return 0;
}

CurlFileDownloader::PImpl::PImpl(Logger & _logger, const std::string & userAgent, const bfs::path & _cacheDir, uint16_t connectionTimeout, uint16_t _retryPause, uint16_t mrr, uint16_t mcr, uint32_t mbs)
:	handle(curl_easy_init()),
	logger(_logger),
	cacheDir(_cacheDir),
	retryPause(_retryPause),
	maxResolveRetries(mrr),
	maxConnectRetries(mcr),
	maxBufferSize(mbs)
{
	setOpt(CURLOPT_USERAGENT, userAgent.c_str());
	setOpt(CURLOPT_FOLLOWLOCATION, 1);
	// needed for multithreading
	setOpt(CURLOPT_NOSIGNAL, 1);
	setOpt(CURLOPT_CONNECTTIMEOUT_MS, connectionTimeout);
	// always enable cookie engine
	setOpt(CURLOPT_COOKIEJAR, nullptr);
	if(this->logger.isEnabled(LogLevel::Debug))
	{
		setOpt(CURLOPT_VERBOSE, 1);
		setOpt(CURLOPT_DEBUGDATA, &this->logger);
		setOpt(CURLOPT_DEBUGFUNCTION, &curlDebugCallback);
	}
	this->logger << LogLevel::Debug << "new curl handle with user-agent: " << userAgent << endl;
}

CurlFileDownloader::PImpl::~PImpl()
{
	curl_easy_cleanup(this->handle);
}

void
CurlFileDownloader::PImpl::performCurl(uint16_t timeout, uint16_t resolveFailed, uint16_t connectFailed)
{
	if(timeout > 0)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(timeout));
	}

	CURLcode code = curl_easy_perform(this->handle);
	switch(code)
	{
		case CURLE_OK:
			// everything okay, but we should also log the http return code
			long returnCode;
			curl_easy_getinfo(this->handle, CURLINFO_RESPONSE_CODE, &returnCode);
			if(returnCode == 200)
			{
				this->logger << LogLevel::Trace << "CURL returned with response code: " << returnCode << endl;
			}
			else
			{
				this->logger << LogLevel::Error << "CURL returned with response code: " << returnCode << endl;
				throw FileDownloaderException(std::string("HTTPS response code ") + utils::String::toString(returnCode));
			}
			break;
		case CURLE_COULDNT_RESOLVE_HOST: // 6
			if(resolveFailed < this->maxResolveRetries)
			{
				this->logger << LogLevel::Warn << "CURL couldn't resolve host, retrying" << endl;
				this->performCurl(this->retryPause, ++resolveFailed, connectFailed);
			}
			else
			{
				std::string message("CURL couldn't resolve host after ");
				message += utils::String::toString(resolveFailed);
				message += " retries";
				this->logger << LogLevel::Error << message << endl;
				throw FileDownloaderException(message);
			}
			break;
		case CURLE_COULDNT_CONNECT: // 7
			if(connectFailed < this->maxConnectRetries)
			{
				this->logger << LogLevel::Warn << "CURL couldn't connect to host, retrying" << endl;
				this->performCurl(this->retryPause, resolveFailed, ++connectFailed);
			}
			else
			{
				std::string message("CURL couldn't connect to host after ");
				message += utils::String::toString(connectFailed);
				message += " retries";
				this->logger << LogLevel::Error << message << endl;
				throw FileDownloaderException(message);
			}
			break;
		default:
			// unhandled error
			handleCurlError(code);
			break;
	}
}

template <typename T>
void
CurlFileDownloader::PImpl::setOpt(CURLoption option, const T & value)
{
	handleCurlError(curl_easy_setopt(this->handle, option, value));
}

void
CurlFileDownloader::PImpl::handleCurlError(CURLcode code)
{
	switch(code)
	{
		case CURLE_OK:
			// everything okay
			break;
		default:
			// unhandled error
			this->logger << LogLevel::Fatal << "CURL error \"" << curl_easy_strerror(code) << "\" (" << code << ") unhandled, please report a bug" << endl;
			throw FileDownloaderException(std::string("unhandled CURL error") + curl_easy_strerror(code));
			break;
	}
}

boost::filesystem::path
CurlFileDownloader::PImpl::resolveDownloadPath(const std::string & url)
{
	std::string uri = url;
	// frist cut the protocoll
	size_t pos = uri.find("://");
	uri.erase(0, pos + 3);
	balgo::replace_all(uri, ":", "_");
	return this->cacheDir / uri;
}

CurlFileDownloader::CurlFileDownloader(Logger & logger, const std::string & userAgent, const bfs::path & cacheDir, uint16_t connectionTimeout,
                                       uint16_t retryPause, uint16_t maxResolveRetries, uint16_t maxConnectRetries, uint32_t maxBufferSize)
:	data(new PImpl(logger, userAgent, cacheDir, connectionTimeout, retryPause, maxResolveRetries, maxConnectRetries, maxBufferSize)){}

CurlFileDownloader::~CurlFileDownloader(){}

static uint64_t
emptyCurlFileDownloadCallback(void * const, size_t size, size_t nrMem, void *)
{
	return size * nrMem;
}

static uint64_t
curlFileDownloadCallback(void * const buffer, size_t size, size_t nrMem, CurlFileDownloadInfo * cfdi)
{
	uint64_t sizeInBytes = size * nrMem;
	if(!cfdi->addData(buffer, sizeInBytes))
	{
		return -1;
	}
	return sizeInBytes;
}

void
CurlFileDownloader::postRequest(const std::string & url, const Form & form)
{
	this->data->logger << LogLevel::Debug << "post request to: " << url << endl;

	if(!form.empty())
	{
		std::ostringstream formLineBuilder;
		for(const FileDownloader::FormField & formField : form)
		{
			formLineBuilder << formField.first << '=' << formField.second << '&';
		}
		this->data->setOpt(CURLOPT_POSTFIELDSIZE, -1L);
		this->data->setOpt(CURLOPT_COPYPOSTFIELDS, formLineBuilder.str().c_str());
	}
	else
	{
		this->data->setOpt(CURLOPT_POSTFIELDSIZE, 0);
	}

	this->data->setOpt(CURLOPT_URL, url.c_str());
	this->data->setOpt(CURLOPT_POST, 1);
	// we don't need the body of the post result (for now)
	this->data->setOpt(CURLOPT_WRITEFUNCTION, &emptyCurlFileDownloadCallback);
	this->data->setOpt(CURLOPT_WRITEDATA, nullptr);
	this->data->performCurl();
	// clear unneded stuff
}

void
CurlFileDownloader::getRequest(const std::string & url, const FileDownloader::DownloadCallback & callback)
{
	bfs::path cacheFilePath = this->data->resolveDownloadPath(url);
	this->data->logger << LogLevel::Debug << "try to download file from: " << url << " at: " << cacheFilePath.string() << endl;
	this->data->setOpt(CURLOPT_URL, url.c_str());
	this->data->setOpt(CURLOPT_HTTPGET, 1);

	CurlFileDownloadInfo cfdi(callback, this->data->maxBufferSize, cacheFilePath);
	this->data->setOpt(CURLOPT_WRITEFUNCTION, &curlFileDownloadCallback);
	this->data->setOpt(CURLOPT_WRITEDATA, &cfdi);
	this->data->performCurl();
	cfdi.callback();
}

static const std::string COOKIE_TRUE{"TRUE"};
static const std::string COOKIE_FALSE{"FALSE"};
typedef std::vector<std::string>::size_type vector_size_type;

constexpr vector_size_type COOKIE_DOMAIN_IDX{0};
constexpr vector_size_type COOKIE_ENTIREDOMAIN_IDX{1};
constexpr vector_size_type COOKIE_PATH_IDX{2};
constexpr vector_size_type COOKIE_SECURE_IDX{3};
constexpr vector_size_type COOKIE_EXPIRY_IDX{4};
constexpr vector_size_type COOKIE_NAME_IDX{5};
constexpr vector_size_type COOKIE_VALUE_IDX{6};

static bool
isSessionCookie(const FileDownloader::Cookie & c)
{
	// check against epoch
	return c.expiry == FileDownloader::Cookie::TimePoint();
}

static bool
isCookieExpiredAndNotSession(const FileDownloader::Cookie & c)
{
	return std::chrono::system_clock::now() >= c.expiry && !isSessionCookie(c);
}

void
CurlFileDownloader::addCookie(const Cookie & c)
{
	if(isCookieExpiredAndNotSession(c))
	{
		this->data->logger << LogLevel::Debug << "outdated cookie not saved!" << endl;
		return;
	}
	std::ostringstream cookieLineBuilder;
	cookieLineBuilder << c.domain << "\tTRUE\t" << c.path << '\t' << (c.secure ? COOKIE_TRUE : COOKIE_FALSE)
	                  << '\t' << utils::String::toString(std::chrono::duration_cast<std::chrono::seconds>(c.expiry.time_since_epoch()).count())
	                  << '\t' << c.name << '\t' << c.value;
	std::string genLine = cookieLineBuilder.str();
	this->data->logger << LogLevel::Debug << "saving cookie line: " << genLine << endl;
	this->data->setOpt(CURLOPT_COOKIELIST, genLine.c_str());
}

void
CurlFileDownloader::addCookies(const CookieBucket & cookies)
{
	for(const Cookie & c : cookies)
	{
		this->addCookie(c);
	}
}

void
CurlFileDownloader::clearCookies()
{
	this->data->setOpt(CURLOPT_COOKIELIST, "ALL");
}

void
CurlFileDownloader::setCookies(const CookieBucket & cookies)
{
	this->clearCookies();
	this->addCookies(cookies);
}

// libcurl is doing some strange stuff under the hood. Make the data reasonable
static std::string &
fixCookieDomain(std::string & domain)
{
	// if the domain is set to TRUE we actually have localhost
	if(domain == COOKIE_TRUE)
	{
		return (domain = "localhost");
	}
	// ignore httpOnly for now
	if(balgo::starts_with(domain, "#HttpOnly_"))
	{
		// reduce length of string by 1 instead of doing -1
		domain.erase(0, sizeof("#HttpOnly"));
	}
	return domain;
}

FileDownloader::CookieBucket
CurlFileDownloader::getCookies()
{
	struct curl_slist * list;
	FileDownloader::CookieBucket result;
	this->data->handleCurlError(curl_easy_getinfo(this->data->handle, CURLINFO_COOKIELIST, &list));

	for(; list != nullptr; list = list->next)
	{
		std::vector<std::string> strings;
		this->data->logger << LogLevel::Debug << "parse Cookie: " << list->data << endl;
		balgo::split(strings, list->data, balgo::is_any_of("\t"));
		Cookie::TimePoint::rep cookieTimeRep(utils::String::toType<Cookie::TimePoint::rep>(strings[COOKIE_EXPIRY_IDX]));
		Cookie::TimePoint cookieTime = Cookie::TimePoint(std::chrono::seconds(cookieTimeRep));
		if(cookieTimeRep != 0 && std::chrono::system_clock::now() > cookieTime)
		{
			this->data->logger << LogLevel::Debug << "Cookie ignored, because it is expired" << endl;
			continue;
		}
		result.push_back(
		{
			strings[COOKIE_NAME_IDX],
			strings[COOKIE_VALUE_IDX],
			std::move(fixCookieDomain(strings[COOKIE_DOMAIN_IDX])),
			strings[COOKIE_PATH_IDX],
			cookieTime,
			(strings[COOKIE_SECURE_IDX] == COOKIE_TRUE)
		});
	}

	curl_slist_free_all(list);
	return result;
}

GAMEKEEPER_NAMESPACE_END(core)
