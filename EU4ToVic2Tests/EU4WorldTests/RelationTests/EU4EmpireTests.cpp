#include "Relations/EU4Empire.h"
#include "gtest/gtest.h"

TEST(EU4World_Relations_EmpireTests, emperorDefaultsToBlank)
{
	std::stringstream input;
	const EU4::EU4Empire empire(input);

	ASSERT_TRUE(empire.getEmperor().empty());
}

TEST(EU4World_Relations_EmpireTests, primitivesCanBeLoaded)
{
	std::stringstream input;
	input << "emperor = HAB\n";
	const EU4::EU4Empire empire(input);

	ASSERT_EQ("HAB", empire.getEmperor());
}
