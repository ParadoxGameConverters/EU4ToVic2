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
#include "../EU4toV2/Source/Mappers/ProvinceMappings/ProvinceMapper.h"
#include "../EU4toV2/Source/Configuration.h"
#include <sstream>



TEST(Mappers_ProvinceMapperTests, emptyMappingsDefaultToEmpty)
{
	std::stringstream input;
	input << "0.0.0.0 = {\n";
	input << "}";

	Configuration testConfiguration;
	mappers::ProvinceMapper theMapper(input, testConfiguration);

	ASSERT_EQ(theMapper.getVic2ProvinceNumbers(1).size(), 0);
}


TEST(Mappers_ProvinceMapperTests, canLookupVic2Provinces)
{
	std::stringstream input;
	input << "0.0.0.0 = {\n";
	input << "	link = { eu4 = 2 eu4 = 1 v2 = 2 v2 = 1 }\n";
	input << "}";

	Configuration testConfiguration;
	mappers::ProvinceMapper theMapper(input, testConfiguration);

	ASSERT_EQ(theMapper.getVic2ProvinceNumbers(1).size(), 2);
	ASSERT_EQ(theMapper.getVic2ProvinceNumbers(1)[0], 2);
	ASSERT_EQ(theMapper.getVic2ProvinceNumbers(1)[1], 1);
	ASSERT_EQ(theMapper.getVic2ProvinceNumbers(2).size(), 2);
	ASSERT_EQ(theMapper.getVic2ProvinceNumbers(2)[0], 2);
	ASSERT_EQ(theMapper.getVic2ProvinceNumbers(2)[1], 1);
}


TEST(Mappers_ProvinceMapperTests, canLookupEU4Provinces)
{
	std::stringstream input;
	input << "0.0.0.0 = {\n";
	input << "	link = { eu4 = 2 eu4 = 1 v2 = 2 v2 = 1 }\n";
	input << "}";

	Configuration testConfiguration;
	mappers::ProvinceMapper theMapper(input, testConfiguration);

	ASSERT_EQ(theMapper.getEU4ProvinceNumbers(1).size(), 2);
	ASSERT_EQ(theMapper.getEU4ProvinceNumbers(1)[0], 2);
	ASSERT_EQ(theMapper.getEU4ProvinceNumbers(1)[1], 1);
	ASSERT_EQ(theMapper.getEU4ProvinceNumbers(2).size(), 2);
	ASSERT_EQ(theMapper.getEU4ProvinceNumbers(2)[0], 2);
	ASSERT_EQ(theMapper.getEU4ProvinceNumbers(2)[1], 1);
}


TEST(Mappers_ProvinceMapperTests, properVesionIsUsed)
{
	std::stringstream input;
	input << "0.0.0.0 = {\n";
	input << "	link = { eu4 = 2 eu4 = 1 v2 = 2 v2 = 1 }\n";
	input << "}";
	input << "1.0.0.0 = {\n";
	input << "	link = { eu4 = 2 eu4 = 1 v2 = 3 v2 = 1 }\n";
	input << "}";
	input << "1.2.0.0 = {\n";
	input << "	link = { eu4 = 2 eu4 = 1 v2 = 4 v2 = 1 }\n";
	input << "}";

	Configuration testConfiguration;
	EU4::Version testVersion("1.1.0.0");
	testConfiguration.setEU4Version(testVersion);
	mappers::ProvinceMapper theMapper(input, testConfiguration);

	ASSERT_EQ(theMapper.getVic2ProvinceNumbers(1).size(), 2);
	ASSERT_EQ(theMapper.getVic2ProvinceNumbers(1)[0], 3);
	ASSERT_EQ(theMapper.getVic2ProvinceNumbers(1)[1], 1);
}


TEST(Mappers_ProvinceMapperTests, resettableProvicnesCanBeFound)
{
	std::stringstream input;
	input << "0.0.0.0 = {\n";
	input << "	link = { eu4 = 2 eu4 = 1 v2 = 2 v2 = 1 resettable = testResettable }\n";
	input << "	link = { eu4 = 4 eu4 = 3 v2 = 4 v2 = 3 resettable = testResettable }\n";
	input << "}";

	Configuration testConfiguration;
	mappers::ProvinceMapper theMapper(input, testConfiguration);

	ASSERT_EQ(theMapper.isProvinceResettable(1, "testResettable"), true);
	ASSERT_EQ(theMapper.isProvinceResettable(2, "testResettable"), true);
	ASSERT_EQ(theMapper.isProvinceResettable(3, "testResettable"), true);
	ASSERT_EQ(theMapper.isProvinceResettable(4, "testResettable"), true);
}