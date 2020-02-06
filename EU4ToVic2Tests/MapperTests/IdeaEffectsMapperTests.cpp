#include "gtest/gtest.h"
#include "../EU4toV2/Source/Mappers/IdeaEffects/IdeaEffectMapper.h"
#include <sstream>



TEST(Mappers_IdeaEffectMapperTests, getEnforceFromIdeaReturnsEmptyForMissingIdea)
{
	std::stringstream input;
	mappers::IdeaEffectMapper theMapper(input);

	auto investment = theMapper.getEnforceFromIdea("missingIdea");
	ASSERT_EQ(investment, "");
}


TEST(Mappers_IdeaEffectMapperTests, getEnforceFromIdeaDefaultsToEmpty)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getEnforceFromIdea("specifiedIdea"), "");
}


TEST(Mappers_IdeaEffectMapperTests, getEnforceFromIdeaCanBeSetForIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tenforce = absolute_monarchy";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getEnforceFromIdea("specifiedIdea"), "absolute_monarchy");
}

TEST(Mappers_IdeaEffectMapperTests, getArmyFromIdeaReturnsZeroForMissingIdea)
{
	std::stringstream input;
	mappers::IdeaEffectMapper theMapper(input);

	auto investment = theMapper.getArmyFromIdea("missingIdea");
	ASSERT_EQ(investment, 0);
}


TEST(Mappers_IdeaEffectMapperTests, getArmyFromIdeaDefaultsToZero)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getArmyFromIdea("specifiedIdea"), 0);
}


TEST(Mappers_IdeaEffectMapperTests, getArmyFromIdeaCanBeSetForIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tarmy = 1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getArmyFromIdea("specifiedIdea"), -4);
}


TEST(Mappers_IdeaEffectMapperTests, getNavyFromIdeaReturnsZeroForMissingIdea)
{
	std::stringstream input;
	mappers::IdeaEffectMapper theMapper(input);

	auto investment = theMapper.getNavyFromIdea("missingIdea");
	ASSERT_EQ(investment, 0);
}


TEST(Mappers_IdeaEffectMapperTests, getNavyFromIdeaDefaultsToZero)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getNavyFromIdea("specifiedIdea"), 0);
}


TEST(Mappers_IdeaEffectMapperTests, getNavyFromIdeaCanBeSetForIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tnavy = 1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getNavyFromIdea("specifiedIdea"), -4);
}


TEST(Mappers_IdeaEffectMapperTests, getCommerceFromIdeaReturnsZeroForMissingIdea)
{
	std::stringstream input;
	mappers::IdeaEffectMapper theMapper(input);

	auto investment = theMapper.getCommerceFromIdea("missingIdea");
	ASSERT_EQ(investment, 0);
}


TEST(Mappers_IdeaEffectMapperTests, getCommerceFromIdeaDefaultsToZero)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getCommerceFromIdea("specifiedIdea"), 0);
}


TEST(Mappers_IdeaEffectMapperTests, getCommerceFromIdeaCanBeSetForIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tcommerce = 1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getCommerceFromIdea("specifiedIdea"), -4);
}



TEST(Mappers_IdeaEffectMapperTests, getCultureFromIdeaReturnsZeroForMissingIdea)
{
	std::stringstream input;
	mappers::IdeaEffectMapper theMapper(input);

	auto investment = theMapper.getCultureFromIdea("missingIdea");
	ASSERT_EQ(investment, 0);
}


TEST(Mappers_IdeaEffectMapperTests, getCultureFromIdeaDefaultsToZero)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getCultureFromIdea("specifiedIdea"), 0);
}


TEST(Mappers_IdeaEffectMapperTests, getCultureFromIdeaCanBeSetForIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tculture = 1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getCultureFromIdea("specifiedIdea"), -4);
}



TEST(Mappers_IdeaEffectMapperTests, getIndustryFromIdeaReturnsZeroForMissingIdea)
{
	std::stringstream input;
	mappers::IdeaEffectMapper theMapper(input);

	auto investment = theMapper.getIndustryFromIdea("missingIdea");
	ASSERT_EQ(investment, 0);
}


TEST(Mappers_IdeaEffectMapperTests, getIndustryFromIdeaDefaultsToZero)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getIndustryFromIdea("specifiedIdea"), 0);
}


TEST(Mappers_IdeaEffectMapperTests, getIndustryFromIdeaCanBeSetForIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tindustry = 1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getIndustryFromIdea("specifiedIdea"), -4);
}


TEST(Mappers_IdeaEffectMapperTests, getLibertyFromIdeaReturnsZeroForMissingIdea)
{
	std::stringstream input;
	mappers::IdeaEffectMapper theMapper(input);

	auto investment = theMapper.getLibertyFromIdea("missingIdea");
	ASSERT_EQ(investment, 0);
}


TEST(Mappers_IdeaEffectMapperTests, getLibertyFromIdeaDefaultsToZero)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getLibertyFromIdea("specifiedIdea"), 0);
}


