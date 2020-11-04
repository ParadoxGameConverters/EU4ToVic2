#include "Provinces/PopRatio.h"
#include "gtest/gtest.h"

TEST(EU4World_Province_PopRatioTests, defaultConstructionCorrect)
{
	const EU4::PopRatio theRatio("theCulture", "theReligion");

	ASSERT_EQ("theCulture", theRatio.getCulture());
	ASSERT_EQ("theReligion", theRatio.getReligion());
	ASSERT_EQ(1.0, theRatio.getUpperRatio());
	ASSERT_EQ(1.0, theRatio.getMiddleRatio());
	ASSERT_EQ(1.0, theRatio.getLowerRatio());
}

TEST(EU4World_Province_PopRatioTests, decayIsByOneQuarterPercentPerYear)
{
	std::set<int> input;

	EU4::PopRatio theRatio("theCulture", "theReligion");
	theRatio.decay(1, 0.0025);

	ASSERT_NEAR(0.9975, theRatio.getUpperRatio(), 0.0001);
	ASSERT_NEAR(0.9975, theRatio.getMiddleRatio(), 0.0001);
	ASSERT_NEAR(0.9975, theRatio.getLowerRatio(), 0.0001);
}

TEST(EU4World_Province_PopRatioTests, increaseIsByOneQuarterPercentPerYear)
{
	std::set<int> input;

	EU4::PopRatio theRatio("theCulture", "theReligion"); // ratios are all at 1, we cannot increase them.
	theRatio.convertTo("newCulture", "newReligion");	  // halving and zeroing ratios to drive them lower.
	theRatio.increase(1, 0.0025);

	ASSERT_NEAR(0.50125, theRatio.getUpperRatio(), 0.0001);
	ASSERT_NEAR(0.50125, theRatio.getMiddleRatio(), 0.0001);
	ASSERT_NEAR(0.0025, theRatio.getLowerRatio(), 0.0001);
}

TEST(EU4World_Province_PopRatioTests, convertingFromDecreasesUpperAndMiddleRatio)
{
	std::set<int> input;

	EU4::PopRatio theRatio("theCulture", "theReligion");
	theRatio.convertFrom();

	ASSERT_NEAR(0.5, theRatio.getUpperRatio(), 0.001);
	ASSERT_NEAR(0.5, theRatio.getMiddleRatio(), 0.001);
}

TEST(EU4World_Province_PopRatioTests, convertingFromDoesNotDecreaseLowerRatio)
{
	std::set<int> input;

	EU4::PopRatio theRatio("theCulture", "theReligion");
	theRatio.convertFrom();

	ASSERT_EQ(1.0, theRatio.getLowerRatio());
}

TEST(EU4World_Province_PopRatioTests, convertingToCultureModifiesCulture)
{
	std::set<int> input;

	EU4::PopRatio theRatio("theCulture", "theReligion");
	theRatio.convertToCulture("newCulture");

	ASSERT_EQ("newCulture", theRatio.getCulture());
}

TEST(EU4World_Province_PopRatioTests, convertingToCultureHalvesUpperAndMiddleRatio)
{
	std::set<int> input;

	EU4::PopRatio theRatio("theCulture", "theReligion");
	theRatio.convertToCulture("newCulture");

	ASSERT_NEAR(0.5, theRatio.getUpperRatio(), 0.001);
	ASSERT_NEAR(0.5, theRatio.getMiddleRatio(), 0.001);
}

TEST(EU4World_Province_PopRatioTests, convertingToCultureZeroesLowerRatio)
{
	std::set<int> input;

	EU4::PopRatio theRatio("theCulture", "theReligion");
	theRatio.convertToCulture("newCulture");

	ASSERT_EQ(0.0, theRatio.getLowerRatio());
}

TEST(EU4World_Province_PopRatioTests, convertingToReligionModifiesReligion)
{
	std::set<int> input;

	EU4::PopRatio theRatio("theCulture", "theReligion");
	theRatio.convertToReligion("newReligion");

	ASSERT_EQ("newReligion", theRatio.getReligion());
}

TEST(EU4World_Province_PopRatioTests, convertingToReligionHalvesUpperAndMiddleRatio)
{
	std::set<int> input;

	EU4::PopRatio theRatio("theCulture", "theReligion");
	theRatio.convertToReligion("newReligion");

	ASSERT_NEAR(0.5, theRatio.getUpperRatio(), 0.001);
	ASSERT_NEAR(0.5, theRatio.getMiddleRatio(), 0.001);
}

TEST(EU4World_Province_PopRatioTests, convertingToReligionZeroesLowerRatio)
{
	std::set<int> input;

	EU4::PopRatio theRatio("theCulture", "theReligion");
	theRatio.convertToReligion("newReligion");

	ASSERT_EQ(0.0, theRatio.getLowerRatio());
}

TEST(EU4World_Province_PopRatioTests, convertingToModifiesReligionAndCulture)
{
	std::set<int> input;

	EU4::PopRatio theRatio("theCulture", "theReligion");
	theRatio.convertTo("newCulture", "newReligion");

	ASSERT_EQ("newCulture", theRatio.getCulture());
	ASSERT_EQ("newReligion", theRatio.getReligion());
}

TEST(EU4World_Province_PopRatioTests, convertingToHavesUpperAndMiddleRatio)
{
	std::set<int> input;

	EU4::PopRatio theRatio("theCulture", "theReligion");
	theRatio.convertTo("newCulture", "newReligion");

	ASSERT_NEAR(0.5, theRatio.getUpperRatio(), 0.001);
	ASSERT_NEAR(0.5, theRatio.getMiddleRatio(), 0.001);
}

TEST(EU4World_Province_PopRatioTests, convertingToZeroesLowerRatio)
{
	std::set<int> input;

	EU4::PopRatio theRatio("theCulture", "theReligion");
	theRatio.convertTo("newCulture", "newReligion");

	ASSERT_EQ(0.0, theRatio.getLowerRatio());
}