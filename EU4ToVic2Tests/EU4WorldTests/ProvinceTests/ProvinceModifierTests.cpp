#include "gtest/gtest.h"
#include "Provinces/ProvinceModifier.h"

TEST(EU4World_Province_ModiferTests, modifierDefaultsToEmpty)
{
	std::stringstream input;
	const EU4::ProvinceModifier theModifier(input);
	
	ASSERT_TRUE(theModifier.getModifier().empty());
}

TEST(EU4World_Province_ModiferTests, modifierCanBeSet)
{
	std::stringstream input;
	input << "modifier=theModifier\n";
	const EU4::ProvinceModifier theModifier(input);
	
	ASSERT_EQ("theModifier", theModifier.getModifier());
}