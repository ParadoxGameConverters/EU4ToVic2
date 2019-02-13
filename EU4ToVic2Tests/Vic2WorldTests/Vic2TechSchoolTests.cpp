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
#include "../EU4toV2/Source/V2World/Vic2TechSchool.h"



TEST(Vic2World_Vic2TechSchoolTests, DefaultNameIsEmpty)
{
	std::istringstream input("");
	Vic2::Vic2TechSchool newTechSchool("", input);
	ASSERT_EQ(std::string(""), newTechSchool.getName());
}

TEST(Vic2World_Vic2TechSchoolTests, NameSet)
{
	std::istringstream input("");
	Vic2::Vic2TechSchool newTechSchool("name", input);
	ASSERT_EQ(std::string("name"), newTechSchool.getName());
}


TEST(Vic2World_Vic2TechSchoolTests, MatchingInvestmentCausesScoreOfZero)
{
	std::istringstream input("");
	Vic2::Vic2TechSchool newTechSchool("", input);
	ASSERT_EQ(0, newTechSchool.calculateComparisonScore(0.2,0.2,0.2,0.2,0.2));
}

TEST(Vic2World_Vic2TechSchoolTests, IncreasingArmyInvestmentIncreasesScore)
{
	std::istringstream input("");
	Vic2::Vic2TechSchool newTechSchool("", input);
	ASSERT_LT(0, newTechSchool.calculateComparisonScore(0.3,0.2,0.2,0.2,0.2));
}

TEST(Vic2World_Vic2TechSchoolTests, DecreasingArmyInvestmentIncreasesScore)
{
	std::istringstream input("");
	Vic2::Vic2TechSchool newTechSchool("", input);
	ASSERT_LT(0, newTechSchool.calculateComparisonScore(0.1,0.2,0.2,0.2,0.2));
}

TEST(Vic2World_Vic2TechSchoolTests, ArmyBonusImported)
{
	std::istringstream input("{\n\t\tarmy_tech_research_bonus=0.1}");
	Vic2::Vic2TechSchool newTechSchool("", input);
	ASSERT_GT(0.0001, abs(newTechSchool.calculateComparisonScore(0.3,0.2,0.2,0.2,0.2)));
}

TEST(Vic2World_Vic2TechSchoolTests, IncreasingCommerceInvestmentIncreasesScore)
{
	std::istringstream input("");
	Vic2::Vic2TechSchool newTechSchool("", input);
	ASSERT_LT(0, newTechSchool.calculateComparisonScore(0.2,0.3,0.2,0.2,0.2));
}

TEST(Vic2World_Vic2TechSchoolTests, DecreasingCommerceInvestmentIncreasesScore)
{
	std::istringstream input("");
	Vic2::Vic2TechSchool newTechSchool("", input);
	ASSERT_LT(0, newTechSchool.calculateComparisonScore(0.2,0.1,0.2,0.2,0.2));
}

TEST(Vic2World_Vic2TechSchoolTests, CommerceBonusImported)
{
	std::istringstream input("{\n\t\tcommerce_tech_research_bonus=0.1}");
	Vic2::Vic2TechSchool newTechSchool("", input);
	ASSERT_GT(0.0001, abs(newTechSchool.calculateComparisonScore(0.2,0.3,0.2,0.2,0.2)));
}

TEST(Vic2World_Vic2TechSchoolTests, IncreasingCultureInvestmentIncreasesScore)
{
	std::istringstream input("");
	Vic2::Vic2TechSchool newTechSchool("", input);
	ASSERT_LT(0, newTechSchool.calculateComparisonScore(0.2,0.2,0.3,0.2,0.2));
}

TEST(Vic2World_Vic2TechSchoolTests, DecreasingCultureInvestmentIncreasesScore)
{
	std::istringstream input("");
	Vic2::Vic2TechSchool newTechSchool("", input);
	ASSERT_LT(0, newTechSchool.calculateComparisonScore(0.2,0.2,0.1,0.2,0.2));
}

TEST(Vic2World_Vic2TechSchoolTests, CultureBonusImported)
{
	std::istringstream input("{\n\t\tculture_tech_research_bonus=0.1}");
	Vic2::Vic2TechSchool newTechSchool("", input);
	ASSERT_GT(0.0001, abs(newTechSchool.calculateComparisonScore(0.2,0.2,0.3,0.2,0.2)));
}

TEST(Vic2World_Vic2TechSchoolTests, IncreasingIndustryInvestmentIncreasesScore)
{
	std::istringstream input("");
	Vic2::Vic2TechSchool newTechSchool("", input);
	ASSERT_LT(0, newTechSchool.calculateComparisonScore(0.2,0.2,0.2,0.3,0.2));
}

TEST(Vic2World_Vic2TechSchoolTests, DecreasingIndustryInvestmentIncreasesScore)
{
	std::istringstream input("");
	Vic2::Vic2TechSchool newTechSchool("", input);
	ASSERT_LT(0, newTechSchool.calculateComparisonScore(0.2,0.2,0.2,0.1,0.2));
}

TEST(Vic2World_Vic2TechSchoolTests, IndustryBonusImported)
{
	std::istringstream input("{\n\t\tindustry_tech_research_bonus=0.1}");
	Vic2::Vic2TechSchool newTechSchool("", input);
	ASSERT_GT(0.0001, abs(newTechSchool.calculateComparisonScore(0.2,0.2,0.2,0.3,0.2)));
}

TEST(Vic2World_Vic2TechSchoolTests, IncreasingNavyInvestmentIncreasesScore)
{
	std::istringstream input("");
	Vic2::Vic2TechSchool newTechSchool("", input);
	ASSERT_LT(0, newTechSchool.calculateComparisonScore(0.2,0.2,0.2,0.2,0.3));
}

TEST(Vic2World_Vic2TechSchoolTests, DecreasingNavyInvestmentIncreasesScore)
{
	std::istringstream input("");
	Vic2::Vic2TechSchool newTechSchool("", input);
	ASSERT_LT(0, newTechSchool.calculateComparisonScore(0.2,0.2,0.2,0.2,0.1));
}

TEST(Vic2World_Vic2TechSchoolTests, NavyBonusImported)
{
	std::istringstream input("{\n\t\tnavy_tech_research_bonus=0.1}");
	Vic2::Vic2TechSchool newTechSchool("", input);
	ASSERT_GT(0.0001, abs(newTechSchool.calculateComparisonScore(0.2,0.2,0.2,0.2,0.3)));
}