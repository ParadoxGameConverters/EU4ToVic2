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
#include "../EU4toV2/Source/Mappers/IdeaEffects/IdeaEffects.h"
#include <sstream>



TEST(Mappers_IdeaEffectsTests, armyInvestmentValueDefaultsToZero)
{
	std::stringstream input;
	input << "= {\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getArmy(), 5);
}


TEST(Mappers_IdeaEffectsTests, armyInvestmentValueCanBeSet)
{
	std::stringstream input;
	input << "= {\n";
	input << "	army = 6\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getArmy(), 6);
}


TEST(Mappers_IdeaEffectsTests, navyInvestmentValueDefaultsToZero)
{
	std::stringstream input;
	input << "= {\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getNavy(), 5);
}


TEST(Mappers_IdeaEffectsTests, navyInvestmentValueCanBeSet)
{
	std::stringstream input;
	input << "= {\n";
	input << "	navy = 1\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getNavy(), 1);
}


TEST(Mappers_IdeaEffectsTests, commerceInvestmentValueDefaultsToZero)
{
	std::stringstream input;
	input << "= {\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getCommerce(), 5);
}


TEST(Mappers_IdeaEffectsTests, commerceInvestmentValueCanBeSet)
{
	std::stringstream input;
	input << "= {\n";
	input << "	commerce = 1\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getCommerce(), 1);
}


TEST(Mappers_IdeaEffectsTests, cultureInvestmentValueDefaultsToZero)
{
	std::stringstream input;
	input << "= {\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getCulture(), 5);
}


TEST(Mappers_IdeaEffectsTests, cultureInvestmentValueCanBeSet)
{
	std::stringstream input;
	input << "= {\n";
	input << "	culture = 1\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getCulture(), 1);
}


TEST(Mappers_IdeaEffectsTests, industryInvestmentValueDefaultsToZero)
{
	std::stringstream input;
	input << "= {\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getIndustry(), 5);
}


TEST(Mappers_IdeaEffectsTests, industryInvestmentValueCanBeSet)
{
	std::stringstream input;
	input << "= {\n";
	input << "	industry = 1\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getIndustry(), 1);
}


TEST(Mappers_IdeaEffectsTests, upperHouseLiberalValueDefaultsToZero)
{
	std::stringstream input;
	input << "= {\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getLiberal(), 5);
}


TEST(Mappers_IdeaEffectsTests, upperHouseLiberalValueCanBeSet)
{
	std::stringstream input;
	input << "= {\n";
	input << "	liberal = 4\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getLiberal(), 4);
}


TEST(Mappers_IdeaEffectsTests, upperHouseReactionaryValueDefaultsToZero)
{
	std::stringstream input;
	input << "= {\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getReactionary(), 5);
}


TEST(Mappers_IdeaEffectsTests, upperHouseReactionaryValueCanBeSet)
{
	std::stringstream input;
	input << "= {\n";
	input << "	reactionary = 7\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getReactionary(), 7);
}


TEST(Mappers_IdeaEffectsTests, orderInfluenceValueDefaultsToZero)
{
	std::stringstream input;
	input << "= {\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getOrder(), 5);
}


TEST(Mappers_IdeaEffectsTests, orderInfluenceValueCanBeSet)
{
	std::stringstream input;
	input << "= {\n";
	input << "	order = 1\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getOrder(), 1);
}


TEST(Mappers_IdeaEffectsTests, libertyInfluenceValueDefaultsToZero)
{
	std::stringstream input;
	input << "= {\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getLiberty(), 5);
}


TEST(Mappers_IdeaEffectsTests, libertyInfluenceValueCanBeSet)
{
	std::stringstream input;
	input << "= {\n";
	input << "	liberty = 1\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getLiberty(), 1);
}


TEST(Mappers_IdeaEffectsTests, equalityInfluenceValueDefaultsToZero)
{
	std::stringstream input;
	input << "= {\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getEquality(), 5);
}


