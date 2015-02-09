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

#include "defaultfixture.h"

#include <gamekeeper/core/osinformation.h>
#include <gamekeeper/core/xdgpaths.h>

#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/operations.hpp>

using namespace gamekeeper::core;
typedef boost::filesystem::path Path;
namespace fs = boost::filesystem;

class XDGPathsTest : public gamekeeper::test::DefaultFicture
{
protected:
	virtual void SetUp() override
	{
		this->osInfo = this->container->resolve<OSInformation>();
		this->userPaths = std::make_unique<XDGPaths>();

		// reset all env values, so that we don't get false positives
		this->osInfo->setEnv("XDG_DATA_HOME", "");
		this->osInfo->setEnv("XDG_DATA_DIRS", "");
		this->osInfo->setEnv("XDG_CONFIG_HOME", "");
		this->osInfo->setEnv("XDG_CONFIG_DIRS", "");
		this->osInfo->setEnv("XDG_CACHE_HOME", "");
		this->osInfo->setEnv("XDG_RUNTIME_DIR", "");

		this->currentTestPathSingle = fs::current_path() / "tests.gtest" / "core" / "xdgpaths" / "single";
		this->currentTestPathMulti1 = fs::current_path() / "tests.gtest" / "core" / "xdgpaths" / "multi1";
		this->currentTestPathMulti2 = fs::current_path() / "tests.gtest" / "core" / "xdgpaths" / "multi2";
	}

	virtual void TearDown() override
	{
		if(fs::exists(fs::current_path() / "tests.gtest"))
		{
			fs::remove_all(fs::current_path() / "tests.gtest");
		}
	}

	std::shared_ptr<OSInformation> osInfo;
	std::unique_ptr<UserPaths> userPaths;
	fs::path currentTestPathSingle;
	fs::path currentTestPathMulti1;
	fs::path currentTestPathMulti2;
};

TEST_F(XDGPathsTest, XDG_DATA_notSet)
{
	EXPECT_TRUE(Path(this->osInfo->getEnv("HOME")) / ".local" / "share" / "gamekeeper" / "test.file" == this->userPaths->getDataFile("test.file"));
}

TEST_F(XDGPathsTest, XDG_DATA_HOME_notSet_XDG_DATA_DIRS_set_butEmpty)
{
	this->osInfo->setEnv("XDG_DATA_DIRS", this->currentTestPathMulti1.string().c_str());
	EXPECT_TRUE(Path(this->osInfo->getEnv("HOME")) / ".local" / "share" / "gamekeeper" / "test.file" == this->userPaths->getDataFile("test.file"));
}

TEST_F(XDGPathsTest, XDG_DATA_HOME_notSet_XDG_DATA_DIRS_set_notEmpty)
{
	this->osInfo->setEnv("XDG_DATA_DIRS", this->currentTestPathMulti1.string().c_str());
	fs::create_directories(this->currentTestPathMulti1 / "gamekeeper");
	fs::ofstream stream(this->currentTestPathMulti1 / "gamekeeper" / "test.file");
	// if the file is open it was already created
	EXPECT_TRUE(stream.is_open());
	EXPECT_TRUE(this->currentTestPathMulti1 / "gamekeeper" / "test.file" == this->userPaths->getDataFile("test.file"));
}

TEST_F(XDGPathsTest, XDG_DATA_HOME_set_XDG_DATA_DIRS_notSet)
{
	this->osInfo->setEnv("XDG_DATA_HOME", this->currentTestPathMulti1.string().c_str());
	EXPECT_TRUE(this->currentTestPathMulti1 / "gamekeeper" / "test.file" == this->userPaths->getDataFile("test.file"));
}

TEST_F(XDGPathsTest, XDG_DATA_HOME_set_XDG_DATA_DIRS_set_butEmpty)
{
	this->osInfo->setEnv("XDG_DATA_HOME", this->currentTestPathSingle.string().c_str());
	this->osInfo->setEnv("XDG_DATA_DIRS", this->currentTestPathMulti1.string().c_str());
	EXPECT_TRUE(this->currentTestPathSingle / "gamekeeper" / "test.file" == this->userPaths->getDataFile("test.file"));
}

TEST_F(XDGPathsTest, XDG_DATA_HOME_set_butEmpty_XDG_DATA_DIRS_set_notEmpty)
{
	this->osInfo->setEnv("XDG_DATA_HOME", this->currentTestPathSingle.string().c_str());
	this->osInfo->setEnv("XDG_DATA_DIRS", this->currentTestPathMulti1.string().c_str());
	fs::create_directories(this->currentTestPathMulti1 / "gamekeeper");
	fs::ofstream stream(this->currentTestPathMulti1 / "gamekeeper" / "test.file");
	// if the file is open it was already created
	EXPECT_TRUE(stream.is_open());
	EXPECT_TRUE(this->currentTestPathMulti1 / "gamekeeper" / "test.file" == this->userPaths->getDataFile("test.file"));
}

