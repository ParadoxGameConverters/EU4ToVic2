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
#include "../EU4toV2/Source/Mappers/Ideas/IdeaEffectMapper.h"
#include <sstream>



TEST(Mappers_IdeaEffectMapperTests, getArmyInvestmentFromIdeaReturnsZeroForMissingIdea)
{
	std::stringstream input;
	mappers::IdeaEffectMapper theMapper(input);

	auto investment = theMapper.getArmyFromIdea("missingIdea", 1);
	ASSERT_EQ(investment, 5);
}


TEST(Mappers_IdeaEffectMapperTests, getArmyInvestmentFromIdeaDefaultsToZero)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getArmyFromIdea("specifiedIdea", 1), 5);
}


TEST(Mappers_IdeaEffectMapperTests, getArmyInvestmentFromIdeaCanBeSet)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tarmy = 1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getArmyFromIdea("specifiedIdea", 1), 1);
}



TEST(Mappers_IdeaEffectMapperTests, getCommerceInvestmentFromIdeaReturnsZeroForMissingIdea)
{
	std::stringstream input;
	mappers::IdeaEffectMapper theMapper(input);

	auto investment = theMapper.getCommerceFromIdea("missingIdea", 1);
	ASSERT_EQ(investment, 0);
}


TEST(Mappers_IdeaEffectMapperTests, getCommerceInvestmentFromIdeaDefaultsToZero)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getCommerceFromIdea("specifiedIdea", 1), 0);
}


TEST(Mappers_IdeaEffectMapperTests, getCommerceInvestmentFromIdeaCanBeSet)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tcommerce_investment=1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getCommerceFromIdea("specifiedIdea", 1), 1);
}


TEST(Mappers_IdeaEffectMapperTests, getCommerceInvestmentFromIdeaCanBeNegative)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tcommerce_investment=-1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getCommerceFromIdea("specifiedIdea", 1), -1);
}


TEST(Mappers_IdeaEffectMapperTests, getCommerceInvestmentFromIdeaScalesByIdeaLevel)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tcommerce_investment=1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getCommerceFromIdea("specifiedIdea", 5), 5);
}


TEST(Mappers_IdeaEffectMapperTests, getCultureInvestmentFromIdeaReturnsZeroForMissingIdea)
{
	std::stringstream input;
	mappers::IdeaEffectMapper theMapper(input);

	auto investment = theMapper.getCultureFromIdea("missingIdea", 1);
	ASSERT_EQ(investment, 0);
}


TEST(Mappers_IdeaEffectMapperTests, getCultureInvestmentFromIdeaDefaultsToZero)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getCultureFromIdea("specifiedIdea", 1), 0);
}


TEST(Mappers_IdeaEffectMapperTests, getCultureInvestmentFromIdeaCanBeSet)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tculture_investment=1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getCultureFromIdea("specifiedIdea", 1), 1);
}


TEST(Mappers_IdeaEffectMapperTests, getCultureInvestmentFromIdeaCanBeNegative)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tculture_investment=-1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getCultureFromIdea("specifiedIdea", 1), -1);
}


TEST(Mappers_IdeaEffectMapperTests, getCultureInvestmentFromIdeaScalesByIdeaLevel)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tculture_investment=1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getCultureFromIdea("specifiedIdea", 5), 5);
}


TEST(Mappers_IdeaEffectMapperTests, getIndustryInvestmentFromIdeaReturnsZeroForMissingIdea)
{
	std::stringstream input;
	mappers::IdeaEffectMapper theMapper(input);

	auto investment = theMapper.getIndustryFromIdea("missingIdea", 1);
	ASSERT_EQ(investment, 0);
}


TEST(Mappers_IdeaEffectMapperTests, getIndustryInvestmentFromIdeaDefaultsToZero)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getIndustryFromIdea("specifiedIdea", 1), 0);
}


TEST(Mappers_IdeaEffectMapperTests, getIndustryInvestmentFromIdeaCanBeSet)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tindustry_investment=1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getIndustryFromIdea("specifiedIdea", 1), 1);
}


TEST(Mappers_IdeaEffectMapperTests, getIndustryInvestmentFromIdeaCanBeNegative)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tindustry_investment=-1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getIndustryFromIdea("specifiedIdea", 1), -1);
}


TEST(Mappers_IdeaEffectMapperTests, getIndustryInvestmentFromIdeaScalesByIdeaLevel)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tindustry_investment=1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getIndustryFromIdea("specifiedIdea", 5), 5);
}


