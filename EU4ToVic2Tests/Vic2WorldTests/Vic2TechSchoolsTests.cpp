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
#include "../EU4toV2/Source/V2World/V2TechSchools.h"



TEST(Vic2World_Vic2TechSchoolsTests, DefaultSchoolIsTraditionalAcademic)
{
	std::stringstream emptyInput;
	std::unique_ptr<Vic2::blockedTechSchools> theBlockedTechSchools = std::make_unique<Vic2::blockedTechSchools>(emptyInput);
	Vic2::TechSchools newTechSchools(emptyInput, theBlockedTechSchools);

	ASSERT_EQ(std::string("traditional_academic"), newTechSchools.findBestTechSchool(0.2, 0.2, 0.2, 0.2, 0.2));
}


TEST(Vic2World_Vic2TechSchoolsTests, InvestmentchangesTechSchool)
{
	std::stringstream emptyInput;
	std::unique_ptr<Vic2::blockedTechSchools> theBlockedTechSchools = std::make_unique<Vic2::blockedTechSchools>(emptyInput);
	std::stringstream techSchoolsInput("traditional_academic = \
	{\
		army_tech_research_bonus = 0\
		commerce_tech_research_bonus = 0\
		culture_tech_research_bonus = 0\
		industry_tech_research_bonus = 0\
		navy_tech_research_bonus = 0\
	}\
	army_tech_school = \
	{\
		army_tech_research_bonus = 0.15\
		commerce_tech_research_bonus = 0\
		culture_tech_research_bonus = -0.1\
		industry_tech_research_bonus = 0.1\
		navy_tech_research_bonus = -0.05\
		unciv_economic_modifier = -0.05\
		unciv_military_modifier = -0.15\
	}");
	Vic2::TechSchools newTechSchools(techSchoolsInput, theBlockedTechSchools);

	ASSERT_NE(std::string("traditional_academic"), newTechSchools.findBestTechSchool(0.7, 0.1, 0.1, 0.1, 0.1));
}


TEST(Vic2World_Vic2TechSchoolsTests, PrussianTechSchoolBlocked)
{
	std::stringstream blockedTechSchoolInput("prussian_tech_school");
	std::unique_ptr<Vic2::blockedTechSchools> theBlockedTechSchools = std::make_unique<Vic2::blockedTechSchools>(blockedTechSchoolInput);
	std::stringstream techSchoolsInput("traditional_academic = \
	{\
		army_tech_research_bonus = 0\
		commerce_tech_research_bonus = 0\
		culture_tech_research_bonus = 0\
		industry_tech_research_bonus = 0\
		navy_tech_research_bonus = 0\
	}\
	prussian_tech_school = {\
		army_tech_research_bonus = 0.15\
		commerce_tech_research_bonus = 0\
		culture_tech_research_bonus = 0.05\
		industry_tech_research_bonus = 0.15\
		navy_tech_research_bonus = -0.15\
		unciv_economic_modifier = -0.05\
		unciv_military_modifier = -0.15\
	}");
	Vic2::TechSchools newTechSchools(techSchoolsInput, theBlockedTechSchools);

	ASSERT_NE(std::string("prussian_tech_school"), newTechSchools.findBestTechSchool(1.0, 0.1, 0.1, 1.0, 0.1));
}


TEST(Vic2World_Vic2TechSchoolsTests, NoInvestmentDoesntCrash)
{
	std::stringstream emptyInput;
	std::unique_ptr<Vic2::blockedTechSchools> theBlockedTechSchools = std::make_unique<Vic2::blockedTechSchools>(emptyInput);
	std::stringstream techSchoolsInput("traditional_academic = \
	{\
		army_tech_research_bonus = 0\
			commerce_tech_research_bonus = 0\
			culture_tech_research_bonus = 0\
			industry_tech_research_bonus = 0\
			navy_tech_research_bonus = 0\
	}\
	army_tech_school = \
	{\
		army_tech_research_bonus = 0.15\
		commerce_tech_research_bonus = 0\
		culture_tech_research_bonus = -0.1\
		industry_tech_research_bonus = 0.1\
		navy_tech_research_bonus = -0.05\
		unciv_economic_modifier = -0.05\
		unciv_military_modifier = -0.15\
	}");
	Vic2::TechSchools newTechSchools(techSchoolsInput, theBlockedTechSchools);

	ASSERT_NO_THROW(newTechSchools.findBestTechSchool(0.0, 0.0, 0.0, 0.0, 0.0));
}