TEST_F(XDGPathsTest, XDG_DATA_HOME_set_notEmpty_XDG_DATA_DIRS_set_notEmpty)
{
	this->osInfo->setEnv("XDG_DATA_HOME", this->currentTestPathSingle.string().c_str());
	this->osInfo->setEnv("XDG_DATA_DIRS", this->currentTestPathMulti1.string().c_str());
	fs::create_directories(this->currentTestPathSingle / "gamekeeper");
	fs::create_directories(this->currentTestPathMulti1 / "gamekeeper");
	fs::ofstream streamSingle(this->currentTestPathSingle / "gamekeeper" / "test.file");
	fs::ofstream streamMulti(this->currentTestPathMulti1 / "gamekeeper" / "test.file");
	// if the files are open it was already created
	EXPECT_TRUE(streamSingle.is_open());
	EXPECT_TRUE(streamMulti.is_open());
	EXPECT_TRUE(this->currentTestPathSingle / "gamekeeper" / "test.file" == this->userPaths->getDataFile("test.file"));
}

TEST_F(XDGPathsTest, XDG_DATA_DIRS_firstEmpty_secondNotEmpty)
{
	this->osInfo->setEnv("XDG_DATA_DIRS", (this->currentTestPathMulti1.string() + this->osInfo->getEnvSeperator() +
		this->currentTestPathMulti2.string()).c_str());
	fs::create_directories(this->currentTestPathMulti2 / "gamekeeper");
	fs::ofstream streamMulti2(this->currentTestPathMulti2 / "gamekeeper" / "test.file");
	// if the files are open it was already created
	EXPECT_TRUE(streamMulti2.is_open());
	EXPECT_TRUE(this->currentTestPathMulti2 / "gamekeeper" / "test.file" == this->userPaths->getDataFile("test.file"));
}

TEST_F(XDGPathsTest, XDG_DATA_DIRS_order)
{
	this->osInfo->setEnv("XDG_DATA_DIRS", (this->currentTestPathMulti1.string() + this->osInfo->getEnvSeperator() +
		this->currentTestPathMulti2.string()).c_str());
	fs::create_directories(this->currentTestPathMulti1 / "gamekeeper");
	fs::create_directories(this->currentTestPathMulti2 / "gamekeeper");
	fs::ofstream streamMulti1(this->currentTestPathMulti1 / "gamekeeper" / "test.file");
	fs::ofstream streamMulti2(this->currentTestPathMulti2 / "gamekeeper" / "test.file");
	// if the files are open it was already created
	EXPECT_TRUE(streamMulti1.is_open());
	EXPECT_TRUE(streamMulti2.is_open());
	EXPECT_TRUE(this->currentTestPathMulti1 / "gamekeeper" / "test.file" == this->userPaths->getDataFile("test.file"));
}

TEST_F(XDGPathsTest, XDG_CONFIG_notSet)
{
	EXPECT_TRUE(Path(this->osInfo->getEnv("HOME")) / ".config" / "gamekeeper" / "test.file" == this->userPaths->getConfigFile("test.file"));
}

TEST_F(XDGPathsTest, XDG_CONFIG_HOME_notSet_XDG_CONFIG_DIRS_set_butEmpty)
{
	this->osInfo->setEnv("XDG_CONFIG_DIRS", this->currentTestPathMulti1.string().c_str());
	EXPECT_TRUE(Path(this->osInfo->getEnv("HOME")) / ".config" / "gamekeeper" / "test.file" == this->userPaths->getConfigFile("test.file"));
}

TEST_F(XDGPathsTest, XDG_CONFIG_HOME_notSet_XDG_CONFIG_DIRS_set_notEmpty)
{
	this->osInfo->setEnv("XDG_CONFIG_DIRS", this->currentTestPathMulti1.string().c_str());
	fs::create_directories(this->currentTestPathMulti1 / "gamekeeper");
	fs::ofstream stream(this->currentTestPathMulti1 / "gamekeeper" / "test.file");
	// if the file is open it was already created
	EXPECT_TRUE(stream.is_open());
	EXPECT_TRUE(this->currentTestPathMulti1 / "gamekeeper" / "test.file" == this->userPaths->getConfigFile("test.file"));
}

TEST_F(XDGPathsTest, XDG_CONFIG_HOME_set_XDG_CONFIG_DIRS_notSet)
{
	this->osInfo->setEnv("XDG_CONFIG_HOME", this->currentTestPathSingle.string().c_str());
	EXPECT_TRUE(this->currentTestPathSingle / "gamekeeper" / "test.file" == this->userPaths->getConfigFile("test.file"));
}

TEST_F(XDGPathsTest, XDG_CONFIG_HOME_set_XDG_CONFIG_DIRS_set_butEmpty)
{
	this->osInfo->setEnv("XDG_CONFIG_HOME", this->currentTestPathSingle.string().c_str());
	this->osInfo->setEnv("XDG_CONFIG_DIRS", this->currentTestPathMulti1.string().c_str());
	EXPECT_TRUE(this->currentTestPathSingle / "gamekeeper" / "test.file" == this->userPaths->getConfigFile("test.file"));
}

