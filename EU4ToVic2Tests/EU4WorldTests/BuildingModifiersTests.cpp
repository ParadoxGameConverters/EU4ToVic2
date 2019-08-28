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
#include "../EU4toV2/Source/EU4World/Buildings/BuildingModifiers.h"
#include <sstream>



TEST(EU4World_BuildingModifiersTests, modifiersDefaultToEmpty)
{
	std::stringstream input;
	EU4::BuildingModifiers theModifiers(input);

	ASSERT_EQ(theModifiers.getAllModifiers().size(), 0);
}


TEST(EU4World_BuildingModifiersTests, nonExistentModifierHasAmountOfZero)
{
	std::stringstream input;

	EU4::BuildingModifiers theModifiers(input);
	ASSERT_EQ(theModifiers.getModifierAmount("non_existent_modifier"), 0);
}


TEST(EU4World_BuildingModifiersTests, modifiercanBeAdded)
{
	std::stringstream input;
	input << "= {\n";
	input << "\tmodifier = 0.5\n";
	input << "}";

	EU4::BuildingModifiers theModifiers(input);

	ASSERT_EQ(theModifiers.getAllModifiers().size(), 1);
	ASSERT_EQ(theModifiers.getAllModifiers().at("modifier"), 0.5);
}


TEST(EU4World_BuildingModifiersTests, modifierAmountsAreReturned)
{
	std::stringstream input;
	input << "= {\n";
	input << "\tmodifier = 0.5\n";
	input << "}";

	EU4::BuildingModifiers theModifiers(input);
	ASSERT_EQ(theModifiers.getModifierAmount("modifier"), 0.5);
}


TEST(EU4World_BuildingModifiersTests, multipleOfSameModifierIsLogged)
{
	std::stringstream input;
	input << "= {\n";
	input << "\tmodifier = 0.5\n";
	input << "\tmodifier = 0.75\n";
	input << "}";

	std::stringstream buffer;
	std::streambuf* sbuf = std::cout.rdbuf();
	std::cout.rdbuf(buffer.rdbuf());
	EU4::BuildingModifiers theModifiers(input);
	std::cout.rdbuf(sbuf);
	
	std::stringstream expectedLog;
	expectedLog << "Multiple instances of modifier modifier in building.\n";

	ASSERT_EQ(expectedLog.str(), buffer.str());
}