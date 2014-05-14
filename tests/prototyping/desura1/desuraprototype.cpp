#include "desuraprototype.h"

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>

#include <gamekeeper/client/autowire.h>
#include <gamekeeper/core/httpfiledownloader.h>
#include <gamekeeper/utils/stringutils.h>

GAMECLIENTUI_CLASS(DesuraPrototype)

namespace client = gamekeeper::client;
namespace core = gamekeeper::core;
namespace po = boost::program_options;
namespace utils = gamekeeper::utils;

GAMECLIENT_ADD_OPTIONS({
	cmd("desura.accountname", po::value<std::string>()->required(), "Desura Account name");
	cmd("desura.password", po::value<std::string>()->required(), "Desura Account password");
})

DesuraPrototype::DesuraPrototype(gamekeeper::core::Logger& _logger)
:	logger(_logger){}

void
DesuraPrototype::init(const ConfigMap & cm)
{
	this->username = cm.at("desura.accountname").as<std::string>().c_str();
	this->userpass = cm.at("desura.password").as<std::string>().c_str();
}

void
DesuraPrototype::onShutdown()
{
}

void
DesuraPrototype::startEventLoop()
{
	std::shared_ptr<core::HttpFileDownloader> hfd = client::Autowire<core::HttpFileDownloader>();
	core::HttpFileDownloader::Form form;
	form["username"] = this->username;
	form["password"] = this->userpass;
	core::HttpFileDownloader::CookieBuket cookies = hfd->downloadFileAndCookiesWithForm("http://api.desura.com/3/memberlogin",
	[](std::basic_istream<gkbyte_t> & is) -> bool
	{
		std::cout << utils::String::createFromStream(is) << std::endl;
		return true;
	}, form);
	std::cout << "freeman: " << cookies["freeman"] << std::endl;
	std::cout << "masterchief: " << cookies["masterchief"] << std::endl;
}
