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



TEST(Mappers_IdeaEffectMapperTests, getArmyFromIdeaReturnsFiveForMissingIdea)
{
	std::stringstream input;
	mappers::IdeaEffectMapper theMapper(input);

	auto investment = theMapper.getArmyFromIdea("missingIdea", 1);
	ASSERT_EQ(investment, 5);
}


TEST(Mappers_IdeaEffectMapperTests, getArmyFromIdeaDefaultsToFive)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getArmyFromIdea("specifiedIdea", 1), 5);
}


TEST(Mappers_IdeaEffectMapperTests, getArmyFromIdeaCanBeSetForCompletedIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tarmy = 1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getArmyFromIdea("specifiedIdea", 7), 1);
}


TEST(Mappers_IdeaEffectMapperTests, getArmyFromIdeaCannotBeSetForIncompleteIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tarmy = 1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getArmyFromIdea("specifiedIdea", 4), 5);
}


TEST(Mappers_IdeaEffectMapperTests, getNavyFromIdeaReturnsFiveForMissingIdea)
{
	std::stringstream input;
	mappers::IdeaEffectMapper theMapper(input);

	auto investment = theMapper.getNavyFromIdea("missingIdea", 1);
	ASSERT_EQ(investment, 5);
}


TEST(Mappers_IdeaEffectMapperTests, getNavyFromIdeaDefaultsToFive)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getNavyFromIdea("specifiedIdea", 1), 5);
}


TEST(Mappers_IdeaEffectMapperTests, getNavyFromIdeaCanBeSetForCompletedIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tnavy = 1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getNavyFromIdea("specifiedIdea", 7), 1);
}


TEST(Mappers_IdeaEffectMapperTests, getNavyFromIdeaCannotBeSetForIncompleteIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tnavy = 1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getNavyFromIdea("specifiedIdea", 4), 5);
}


TEST(Mappers_IdeaEffectMapperTests, getCommerceFromIdeaReturnsFiveForMissingIdea)
{
	std::stringstream input;
	mappers::IdeaEffectMapper theMapper(input);

	auto investment = theMapper.getCommerceFromIdea("missingIdea", 1);
	ASSERT_EQ(investment, 5);
}


TEST(Mappers_IdeaEffectMapperTests, getCommerceFromIdeaDefaultsToFive)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getCommerceFromIdea("specifiedIdea", 1), 5);
}


TEST(Mappers_IdeaEffectMapperTests, getCommerceFromIdeaCanBeSetForCompletedIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tcommerce = 1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getCommerceFromIdea("specifiedIdea", 7), 1);
}


TEST(Mappers_IdeaEffectMapperTests, getCommerceFromIdeaCannotBeSetForIncompleteIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tcommerce = 1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getCommerceFromIdea("specifiedIdea", 4), 5);
}


TEST(Mappers_IdeaEffectMapperTests, getCultureFromIdeaReturnsFiveForMissingIdea)
{
	std::stringstream input;
	mappers::IdeaEffectMapper theMapper(input);

	auto investment = theMapper.getCultureFromIdea("missingIdea", 1);
	ASSERT_EQ(investment, 5);
}


TEST(Mappers_IdeaEffectMapperTests, getCultureFromIdeaDefaultsToFive)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getCultureFromIdea("specifiedIdea", 1), 5);
}


TEST(Mappers_IdeaEffectMapperTests, getCultureFromIdeaCanBeSetForCompletedIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tculture = 1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getCultureFromIdea("specifiedIdea", 7), 1);
}


TEST(Mappers_IdeaEffectMapperTests, getCultureFromIdeaCannotBeSetForIncompleteIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tculture = 1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getCultureFromIdea("specifiedIdea", 4), 5);
}


TEST(Mappers_IdeaEffectMapperTests, getIndustryFromIdeaReturnsFiveForMissingIdea)
{
	std::stringstream input;
	mappers::IdeaEffectMapper theMapper(input);

	auto investment = theMapper.getIndustryFromIdea("missingIdea", 1);
	ASSERT_EQ(investment, 5);
}


TEST(Mappers_IdeaEffectMapperTests, getIndustryFromIdeaDefaultsToFive)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getIndustryFromIdea("specifiedIdea", 1), 5);
}


TEST(Mappers_IdeaEffectMapperTests, getIndustryFromIdeaCanBeSetForCompletedIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tindustry = 1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getIndustryFromIdea("specifiedIdea", 7), 1);
}


TEST(Mappers_IdeaEffectMapperTests, getIndustryFromIdeaCannotBeSetForIncompleteIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tindustry = 1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getIndustryFromIdea("specifiedIdea", 4), 5);
}