TEST(Mappers_IdeaEffectMapperTests, getNavyInvestmentFromIdeaReturnsZeroForMissingIdea)
{
	std::stringstream input;
	mappers::IdeaEffectMapper theMapper(input);

	auto investment = theMapper.getNavyFromIdea("missingIdea", 1);
	ASSERT_EQ(investment, 0);
}


TEST(Mappers_IdeaEffectMapperTests, getNavyInvestmentFromIdeaDefaultsToZero)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getNavyFromIdea("specifiedIdea", 1), 0);
}


TEST(Mappers_IdeaEffectMapperTests, getNavyInvestmentFromIdeaCanBeSet)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tnavy_investment=1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getNavyFromIdea("specifiedIdea", 1), 1);
}


TEST(Mappers_IdeaEffectMapperTests, getNavyInvestmentFromIdeaCanBeNegative)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tnavy_investment=-1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getNavyFromIdea("specifiedIdea", 1), -1);
}


TEST(Mappers_IdeaEffectMapperTests, getNavyInvestmentFromIdeaScalesByIdeaLevel)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tnavy_investment=1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getNavyFromIdea("specifiedIdea", 5), 5);
}


TEST(Mappers_IdeaEffectMapperTests, getUHLiberalFromIdeaReturnsZeroForMissingIdea)
{
	std::stringstream input;
	mappers::IdeaEffectMapper theMapper(input);

	auto UHLiberal = theMapper.getLiberalFromIdea("missingIdea", 0);
	ASSERT_NEAR(UHLiberal, 0.0, 0.0000001);
}


TEST(Mappers_IdeaEffectMapperTests, getUHLiberalFromIdeaDefaultsToZero)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_NEAR(theMapper.getLiberalFromIdea("specifiedIdea", 0), 0.0, 0.0000001);
}


TEST(Mappers_IdeaEffectMapperTests, getUHLiberalFromIdeaCanBeSet)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tupper_house_liberal=0.0125";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_NEAR(theMapper.getLiberalFromIdea("specifiedIdea", 0), 0.0125, 0.0000001);
}


TEST(Mappers_IdeaEffectMapperTests, getUHLiberalFromIdeaScalesByIdeaLevel)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tupper_house_liberal=0.0125";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_NEAR(theMapper.getLiberalFromIdea("specifiedIdea", 3), 0.05, 0.0000001);
}


TEST(Mappers_IdeaEffectMapperTests, getUHReactionaryFromIdeaReturnsZeroForMissingIdea)
{
	std::stringstream input;
	mappers::IdeaEffectMapper theMapper(input);

	auto UHRadical = theMapper.getReactionaryFromIdea("missingIdea", 0);
	ASSERT_NEAR(UHRadical, 0.0, 0.0000001);
}


TEST(Mappers_IdeaEffectMapperTests, getUHReactionaryFromIdeaDefaultsToZero)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_NEAR(theMapper.getReactionaryFromIdea("specifiedIdea", 0), 0.0, 0.0000001);
}


TEST(Mappers_IdeaEffectMapperTests, getUHReactionaryFromIdeaCanBeSet)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tupper_house_reactionary=0.0125";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_NEAR(theMapper.getReactionaryFromIdea("specifiedIdea", 0), 0.0125, 0.0000001);
}


TEST(Mappers_IdeaEffectMapperTests, getUHReactionaryFromIdeaScalesByIdeaLevel)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tupper_house_reactionary=0.0125";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_NEAR(theMapper.getReactionaryFromIdea("specifiedIdea", 3), 0.05, 0.0000001);
}


TEST(Mappers_IdeaEffectMapperTests, getLiteracyFromIdeaReturnsZeroForMissingIdea)
{
	std::stringstream input;
	mappers::IdeaEffectMapper theMapper(input);

	auto literacy = theMapper.getLiteracyFromIdea("missingIdea", 0);
	ASSERT_NEAR(literacy, 0.0, 0.0000001);
}


TEST(Mappers_IdeaEffectMapperTests, getLiteracyFromIdeaDefaultsToZero)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_NEAR(theMapper.getLiteracyFromIdea("specifiedIdea", 7), 0.00, 0.0000001);
}


TEST(Mappers_IdeaEffectMapperTests, getLiteracyFromIdeaCanBeSet)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tliteracy={ 2 }\n";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_NEAR(theMapper.getLiteracyFromIdea("specifiedIdea", 7), 0.1, 0.0000001);
}


TEST(Mappers_IdeaEffectMapperTests, getLiteracyFromIdeaCanBeSetByMultipleLevels)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tliteracy={ 2 4 }\n";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_NEAR(theMapper.getLiteracyFromIdea("specifiedIdea", 7), 0.2, 0.0000001);
}


