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

#include <gamekeeper/backend/oauthloginhandler.h>

#include <sstream>
#include <vector>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#include <gamekeeper/backend/httppostloginhandler.h>
#include <gamekeeper/core/filedownloader.h>

namespace balgo = boost::algorithm;

GAMEKEEPER_NAMESPACE_START(backend)

class OAuthLoginHandler::PImpl
{
public:
	PImpl(std::map<std::string, std::string> &, std::shared_ptr<core::FileDownloader>, std::shared_ptr<AuthManager>);

	std::shared_ptr<core::FileDownloader> fd;
	std::shared_ptr<AuthManager> am;
	std::string tokenGroup;
	std::vector<std::string> providers;
	std::string clientId;
	std::string redirectUri;
	std::string scope;
};

OAuthLoginHandler::PImpl::PImpl(std::map<std::string, std::string> & config, std::shared_ptr<core::FileDownloader> _fd,
                                std::shared_ptr<AuthManager> _am)
:	fd(_fd),
	am(_am),
	tokenGroup(config.at("store.name")),
	clientId(config.at("auth_oauth.client_id")),
	redirectUri(config.at("auth_oauth.redirect_uri")),
	scope(config.at("auth_oauth.scope"))
{
	balgo::split(this->providers, config.at("auth_oauth.providers"), boost::is_any_of(", "), balgo::token_compress_on);
}

OAuthLoginHandler::OAuthLoginHandler(std::map<std::string, std::string> & config, std::shared_ptr<core::FileDownloader> fd,
                                     std::shared_ptr<AuthManager> am)
:	data(new OAuthLoginHandler::PImpl(config, fd, am)){}

OAuthLoginHandler::~OAuthLoginHandler(){}

bool
OAuthLoginHandler::login(const std::string & username, const std::string & password, const StringMap & fields)
{
	// login into google first
	// but we need the GALX cookie first
	this->data->fd->postRequest("https://accounts.google.com/ServiceLoginAuth");
	auto galx = this->data->fd->getCookies();

	std::map<std::string,std::string> configMap
	{
		{"store.name", this->data->tokenGroup},
		{"auth_http_post.loginurl", "https://accounts.google.com/ServiceLoginAuth"},
		{"auth_http_post.logouturl", "https://www.google.de/accounts/Logout2"},
		{"auth_http_post.username", "Email"},
		{"auth_http_post.password", "Passwd"}
	};
	StringMap galxFields;
	for(const auto & cookie : galx)
	{
		galxFields[cookie.name] = cookie.value;
	}

	HTTPPostLoginHandler hplh(configMap, this->data->fd, this->data->am);
//	if(!hplh.isLoggedIn())
//	{
		hplh.login(username, password, galxFields);
//	}

	core::FileDownloader::Form form
	({
		{"response_type", "token"},
		{"client_id", this->data->clientId},
		{"redirect_uri", this->data->redirectUri},
		{"scope", this->data->scope}
	});
	this->data->fd->postRequest("https://accounts.google.com/o/oauth2/auth", form);

	// logout from provider, because we don't need it anymore
	hplh.logout();

	return false;
}

void
OAuthLoginHandler::logout()
{
}

bool
OAuthLoginHandler::isLoggedIn() const
{
	return false;
}

void
OAuthLoginHandler::downloadFile(const std::string & url, core::FileDownloader::DownloadCallback callback)
{
}

GAMEKEEPER_NAMESPACE_END(backend)
