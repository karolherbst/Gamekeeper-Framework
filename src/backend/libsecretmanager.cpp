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

#include <gamekeeper/backend/libsecretmanager.h>
#include <gamekeeper/core/logger.h>
#include <gamekeeper/core/loggerFactory.h>
#include <gamekeeper/core/loggerStream.h>
#include <gamekeeper/utils/stringutils.h>

#include <libsecret/secret.h>

GAMEKEEPER_NAMESPACE_START(backend)

using core::endl;
using core::LogLevel;

#define GPOINTER_CAST(value) const_cast<gpointer>(static_cast<const void *>(value))

static constexpr char GK_TOKEN_SCHEMA_NAME[] = "org.gamekeeper.authmanager.libsecret.token";

static constexpr char GK_TOKEN_GROUP[] = "group";
static constexpr char GK_TOKEN_KEY[] = "key";
static constexpr char GK_TOKEN_EXPIRY[] = "expiry";
static constexpr char GK_TOKEN_APPLICATION[] = "application";
static constexpr char GK_TOKEN_APPLICATION_VALUE[] = "gamekeeper";
static constexpr char GK_TOKEN_PASSWORD_CONTENT_TYPE[] = "text/plain";

static SecretSchema GK_TOKEN_SCHEMA_TEMPLATE
{
	.name = GK_TOKEN_SCHEMA_NAME,
	.flags = SECRET_SCHEMA_NONE,
	.attributes =
	{
		{
			.name = GK_TOKEN_GROUP,
			.type = SECRET_SCHEMA_ATTRIBUTE_STRING
		},
		{
			.name = GK_TOKEN_KEY,
			.type = SECRET_SCHEMA_ATTRIBUTE_STRING
		},
		{
			.name = GK_TOKEN_EXPIRY,
			.type = SECRET_SCHEMA_ATTRIBUTE_INTEGER
		},
		// always gamekeeper
		{
			.name = GK_TOKEN_APPLICATION,
			.type = SECRET_SCHEMA_ATTRIBUTE_STRING
		},
		{
			.name = nullptr
		}
	}
};

struct SchemaAttributesWrapper
{
	void fillSchemaAndAttributes(const AuthManager::Token & token);
	~SchemaAttributesWrapper();
	SecretSchema schema = GK_TOKEN_SCHEMA_TEMPLATE;
	GHashTable * attributes = g_hash_table_new(&g_direct_hash, &g_direct_equal);
private:
	std::string expiry;
};

SchemaAttributesWrapper::~SchemaAttributesWrapper()
{
	g_hash_table_destroy(attributes);
}

void
SchemaAttributesWrapper::fillSchemaAndAttributes(const AuthManager::Token & token)
{
	// save trivial stuff first
	g_hash_table_insert(this->attributes, GPOINTER_CAST(GK_TOKEN_GROUP), GPOINTER_CAST(token.group.c_str()));
	g_hash_table_insert(this->attributes, GPOINTER_CAST(GK_TOKEN_KEY), GPOINTER_CAST(token.key.c_str()));
	this->expiry = utils::String::toString(std::chrono::duration_cast<std::chrono::seconds>(token.expiry.time_since_epoch()).count());
	g_hash_table_insert(this->attributes, GPOINTER_CAST(GK_TOKEN_EXPIRY), GPOINTER_CAST(this->expiry.c_str()));
	g_hash_table_insert(this->attributes, GPOINTER_CAST(GK_TOKEN_APPLICATION), GPOINTER_CAST(GK_TOKEN_APPLICATION_VALUE));

	// we begin at the fourth attribute
	uint8_t i = 4;
	for(const auto & p : token.properties)
	{
		this->schema.attributes[i].name = p.first.c_str();
		this->schema.attributes[i].type = SECRET_SCHEMA_ATTRIBUTE_STRING;
		g_hash_table_insert(this->attributes, GPOINTER_CAST(p.first.c_str()), GPOINTER_CAST(p.second.c_str()));
		i++;
	}
	this->schema.attributes[i].name = nullptr;
}

class LibSecretManager::PImpl
{
public:
	PImpl(core::Logger &);
	std::shared_ptr<SecretService> secretService;
	core::Logger & logger;
};