TEST(Mappers_IdeaEffectMapperTests, getLiteracyFromIdeaCanBeSetByOutOfOrderLevels)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tliteracy={ 4 2 }\n";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_NEAR(theMapper.getLiteracyFromIdea("specifiedIdea", 7), 0.2, 0.0000001);
}


TEST(Mappers_IdeaEffectMapperTests, getOrderInfluenceFromIdeaReturnsZeroForMissingIdea)
{
	std::stringstream input;
	mappers::IdeaEffectMapper theMapper(input);

	auto investment = theMapper.getOrderFromIdea("missingIdea", 1);
	ASSERT_EQ(investment, 0);
}


TEST(Mappers_IdeaEffectMapperTests, getOrderInfluenceFromIdeaDefaultsToZero)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getOrderFromIdea("specifiedIdea", 1), 0);
}


TEST(Mappers_IdeaEffectMapperTests, getOrderInfluenceFromIdeaCanBeSet)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tNV_order=1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getOrderFromIdea("specifiedIdea", 0), 1);
}


TEST(Mappers_IdeaEffectMapperTests, getOrderInfluenceFromIdeaCanBeNegative)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tNV_order=-1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getOrderFromIdea("specifiedIdea", 0), -1);
}


TEST(Mappers_IdeaEffectMapperTests, getOrderInfluenceFromIdeaScalesByIdeaLevel)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tNV_order=1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getOrderFromIdea("specifiedIdea", 5), 6);
}


TEST(Mappers_IdeaEffectMapperTests, getOrderInfluenceFromIdeaGivesBonusForCompletingTheIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tNV_order=1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getOrderFromIdea("specifiedIdea", 7), 9);
}


TEST(Mappers_IdeaEffectMapperTests, getLibertyInfluenceFromIdeaReturnsZeroForMissingIdea)
{
	std::stringstream input;
	mappers::IdeaEffectMapper theMapper(input);

	auto investment = theMapper.getLibertyFromIdea("missingIdea", 1);
	ASSERT_EQ(investment, 0);
}


TEST(Mappers_IdeaEffectMapperTests, getLibertyInfluenceFromIdeaDefaultsToZero)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getLibertyFromIdea("specifiedIdea", 1), 0);
}


TEST(Mappers_IdeaEffectMapperTests, getLibertyInfluenceFromIdeaCanBeSet)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tNV_liberty=1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getLibertyFromIdea("specifiedIdea", 0), 1);
}


TEST(Mappers_IdeaEffectMapperTests, getLibertyInfluenceFromIdeaCanBeNegative)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tNV_liberty=-1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getLibertyFromIdea("specifiedIdea", 0), -1);
}


TEST(Mappers_IdeaEffectMapperTests, getLibertyInfluenceFromIdeaScalesByIdeaLevel)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tNV_liberty=1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getLibertyFromIdea("specifiedIdea", 5), 6);
}


TEST(Mappers_IdeaEffectMapperTests, getLibertyInfluenceFromIdeaGivesBonusForCompletingTheIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tNV_liberty=1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getLibertyFromIdea("specifiedIdea", 7), 9);
}


TEST(Mappers_IdeaEffectMapperTests, getEqualityInfluenceFromIdeaReturnsZeroForMissingIdea)
{
	std::stringstream input;
	mappers::IdeaEffectMapper theMapper(input);

	auto investment = theMapper.getEqualityFromIdea("missingIdea", 1);
	ASSERT_EQ(investment, 0);
}


TEST(Mappers_IdeaEffectMapperTests, getEqualityInfluenceFromIdeaDefaultsToZero)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getEqualityFromIdea("specifiedIdea", 1), 0);
}


TEST(Mappers_IdeaEffectMapperTests, getEqualityInfluenceFromIdeaCanBeSet)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tNV_equality=1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getEqualityFromIdea("specifiedIdea", 0), 1);
}


TEST(Mappers_IdeaEffectMapperTests, getEqualityInfluenceFromIdeaCanBeNegative)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tNV_equality=-1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getEqualityFromIdea("specifiedIdea", 0), -1);
}


TEST(Mappers_IdeaEffectMapperTests, getEqualityInfluenceFromIdeaScalesByIdeaLevel)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tNV_equality=1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getEqualityFromIdea("specifiedIdea", 5), 6);
}


TEST(Mappers_IdeaEffectMapperTests, getEqualityInfluenceFromIdeaGivesBonusForCompletingTheIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tNV_equality =1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getEqualityFromIdea("specifiedIdea", 7), 9);
}

