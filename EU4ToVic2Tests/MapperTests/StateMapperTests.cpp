#include "../EU4toV2/Source/Mappers/StateMapper/StateMapper.h"
#include "gtest/gtest.h"



TEST(Mappers_StateMapperTests, getNoProvincesForNonexistentState)
{
	std::stringstream input("");
	mappers::StateMapper theStateMapper(input);

	ASSERT_EQ(theStateMapper.getAllProvincesInState(1).size(), 0);
}


TEST(Mappers_StateMapperTests, getOnlySelfForOneProvinceState)
{
	std::stringstream input("STATE_1 = { 1 }");
	mappers::StateMapper theStateMapper(input);

	ASSERT_EQ(theStateMapper.getAllProvincesInState(1).size(), 1);
}


TEST(Mappers_StateMapperTests, getAllProvincesForMultiProvinceState)
{
	std::stringstream input("STATE_1 = { 1 2 3 }");
	mappers::StateMapper theStateMapper(input);

	ASSERT_EQ(theStateMapper.getAllProvincesInState(1).size(), 3);
}