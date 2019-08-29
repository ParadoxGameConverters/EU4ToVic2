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
#include "../EU4toV2/Source/EU4World/Buildings/Building.h"
#include <sstream>



TEST(EU4World_BuildingTests, costDefaultsToZero)
{
	std::stringstream input;
	EU4::Building theBuilding(input);

	ASSERT_EQ(theBuilding.getCost(), 0);
}


TEST(EU4World_BuildingTests, costCanBeSet)
{
	std::stringstream input;
	input << "= {\n";
	input << "\tcost = 100\n";
	input << "}";
	EU4::Building theBuilding(input);

	ASSERT_EQ(theBuilding.getCost(), 100);
}


TEST(EU4World_BuildingTests, modifiersDefaultToEmpty)
{
	std::stringstream input;
	EU4::Building theBuilding(input);

	ASSERT_EQ(theBuilding.getBuildingModifiers().getAllModifiers().size(), 0);
}


TEST(EU4World_BuildingTests, modifiersCanBeAdded)
{
	std::stringstream input;
	input << "= {\n";
	input << "\tmodifier = {\n";
	input << "\t\taModifier = 0.5\n";
	input << "\t\tanotherModifier = 0.75\n";
	input << "\t}\n";
	input << "}";
	EU4::Building theBuilding(input);

	ASSERT_EQ(theBuilding.getBuildingModifiers().getAllModifiers().size(), 2);
	ASSERT_EQ(theBuilding.getBuildingModifiers().getAllModifiers().at("aModifier"), 0.5);
	ASSERT_EQ(theBuilding.getBuildingModifiers().getAllModifiers().at("anotherModifier"), 0.75);
}


TEST(EU4World_BuildingTests, manufacturyDefaultsToFalse)
{
	std::stringstream input;
	EU4::Building theBuilding(input);

	ASSERT_FALSE(theBuilding.isManufactory());
}


TEST(EU4World_BuildingTests, manufacturyCanBeSet)
{
	std::stringstream input;
	input << "= {\n";
	input << "\tmanufactory = {\n";
	input << "\t\tnaval_supplies\n";
	input << "\t\tfish\n";
	input << "\t\tsalt\n";
	input << "\t}\n";
	input << "}";
	EU4::Building theBuilding(input);

	ASSERT_TRUE(theBuilding.isManufactory());
}