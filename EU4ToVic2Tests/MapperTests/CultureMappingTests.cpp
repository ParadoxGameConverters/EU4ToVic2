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
#include "../EU4toV2/Source/Mappers/CultureMapping.h"
#include "../Mocks/RegionsMock.h"
#include <sstream>



TEST(Mappers_CultureMappingTests, nonMatchingSourceReturnsEmptyOptional)
{
	std::stringstream input;
	input << "= {}";

	std::map<mappers::distinguisherTypes, std::string> distinguishers;
	mappers::cultureMapping theMapping("sourceCulture", "destCulture", distinguishers);

	mockRegions regions;
	std::optional<std::string> match = theMapping.cultureMatch(regions, "nonMatchingCulture", "", -1, "");
	ASSERT_FALSE(match);
}


TEST(Mappers_CultureMappingTests, matchingSourceReturnsDestinationCulture)
{
	std::stringstream input;
	input << "= {}";

	std::map<mappers::distinguisherTypes, std::string> distinguishers;
	mappers::cultureMapping theMapping("sourceCulture", "destCulture", distinguishers);

	mockRegions regions;
	std::optional<std::string> match = theMapping.cultureMatch(regions, "sourceCulture", "", -1, "");
	ASSERT_TRUE(match);
	ASSERT_EQ(*match, "destCulture");
}


TEST(Mappers_CultureMappingTests, ownerDistinguisherWrongNoMatch)
{
	std::stringstream input;
	input << "= {}";

	std::map<mappers::distinguisherTypes, std::string> distinguishers;
	distinguishers.insert(std::make_pair(mappers::distinguisherTypes::owner, "OWN"));

	mappers::cultureMapping theMapping("sourceCulture", "destCulture", distinguishers);

	mockRegions regions;
	std::optional<std::string> match = theMapping.cultureMatch(regions, "sourceCulture", "", -1, "NOT");
	ASSERT_FALSE(match);
}


TEST(Mappers_CultureMappingTests, ownerDistinguisherRightMatch)
{
	std::stringstream input;
	input << "= {}";

	std::map<mappers::distinguisherTypes, std::string> distinguishers;
	distinguishers.insert(std::make_pair(mappers::distinguisherTypes::owner, "OWN"));

	mappers::cultureMapping theMapping("sourceCulture", "destCulture", distinguishers);

	mockRegions regions;
	std::optional<std::string> match = theMapping.cultureMatch(regions, "sourceCulture", "", -1, "OWN");
	ASSERT_TRUE(match);
	ASSERT_EQ(*match, "destCulture");
}


TEST(Mappers_CultureMappingTests, religionDistinguisherWrongNoMatch)
{
	std::stringstream input;
	input << "= {}";

	std::map<mappers::distinguisherTypes, std::string> distinguishers;
	distinguishers.insert(std::make_pair(mappers::distinguisherTypes::religion, "religion"));

	mappers::cultureMapping theMapping("sourceCulture", "destCulture", distinguishers);

	mockRegions regions;
	std::optional<std::string> match = theMapping.cultureMatch(regions, "sourceCulture", "wrongReligion", -1, "");
	ASSERT_FALSE(match);
}


TEST(Mappers_CultureMappingTests, religionDistinguisherRightMatch)
{
	std::stringstream input;
	input << "= {}";

	std::map<mappers::distinguisherTypes, std::string> distinguishers;
	distinguishers.insert(std::make_pair(mappers::distinguisherTypes::religion, "religion"));

	mappers::cultureMapping theMapping("sourceCulture", "destCulture", distinguishers);

	mockRegions regions;
	std::optional<std::string> match = theMapping.cultureMatch(regions, "sourceCulture", "religion", -1, "");
	ASSERT_TRUE(match);
	ASSERT_EQ(*match, "destCulture");
}


TEST(Mappers_CultureMappingTests, provinceDistinguisherWrongNoMatch)
{
	std::stringstream input;
	input << "= {}";

	std::map<mappers::distinguisherTypes, std::string> distinguishers;
	distinguishers.insert(std::make_pair(mappers::distinguisherTypes::province, "42"));

	mappers::cultureMapping theMapping("sourceCulture", "destCulture", distinguishers);

	mockRegions regions;
	std::optional<std::string> match = theMapping.cultureMatch(regions, "sourceCulture", "", 12, "");
	ASSERT_FALSE(match);
}


TEST(Mappers_CultureMappingTests, provinceDistinguisherRightMatch)
{
	std::stringstream input;
	input << "= {}";

	std::map<mappers::distinguisherTypes, std::string> distinguishers;
	distinguishers.insert(std::make_pair(mappers::distinguisherTypes::province, "42"));

	mappers::cultureMapping theMapping("sourceCulture", "destCulture", distinguishers);

	mockRegions regions;
	std::optional<std::string> match = theMapping.cultureMatch(regions, "sourceCulture", "", 42, "");
	ASSERT_TRUE(match);
	ASSERT_EQ(*match, "destCulture");
}


TEST(Mappers_CultureMappingTests, regionDistinguisherWrongNoMatch)
{
	std::stringstream input;
	input << "= {}";

	std::map<mappers::distinguisherTypes, std::string> distinguishers;
	distinguishers.insert(std::make_pair(mappers::distinguisherTypes::region, "theRegion"));

	mappers::cultureMapping theMapping("sourceCulture", "destCulture", distinguishers);

	mockRegions regions;
	EXPECT_CALL(regions, provinceInRegion(42, "theRegion")).WillOnce(testing::Return(false));

	std::optional<std::string> match = theMapping.cultureMatch(regions, "sourceCulture", "", 42, "");
	ASSERT_FALSE(match);
}


TEST(Mappers_CultureMappingTests, regionDistinguisherRightMatch)
{
	std::stringstream input;
	input << "= {}";

	std::map<mappers::distinguisherTypes, std::string> distinguishers;
	distinguishers.insert(std::make_pair(mappers::distinguisherTypes::region, "theRegion"));

	mappers::cultureMapping theMapping("sourceCulture", "destCulture", distinguishers);

	mockRegions regions;
	EXPECT_CALL(regions, provinceInRegion(42, "theRegion")).WillOnce(testing::Return(true));

	std::optional<std::string> match = theMapping.cultureMatch(regions, "sourceCulture", "", 42, "");
	ASSERT_TRUE(match);
	ASSERT_EQ(*match, "destCulture");
}