#include <gamelib/core/common.h>
#include <gamelib/core/linuxinformation.h>

#include <gtest/gtest.h>

using namespace gamelib::core;

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

TEST_F(LinuxInformationTest, testEnvMethodConsistent) {
	// test that the stored char is not the same
	std::string home = osInfo->getEnv("HOME");
	std::string path = osInfo->getEnv("PATH");

	EXPECT_NE(home, path);
}
