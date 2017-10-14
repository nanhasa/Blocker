#include "3rdParty/gtest/gtest.h"

#include <Windows.h>

#include <fstream>
#include "Utility/config.h"
#include "Utility/locator.h"

//Hide functions from other files
namespace {

	class ConfigTest : public testing::Test {
	protected:

		// Function called before every TEST_F call
		void SetUp() override
		{
			CreateDirectoryA("../Data", NULL);
			CreateDirectoryA("../Data/Config", NULL);
			std::ofstream ofs("../Data/Config/config.txt", std::ofstream::out | std::ofstream::trunc);
			ASSERT_TRUE(ofs.is_open());
			ofs << "# Normal comment line\n"
				<< " # Space before comment character\n"
				<< "     # Several spaces before comment character\n"
				<< "\t# Tab before comment character\n"
				<< "\n"
				<< "String=value\n"
				<< "\n"
				<< "IntOneDigitPositive=1\n"
				<< "IntTwoDigitPositive=11\n"
				<< "IntOneDigitNegative=-1\n"
				<< "IntTwoDigitNegative=-11\n"
				<< "\n"
				<< "FloatInteger=1\n"
				<< "FloatNegative=-1.2345f\n"
				<< "FloatPositive=1.2345\n"
				<< "\n"
				<< "BoolTrue=true\n"
				<< "BoolCapitalTrue=TRUE\n"
				<< "BoolDigitTrue=1\n"
				<< "BoolFalse=false\n"
				<< "BoolCapitalFalse=FALSE\n"
				<< "BoolFalseDigit=0\n"
				<< "\n"
				<< "Vec3OneIntegerPositive=1,2,3\n"
				<< "Vec3TwoIntegerPositive=11,22,33\n"
				<< "Vec3OneIntegerNegative=-1,-2,-3\n"
				<< "Vec3TwoIntegerNegative=-11,-22,-33\n"
				<< "Vec3FloatPositive=1.23f,4.56f,7.89f\n"
				<< "Vec3FloatNegative=-1.23f,-4.56f,-7.89f\n"
				<< "\n"
				<< "# Sad trails\n"
				<< "NoValue=\n"
				<< "NoEquals\n"
				<< "=NoName\n"
				<< "IntLeadingSpace= 1\n"
				<< "IntTrailingSpace=1 \n"
				<< "FloatLeadingSpace= 0.123f\n"
				<< "FloatTrailingSpace=0.123f \n"
				<< "BoolLeadingSpace= true\n"
				<< "BoolTrailingSpace=true \n"
				<< "Vec3TooFewValues=1,2\n"
				<< "Vec3TooManyValues=1,2,3,4\n"
				<< "Vec3LeadingComma=,1,2,3\n"
				<< "Vec3TrailingComma=1,2,3,\n"
				<< "Vec3LeadingSpace= 1,2,3\n"
				<< "Vec3TrailingSpace=1,2,3 \n"
				<< "Vec3SpacesBetween=1, 2, 3\n"
				<< "Vec3MoreThanOneSpacesBetween=1,  2,   3" << std::endl;

			Locator::provideConfig(std::make_unique<Config>());
		}
	};

	TEST_F(ConfigTest, GetMissingInt)
	{
		EXPECT_EQ(Locator::getConfig()->get("IntMissing", 0), 0);
	}

	TEST_F(ConfigTest, GetInt)
	{
		EXPECT_EQ(Locator::getConfig()->get("IntOneDigitPositive", 0), 1);
		EXPECT_EQ(Locator::getConfig()->get("IntTwoDigitPositive", 0), 11);
		EXPECT_EQ(Locator::getConfig()->get("IntOneDigitNegative", 0), -1);
		EXPECT_EQ(Locator::getConfig()->get("IntTwoDigitNegative", 0), -11);
	}

	TEST_F(ConfigTest, GetMissingFloat)
	{
		EXPECT_FLOAT_EQ(Locator::getConfig()->get("FloatMissing", 0.0f), 0.0f);
	}

	TEST_F(ConfigTest, GetFloat)
	{
		EXPECT_FLOAT_EQ(Locator::getConfig()->get("FloatInteger", 0.0f), 1);
		EXPECT_FLOAT_EQ(Locator::getConfig()->get("FloatNegative", 0.0f), -1.2345f);
		EXPECT_FLOAT_EQ(Locator::getConfig()->get("FloatPositive", 0.0f), 1.2345f);
	}

