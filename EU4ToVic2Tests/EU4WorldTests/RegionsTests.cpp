#include "gtest/gtest.h"
#include "../EU4toV2/Source/EU4World/Regions/Regions.h"
#include "../EU4toV2/Source/EU4World/Regions/Areas.h"
#include "../EU4toV2/Source/EU4World/Regions/SuperRegions.h"
#include <sstream>



TEST(EU4World_RegionsTests, oldStyleNoProvincesMeansNoProvinces)
{
	std::stringstream areasInput;
	areasInput << "test_region = {\n";
	areasInput << "}";
	EU4::Areas theAreas(areasInput);

	EU4::Regions theRegions(theAreas);

	ASSERT_FALSE(theRegions.provinceInRegion(1, "test_region"));
}


TEST(EU4World_RegionsTests, oldStyleProvincesCanBeSet)
{
	std::stringstream areasInput;
	areasInput << "test_region = {\n";
	areasInput << "\t1 2 3\n";
	areasInput << "}";
	EU4::Areas theAreas(areasInput);

	EU4::Regions theRegions(theAreas);

	ASSERT_TRUE(theRegions.provinceInRegion(1, "test_region"));
}


TEST(EU4World_RegionsTests, newStyleNoAreasMeansNoProvinces)
{
	std::stringstream superRegionsInput;
	superRegionsInput << "test_superregion = {";
	superRegionsInput << "\ttest_region";
	superRegionsInput << "}";
	EU4::SuperRegions superRegions(superRegionsInput);
	
	std::stringstream regionsInput;
	regionsInput << "test_region = {\n";
	regionsInput << "\tareas = {\n";
	regionsInput << "\t}\n";
	regionsInput << "}";

	std::stringstream areasInput;
	areasInput << "test_area = {\n";
	areasInput << "\t1 2 3\n";
	areasInput << "}";
	EU4::Areas theAreas(areasInput);

	EU4::Regions theRegions(superRegions, theAreas, regionsInput);

	ASSERT_FALSE(theRegions.provinceInRegion(1, "test_region"));
}


TEST(EU4World_RegionsTests, newStyleNoProvincesMeansNoProvinces)
{
	std::stringstream superRegionsInput;
	superRegionsInput << "test_superregion = {";
	superRegionsInput << "\ttest_region";
	superRegionsInput << "}";
	EU4::SuperRegions superRegions(superRegionsInput);

	std::stringstream regionsInput;
	regionsInput << "test_region = {\n";
	regionsInput << "\tareas = {\n";
	regionsInput << "\t\ttest_area\n";
	regionsInput << "\t}\n";
	regionsInput << "}";

	std::stringstream areasInput;
	areasInput << "test_area = {\n";
	areasInput << "}";
	EU4::Areas theAreas(areasInput);

	EU4::Regions theRegions(superRegions, theAreas, regionsInput);

	ASSERT_FALSE(theRegions.provinceInRegion(1, "test_region"));
}


TEST(EU4World_RegionsTests, newStyleAreasProvideProvinces)
{
	std::stringstream superRegionsInput;
	superRegionsInput << "test_superregion = {";
	superRegionsInput << "\ttest_region";
	superRegionsInput << "}";
	EU4::SuperRegions superRegions(superRegionsInput);

	std::stringstream regionsInput;
	regionsInput << "test_region = {\n";
	regionsInput << "\tareas = {\n";
	regionsInput << "\t\ttest_area\n";
	regionsInput << "\t}\n";
	regionsInput << "}";

	std::stringstream areasInput;
	areasInput << "test_area = {\n";
	areasInput << "\t1 2 3\n";
	areasInput << "}";
	EU4::Areas theAreas(areasInput);

	EU4::Regions theRegions(superRegions, theAreas, regionsInput);

	ASSERT_TRUE(theRegions.provinceInRegion(1, "test_region"));
}

TEST(EU4World_RegionsTests, newStyleAreasProvideProvincesBySuperRegion)
{
	std::stringstream superRegionsInput;
	superRegionsInput << "test_superregion = {";
	superRegionsInput << "\ttest_region";
	superRegionsInput << "}";
	EU4::SuperRegions superRegions(superRegionsInput);

	std::stringstream regionsInput;
	regionsInput << "test_region = {\n";
	regionsInput << "\tareas = {\n";
	regionsInput << "\t\ttest_area\n";
	regionsInput << "\t}\n";
	regionsInput << "}";

	std::stringstream areasInput;
	areasInput << "test_area = {\n";
	areasInput << "\t1 2 3\n";
	areasInput << "}";
	EU4::Areas theAreas(areasInput);

	EU4::Regions theRegions(superRegions, theAreas, regionsInput);

	ASSERT_TRUE(theRegions.provinceInRegion(1, "test_superregion"));
}

TEST(EU4World_RegionsTests, newStyleAreasProvideProvincesByArea)
{
	std::stringstream superRegionsInput;
	superRegionsInput << "test_superregion = {";
	superRegionsInput << "\ttest_region";
	superRegionsInput << "}";
	EU4::SuperRegions superRegions(superRegionsInput);

	std::stringstream regionsInput;
	regionsInput << "test_region = {\n";
	regionsInput << "\tareas = {\n";
	regionsInput << "\t\ttest_area\n";
	regionsInput << "\t}\n";
	regionsInput << "}";

	std::stringstream areasInput;
	areasInput << "test_area = {\n";
	areasInput << "\t1 2 3\n";
	areasInput << "}";
	EU4::Areas theAreas(areasInput);

	EU4::Regions theRegions(superRegions, theAreas, regionsInput);

	ASSERT_TRUE(theRegions.provinceInRegion(1, "test_area"));
}