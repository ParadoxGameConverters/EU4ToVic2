#include "../EU4toV2/Source/Mappers/ProvinceMappings/ProvinceMapping.h"
#include "gtest/gtest.h"
#include <sstream>

TEST(Mappers_ProvinceMappingTests, EU4ProvinceCanBeAdded)
{
	std::stringstream input;
	input << "= { eu4 = 2 eu4 = 1 }";

	mappers::ProvinceMapping theMapper(input);

	ASSERT_EQ(theMapper.getEU4Provinces()[0], 2);
	ASSERT_EQ(theMapper.getEU4Provinces()[1], 1);
}

TEST(Mappers_ProvinceMappingTests, Vic2ProvinceCanBeAdded)
{
	std::stringstream input;
	input << "= { v2 = 2 v2 = 1 }";

	mappers::ProvinceMapping theMapper(input);

	ASSERT_EQ(theMapper.getVic2Provinces()[0], 2);
	ASSERT_EQ(theMapper.getVic2Provinces()[1], 1);
}
