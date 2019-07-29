/*Copyright (c) 2019 The Paradox Game Converters Project

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/



#include "gtest/gtest.h"
#include "../EU4toV2/Source/EU4World/Regions/Regions.h"
#include "../EU4toV2/Source/EU4World/Regions/Areas.h"
#include <sstream>



TEST(EU4World_RegionsTests, oldStyleNoProvincesMeansNoProvinces)
{
	std::stringstream areasInput;
	areasInput << "test_region = {\n";
	areasInput << "}";
	EU4::areas theAreas(areasInput);

	EU4::Regions theRegions(theAreas);

	ASSERT_FALSE(theRegions.provinceInRegion(1, "test_region"));
}


TEST(EU4World_RegionsTests, oldStyleProvincesCanBeSet)
{
	std::stringstream areasInput;
	areasInput << "test_region = {\n";
	areasInput << "\t1 2 3\n";
	areasInput << "}";
	EU4::areas theAreas(areasInput);

	EU4::Regions theRegions(theAreas);

	ASSERT_TRUE(theRegions.provinceInRegion(1, "test_region"));
}


TEST(EU4World_RegionsTests, newStyleNoAreasMeansNoProvinces)
{
	std::stringstream regionsInput;
	regionsInput << "test_region = {\n";
	regionsInput << "\tareas = {\n";
	regionsInput << "\t}\n";
	regionsInput << "}";

	std::stringstream areasInput;
	areasInput << "test_area = {\n";
	areasInput << "\t1 2 3\n";
	areasInput << "}";
	EU4::areas theAreas(areasInput);

	EU4::Regions theRegions(theAreas, regionsInput);

	ASSERT_FALSE(theRegions.provinceInRegion(1, "test_region"));
}


TEST(EU4World_RegionsTests, newStyleNoProvincesMeansNoProvinces)
{
	std::stringstream regionsInput;
	regionsInput << "test_region = {\n";
	regionsInput << "\tareas = {\n";
	regionsInput << "\t\ttest_area\n";
	regionsInput << "\t}\n";
	regionsInput << "}";

	std::stringstream areasInput;
	areasInput << "test_area = {\n";
	areasInput << "}";
	EU4::areas theAreas(areasInput);

	EU4::Regions theRegions(theAreas, regionsInput);

	ASSERT_FALSE(theRegions.provinceInRegion(1, "test_region"));
}


TEST(EU4World_RegionsTests, newStyleAreasProvideProvinces)
{
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
	EU4::areas theAreas(areasInput);

	EU4::Regions theRegions(theAreas, regionsInput);

	ASSERT_TRUE(theRegions.provinceInRegion(1, "test_region"));
}