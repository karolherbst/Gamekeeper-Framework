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

#include <gamekeeper/backend/httppostloginhandler.h>

#include <boost/algorithm/string/replace.hpp>

#include <gamekeeper/backend/authmanager.h>
#include <gamekeeper/core/filedownloader.h>

namespace balgo = boost::algorithm;

GAMEKEEPER_NAMESPACE_START(backend)

class HTTPPostLoginHandler::PImpl
{
public:
	PImpl(std::map<std::string, std::string> &, std::shared_ptr<core::FileDownloader>, std::shared_ptr<AuthManager>);

	std::shared_ptr<core::FileDownloader> hfd;
	std::shared_ptr<AuthManager> am;
	std::string loginUrl;
	std::string logoutUrl;
	std::string usernameField;
	std::string passwordField;
	std::string tokenGroup;
	std::string username;
};

HTTPPostLoginHandler::PImpl::PImpl(std::map<std::string, std::string> & config, std::shared_ptr<core::FileDownloader> _hfd,
                                   std::shared_ptr<AuthManager> _am)
:	hfd(_hfd),
	am(_am),
	loginUrl(config.at("auth.loginurl")),
	logoutUrl(config.at("auth.logouturl")),
	usernameField(config.at("authfield.username")),
	passwordField(config.at("authfield.password")),
	tokenGroup(config.at("store.name"))
{
	this->hfd->setCookies(this->am->readAllTokens(this->tokenGroup));
}

HTTPPostLoginHandler::HTTPPostLoginHandler(std::map<std::string, std::string> & config, std::shared_ptr<core::FileDownloader> hfd,
                                           std::shared_ptr<AuthManager> am)
:	data(new HTTPPostLoginHandler::PImpl(config, hfd, am)){}

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
	this->data->hfd->postRequest(this->data->loginUrl, form);
	core::FileDownloader::CookieBucket cs = this->data->hfd->getCookies();

	// save tokens
	for(const auto & t : cs)
	{
		this->data->am->saveToken(t.first, t.second, this->data->tokenGroup);
	}

	return !cs.empty();
}

void
HTTPPostLoginHandler::logout()
{
	this->data->hfd->postRequest(this->data->logoutUrl);
	// forciblie clear cookies
	this->data->hfd->clearCookies();
}

bool
HTTPPostLoginHandler::isLoggedIn() const
{
	return !this->data->hfd->getCookies().empty();
}

void
HTTPPostLoginHandler::downloadFile(const std::string & url, core::FileDownloader::DownloadCallback callback)
{
	std::string parsedString = url;
	balgo::replace_all(parsedString, "${authfield.username}", this->data->username);
	this->data->hfd->getRequest(parsedString, callback);
}

GAMEKEEPER_NAMESPACE_END(backend)