TEST(Mappers_IdeaEffectMapperTests, getLibertyFromIdeaReturnsFiveForMissingIdea)
{
	std::stringstream input;
	mappers::IdeaEffectMapper theMapper(input);

	auto investment = theMapper.getLibertyFromIdea("missingIdea", 1);
	ASSERT_EQ(investment, 5);
}


TEST(Mappers_IdeaEffectMapperTests, getLibertyFromIdeaDefaultsToFive)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getLibertyFromIdea("specifiedIdea", 1), 5);
}


TEST(Mappers_IdeaEffectMapperTests, getLibertyFromIdeaCanBeSetForCompletedIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tliberty = 1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getLibertyFromIdea("specifiedIdea", 7), 1);
}


TEST(Mappers_IdeaEffectMapperTests, getLibertyFromIdeaCannotBeSetForIncompleteIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tliberty = 1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getLibertyFromIdea("specifiedIdea", 4), 5);
}


TEST(Mappers_IdeaEffectMapperTests, getEqualityFromIdeaReturnsFiveForMissingIdea)
{
	std::stringstream input;
	mappers::IdeaEffectMapper theMapper(input);

	auto investment = theMapper.getEqualityFromIdea("missingIdea", 1);
	ASSERT_EQ(investment, 5);
}


TEST(Mappers_IdeaEffectMapperTests, getEqualityFromIdeaDefaultsToFive)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getEqualityFromIdea("specifiedIdea", 1), 5);
}


TEST(Mappers_IdeaEffectMapperTests, getEqualityFromIdeaCanBeSetForCompletedIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tequality = 1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getEqualityFromIdea("specifiedIdea", 7), 1);
}


TEST(Mappers_IdeaEffectMapperTests, getEqualityFromIdeaCannotBeSetForIncompleteIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tequality = 1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getEqualityFromIdea("specifiedIdea", 4), 5);
}


TEST(Mappers_IdeaEffectMapperTests, getOrderFromIdeaReturnsFiveForMissingIdea)
{
	std::stringstream input;
	mappers::IdeaEffectMapper theMapper(input);

	auto investment = theMapper.getOrderFromIdea("missingIdea", 1);
	ASSERT_EQ(investment, 5);
}


TEST(Mappers_IdeaEffectMapperTests, getOrderFromIdeaDefaultsToFive)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getOrderFromIdea("specifiedIdea", 1), 5);
}


TEST(Mappers_IdeaEffectMapperTests, getOrderFromIdeaCanBeSetForCompletedIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\torder = 1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getOrderFromIdea("specifiedIdea", 7), 1);
}


TEST(Mappers_IdeaEffectMapperTests, getOrderFromIdeaCannotBeSetForIncompleteIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\torder = 1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getOrderFromIdea("specifiedIdea", 4), 5);
}


TEST(Mappers_IdeaEffectMapperTests, getLiteracyFromIdeaReturnsFiveForMissingIdea)
{
	std::stringstream input;
	mappers::IdeaEffectMapper theMapper(input);

	auto investment = theMapper.getLiteracyFromIdea("missingIdea", 1);
	ASSERT_EQ(investment, 5);
}


TEST(Mappers_IdeaEffectMapperTests, getLiteracyFromIdeaDefaultsToFive)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getLiteracyFromIdea("specifiedIdea", 1), 5);
}


TEST(Mappers_IdeaEffectMapperTests, getLiteracyFromIdeaCanBeSetForCompletedIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tliteracy = 1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getLiteracyFromIdea("specifiedIdea", 7), 1);
}


TEST(Mappers_IdeaEffectMapperTests, getLiteracyFromIdeaCannotBeSetForIncompleteIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tliteracy = 1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getLiteracyFromIdea("specifiedIdea", 4), 5);
}


TEST(Mappers_IdeaEffectMapperTests, getLiberalFromIdeaReturnsFiveForMissingIdea)
{
	std::stringstream input;
	mappers::IdeaEffectMapper theMapper(input);

	auto investment = theMapper.getLiberalFromIdea("missingIdea", 1);
	ASSERT_EQ(investment, 5);
}


TEST(Mappers_IdeaEffectMapperTests, getLiberalFromIdeaDefaultsToFive)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getLiberalFromIdea("specifiedIdea", 1), 5);
}


TEST(Mappers_IdeaEffectMapperTests, getLiberalFromIdeaCanBeSetForCompletedIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tliberal = 1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getLiberalFromIdea("specifiedIdea", 7), 1);
}


