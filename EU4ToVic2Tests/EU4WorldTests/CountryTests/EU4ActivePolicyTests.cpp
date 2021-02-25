#include "gtest/gtest.h"
#include "Country/EU4ActivePolicy.h"
#include <sstream>



TEST(EU4World_ActivePolicyTests, policyDefaultsToEmpty)
{
	std::stringstream input;
	input << "={}";

	EU4::EU4ActivePolicy thePolicy(input);
	ASSERT_EQ(thePolicy.getPolicy(), "");
}


TEST(EU4World_ActivePolicyTests, dateDefaultsToEmpty)
{
	std::stringstream input;
	input << "={}";

	EU4::EU4ActivePolicy thePolicy(input);
	ASSERT_EQ(thePolicy.getDate(), "");
}


TEST(EU4World_ActivePolicyTests, policyCanBeImported)
{
	std::stringstream input;
	input << "={\n";
	input << "\t\t\tpolicy=\"agricultural_cultivations\"\n";
	input << "\t\t\tdate=1815.11.2\n";
	input << "\t\t}\n";

	EU4::EU4ActivePolicy thePolicy(input);
	ASSERT_EQ(thePolicy.getPolicy(), "agricultural_cultivations");
}


TEST(EU4World_ActivePolicyTests, dateCanBeImported)
{
	std::stringstream input;
	input << "={\n";
	input << "\t\t\tpolicy=\"agricultural_cultivations\"\n";
	input << "\t\t\tdate=1815.11.2\n";
	input << "\t\t}\n";

	EU4::EU4ActivePolicy thePolicy(input);
	ASSERT_EQ(thePolicy.getDate(), "1815.11.2");
}