TEST(Mappers_IdeaEffectMapperTests, getLibertyFromIdeaCanBeSetForIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tliberty = 1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getLibertyFromIdea("specifiedIdea"), -4);
}


TEST(Mappers_IdeaEffectMapperTests, getEqualityFromIdeaReturnsZeroForMissingIdea)
{
	std::stringstream input;
	mappers::IdeaEffectMapper theMapper(input);

	auto investment = theMapper.getEqualityFromIdea("missingIdea");
	ASSERT_EQ(investment, 0);
}


TEST(Mappers_IdeaEffectMapperTests, getEqualityFromIdeaDefaultsToZero)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getEqualityFromIdea("specifiedIdea"), 0);
}


TEST(Mappers_IdeaEffectMapperTests, getEqualityFromIdeaCanBeSetForIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tequality = 1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getEqualityFromIdea("specifiedIdea"), -4);
}


TEST(Mappers_IdeaEffectMapperTests, getOrderFromIdeaReturnsZeroForMissingIdea)
{
	std::stringstream input;
	mappers::IdeaEffectMapper theMapper(input);

	auto investment = theMapper.getOrderFromIdea("missingIdea");
	ASSERT_EQ(investment, 0);
}


TEST(Mappers_IdeaEffectMapperTests, getOrderFromIdeaDefaultsToZero)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getOrderFromIdea("specifiedIdea"), 0);
}


TEST(Mappers_IdeaEffectMapperTests, getOrderFromIdeaCanBeSetForIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\torder = 1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getOrderFromIdea("specifiedIdea"), -4);
}


TEST(Mappers_IdeaEffectMapperTests, getLiteracyFromIdeaReturnsZeroForMissingIdea)
{
	std::stringstream input;
	mappers::IdeaEffectMapper theMapper(input);

	auto investment = theMapper.getLiteracyFromIdea("missingIdea");
	ASSERT_EQ(investment, 0);
}


TEST(Mappers_IdeaEffectMapperTests, getLiteracyFromIdeaDefaultsToZero)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getLiteracyFromIdea("specifiedIdea"), 0);
}


TEST(Mappers_IdeaEffectMapperTests, getLiteracyFromIdeaCanBeSetForIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tliteracy = 1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getLiteracyFromIdea("specifiedIdea"), -4);
}



TEST(Mappers_IdeaEffectMapperTests, getLiberalFromIdeaReturnsZeroForMissingIdea)
{
	std::stringstream input;
	mappers::IdeaEffectMapper theMapper(input);

	auto investment = theMapper.getLiberalFromIdea("missingIdea");
	ASSERT_EQ(investment, 0);
}


TEST(Mappers_IdeaEffectMapperTests, getLiberalFromIdeaDefaultsToZero)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getLiberalFromIdea("specifiedIdea"), 0);
}


TEST(Mappers_IdeaEffectMapperTests, getLiberalFromIdeaCanBeSetForIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tliberal = 1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getLiberalFromIdea("specifiedIdea"), -4);
}



TEST(Mappers_IdeaEffectMapperTests, getReactionaryFromIdeaReturnsZeroForMissingIdea)
{
	std::stringstream input;
	mappers::IdeaEffectMapper theMapper(input);

	auto investment = theMapper.getReactionaryFromIdea("missingIdea");
	ASSERT_EQ(investment, 0);
}


TEST(Mappers_IdeaEffectMapperTests, getReactionaryFromIdeaDefaultsToZero)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getReactionaryFromIdea("specifiedIdea"), 0);
}


TEST(Mappers_IdeaEffectMapperTests, getReactionaryFromIdeaCanBeSetForIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\treactionary = 1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getReactionaryFromIdea("specifiedIdea"), -4);
}



TEST(Mappers_IdeaEffectMapperTests, getSlaveryFromIdeaReturnsZeroForMissingIdea)
{
	std::stringstream input;
	mappers::IdeaEffectMapper theMapper(input);

	auto investment = theMapper.getSlaveryFromIdea("missingIdea");
	ASSERT_EQ(investment, 0);
}


TEST(Mappers_IdeaEffectMapperTests, getSlaveryFromIdeaDefaultsToZero)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getSlaveryFromIdea("specifiedIdea"), 0);
}


TEST(Mappers_IdeaEffectMapperTests, getSlaveryFromIdeaCanBeSetForIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tslavery = 1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getSlaveryFromIdea("specifiedIdea"), -4);
}



TEST(Mappers_IdeaEffectMapperTests, getUpper_house_compositionFromIdeaReturnsZeroForMissingIdea)
{
	std::stringstream input;
	mappers::IdeaEffectMapper theMapper(input);

	auto investment = theMapper.getUpper_house_compositionFromIdea("missingIdea");
	ASSERT_EQ(investment, 0);
}