TEST_F(XDGPathsTest, XDG_CONFIG_HOME_set_butEmpty_XDG_CONFIG_DIRS_set_notEmpty)
{
	this->osInfo->setEnv("XDG_CONFIG_HOME", this->currentTestPathSingle.string().c_str());
	this->osInfo->setEnv("XDG_CONFIG_DIRS", this->currentTestPathMulti1.string().c_str());
	fs::create_directories(this->currentTestPathMulti1 / "gamekeeper");
	fs::ofstream stream(this->currentTestPathMulti1 / "gamekeeper" / "test.file");
	// if the file is open it was already created
	EXPECT_TRUE(stream.is_open());
	EXPECT_TRUE(this->currentTestPathMulti1 / "gamekeeper" / "test.file" == this->userPaths->getConfigFile("test.file"));
}

TEST_F(XDGPathsTest, XDG_CONFIG_HOME_set_notEmpty_XDG_CONFIG_DIRS_set_notEmpty)
{
	this->osInfo->setEnv("XDG_CONFIG_HOME", this->currentTestPathSingle.string().c_str());
	this->osInfo->setEnv("XDG_CONFIG_DIRS", this->currentTestPathMulti1.string().c_str());
	fs::create_directories(this->currentTestPathSingle / "gamekeeper");
	fs::create_directories(this->currentTestPathMulti1 / "gamekeeper");
	fs::ofstream streamSingle(this->currentTestPathSingle / "gamekeeper" / "test.file");
	fs::ofstream streamMulti(this->currentTestPathMulti1 / "gamekeeper" / "test.file");
	// if the files are open it was already created
	EXPECT_TRUE(streamSingle.is_open());
	EXPECT_TRUE(streamMulti.is_open());
	EXPECT_TRUE(this->currentTestPathSingle / "gamekeeper" / "test.file" == this->userPaths->getConfigFile("test.file"));
}

TEST_F(XDGPathsTest, XDG_CONFIG_DIRS_firstEmpty_secondNotEmpty)
{
	this->osInfo->setEnv("XDG_CONFIG_DIRS", (this->currentTestPathMulti1.string() + this->osInfo->getEnvSeperator() +
		this->currentTestPathMulti2.string()).c_str());
	fs::create_directories(this->currentTestPathMulti2 / "gamekeeper");
	fs::ofstream streamMulti2(this->currentTestPathMulti2 / "gamekeeper" / "test.file");
	// if the files are open it was already created
	EXPECT_TRUE(streamMulti2.is_open());
	EXPECT_TRUE(this->currentTestPathMulti2 / "gamekeeper" / "test.file" == this->userPaths->getConfigFile("test.file"));
}

TEST_F(XDGPathsTest, XDG_CONFIG_DIRS_order)
{
	this->osInfo->setEnv("XDG_CONFIG_DIRS", (this->currentTestPathMulti1.string() + this->osInfo->getEnvSeperator() +
		this->currentTestPathMulti2.string()).c_str());
	fs::create_directories(this->currentTestPathMulti1 / "gamekeeper");
	fs::create_directories(this->currentTestPathMulti2 / "gamekeeper");
	fs::ofstream streamMulti1(this->currentTestPathMulti1 / "gamekeeper" / "test.file");
	fs::ofstream streamMulti2(this->currentTestPathMulti2 / "gamekeeper" / "test.file");
	// if the files are open it was already created
	EXPECT_TRUE(streamMulti1.is_open());
	EXPECT_TRUE(streamMulti2.is_open());
	EXPECT_TRUE(this->currentTestPathMulti1 / "gamekeeper" / "test.file" == this->userPaths->getConfigFile("test.file"));
}

TEST_F(XDGPathsTest, XDG_CACHE_HOME_notSet)
{
	EXPECT_TRUE(Path(this->osInfo->getEnv("HOME")) / ".cache" / "gamekeeper" / "test.file" == this->userPaths->getCacheFile("test.file"));
}

TEST_F(XDGPathsTest, XDG_CACHE_HOME_set)
{
	this->osInfo->setEnv("XDG_CACHE_HOME", this->currentTestPathSingle.string().c_str());
	EXPECT_TRUE(this->currentTestPathSingle / "gamekeeper" / "test.file" == this->userPaths->getCacheFile("test.file"));
}

TEST_F(XDGPathsTest, XDG_RUNTIME_DIR_notSet)
{
	// go two steps back to eliminate file name and gamekeeper
	Path resolved = this->userPaths->getRuntimeFile("test.file").parent_path().parent_path();
	EXPECT_TRUE(fs::is_directory(resolved));

	// check required ownership
	fs::file_status fs = status(resolved);
	EXPECT_NE(0, fs.permissions() & (fs::owner_read | fs::owner_write | fs::owner_exe));

	// check for write access
	fs::ofstream stream(resolved / "gamekeeper.file");
	EXPECT_TRUE(stream.is_open());
}

TEST_F(XDGPathsTest, XDG_RUNTIME_DIR_set)
{
	this->osInfo->setEnv("XDG_RUNTIME_DIR", this->currentTestPathSingle.string().c_str());
	EXPECT_TRUE(this->currentTestPathSingle / "gamekeeper" / "test.file" == this->userPaths->getRuntimeFile("test.file"));
}