TEST(Mappers_IdeaEffectsTests, equalityInfluenceValueCanBeSet)
{
	std::stringstream input;
	input << "= {\n";
	input << "	equality = 1\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getEquality(), 1);
}


TEST(Mappers_IdeaEffectsTests, literacyLevelsDefaultsToFive)
{
	std::stringstream input;
	input << "= {\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getLiteracy(), 5);
}


TEST(Mappers_IdeaEffectsTests, literacyLevelsCanBeSet)
{
	std::stringstream input;
	input << "= {\n";
	input << "	literacy = 7\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getLiteracy(), 7);
}

TEST(Mappers_IdeaEffectsTests, slaveryLevelsDefaultsToFive)
{
	std::stringstream input;
	input << "= {\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getSlavery(), 5);
}


TEST(Mappers_IdeaEffectsTests, slaveryLevelsCanBeSet)
{
	std::stringstream input;
	input << "= {\n";
	input << "	slavery = 7\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getSlavery(), 7);
}


TEST(Mappers_IdeaEffectsTests, upper_house_compositionLevelsDefaultsToFive)
{
	std::stringstream input;
	input << "= {\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getUpper_house_composition(), 5);
}


TEST(Mappers_IdeaEffectsTests, upper_house_compositionLevelsCanBeSet)
{
	std::stringstream input;
	input << "= {\n";
	input << "	upper_house_composition = 7\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getUpper_house_composition(), 7);
}


TEST(Mappers_IdeaEffectsTests, vote_franchiseLevelsDefaultsToFive)
{
	std::stringstream input;
	input << "= {\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getVote_franchise(), 5);
}


TEST(Mappers_IdeaEffectsTests, vote_franchiseLevelsCanBeSet)
{
	std::stringstream input;
	input << "= {\n";
	input << "	vote_franchise = 7\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getVote_franchise(), 7);
}


TEST(Mappers_IdeaEffectsTests, voting_systemLevelsDefaultsToFive)
{
	std::stringstream input;
	input << "= {\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getVoting_system(), 5);
}


TEST(Mappers_IdeaEffectsTests, voting_systemLevelsCanBeSet)
{
	std::stringstream input;
	input << "= {\n";
	input << "	voting_system = 7\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getVoting_system(), 7);
}


TEST(Mappers_IdeaEffectsTests, public_meetingsLevelsDefaultsToFive)
{
	std::stringstream input;
	input << "= {\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getPublic_meetings(), 5);
}


TEST(Mappers_IdeaEffectsTests, public_meetingsLevelsCanBeSet)
{
	std::stringstream input;
	input << "= {\n";
	input << "	public_meetings = 7\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getPublic_meetings(), 7);
}


TEST(Mappers_IdeaEffectsTests, press_rightsLevelsDefaultsToFive)
{
	std::stringstream input;
	input << "= {\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getPress_rights(), 5);
}


TEST(Mappers_IdeaEffectsTests, press_rightsLevelsCanBeSet)
{
	std::stringstream input;
	input << "= {\n";
	input << "	press_rights = 7\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getPress_rights(), 7);
}


TEST(Mappers_IdeaEffectsTests, trade_unionsLevelsDefaultsToFive)
{
	std::stringstream input;
	input << "= {\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getTrade_unions(), 5);
}


TEST(Mappers_IdeaEffectsTests, trade_unionsLevelsCanBeSet)
{
	std::stringstream input;
	input << "= {\n";
	input << "	trade_unions = 7\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getTrade_unions(), 7);
}


TEST(Mappers_IdeaEffectsTests, political_partiesLevelsDefaultsToFive)
{
	std::stringstream input;
	input << "= {\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getPolitical_parties(), 5);
}


TEST(Mappers_IdeaEffectsTests, political_partiesLevelsCanBeSet)
{
	std::stringstream input;
	input << "= {\n";
	input << "	political_parties = 7\n";
	input << "}";

	mappers::IdeaEffects ideaEffects(input);

	ASSERT_EQ(ideaEffects.getPolitical_parties(), 7);
}