TEST(Mappers_IdeaEffectMapperTests, getLiberalFromIdeaCannotBeSetForIncompleteIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tliberal = 1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getLiberalFromIdea("specifiedIdea", 4), 5);
}


TEST(Mappers_IdeaEffectMapperTests, getReactionaryFromIdeaReturnsFiveForMissingIdea)
{
	std::stringstream input;
	mappers::IdeaEffectMapper theMapper(input);

	auto investment = theMapper.getReactionaryFromIdea("missingIdea", 1);
	ASSERT_EQ(investment, 5);
}


TEST(Mappers_IdeaEffectMapperTests, getReactionaryFromIdeaDefaultsToFive)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getReactionaryFromIdea("specifiedIdea", 1), 5);
}


TEST(Mappers_IdeaEffectMapperTests, getReactionaryFromIdeaCanBeSetForCompletedIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\treactionary = 1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getReactionaryFromIdea("specifiedIdea", 7), 1);
}


TEST(Mappers_IdeaEffectMapperTests, getReactionaryFromIdeaCannotBeSetForIncompleteIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\treactionary = 1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getReactionaryFromIdea("specifiedIdea", 4), 5);
}


TEST(Mappers_IdeaEffectMapperTests, getSlaveryFromIdeaReturnsFiveForMissingIdea)
{
	std::stringstream input;
	mappers::IdeaEffectMapper theMapper(input);

	auto investment = theMapper.getSlaveryFromIdea("missingIdea", 1);
	ASSERT_EQ(investment, 5);
}


TEST(Mappers_IdeaEffectMapperTests, getSlaveryFromIdeaDefaultsToFive)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getSlaveryFromIdea("specifiedIdea", 1), 5);
}


TEST(Mappers_IdeaEffectMapperTests, getSlaveryFromIdeaCanBeSetForCompletedIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tslavery = 1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getSlaveryFromIdea("specifiedIdea", 7), 1);
}


TEST(Mappers_IdeaEffectMapperTests, getSlaveryFromIdeaCannotBeSetForIncompleteIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tslavery = 1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getSlaveryFromIdea("specifiedIdea", 4), 5);
}


TEST(Mappers_IdeaEffectMapperTests, getUpper_house_compositionFromIdeaReturnsFiveForMissingIdea)
{
	std::stringstream input;
	mappers::IdeaEffectMapper theMapper(input);

	auto investment = theMapper.getUpper_house_compositionFromIdea("missingIdea", 1);
	ASSERT_EQ(investment, 5);
}


TEST(Mappers_IdeaEffectMapperTests, getUpper_house_compositionFromIdeaDefaultsToFive)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getUpper_house_compositionFromIdea("specifiedIdea", 1), 5);
}


TEST(Mappers_IdeaEffectMapperTests, getUpper_house_compositionFromIdeaCanBeSetForCompletedIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tupper_house_composition = 1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getUpper_house_compositionFromIdea("specifiedIdea", 7), 1);
}


TEST(Mappers_IdeaEffectMapperTests, getUpper_house_compositionFromIdeaCannotBeSetForIncompleteIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tupper_house_composition = 1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getUpper_house_compositionFromIdea("specifiedIdea", 4), 5);
}


TEST(Mappers_IdeaEffectMapperTests, getVote_franchiseFromIdeaReturnsFiveForMissingIdea)
{
	std::stringstream input;
	mappers::IdeaEffectMapper theMapper(input);

	auto investment = theMapper.getVote_franchiseFromIdea("missingIdea", 1);
	ASSERT_EQ(investment, 5);
}


TEST(Mappers_IdeaEffectMapperTests, getVote_franchiseFromIdeaDefaultsToFive)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getVote_franchiseFromIdea("specifiedIdea", 1), 5);
}


TEST(Mappers_IdeaEffectMapperTests, getVote_franchiseFromIdeaCanBeSetForCompletedIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tvote_franchise = 1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getVote_franchiseFromIdea("specifiedIdea", 7), 1);
}


TEST(Mappers_IdeaEffectMapperTests, getVote_franchiseFromIdeaCannotBeSetForIncompleteIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tvote_franchise = 1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getVote_franchiseFromIdea("specifiedIdea", 4), 5);
}


TEST(Mappers_IdeaEffectMapperTests, getVoting_systemFromIdeaReturnsFiveForMissingIdea)
{
	std::stringstream input;
	mappers::IdeaEffectMapper theMapper(input);

	auto investment = theMapper.getVoting_systemFromIdea("missingIdea", 1);
	ASSERT_EQ(investment, 5);
}


TEST(Mappers_IdeaEffectMapperTests, getVoting_systemFromIdeaDefaultsToFive)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getVoting_systemFromIdea("specifiedIdea", 1), 5);
}


