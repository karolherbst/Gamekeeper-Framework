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

#include <unordered_map>

#include <boost/algorithm/string/replace.hpp>

#include <gamekeeper/core/filedownloader.h>

namespace balgo = boost::algorithm;

GAMEKEEPER_NAMESPACE_START(backend)

typedef std::unordered_map<std::string, std::string> Tokens;

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
	Tokens tokens;
};

HTTPPostLoginHandler::PImpl::PImpl(std::map<std::string, std::string> & config, std::shared_ptr<core::FileDownloader> _hfd)
:	hfd(_hfd),
	loginUrl(config.at("auth.loginurl")),
	logoutUrl(config.at("auth.logouturl")),
	usernameField(config.at("authfield.username")),
	passwordField(config.at("authfield.password")){}

HTTPPostLoginHandler::HTTPPostLoginHandler(std::map<std::string, std::string> & config, std::shared_ptr<core::FileDownloader> hfd)
:	data(new HTTPPostLoginHandler::PImpl(config, hfd)){}

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
	this->data->tokens = this->data->hfd->doPostRequestForCookies(this->data->loginUrl.c_str(), form);
	return !this->data->tokens.empty();
}

void
HTTPPostLoginHandler::logout()
{
	this->data->hfd->downloadFileWithCookies(this->data->logoutUrl.c_str(),
	[](std::basic_istream<gkbyte_t> & is) -> bool
	{
		return true;
	}, this->data->tokens);
	this->data->tokens.clear();
}

void
HTTPPostLoginHandler::downloadFile(const std::string & url, core::FileDownloader::DownloadCallback callback)
{
	std::string parsedString = url;
	balgo::replace_all(parsedString, "${authfield.username}", this->data->username);
	this->data->hfd->downloadFileWithCookies(parsedString.c_str(), callback, this->data->tokens);
}

GAMEKEEPER_NAMESPACE_END(backend)