TEST(Mappers_IdeaEffectMapperTests, getUpper_house_compositionFromIdeaDefaultsToZero)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getUpper_house_compositionFromIdea("specifiedIdea"), 0);
}


TEST(Mappers_IdeaEffectMapperTests, getUpper_house_compositionFromIdeaCanBeSetForIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tupper_house_composition = 1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getUpper_house_compositionFromIdea("specifiedIdea"), -4);
}



TEST(Mappers_IdeaEffectMapperTests, getVote_franchiseFromIdeaReturnsZeroForMissingIdea)
{
	std::stringstream input;
	mappers::IdeaEffectMapper theMapper(input);

	auto investment = theMapper.getVote_franchiseFromIdea("missingIdea");
	ASSERT_EQ(investment, 0);
}


TEST(Mappers_IdeaEffectMapperTests, getVote_franchiseFromIdeaDefaultsToZero)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getVote_franchiseFromIdea("specifiedIdea"), 0);
}


TEST(Mappers_IdeaEffectMapperTests, getVote_franchiseFromIdeaCanBeSetForIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tvote_franchise = 1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getVote_franchiseFromIdea("specifiedIdea"), -4);
}


TEST(Mappers_IdeaEffectMapperTests, getVoting_systemFromIdeaReturnsZeroForMissingIdea)
{
	std::stringstream input;
	mappers::IdeaEffectMapper theMapper(input);

	auto investment = theMapper.getVoting_systemFromIdea("missingIdea");
	ASSERT_EQ(investment, 0);
}


TEST(Mappers_IdeaEffectMapperTests, getVoting_systemFromIdeaDefaultsToZero)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getVoting_systemFromIdea("specifiedIdea"), 0);
}


TEST(Mappers_IdeaEffectMapperTests, getVoting_systemFromIdeaCanBeSetForIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tvoting_system = 1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getVoting_systemFromIdea("specifiedIdea"), -4);
}


TEST(Mappers_IdeaEffectMapperTests, getPublic_meetingsFromIdeaReturnsZeroForMissingIdea)
{
	std::stringstream input;
	mappers::IdeaEffectMapper theMapper(input);

	auto investment = theMapper.getPublic_meetingsFromIdea("missingIdea");
	ASSERT_EQ(investment, 0);
}


TEST(Mappers_IdeaEffectMapperTests, getPublic_meetingsFromIdeaDefaultsToZero)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getPublic_meetingsFromIdea("specifiedIdea"), 0);
}


TEST(Mappers_IdeaEffectMapperTests, getPublic_meetingsFromIdeaCanBeSetForIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tpublic_meetings = 1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getPublic_meetingsFromIdea("specifiedIdea"), -4);
}



TEST(Mappers_IdeaEffectMapperTests, getPress_rightsFromIdeaReturnsZeroForMissingIdea)
{
	std::stringstream input;
	mappers::IdeaEffectMapper theMapper(input);

	auto investment = theMapper.getPress_rightsFromIdea("missingIdea");
	ASSERT_EQ(investment, 0);
}


TEST(Mappers_IdeaEffectMapperTests, getPress_rightsFromIdeaDefaultsToZero)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getPress_rightsFromIdea("specifiedIdea"), 0);
}


TEST(Mappers_IdeaEffectMapperTests, getPress_rightsFromIdeaCanBeSetForIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tpress_rights = 1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getPress_rightsFromIdea("specifiedIdea"), -4);
}


TEST(Mappers_IdeaEffectMapperTests, getTrade_unionsFromIdeaReturnsZeroForMissingIdea)
{
	std::stringstream input;
	mappers::IdeaEffectMapper theMapper(input);

	auto investment = theMapper.getTrade_unionsFromIdea("missingIdea");
	ASSERT_EQ(investment, 0);
}


TEST(Mappers_IdeaEffectMapperTests, getTrade_unionsFromIdeaDefaultsToZero)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getTrade_unionsFromIdea("specifiedIdea"), 0);
}


TEST(Mappers_IdeaEffectMapperTests, getTrade_unionsFromIdeaCanBeSetForIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\ttrade_unions = 1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getTrade_unionsFromIdea("specifiedIdea"), -4);
}




TEST(Mappers_IdeaEffectMapperTests, getPolitical_partiesFromIdeaReturnsZeroForMissingIdea)
{
	std::stringstream input;
	mappers::IdeaEffectMapper theMapper(input);

	auto investment = theMapper.getPolitical_partiesFromIdea("missingIdea");
	ASSERT_EQ(investment, 0);
}


TEST(Mappers_IdeaEffectMapperTests, getPolitical_partiesFromIdeaDefaultsToZero)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getPolitical_partiesFromIdea("specifiedIdea"), 0);
}


TEST(Mappers_IdeaEffectMapperTests, getPolitical_partiesFromIdeaCanBeSetForIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tpolitical_parties = 1";
	input << "}";
	mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(theMapper.getPolitical_partiesFromIdea("specifiedIdea"), -4);
}
