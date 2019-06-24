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
	input << "={\n";
	input << "-1={}";
	input << "}\n";
	input << "}";

	EU4::Provinces theProvinces(input);
	ASSERT_EQ(theProvinces.getAllProvinces().size(), 1);
}


TEST(EU4World_ProvincesTests, gettingNonExsistentProvinceThrowsException)
{
	std::stringstream input;
	input << "={\n";
	input << "={\n";
	input << "-1={}";
	input << "}\n";
	input << "}";

	EU4::Provinces theProvinces(input);
	ASSERT_THROW(theProvinces.getProvince(42), std::range_error);
}


TEST(EU4World_ProvincesTests, canGetProvince)
{
	std::stringstream input;
	input << "={\n";
	input << "={\n";
	input << "-1={}";
	input << "}\n";
	input << "}";

	EU4::Provinces theProvinces(input);
	ASSERT_EQ(theProvinces.getProvince(1).getNum(), 1);
}

/* No longet tested, as it requires file I/O
TEST(EU4World_ProvincesTests, checkAllProvincesMappedNotesMissingProvince)
{
	std::stringstream input;
	input << "={\n";
	input << "={\n";
	input << "-1={}";
	input << "}\n";
	input << "}";
	EU4::Provinces theProvinces(input);

	std::stringstream provinceMapperInput;
	provinceMapperInput << "0.0.0.0 = {\n";
	provinceMapperInput << "}";
	Configuration testConfig;
	mappers::ProvinceMapper mapper(provinceMapperInput, testConfig);

	std::stringstream buffer;
	std::streambuf* sbuf = std::cout.rdbuf();
	std::cout.rdbuf(buffer.rdbuf());
	theProvinces.checkAllProvincesMapped(mapper);
	std::cout.rdbuf(sbuf);

	ASSERT_EQ(buffer.str(), "No mapping for province 1\n");
}*/