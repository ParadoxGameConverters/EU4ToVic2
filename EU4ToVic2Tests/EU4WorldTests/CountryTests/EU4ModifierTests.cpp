#include "Country/EU4Modifier.h"
#include "gtest/gtest.h"

TEST(EU4World_Country_EU4ModifierTests, primitivesDefaultToBlank)
{
	std::stringstream input;
	const EU4::EU4Modifier modifier(input);

	ASSERT_TRUE(modifier.getModifier().empty());
}

TEST(EU4World_Country_EU4ModifierTests, reformsCanBeLoaded)
{
	std::stringstream input;
	input << "modifier = a_modifier\n";
	const EU4::EU4Modifier modifier(input);

	ASSERT_EQ("a_modifier", modifier.getModifier());
}
