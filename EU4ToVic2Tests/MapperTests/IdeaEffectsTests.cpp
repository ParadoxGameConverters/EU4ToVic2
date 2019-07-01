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
#include "../EU4toV2/Source/Mappers/Ideas/IdeaEffects.h"
#include <sstream>



TEST(Mappers_IdeaEffectsTests, armyInvestmentValueDefaultsToZero)
{
	std::stringstream input;
	input << "= {\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getArmyInvestmentValue(), 0);
}


TEST(Mappers_IdeaEffectsTests, armyInvestmentValueCanBeSet)
{
	std::stringstream input;
	input << "= {\n";
	input << "	army_investment = -1\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getArmyInvestmentValue(), -1);
}


TEST(Mappers_IdeaEffectsTests, navyInvestmentValueDefaultsToZero)
{
	std::stringstream input;
	input << "= {\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getNavyInvestmentValue(), 0);
}


TEST(Mappers_IdeaEffectsTests, navyInvestmentValueCanBeSet)
{
	std::stringstream input;
	input << "= {\n";
	input << "	navy_investment = 1\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getNavyInvestmentValue(), 1);
}


TEST(Mappers_IdeaEffectsTests, commerceInvestmentValueDefaultsToZero)
{
	std::stringstream input;
	input << "= {\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getCommerceInvestmentValue(), 0);
}


TEST(Mappers_IdeaEffectsTests, commerceInvestmentValueCanBeSet)
{
	std::stringstream input;
	input << "= {\n";
	input << "	commerce_investment = 1\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getCommerceInvestmentValue(), 1);
}


TEST(Mappers_IdeaEffectsTests, cultureInvestmentValueDefaultsToZero)
{
	std::stringstream input;
	input << "= {\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getCultureInvestmentValue(), 0);
}


TEST(Mappers_IdeaEffectsTests, cultureInvestmentValueCanBeSet)
{
	std::stringstream input;
	input << "= {\n";
	input << "	culture_investment = 1\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getCultureInvestmentValue(), 1);
}


TEST(Mappers_IdeaEffectsTests, industryInvestmentValueDefaultsToZero)
{
	std::stringstream input;
	input << "= {\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getIndustryInvestmentValue(), 0);
}


TEST(Mappers_IdeaEffectsTests, industryInvestmentValueCanBeSet)
{
	std::stringstream input;
	input << "= {\n";
	input << "	industry_investment = 1\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getIndustryInvestmentValue(), 1);
}


TEST(Mappers_IdeaEffectsTests, armyTechScoreValueDefaultsToZero)
{
	std::stringstream input;
	input << "= {\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getArmyTechScoreValue(), 0.0);
}


TEST(Mappers_IdeaEffectsTests, armyTechScoreValueCanBeSet)
{
	std::stringstream input;
	input << "= {\n";
	input << "	army_tech_score = 0.25\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getArmyTechScoreValue(), 0.25);
}


TEST(Mappers_IdeaEffectsTests, navyTechScoreValueDefaultsToZero)
{
	std::stringstream input;
	input << "= {\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getNavyTechScoreValue(), 0.0);
}


TEST(Mappers_IdeaEffectsTests, navyTechScoreValueCanBeSet)
{
	std::stringstream input;
	input << "= {\n";
	input << "	navy_tech_score = 0.25\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getNavyTechScoreValue(), 0.25);
}


TEST(Mappers_IdeaEffectsTests, commerceTechScoreValueDefaultsToZero)
{
	std::stringstream input;
	input << "= {\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getCommerceTechScoreValue(), 0.0);
}


TEST(Mappers_IdeaEffectsTests, commerceTechScoreValueCanBeSet)
{
	std::stringstream input;
	input << "= {\n";
	input << "	commerce_tech_score = 0.25\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getCommerceTechScoreValue(), 0.25);
}


TEST(Mappers_IdeaEffectsTests, cultureTechScoreValueDefaultsToZero)
{
	std::stringstream input;
	input << "= {\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getCultureTechScoreValue(), 0.0);
}


TEST(Mappers_IdeaEffectsTests, cultureTechScoreValueCanBeSet)
{
	std::stringstream input;
	input << "= {\n";
	input << "	culture_tech_score = 0.25\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getCultureTechScoreValue(), 0.25);
}


TEST(Mappers_IdeaEffectsTests, industryTechScoreValueDefaultsToZero)
{
	std::stringstream input;
	input << "= {\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getIndustryTechScoreValue(), 0.0);
}


TEST(Mappers_IdeaEffectsTests, industryTechScoreValueCanBeSet)
{
	std::stringstream input;
	input << "= {\n";
	input << "	industry_tech_score = 0.25\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getIndustryTechScoreValue(), 0.25);
}


TEST(Mappers_IdeaEffectsTests, upperHouseLiberalValueDefaultsToZero)
{
	std::stringstream input;
	input << "= {\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getUpperHouseLiberalValue(), 0.0);
}


TEST(Mappers_IdeaEffectsTests, upperHouseLiberalValueCanBeSet)
{
	std::stringstream input;
	input << "= {\n";
	input << "	upper_house_liberal = 0.25\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getUpperHouseLiberalValue(), 0.25);
}


TEST(Mappers_IdeaEffectsTests, upperHouseReactionaryValueDefaultsToZero)
{
	std::stringstream input;
	input << "= {\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getUpperHouseReactionaryValue(), 0.0);
}


TEST(Mappers_IdeaEffectsTests, upperHouseReactionaryValueCanBeSet)
{
	std::stringstream input;
	input << "= {\n";
	input << "	upper_house_reactionary = 0.25\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getUpperHouseReactionaryValue(), 0.25);
}


TEST(Mappers_IdeaEffectsTests, orderInfluenceValueDefaultsToZero)
{
	std::stringstream input;
	input << "= {\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getOrderInfluenceValue(), 0);
}


TEST(Mappers_IdeaEffectsTests, orderInfluenceValueCanBeSet)
{
	std::stringstream input;
	input << "= {\n";
	input << "	NV_order = 1\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getOrderInfluenceValue(), 1);
}


TEST(Mappers_IdeaEffectsTests, libertyInfluenceValueDefaultsToZero)
{
	std::stringstream input;
	input << "= {\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getLibertyInfluenceValue(), 0);
}


TEST(Mappers_IdeaEffectsTests, libertyInfluenceValueCanBeSet)
{
	std::stringstream input;
	input << "= {\n";
	input << "	NV_liberty = 1\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getLibertyInfluenceValue(), 1);
}


TEST(Mappers_IdeaEffectsTests, equalityInfluenceValueDefaultsToZero)
{
	std::stringstream input;
	input << "= {\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getLibertyInfluenceValue(), 0);
}


TEST(Mappers_IdeaEffectsTests, equalityInfluenceValueCanBeSet)
{
	std::stringstream input;
	input << "= {\n";
	input << "	NV_equality = 1\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getEqualityInfluenceValue(), 1);
}


TEST(Mappers_IdeaEffectsTests, literacyLevelsDefaultsToEmpty)
{
	std::stringstream input;
	input << "= {\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getLiteracyLevels().size(), 0);
}


TEST(Mappers_IdeaEffectsTests, literacyLevelsCanBeAdded)
{
	std::stringstream input;
	input << "= {\n";
	input << "	literacy = { 2 4 }\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getLiteracyLevels().size(), 2);
	ASSERT_EQ(ideaEffects.getLiteracyLevels().count(2), 1);
	ASSERT_EQ(ideaEffects.getLiteracyLevels().count(4), 1);
}