	TEST_F(ConfigTest, GetMissingBool)
	{
		EXPECT_FALSE(Locator::getConfig()->get("BoolMissing", false));
		EXPECT_TRUE(Locator::getConfig()->get("BoolMissing", true));
	}

	TEST_F(ConfigTest, GetBool)
	{
		EXPECT_TRUE(Locator::getConfig()->get("BoolTrue", false));
		EXPECT_TRUE(Locator::getConfig()->get("BoolCapitalTrue", false));
		EXPECT_TRUE(Locator::getConfig()->get("BoolDigitTrue", false));
		EXPECT_FALSE(Locator::getConfig()->get("BoolFalse", true)); 
		EXPECT_FALSE(Locator::getConfig()->get("BoolCapitalFalse", true));
		EXPECT_FALSE(Locator::getConfig()->get("BoolFalseDigit", true));
	}

	TEST_F(ConfigTest, GetMissingVec3)
	{
		EXPECT_EQ(Locator::getConfig()->get("Vec3Missing", glm::vec3(3.0f, 2.0f, 1.0f)), glm::vec3(3.0f, 2.0f, 1.0f));
	}

	TEST_F(ConfigTest, GetVec3)
	{
		EXPECT_EQ(Locator::getConfig()->get("Vec3OneIntegerPositive", glm::vec3()), glm::vec3(1, 2, 3));
		EXPECT_EQ(Locator::getConfig()->get("Vec3TwoIntegerPositive", glm::vec3()), glm::vec3(11, 22, 33));
		EXPECT_EQ(Locator::getConfig()->get("Vec3OneIntegerNegative", glm::vec3()), glm::vec3(-1, -2, -3));
		EXPECT_EQ(Locator::getConfig()->get("Vec3TwoIntegerNegative", glm::vec3()), glm::vec3(-11, -22, -33));
		EXPECT_EQ(Locator::getConfig()->get("Vec3FloatPositive", glm::vec3()), glm::vec3(1.23f, 4.56f, 7.89f));
		EXPECT_EQ(Locator::getConfig()->get("Vec3FloatNegative", glm::vec3()), glm::vec3(-1.23f, -4.56f, -7.89f));
	}

	TEST_F(ConfigTest, IncorrectLine)
	{
		EXPECT_EQ(Locator::getConfig()->get("NoValue", std::string("")), "");
		EXPECT_EQ(Locator::getConfig()->get("NoEquals", std::string("")), "");
	}

	TEST_F(ConfigTest, IntWithSpaces)
	{
		EXPECT_EQ(Locator::getConfig()->get("IntLeadingSpace", 0), 1);
		EXPECT_EQ(Locator::getConfig()->get("IntTrailingSpace", 0), 1);
	}

	TEST_F(ConfigTest, FloatWithSpaces)
	{
		EXPECT_FLOAT_EQ(Locator::getConfig()->get("FloatLeadingSpace", 0.0f), 0.123f);
		EXPECT_FLOAT_EQ(Locator::getConfig()->get("FloatTrailingSpace", 0.0f), 0.123f);
	}

	TEST_F(ConfigTest, BoolWithSpaces)
	{
		EXPECT_TRUE(Locator::getConfig()->get("BoolLeadingSpace", false));
		EXPECT_TRUE(Locator::getConfig()->get("BoolTrailingSpace", false));
	}

	TEST_F(ConfigTest, Vec3WithDifferentNotations)
	{
		EXPECT_EQ(Locator::getConfig()->get("Vec3TooFewValues", glm::vec3()), glm::vec3());
		EXPECT_EQ(Locator::getConfig()->get("Vec3TooManyValues", glm::vec3()), glm::vec3(1, 2, 3));
		EXPECT_EQ(Locator::getConfig()->get("Vec3LeadingComma", glm::vec3()), glm::vec3());
		EXPECT_EQ(Locator::getConfig()->get("Vec3TrailingComma", glm::vec3()), glm::vec3(1, 2, 3));
		EXPECT_EQ(Locator::getConfig()->get("Vec3LeadingSpace", glm::vec3()), glm::vec3(1, 2, 3));
		EXPECT_EQ(Locator::getConfig()->get("Vec3TrailingSpace", glm::vec3()), glm::vec3(1, 2, 3));
		EXPECT_EQ(Locator::getConfig()->get("Vec3SpacesBetween", glm::vec3()), glm::vec3(1, 2, 3));
		EXPECT_EQ(Locator::getConfig()->get("Vec3MoreThanOneSpacesBetween", glm::vec3()), glm::vec3(1, 2, 3));
	}
}
