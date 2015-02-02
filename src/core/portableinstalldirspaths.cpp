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

#include <gamekeeper/core/portableinstalldirspaths.h>

#include <gamekeeper/core/osinformation.h>

namespace bfs = boost::filesystem;

GAMEKEEPER_NAMESPACE_START(core)

static bfs::path exePath;

class PortableInstallDirsPaths::PImpl
{
public:
	PImpl();
	bfs::path dataPath;
};

PortableInstallDirsPaths::PImpl::PImpl()
:	dataPath(OSInformation::get().getExecPath().parent_path() / "data"){}

PortableInstallDirsPaths::PortableInstallDirsPaths()
:	data(new PImpl()){}

PortableInstallDirsPaths::~PortableInstallDirsPaths(){}

const bfs::path &
PortableInstallDirsPaths::getDataPath()
{
	return this->data->dataPath;
}

GAMEKEEPER_NAMESPACE_END(core)
