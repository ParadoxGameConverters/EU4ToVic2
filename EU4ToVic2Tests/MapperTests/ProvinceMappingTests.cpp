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



TEST(Mappers_ProvinceMappingTests, ZeroIsInsertedIfNoEU4Provinces)
{
	std::stringstream input;
	input << "= {}";

	mappers::ProvinceMapping theMapper(input);

	ASSERT_EQ(theMapper.getEU4Provinces().size(), 1);
	ASSERT_EQ(theMapper.getEU4Provinces()[0], 0);
}


TEST(Mappers_ProvinceMappingTests, EU4ProvinceCanBeAdded)
{
	std::stringstream input;
	input << "= { eu4 = 2 eu4 = 1 }";

	mappers::ProvinceMapping theMapper(input);

	ASSERT_EQ(theMapper.getEU4Provinces()[0], 2);
	ASSERT_EQ(theMapper.getEU4Provinces()[1], 1);
}


TEST(Mappers_ProvinceMappingTests, ZeroIsInsertedIfNoVic2Provinces)
{
	std::stringstream input;
	input << "= {}";

	mappers::ProvinceMapping theMapper(input);

	ASSERT_EQ(theMapper.getVic2Provinces().size(), 1);
	ASSERT_EQ(theMapper.getVic2Provinces()[0], 0);
}


TEST(Mappers_ProvinceMappingTests, Vic2ProvinceCanBeAdded)
{
	std::stringstream input;
	input << "= { v2 = 2 v2 = 1 }";

	mappers::ProvinceMapping theMapper(input);

	ASSERT_EQ(theMapper.getVic2Provinces()[0], 2);
	ASSERT_EQ(theMapper.getVic2Provinces()[1], 1);
}


TEST(Mappers_ProvinceMappingTests, resettableRegionsDefaultsToEmpty)
{
	std::stringstream input;
	input << "= {}";

	mappers::ProvinceMapping theMapper(input);

	ASSERT_EQ(theMapper.getResettableRegions().size(), 0);
}


TEST(Mappers_ProvinceMappingTests, resettableRegionsCanBeAdded)
{
	std::stringstream input;
	input << "= { resettable = regionOne resettable = regionTwo }";

	mappers::ProvinceMapping theMapper(input);

	ASSERT_EQ(theMapper.getResettableRegions().count("regionOne"), 1);
	ASSERT_EQ(theMapper.getResettableRegions().count("regionTwo"), 1);
}