LibSecretManager::PImpl::PImpl(core::Logger & _logger)
:	secretService(secret_service_get_sync(SECRET_SERVICE_OPEN_SESSION, nullptr, nullptr), g_object_unref),
	logger(_logger){}

LibSecretManager::LibSecretManager(std::shared_ptr<core::LoggerFactory> lf)
:	data(new LibSecretManager::PImpl(lf->getComponentLogger("Auth.libsecret"))){}

LibSecretManager::~LibSecretManager()
{
	secret_service_disconnect();
}

void
LibSecretManager::saveToken(const AuthManager::Token & token)
{
	SchemaAttributesWrapper w;
	w.fillSchemaAndAttributes(token);
	secret_service_store_sync(this->data->secretService.get(),
	                          &w.schema,
	                          w.attributes,
	                          nullptr,
	                          (std::string("gamekeeper token for: ") + token.group).c_str(),
	                          secret_value_new(token.value.c_str(), token.value.length(), GK_TOKEN_PASSWORD_CONTENT_TYPE),
	                          nullptr,
	                          nullptr);
}

void
LibSecretManager::removeToken(const AuthManager::Token & token)
{
	SchemaAttributesWrapper w;
	w.fillSchemaAndAttributes(token);
	secret_service_clear_sync(this->data->secretService.get(), &w.schema, w.attributes, nullptr, nullptr);
}

AuthManager::Tokens
LibSecretManager::readAllTokens(const std::string & group)
{
	AuthManager::Tokens tokens;
	GHashTable * attributes = g_hash_table_new(&g_str_hash, &g_str_equal);

	g_hash_table_insert(attributes, GPOINTER_CAST(GK_TOKEN_APPLICATION), GPOINTER_CAST(GK_TOKEN_APPLICATION_VALUE));
	g_hash_table_insert(attributes, GPOINTER_CAST(GK_TOKEN_GROUP), GPOINTER_CAST(group.c_str()));

	GList * list = secret_service_search_sync(this->data->secretService.get(), nullptr, attributes, static_cast<SecretSearchFlags>(SECRET_SEARCH_ALL | SECRET_SEARCH_UNLOCK | SECRET_SEARCH_LOAD_SECRETS), nullptr, nullptr);
	g_hash_table_destroy(attributes);

	for(GList * it = list; it != nullptr; it = it->next)
	{
		SecretItem * item = static_cast<SecretItem *>(it->data);
		gboolean itemLocked;
		g_object_get(item, "locked", &itemLocked, nullptr);
		if(itemLocked == TRUE)
		{
			this->data->logger << LogLevel::Error << "couldn't unlock keys for group \"" << group << "\". aborting" << endl;
			return tokens;
		}

		SecretValue * value = secret_item_get_secret(item);
		GHashTable * atts = secret_item_get_attributes(item);
		gsize * secretSize;

		AuthManager::Token token(static_cast<gchar *>(g_hash_table_lookup(atts, GK_TOKEN_KEY)),
		                         secret_value_get_text(value),
		                         group,
		                         utils::String::toType<AuthManager::Token::TimePoint::rep>(static_cast<gchar *>(g_hash_table_lookup(atts, GK_TOKEN_EXPIRY))));

		g_hash_table_foreach(atts, [](gpointer keyPtr, gpointer valuePtr, gpointer tokenPtr)
		{
			gchar * key = static_cast<gchar *>(keyPtr);
			gchar * value = static_cast<gchar *>(valuePtr);
			AuthManager::Token * token = static_cast<AuthManager::Token *>(tokenPtr);

			// key and expiry are a special cases
			if(std::string(GK_TOKEN_KEY) != key && std::string(GK_TOKEN_EXPIRY) != key)
			{
				token->properties.insert(std::make_pair(key, value));
			}
		}, &token);

		g_hash_table_unref(atts);
		secret_value_unref(value);
		
		// after we parse everything check expiry now
		if(std::chrono::system_clock::now() >= token.expiry)
		{
			this->data->logger << LogLevel::Info << "deleted token in group \"" << group << "\"" << endl;
			this->removeToken(token);
		}
		else
		{
			tokens.push_back(std::move(token));
		}
	}

	g_list_free(list);

	return tokens;
}

GAMEKEEPER_NAMESPACE_END(backend)
