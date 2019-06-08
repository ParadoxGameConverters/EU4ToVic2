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
#include "../EU4toV2/Source/Mappers/ProvinceMappings/ProvinceMapping.h"
#include <sstream>



TEST(Mappers_ProvinceMappingTests, EU4ProvincesDefaultToEmpty)
{
	std::stringstream input;
	input << "= {}";

	mappers::ProvinceMapping theMapper(input);

	ASSERT_EQ(theMapper.getEU4Provinces().size(), 0);
}


TEST(Mappers_ProvinceMappingTests, EU4ProvinceCanBeAdded)
{
	std::stringstream input;
	input << "= { eu4 = 1 eu4 = 2 }";

	mappers::ProvinceMapping theMapper(input);

	ASSERT_EQ(theMapper.getEU4Provinces().count(1), 1);
	ASSERT_EQ(theMapper.getEU4Provinces().count(2), 1);
}


TEST(Mappers_ProvinceMappingTests, Vic2ProvincesDefaultToEmpty)
{
	std::stringstream input;
	input << "= {}";

	mappers::ProvinceMapping theMapper(input);

	ASSERT_EQ(theMapper.getVic2Provinces().size(), 0);
}


TEST(Mappers_ProvinceMappingTests, Vic2ProvinceCanBeAdded)
{
	std::stringstream input;
	input << "= { v2 = 1 v2 = 2 }";

	mappers::ProvinceMapping theMapper(input);

	ASSERT_EQ(theMapper.getVic2Provinces().count(1), 1);
	ASSERT_EQ(theMapper.getVic2Provinces().count(2), 1);
}