#include "gtest/gtest.h"
#include "../EU4toV2/Source/EU4World/Provinces/Provinces.h"
#include "../EU4toV2/Source/Mappers/ProvinceMappings/ProvinceMapper.h"
#include <sstream>



TEST(EU4World_ProvincesTests, defaultProvincesIsEmpty)
{
	std::stringstream input;
	input << "={}";

	EU4::Provinces theProvinces(input);
	ASSERT_EQ(theProvinces.getAllProvinces().size(), 0);
}


TEST(EU4World_ProvincesTests, provincesCanBeInput)
{
	std::stringstream input;
	input << "={\n";
	input << "-1={}";
	input << "}";

	EU4::Provinces theProvinces(input);
	ASSERT_EQ(theProvinces.getAllProvinces().size(), 1);
}


TEST(EU4World_ProvincesTests, gettingNonExsistentProvinceThrowsException)
{
	std::stringstream input;
	input << "={\n";
	input << "-1={}";
	input << "}";

	EU4::Provinces theProvinces(input);
	ASSERT_THROW(theProvinces.getProvince(42), std::range_error);
}


TEST(EU4World_ProvincesTests, canGetProvince)
{
	std::stringstream input;
	input << "={\n";
	input << "-1={}";
	input << "}";

	EU4::Provinces theProvinces(input);
	ASSERT_EQ(theProvinces.getProvince(1)->getNum(), 1);
}
