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

TEST(EU4World_Province_PopRatioTests, decayByAssimilationValuePerYear)
{
	std::set<int> input;

	EU4::PopRatio theRatio("theCulture", "theReligion");
	theRatio.decay(1, 0.003); // default assimilation value "1" is 0.003 (factor = (1.0 - pow(0.7, value)) / 100;)

	ASSERT_NEAR(0.997, theRatio.getUpperRatio(), 0.0001);
	ASSERT_NEAR(0.997, theRatio.getMiddleRatio(), 0.0001);
	ASSERT_NEAR(0.997, theRatio.getLowerRatio(), 0.0001);
}

TEST(EU4World_Province_PopRatioTests, decayForHigherAssimilationValue)
{
	std::set<int> input;

	EU4::PopRatio theRatio("theCulture", "theReligion");
	theRatio.decay(1, 0.07599); // assimilation value "4" is 0.07599 (factor = (1.0 - pow(0.7, 4)) / 100;)

	ASSERT_NEAR(0.92401, theRatio.getUpperRatio(), 0.0001);
	ASSERT_NEAR(0.92401, theRatio.getMiddleRatio(), 0.0001);
	ASSERT_NEAR(0.92401, theRatio.getLowerRatio(), 0.0001);
}

TEST(EU4World_Province_PopRatioTests, decayByZeroAssimilationDoesNoAffectRatios)
{
	std::set<int> input;

	EU4::PopRatio theRatio("theCulture", "theReligion");
	theRatio.decay(1, 0); // No assimilation

	ASSERT_NEAR(1, theRatio.getUpperRatio(), 0.0001);
	ASSERT_NEAR(1, theRatio.getMiddleRatio(), 0.0001);
	ASSERT_NEAR(1, theRatio.getLowerRatio(), 0.0001);
}

TEST(EU4World_Province_PopRatioTests, increaseByDefaultAssimilationValue)
{
	std::set<int> input;

	EU4::PopRatio theRatio("theCulture", "theReligion"); // ratios are all at 1, we cannot increase them.
	theRatio.convertTo("newCulture", "newReligion");	  // halving and zeroing ratios to drive them lower.
	theRatio.increase(1, 0.003);								  // default assimilation value "1" is 0.003 (factor = (1.0 - pow(0.7, value)) / 100;)

	ASSERT_NEAR(0.5015, theRatio.getUpperRatio(), 0.0001);
	ASSERT_NEAR(0.5015, theRatio.getMiddleRatio(), 0.0001);
	ASSERT_NEAR(0.003, theRatio.getLowerRatio(), 0.0001);
}

TEST(EU4World_Province_PopRatioTests, increaseByHigherAssimilationValue)
{
	std::set<int> input;

	EU4::PopRatio theRatio("theCulture", "theReligion"); // ratios are all at 1, we cannot increase them.
	theRatio.convertTo("newCulture", "newReligion");	  // halving and zeroing ratios to drive them lower.
	theRatio.increase(1, 0.07599);							  // assimilation value "4" is 0.07599 (factor = (1.0 - pow(0.7, 4)) / 100;)

	ASSERT_NEAR(0.537995, theRatio.getUpperRatio(), 0.0001);
	ASSERT_NEAR(0.537995, theRatio.getMiddleRatio(), 0.0001);
	ASSERT_NEAR(0.07599, theRatio.getLowerRatio(), 0.0001);
}

TEST(EU4World_Province_PopRatioTests, increaseByZeroDoesNotAffectPopRatios)
{
	std::set<int> input;

	EU4::PopRatio theRatio("theCulture", "theReligion"); // ratios are all at 1, we cannot increase them.
	theRatio.convertTo("newCulture", "newReligion");	  // halving and zeroing ratios to drive them lower.
	theRatio.increase(1, 0);									  // No assimilation

	ASSERT_NEAR(0.5, theRatio.getUpperRatio(), 0.0001);
	ASSERT_NEAR(0.5, theRatio.getMiddleRatio(), 0.0001);
	ASSERT_NEAR(0, theRatio.getLowerRatio(), 0.0001);
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