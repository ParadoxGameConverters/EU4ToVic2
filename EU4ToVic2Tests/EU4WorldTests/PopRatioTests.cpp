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
#include "../EU4toV2/Source/EU4World/Provinces/PopRatio.h"



TEST(EU4World_PopRatioTests, defaultConstructionCorrect)
{
	std::set<int> input;

	EU4::PopRatio theRatio("theCulture", "theReligion");

	ASSERT_EQ(theRatio.getCulture(), "theCulture");
	ASSERT_EQ(theRatio.getReligion(), "theReligion");
	ASSERT_EQ(theRatio.getUpperRatio(), 1.0);
	ASSERT_EQ(theRatio.getMiddleRatio(), 1.0);
	ASSERT_EQ(theRatio.getLowerRatio(), 1.0);
}


TEST(EU4World_PopRatioTests, decayIsByOneQuarterPercentPerYear)
{
	std::set<int> input;

	EU4::PopRatio theRatio("theCulture", "theReligion");
	theRatio.convertFrom();
	theRatio.decay(1, 0.0025);

	ASSERT_EQ(theRatio.getUpperRatio(), 0.49875);
	ASSERT_EQ(theRatio.getMiddleRatio(), 0.49875);
	ASSERT_EQ(theRatio.getLowerRatio(), 0.9975);
}


TEST(EU4World_PopRatioTests, increaseIsByOneQuarterPercentPerYear)
{
	std::set<int> input;

	EU4::PopRatio theRatio("theCulture", "theReligion");
	theRatio.convertTo("newCulture", "newReligion");
	theRatio.increase(1, 0.0025);

	ASSERT_EQ(theRatio.getUpperRatio(), 0.50125);
	ASSERT_EQ(theRatio.getMiddleRatio(), 0.50125);
	ASSERT_EQ(theRatio.getLowerRatio(), 0.0025);
}


TEST(EU4World_PopRatioTests, convertingFromDecreasesUpperRatio)
{
	std::set<int> input;

	EU4::PopRatio theRatio("theCulture", "theReligion");
	theRatio.convertFrom();

	ASSERT_EQ(theRatio.getUpperRatio(), 0.5);
}


TEST(EU4World_PopRatioTests, convertingFromDecreasesMiddleRatio)
{
	std::set<int> input;

	EU4::PopRatio theRatio("theCulture", "theReligion");
	theRatio.convertFrom();

	ASSERT_EQ(theRatio.getMiddleRatio(), 0.5);
}


TEST(EU4World_PopRatioTests, convertingFromDoesNotDecreaseLowerRatio)
{
	std::set<int> input;

	EU4::PopRatio theRatio("theCulture", "theReligion");
	theRatio.convertFrom();

	ASSERT_EQ(theRatio.getLowerRatio(), 1.0);
}


TEST(EU4World_PopRatioTests, convertingToCultureModifiesCulture)
{
	std::set<int> input;

	EU4::PopRatio theRatio("theCulture", "theReligion");
	theRatio.convertToCulture("newCulture");

	ASSERT_EQ(theRatio.getCulture(), "newCulture");
}


TEST(EU4World_PopRatioTests, convertingToCultureModifiesUpperRatio)
{
	std::set<int> input;

	EU4::PopRatio theRatio("theCulture", "theReligion");
	theRatio.convertToCulture("newCulture");

	ASSERT_EQ(theRatio.getUpperRatio(), 0.5);
}


TEST(EU4World_PopRatioTests, convertingToCultureModifiesMiddleRatio)
{
	std::set<int> input;

	EU4::PopRatio theRatio("theCulture", "theReligion");
	theRatio.convertToCulture("newCulture");

	ASSERT_EQ(theRatio.getMiddleRatio(), 0.5);
}


TEST(EU4World_PopRatioTests, convertingToCultureModifiesLowerRatio)
{
	std::set<int> input;

	EU4::PopRatio theRatio("theCulture", "theReligion");
	theRatio.convertToCulture("newCulture");

	ASSERT_EQ(theRatio.getLowerRatio(), 0.0);
}


TEST(EU4World_PopRatioTests, convertingToReligionModifiesReligion)
{
	std::set<int> input;

	EU4::PopRatio theRatio("theCulture", "theReligion");
	theRatio.convertToReligion("newReligion");

	ASSERT_EQ(theRatio.getReligion(), "newReligion");
}


TEST(EU4World_PopRatioTests, convertingToReligionModifiesUpperRatio)
{
	std::set<int> input;

	EU4::PopRatio theRatio("theCulture", "theReligion");
	theRatio.convertToReligion("newReligion");

	ASSERT_EQ(theRatio.getUpperRatio(), 0.5);
}


TEST(EU4World_PopRatioTests, convertingToReligionModifiesMiddleRatio)
{
	std::set<int> input;

	EU4::PopRatio theRatio("theCulture", "theReligion");
	theRatio.convertToReligion("newReligion");

	ASSERT_EQ(theRatio.getMiddleRatio(), 0.5);
}


TEST(EU4World_PopRatioTests, convertingToReligionModifiesLowerRatio)
{
	std::set<int> input;

	EU4::PopRatio theRatio("theCulture", "theReligion");
	theRatio.convertToReligion("newReligion");

	ASSERT_EQ(theRatio.getLowerRatio(), 0.0);
}


TEST(EU4World_PopRatioTests, convertingToModifiesCulture)
{
	std::set<int> input;

	EU4::PopRatio theRatio("theCulture", "theReligion");
	theRatio.convertTo("newCulture", "newReligion");

	ASSERT_EQ(theRatio.getCulture(), "newCulture");
}


TEST(EU4World_PopRatioTests, convertingToModifiesReligion)
{
	std::set<int> input;

	EU4::PopRatio theRatio("theCulture", "theReligion");
	theRatio.convertTo("newCulture", "newReligion");

	ASSERT_EQ(theRatio.getReligion(), "newReligion");
}


TEST(EU4World_PopRatioTests, convertingToModifiesUpperRatio)
{
	std::set<int> input;

	EU4::PopRatio theRatio("theCulture", "theReligion");
	theRatio.convertTo("newCulture", "newReligion");

	ASSERT_EQ(theRatio.getUpperRatio(), 0.5);
}


TEST(EU4World_PopRatioTests, convertingToModifiesMiddleRatio)
{
	std::set<int> input;

	EU4::PopRatio theRatio("theCulture", "theReligion");
	theRatio.convertTo("newCulture", "newReligion");

	ASSERT_EQ(theRatio.getMiddleRatio(), 0.5);
}


TEST(EU4World_PopRatioTests, convertingToModifiesLowerRatio)
{
	std::set<int> input;

	EU4::PopRatio theRatio("theCulture", "theReligion");
	theRatio.convertTo("newCulture", "newReligion");

	ASSERT_EQ(theRatio.getLowerRatio(), 0.0);
}