TEST(Mappers_IdeaEffectMapperTests, getVoting_systemFromIdeaCanBeSetForCompletedIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tvoting_system = 1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getVoting_systemFromIdea("specifiedIdea", 7), 1);
}


TEST(Mappers_IdeaEffectMapperTests, getVoting_systemFromIdeaCannotBeSetForIncompleteIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tvoting_system = 1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getVoting_systemFromIdea("specifiedIdea", 4), 5);
}


TEST(Mappers_IdeaEffectMapperTests, getPublic_meetingsFromIdeaReturnsFiveForMissingIdea)
{
	std::stringstream input;
	mappers::IdeaEffectMapper theMapper(input);

	auto investment = theMapper.getPublic_meetingsFromIdea("missingIdea", 1);
	ASSERT_EQ(investment, 5);
}


TEST(Mappers_IdeaEffectMapperTests, getPublic_meetingsFromIdeaDefaultsToFive)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getPublic_meetingsFromIdea("specifiedIdea", 1), 5);
}


TEST(Mappers_IdeaEffectMapperTests, getPublic_meetingsFromIdeaCanBeSetForCompletedIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tpublic_meetings = 1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getPublic_meetingsFromIdea("specifiedIdea", 7), 1);
}


TEST(Mappers_IdeaEffectMapperTests, getPublic_meetingsFromIdeaCannotBeSetForIncompleteIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tpublic_meetings = 1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getPublic_meetingsFromIdea("specifiedIdea", 4), 5);
}


TEST(Mappers_IdeaEffectMapperTests, getPress_rightsFromIdeaReturnsFiveForMissingIdea)
{
	std::stringstream input;
	mappers::IdeaEffectMapper theMapper(input);

	auto investment = theMapper.getPress_rightsFromIdea("missingIdea", 1);
	ASSERT_EQ(investment, 5);
}


TEST(Mappers_IdeaEffectMapperTests, getPress_rightsFromIdeaDefaultsToFive)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getPress_rightsFromIdea("specifiedIdea", 1), 5);
}


TEST(Mappers_IdeaEffectMapperTests, getPress_rightsFromIdeaCanBeSetForCompletedIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tpress_rights = 1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getPress_rightsFromIdea("specifiedIdea", 7), 1);
}


TEST(Mappers_IdeaEffectMapperTests, getPress_rightsFromIdeaCannotBeSetForIncompleteIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tpress_rights = 1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getPress_rightsFromIdea("specifiedIdea", 4), 5);
}


TEST(Mappers_IdeaEffectMapperTests, getTrade_unionsFromIdeaReturnsFiveForMissingIdea)
{
	std::stringstream input;
	mappers::IdeaEffectMapper theMapper(input);

	auto investment = theMapper.getTrade_unionsFromIdea("missingIdea", 1);
	ASSERT_EQ(investment, 5);
}


TEST(Mappers_IdeaEffectMapperTests, getTrade_unionsFromIdeaDefaultsToFive)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getTrade_unionsFromIdea("specifiedIdea", 1), 5);
}


TEST(Mappers_IdeaEffectMapperTests, getTrade_unionsFromIdeaCanBeSetForCompletedIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\ttrade_unions = 1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getTrade_unionsFromIdea("specifiedIdea", 7), 1);
}


TEST(Mappers_IdeaEffectMapperTests, getTrade_unionsFromIdeaCannotBeSetForIncompleteIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\ttrade_unions = 1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getTrade_unionsFromIdea("specifiedIdea", 4), 5);
}


TEST(Mappers_IdeaEffectMapperTests, getPolitical_partiesFromIdeaReturnsFiveForMissingIdea)
{
	std::stringstream input;
	mappers::IdeaEffectMapper theMapper(input);

	auto investment = theMapper.getPolitical_partiesFromIdea("missingIdea", 1);
	ASSERT_EQ(investment, 5);
}


TEST(Mappers_IdeaEffectMapperTests, getPolitical_partiesFromIdeaDefaultsToFive)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getPolitical_partiesFromIdea("specifiedIdea", 1), 5);
}


TEST(Mappers_IdeaEffectMapperTests, getPolitical_partiesFromIdeaCanBeSetForCompletedIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tpolitical_parties = 1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getPolitical_partiesFromIdea("specifiedIdea", 7), 1);
}


TEST(Mappers_IdeaEffectMapperTests, getPolitical_partiesFromIdeaCannotBeSetForIncompleteIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tpolitical_parties = 1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getArmyFromIdea("specifiedIdea", 4), 5);
}
