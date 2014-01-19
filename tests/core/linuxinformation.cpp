#include <gamelib/core/common.h>
#include <gamelib/core/linuxinformation.h>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include <gtest/gtest.h>

using namespace gamelib::core;
using namespace boost::filesystem;

class LinuxInformationTest : public testing::Test
{
protected:
	virtual void SetUp() override
	{
		this->osInfo = new LinuxInformation();
	}

	virtual void TearDown() override
	{
		delete this->osInfo;
	}

	OSInformation * osInfo = nullptr;
};

TEST_F(LinuxInformationTest, getEnvNotEmpty)
{
	std::string home = osInfo->getEnv("HOME");
	EXPECT_FALSE(home.empty());
}

TEST_F(LinuxInformationTest, getEnvHomeReturnsFile)
{
	path homePath(osInfo->getEnv("HOME"));
	EXPECT_TRUE(exists(homePath));
	EXPECT_TRUE(is_directory(homePath));
}

TEST_F(LinuxInformationTest, getEnvConsistency)
{
	// test that the stored char is not the same
	std::string home = osInfo->getEnv("HOME");
	std::string path = osInfo->getEnv("PATH");

	EXPECT_NE(home, path);
}
