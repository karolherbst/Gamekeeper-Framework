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

#include <gamekeeper/backend/gnomekeyringmanager.h>

#include <gnome-keyring.h>

GAMEKEEPER_NAMESPACE_START(backend)

static const GnomeKeyringPasswordSchema GNOME_KEYRING_MANAGER_PASSWORD_SCHEMA =
{
	.item_type = GNOME_KEYRING_ITEM_GENERIC_SECRET,
	.attributes =
	{
		{"key", GNOME_KEYRING_ATTRIBUTE_TYPE_STRING},
		{"group", GNOME_KEYRING_ATTRIBUTE_TYPE_STRING},
		{"application", GNOME_KEYRING_ATTRIBUTE_TYPE_STRING},
		{nullptr, static_cast<GnomeKeyringAttributeType>(0)}
	}
};

void
GnomeKeyringManager::saveToken(const std::string & key, const std::string & value, const std::string & group)
{
	gnome_keyring_store_password_sync(&GNOME_KEYRING_MANAGER_PASSWORD_SCHEMA, nullptr, (std::string("gamekeeper token for: ") + group).c_str(),
	                                  value.c_str(),
	                                  "key", key.c_str(),
	                                  "group", group.c_str(),
	                                  "application", "gamekeeper",
	                                  nullptr);
}

std::string
GnomeKeyringManager::readToken(const std::string & key, const std::string & group)
{
	gchar * password;
	if(gnome_keyring_find_password_sync(&GNOME_KEYRING_MANAGER_PASSWORD_SCHEMA, &password,
	                                    "key", key.c_str(),
	                                    "group", group.c_str(),
	                                    "application", "gamekeeper",
	                                    nullptr) == GNOME_KEYRING_RESULT_OK)
	{
		std::string result = password;
		gnome_keyring_free_password(password);
		return result;
	}
	return "";
}

std::unordered_map<std::string, std::string>
GnomeKeyringManager::readAllTokens(const std::string & group)
{
	std::unordered_map<std::string, std::string> result;
	GList * list;

	if(gnome_keyring_find_itemsv_sync(GNOME_KEYRING_ITEM_GENERIC_SECRET, &list,
	                                  "group", GNOME_KEYRING_ATTRIBUTE_TYPE_STRING, group.c_str(),
	                                  "application", GNOME_KEYRING_ATTRIBUTE_TYPE_STRING, "gamekeeper",
	                                  nullptr) == GNOME_KEYRING_RESULT_OK)
	{
		GnomeKeyringFound * token;
		GnomeKeyringAttribute * attribute;
		for(GList * c = list; c != nullptr; c = g_list_next(c))
		{
			token = static_cast<GnomeKeyringFound *>(c->data);
			for(guint i = 0; i < token->attributes->len; i++)
			{
				attribute = &static_cast<GnomeKeyringAttribute *>(static_cast<void *>(token->attributes->data))[i];
				if(std::string(attribute->name) == "key")
				{
					result.insert(std::make_pair(attribute->value.string, token->secret));
				}
			}
		}
		gnome_keyring_found_list_free(list);
	}

	return result;
}

GAMEKEEPER_NAMESPACE_END(backend)
