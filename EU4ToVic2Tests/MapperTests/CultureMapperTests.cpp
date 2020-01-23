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
#include "../EU4toV2/Source/Mappers/CultureMapper/CultureMapper.h"
#include "../Mocks/RegionsMock.h"
#include <sstream>



TEST(Mappers_CultureMapperTests, nonMatchingSourceReturnsEmptyOptional)
{
	std::stringstream input;
	input << "link = {\n";
	input << "\teu4 = sourceCulture\n";
	input << "\tvic2 = destinationCulture\n";
	input << "}";

	mappers::CultureMapper theMapper(input);

	mockRegions regions;
	std::optional<std::string> match = theMapper.cultureMatch(regions, "nonMatchingCulture", "", -1, "");
	ASSERT_FALSE(match);
}


TEST(Mappers_CultureMapperTests, matchingSourceReturnsDestinationCulture)
{
	std::stringstream input;
	input << "link = {\n";
	input << "\teu4 = sourceCulture\n";
	input << "\tvic2 = destinationCulture\n";
	input << "}";

	mappers::CultureMapper theMapper(input);

	mockRegions regions;
	std::optional<std::string> match = theMapper.cultureMatch(regions, "sourceCulture", "", -1, "");
	ASSERT_TRUE(match);
}


TEST(Mappers_CultureMapperTests, ownerDistinguisherWrongNoMatch)
{
	std::stringstream input;
	input << "link = {\n";
	input << "\teu4 = sourceCulture\n";
	input << "\tvic2 = destinationCulture\n";
	input << "\towner = OWN\n";
	input << "}";

	mappers::CultureMapper theMapper(input);

	mockRegions regions;
	std::optional<std::string> match = theMapper.cultureMatch(regions, "nonMatchingCulture", "", -1, "NOT");
	ASSERT_FALSE(match);
}


TEST(Mappers_CultureMapperTests, ownerDistinguisherRightMatch)
{
	std::stringstream input;
	input << "link = {\n";
	input << "\teu4 = sourceCulture\n";
	input << "\tvic2 = destinationCulture\n";
	input << "\towner = OWN\n";
	input << "}";

	mappers::CultureMapper theMapper(input);

	mockRegions regions;
	std::optional<std::string> match = theMapper.cultureMatch(regions, "sourceCulture", "", -1, "OWN");
	ASSERT_TRUE(match);
}


TEST(Mappers_CultureMapperTests, religionDistinguisherWrongNoMatch)
{
	std::stringstream input;
	input << "link = {\n";
	input << "\teu4 = sourceCulture\n";
	input << "\tvic2 = destinationCulture\n";
	input << "\treligion = theReligion\n";
	input << "}";

	mappers::CultureMapper theMapper(input);

	mockRegions regions;
	std::optional<std::string> match = theMapper.cultureMatch(regions, "nonMatchingCulture", "wrongReligion", -1, "");
	ASSERT_FALSE(match);
}


TEST(Mappers_CultureMapperTests, religionDistinguisherRightMatch)
{
	std::stringstream input;
	input << "link = {\n";
	input << "\teu4 = sourceCulture\n";
	input << "\tvic2 = destinationCulture\n";
	input << "\treligion = theReligion\n";
	input << "}";

	mappers::CultureMapper theMapper(input);

	mockRegions regions;
	std::optional<std::string> match = theMapper.cultureMatch(regions, "sourceCulture", "theReligion", -1, "");
	ASSERT_TRUE(match);
}


TEST(Mappers_CultureMapperTests, provinceDistinguisherWrongNoMatch)
{
	std::stringstream input;
	input << "link = {\n";
	input << "\teu4 = sourceCulture\n";
	input << "\tvic2 = destinationCulture\n";
	input << "\tprovinceid = 42\n";
	input << "}";

	mappers::CultureMapper theMapper(input);

	mockRegions regions;
	std::optional<std::string> match = theMapper.cultureMatch(regions, "nonMatchingCulture", "", 21, "");
	ASSERT_FALSE(match);
}


TEST(Mappers_CultureMapperTests, provinceDistinguisherRightMatch)
{
	std::stringstream input;
	input << "link = {\n";
	input << "\teu4 = sourceCulture\n";
	input << "\tvic2 = destinationCulture\n";
	input << "\tprovinceid = 42\n";
	input << "}";

	mappers::CultureMapper theMapper(input);

	mockRegions regions;
	std::optional<std::string> match = theMapper.cultureMatch(regions, "sourceCulture", "", 42, "");
	ASSERT_TRUE(match);
}


TEST(Mappers_CultureMapperTests, regionDistinguisherWrongNoMatch)
{
	std::stringstream input;
	input << "link = {\n";
	input << "\teu4 = sourceCulture\n";
	input << "\tvic2 = destinationCulture\n";
	input << "\tregion = theRegion";
	input << "}";

	mappers::CultureMapper theMapper(input);

	mockRegions regions;
	EXPECT_CALL(regions, regionIsValid("theRegion")).WillOnce(testing::Return(true));
	EXPECT_CALL(regions, provinceInRegion(42, "theRegion")).WillOnce(testing::Return(false));

	std::optional<std::string> match = theMapper.cultureMatch(regions, "sourceCulture", "", 42, "theRegion");
	ASSERT_FALSE(match);
}


TEST(Mappers_CultureMapperTests, regionDistinguisherRightMatch)
{
	std::stringstream input;
	input << "link = {\n";
	input << "\teu4 = sourceCulture\n";
	input << "\tvic2 = destinationCulture\n";
	input << "\tregion = theRegion";
	input << "}";

	mappers::CultureMapper theMapper(input);

	mockRegions regions;
	EXPECT_CALL(regions, regionIsValid("theRegion")).WillOnce(testing::Return(true));
	EXPECT_CALL(regions, provinceInRegion(42, "theRegion")).WillOnce(testing::Return(true));

	std::optional<std::string> match = theMapper.cultureMatch(regions, "sourceCulture", "", 42, "");
	ASSERT_TRUE(match);
}


TEST(Mappers_CultureMapperTests, multipleSourceCulturesMatch)
{
	std::stringstream input;
	input << "link = {\n";
	input << "\teu4 = sourceCulture\n";
	input << "\teu4 = sourceCulture2\n";
	input << "\tvic2 = destinationCulture\n";
	input << "}";

	mappers::CultureMapper theMapper(input);

	mockRegions regions;
	std::optional<std::string> match = theMapper.cultureMatch(regions, "sourceCulture2", "", -1, "");
	ASSERT_TRUE(match);
}