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

#include <gamekeeper/backend/security/authmanager.h>
#include <gamekeeper/backend/security/generictoken.h>
#include <gamekeeper/backend/security/token.h>
#include <gamekeeper/core/network/cookie.h>
#include <gamekeeper/core/network/filedownloader.h>
#include <gamekeeper/utils/stringutils.h>

namespace balgo = boost::algorithm;

GAMEKEEPER_NAMESPACE_START(backend)

static const std::string COOKIE_DOMAIN{"domain"};
static const std::string COOKIE_PATH{"path"};
static const std::string COOKIE_EXPIRY{"expiry"};
static const std::string COOKIE_SECURE{"secure"};

class HTTPPostLoginHandler::PImpl
{
public:
	PImpl(std::map<std::string, std::string> &, std::shared_ptr<core::network::FileDownloader>, std::shared_ptr<security::AuthManager>);

	std::shared_ptr<core::network::FileDownloader> hfd;
	std::shared_ptr<security::AuthManager> am;
	std::string loginUrl;
	std::string logoutUrl;
	std::string usernameField;
	std::string passwordField;
	std::string tokenGroup;
	std::string username;
	std::vector<std::string> cookieWhitelist;

	bool checkCookiesValidForAuth();
};

HTTPPostLoginHandler::PImpl::PImpl(std::map<std::string, std::string> & config, std::shared_ptr<core::network::FileDownloader> _hfd,
                                   std::shared_ptr<security::AuthManager> _am)
:	hfd(std::move(_hfd)),
	am(std::move(_am)),
	loginUrl(config["auth.loginurl"]),
	logoutUrl(config["auth.logouturl"]),
	usernameField(config["authfield.username"]),
	passwordField(config["authfield.password"]),
	tokenGroup(config["store.name"])
{
	auto it = config.find("authtoken.keys");
	if(it != config.end())
	{
		balgo::split(this->cookieWhitelist, (*it).second, balgo::is_any_of(", "), balgo::token_compress_on);
	}

	// we might not get an authmanager
	if(this->am)
	{
		core::network::FileDownloader::CookieBucket cookies;
		for(const auto & t : this->am->readAllTokens(this->tokenGroup))
		{
			cookies.push_back({t->getKey().at("name"), t->getValue(), t->getKey().at("domain"), t->getKey().at("path"), t->getExpiry(),
			                   t->getKey().at("secure") == GK_TRUE_STRING ? true : false});
		}
		this->hfd->setCookies(cookies);
	}
}

bool
HTTPPostLoginHandler::PImpl::checkCookiesValidForAuth()
{
	auto cookies = this->hfd->getCookies();
	// if no whitelist, fallback to empty check
	if(this->cookieWhitelist.empty())
	{
		return !cookies.empty();
	}

	// first get all names
	std::vector<std::string> names;
	for(const core::network::Cookie & c : cookies)
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

HTTPPostLoginHandler::HTTPPostLoginHandler(std::map<std::string, std::string> & config, std::shared_ptr<core::network::FileDownloader> hfd,
                                           std::shared_ptr<security::AuthManager> am)
:	data(std::make_unique<HTTPPostLoginHandler::PImpl>(config, hfd, am)){}

HTTPPostLoginHandler::~HTTPPostLoginHandler() = default;

bool
HTTPPostLoginHandler::login(const std::string & username, const std::string & password)
{
	// save username
	this->data->username = username;

	core::network::FileDownloader::Form form
	{
		{this->data->usernameField, username},
		{this->data->passwordField, password}
	};

	try
	{
		this->data->hfd->postRequest(this->data->loginUrl, form);
	}
	catch(const core::network::FileDownloader::FileDownloaderException & e)
	{
		return false;
	}

	bool validTokens = this->data->checkCookiesValidForAuth();
	// save tokens, if we have an authmanager
	if(this->data->am && validTokens)
	{
		for(const core::network::Cookie & c : this->data->hfd->getCookies())
		{
			this->data->am->saveToken(security::GenericToken(
				{
					{"name", c.name},
					{"domain", c.domain},
					{"path", c.path},
					{"secure", c.secure ? GK_TRUE_STRING : GK_FALSE_STRING},
				}, c.value, this->data->tokenGroup, c.expiry));
		}
	}

	return validTokens;
}

void
HTTPPostLoginHandler::logout()
{
	// remove stored tokens first, if anything goes wrong after, we are still in a good state
	for(const core::network::Cookie & c : this->data->hfd->getCookies())
	{
		this->data->am->removeToken(security::GenericToken(
			{
				{"name", c.name},
				{"domain", c.domain},
				{"path", c.path},
				{"secure", c.secure ? GK_TRUE_STRING : GK_FALSE_STRING},
			}, c.value, this->data->tokenGroup, c.expiry));
	}
	this->data->hfd->postRequest(this->data->logoutUrl);
	// forcibly clear cookies
	this->data->hfd->clearCookies();
}

bool
HTTPPostLoginHandler::isLoggedIn() const
{
	return this->data->checkCookiesValidForAuth();
}

void
HTTPPostLoginHandler::downloadFile(const std::string & url, core::network::FileDownloader::DownloadCallback callback)
{
	std::string parsedString = url;
	balgo::replace_all(parsedString, "${authfield.username}", this->data->username);
	this->data->hfd->getRequest(parsedString, callback);
}

GAMEKEEPER_NAMESPACE_END(backend)
