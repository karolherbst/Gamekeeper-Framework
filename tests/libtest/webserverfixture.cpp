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

#include "webserverfixture.h"

#include <forward_list>
#include <future>

#include <gamekeeper/core/exception.h>
#include <gamekeeper/core/threadfactory.h>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/lexical_cast.hpp>

#include <Hypodermic/IContainer.h>

#include <mongoose.h>

GAMEKEEPER_NAMESPACE_START(test)

namespace algo = boost::algorithm;
namespace fs = boost::filesystem;
using boost::lexical_cast;
using gamekeeper::core::ThreadFactory;

static const fs::path fileServerRoot = WEBSERVERPATH;

WebServerFictureAspect::WebServerFictureAspect(std::shared_ptr<Hypodermic::IContainer> container)
{
	this->server = mg_create_server(nullptr, [](mg_connection * conn, mg_event event) -> int
	{
		if(event == MG_REQUEST)
		{
			std::vector<std::string> splittedPaths;
			std::string uri = std::string(&conn->uri[1]);
			algo::split(splittedPaths, uri, algo::is_any_of("/"), algo::token_compress_on);

			if(splittedPaths[0] == "files")
			{
				splittedPaths.erase(splittedPaths.begin());
				fs::path fileResource = fileServerRoot;
				for(std::string path : splittedPaths)
				{
					fileResource /= path;
				}
				if(fs::exists(fileResource))
				{
					fs::ifstream fs(fileResource);
					mg_send_status(conn, 200);

					if(fs.peek() != std::ifstream::traits_type::eof())
					{
						char buffer[8192];
						while(fs)
						{
							fs.read(buffer, 8192);
							mg_send_data(conn, buffer, fs.gcount());
						}
					}
					mg_send_data(conn, nullptr, 0);
					return MG_TRUE;
				}
				mg_send_status(conn, 404);
				return MG_TRUE;
			}
			else if(splittedPaths[0] == "bigfile")
			{
				if(splittedPaths.size() < 2)
				{
					mg_send_status(conn, 404);
					return MG_TRUE;
				}
				uint64_t size = lexical_cast<uint64_t>(splittedPaths[1]);
				mg_send_status(conn, 200);
				uint64_t chunks = (size / 1024);
				char buffer[1024] = {0};
				for(uint64_t i = 0; i < chunks; i++)
				{
					mg_send_data(conn, buffer, 1024);
				}
				uint16_t rest = size - chunks * 1024;
				if(rest != 0)
				{
					mg_send_data(conn, buffer, rest);
				}
				mg_send_data(conn, nullptr, 0);
				return MG_TRUE;
			}
			else if(splittedPaths[0] == "cookies")
			{
				if(splittedPaths.size() < 3)
				{
					mg_send_status(conn, 404);
					return MG_TRUE;
				}
				mg_send_status(conn, 200);
				mg_send_header(conn, "Set-Cookie", (splittedPaths[1] + '=' + splittedPaths[2]).c_str());
				mg_send_data(conn, "", 1);
				return MG_TRUE;
			}
			return MG_TRUE;
		}
		else if(event == MG_POLL)
		{
			return MG_FALSE;
		}
		return MG_TRUE;
	});
	const char * msg = mg_set_option(this->server, "listening_port", "8080");
	if(msg != nullptr)
	{
		throw core::GameKeeperMessageException(std::string("couldn't setup embedded webserver: ") + msg);
	}
	container->resolve<ThreadFactory>()->createThread("mongoose server thread",
		[this](ThreadFactory::ThreadFunction::argument_type interrupted) -> void
	{
		mg_server * copyServer = this->server;
		while(!interrupted)
		{
			mg_poll_server(copyServer, 1000);
		}
		mg_destroy_server(&this->server);
	});
}

GAMEKEEPER_NAMESPACE_END(test)
