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

#include <gamekeeper/backend/httppostloginhandler.h>

#include <algorithm>
#include <vector>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/split.hpp>

#include <gamekeeper/core/filedownloader.h>

namespace balgo = boost::algorithm;

GAMEKEEPER_NAMESPACE_START(backend)

class HTTPPostLoginHandler::PImpl
{
public:
	PImpl(std::map<std::string, std::string> &, std::shared_ptr<core::FileDownloader>);

	std::shared_ptr<core::FileDownloader> hfd;
	std::string loginUrl;
	std::string logoutUrl;
	std::string usernameField;
	std::string passwordField;
	std::string username;
	std::vector<std::string> cookieWhitelist;

	bool checkAuthCookies();
};

HTTPPostLoginHandler::PImpl::PImpl(std::map<std::string, std::string> & config, std::shared_ptr<core::FileDownloader> _hfd)
:	hfd(_hfd),
	loginUrl(config.at("auth.loginurl")),
	logoutUrl(config.at("auth.logouturl")),
	usernameField(config.at("authfield.username")),
	passwordField(config.at("authfield.password"))
{
	auto it = config.find("authtoken.keys");
	if(it != config.end())
	{
		balgo::split(this->cookieWhitelist, (*it).second, balgo::is_any_of(", "), balgo::token_compress_on);
	}
}

bool
HTTPPostLoginHandler::PImpl::checkAuthCookies()
{
	auto cookies = this->hfd->getCookies();
	// if no whitelist, fallback to empty check
	if(this->cookieWhitelist.empty())
	{
		return !cookies.empty();
	}

	// first get all names
	std::vector<std::string> names;
	for(const core::FileDownloader::Cookie & c : cookies)
	{
		names.push_back(c.name);
	}

	// now check the whitelist
	for(const std::string & w : this->cookieWhitelist)
	{
		if(std::find(names.begin(), names.end(), w) == names.end())
		{
			return false;
		}
	}
	return !cookies.empty();
}


HTTPPostLoginHandler::HTTPPostLoginHandler(std::map<std::string, std::string> & config, std::shared_ptr<core::FileDownloader> hfd)
:	data(new HTTPPostLoginHandler::PImpl(config, hfd)){}

HTTPPostLoginHandler::~HTTPPostLoginHandler(){}

bool
HTTPPostLoginHandler::login(const std::string & username, const std::string & password)
{
	// save username
	this->data->username = username;

	core::FileDownloader::Form form
	{
		{this->data->usernameField, username},
		{this->data->passwordField, password}
	};

	try
	{
		this->data->hfd->postRequest(this->data->loginUrl, form);
	}
	catch(const core::FileDownloader::FileDownloaderException & e)
	{
		return false;
	}
	return this->data->checkAuthCookies();
}

void
HTTPPostLoginHandler::logout()
{
	this->data->hfd->postRequest(this->data->logoutUrl);
}

void
HTTPPostLoginHandler::downloadFile(const std::string & url, core::FileDownloader::DownloadCallback callback)
{
	std::string parsedString = url;
	balgo::replace_all(parsedString, "${authfield.username}", this->data->username);
	this->data->hfd->getRequest(parsedString, callback);
}

GAMEKEEPER_NAMESPACE_END(backend)
