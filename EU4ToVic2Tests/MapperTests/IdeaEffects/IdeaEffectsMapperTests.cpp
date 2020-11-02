#include "IdeaEffects/IdeaEffectMapper.h"
#include "gtest/gtest.h"
#include <sstream>

TEST(Mappers_IdeaEffectMapperTests, getEnforceFromIdeaReturnsNullForMissingIdea)
{
	std::stringstream input;
	const mappers::IdeaEffectMapper theMapper(input);

	const auto investment = theMapper.getEnforceFromIdea("missingIdea");
	ASSERT_FALSE(investment);
}

TEST(Mappers_IdeaEffectMapperTests, getEnforceFromIdeaDefaultsToNull)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	const mappers::IdeaEffectMapper theMapper(input);

	ASSERT_FALSE(theMapper.getEnforceFromIdea("specifiedIdea"));
}

TEST(Mappers_IdeaEffectMapperTests, getEnforceFromIdeaCanBeSetForIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tenforce = absolute_monarchy";
	input << "}";
	const mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ("absolute_monarchy", theMapper.getEnforceFromIdea("specifiedIdea"));
}

TEST(Mappers_IdeaEffectMapperTests, getArmyFromIdeaReturnsNullForMissingIdea)
{
	std::stringstream input;
	const mappers::IdeaEffectMapper theMapper(input);

	const auto investment = theMapper.getArmyFromIdea("missingIdea");
	ASSERT_FALSE(investment);
}

TEST(Mappers_IdeaEffectMapperTests, getArmyFromIdeaDefaultsToNull)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	const mappers::IdeaEffectMapper theMapper(input);

	ASSERT_FALSE(theMapper.getArmyFromIdea("specifiedIdea"));
}

TEST(Mappers_IdeaEffectMapperTests, getArmyFromIdeaCanBeSetForIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tarmy = 1";
	input << "}";
	const mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(-4, theMapper.getArmyFromIdea("specifiedIdea"));
}

TEST(Mappers_IdeaEffectMapperTests, getNavyFromIdeaReturnsNullForMissingIdea)
{
	std::stringstream input;
	const mappers::IdeaEffectMapper theMapper(input);

	const auto investment = theMapper.getNavyFromIdea("missingIdea");
	ASSERT_FALSE(investment);
}

TEST(Mappers_IdeaEffectMapperTests, getNavyFromIdeaDefaultsToNull)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	const mappers::IdeaEffectMapper theMapper(input);

	ASSERT_FALSE(theMapper.getNavyFromIdea("specifiedIdea"));
}

TEST(Mappers_IdeaEffectMapperTests, getNavyFromIdeaCanBeSetForIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tnavy = 1";
	input << "}";
	const mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(-4, theMapper.getNavyFromIdea("specifiedIdea"));
}

TEST(Mappers_IdeaEffectMapperTests, getCommerceFromIdeaReturnsNullForMissingIdea)
{
	std::stringstream input;
	const mappers::IdeaEffectMapper theMapper(input);

	const auto investment = theMapper.getCommerceFromIdea("missingIdea");
	ASSERT_FALSE(investment);
}

TEST(Mappers_IdeaEffectMapperTests, getCommerceFromIdeaDefaultsToNull)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	const mappers::IdeaEffectMapper theMapper(input);

	ASSERT_FALSE(theMapper.getCommerceFromIdea("specifiedIdea"));
}

TEST(Mappers_IdeaEffectMapperTests, getCommerceFromIdeaCanBeSetForIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tcommerce = 1";
	input << "}";
	const mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(-4, theMapper.getCommerceFromIdea("specifiedIdea"));
}

TEST(Mappers_IdeaEffectMapperTests, getCultureFromIdeaReturnsNullForMissingIdea)
{
	std::stringstream input;
	const mappers::IdeaEffectMapper theMapper(input);

	const auto investment = theMapper.getCultureFromIdea("missingIdea");
	ASSERT_FALSE(investment);
}

TEST(Mappers_IdeaEffectMapperTests, getCultureFromIdeaDefaultsToNull)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	const mappers::IdeaEffectMapper theMapper(input);

	ASSERT_FALSE(theMapper.getCultureFromIdea("specifiedIdea"));
}

TEST(Mappers_IdeaEffectMapperTests, getCultureFromIdeaCanBeSetForIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tculture = 1";
	input << "}";
	const mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(-4, theMapper.getCultureFromIdea("specifiedIdea"));
}

TEST(Mappers_IdeaEffectMapperTests, getIndustryFromIdeaReturnsNullForMissingIdea)
{
	std::stringstream input;
	const mappers::IdeaEffectMapper theMapper(input);

	const auto investment = theMapper.getIndustryFromIdea("missingIdea");
	ASSERT_FALSE(investment);
}

TEST(Mappers_IdeaEffectMapperTests, getIndustryFromIdeaDefaultsToNull)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	const mappers::IdeaEffectMapper theMapper(input);

	ASSERT_FALSE(theMapper.getIndustryFromIdea("specifiedIdea"));
}

TEST(Mappers_IdeaEffectMapperTests, getIndustryFromIdeaCanBeSetForIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tindustry = 1";
	input << "}";
	const mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(-4, theMapper.getIndustryFromIdea("specifiedIdea"));
}

TEST(Mappers_IdeaEffectMapperTests, getLibertyFromIdeaReturnsNullForMissingIdea)
{
	std::stringstream input;
	const mappers::IdeaEffectMapper theMapper(input);

	const auto investment = theMapper.getLibertyFromIdea("missingIdea");
	ASSERT_FALSE(investment);
}

TEST(Mappers_IdeaEffectMapperTests, getLibertyFromIdeaDefaultsToNull)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	const mappers::IdeaEffectMapper theMapper(input);

	ASSERT_FALSE(theMapper.getLibertyFromIdea("specifiedIdea"));
}

TEST(Mappers_IdeaEffectMapperTests, getLibertyFromIdeaCanBeSetForIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tliberty = 1";
	input << "}";
	const mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(-4, theMapper.getLibertyFromIdea("specifiedIdea"));
}

TEST(Mappers_IdeaEffectMapperTests, getEqualityFromIdeaReturnsNullForMissingIdea)
{
	std::stringstream input;
	const mappers::IdeaEffectMapper theMapper(input);

	const auto investment = theMapper.getEqualityFromIdea("missingIdea");
	ASSERT_FALSE(investment);
}

TEST(Mappers_IdeaEffectMapperTests, getEqualityFromIdeaDefaultsToNull)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	const mappers::IdeaEffectMapper theMapper(input);

	ASSERT_FALSE(theMapper.getEqualityFromIdea("specifiedIdea"));
}

TEST(Mappers_IdeaEffectMapperTests, getEqualityFromIdeaCanBeSetForIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tequality = 1";
	input << "}";
	const mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(-4, theMapper.getEqualityFromIdea("specifiedIdea"));
}

TEST(Mappers_IdeaEffectMapperTests, getOrderFromIdeaReturnsNullForMissingIdea)
{
	std::stringstream input;
	const mappers::IdeaEffectMapper theMapper(input);

	const auto investment = theMapper.getOrderFromIdea("missingIdea");
	ASSERT_FALSE(investment);
}

TEST(Mappers_IdeaEffectMapperTests, getOrderFromIdeaDefaultsToNull)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	const mappers::IdeaEffectMapper theMapper(input);

	ASSERT_FALSE(theMapper.getOrderFromIdea("specifiedIdea"));
}

TEST(Mappers_IdeaEffectMapperTests, getOrderFromIdeaCanBeSetForIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\torder = 1";
	input << "}";
	const mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(-4, theMapper.getOrderFromIdea("specifiedIdea"));
}

TEST(Mappers_IdeaEffectMapperTests, getLiteracyFromIdeaReturnsNullForMissingIdea)
{
	std::stringstream input;
	const mappers::IdeaEffectMapper theMapper(input);

	const auto investment = theMapper.getLiteracyFromIdea("missingIdea");
	ASSERT_FALSE(investment);
}

TEST(Mappers_IdeaEffectMapperTests, getLiteracyFromIdeaDefaultsToNull)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	const mappers::IdeaEffectMapper theMapper(input);

	ASSERT_FALSE(theMapper.getLiteracyFromIdea("specifiedIdea"));
}

TEST(Mappers_IdeaEffectMapperTests, getLiteracyFromIdeaCanBeSetForIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tliteracy = 1";
	input << "}";
	const mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(-4, theMapper.getLiteracyFromIdea("specifiedIdea"));
}

TEST(Mappers_IdeaEffectMapperTests, getLiberalFromIdeaReturnsNullForMissingIdea)
{
	std::stringstream input;
	const mappers::IdeaEffectMapper theMapper(input);

	const auto investment = theMapper.getLiberalFromIdea("missingIdea");
	ASSERT_FALSE(investment);
}

TEST(Mappers_IdeaEffectMapperTests, getLiberalFromIdeaDefaultsToNull)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	const mappers::IdeaEffectMapper theMapper(input);

	ASSERT_FALSE(theMapper.getLiberalFromIdea("specifiedIdea"));
}

TEST(Mappers_IdeaEffectMapperTests, getLiberalFromIdeaCanBeSetForIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tliberal = 1";
	input << "}";
	const mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(-4, theMapper.getLiberalFromIdea("specifiedIdea"));
}

TEST(Mappers_IdeaEffectMapperTests, getReactionaryFromIdeaReturnsNullForMissingIdea)
{
	std::stringstream input;
	const mappers::IdeaEffectMapper theMapper(input);

	const auto investment = theMapper.getReactionaryFromIdea("missingIdea");
	ASSERT_FALSE(investment);
}

TEST(Mappers_IdeaEffectMapperTests, getReactionaryFromIdeaDefaultsToNull)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	const mappers::IdeaEffectMapper theMapper(input);

	ASSERT_FALSE(theMapper.getReactionaryFromIdea("specifiedIdea"));
}

TEST(Mappers_IdeaEffectMapperTests, getReactionaryFromIdeaCanBeSetForIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\treactionary = 1";
	input << "}";
	const mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(-4, theMapper.getReactionaryFromIdea("specifiedIdea"));
}

TEST(Mappers_IdeaEffectMapperTests, getSlaveryFromIdeaReturnsNullForMissingIdea)
{
	std::stringstream input;
	const mappers::IdeaEffectMapper theMapper(input);

	const auto investment = theMapper.getSlaveryFromIdea("missingIdea");
	ASSERT_FALSE(investment);
}

TEST(Mappers_IdeaEffectMapperTests, getSlaveryFromIdeaDefaultsToNull)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	const mappers::IdeaEffectMapper theMapper(input);

	ASSERT_FALSE(theMapper.getSlaveryFromIdea("specifiedIdea"));
}

TEST(Mappers_IdeaEffectMapperTests, getSlaveryFromIdeaCanBeSetForIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tslavery = 1";
	input << "}";
	const mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(-4, theMapper.getSlaveryFromIdea("specifiedIdea"));
}

TEST(Mappers_IdeaEffectMapperTests, getUpper_house_compositionFromIdeaReturnsNullForMissingIdea)
{
	std::stringstream input;
	const mappers::IdeaEffectMapper theMapper(input);

	const auto investment = theMapper.getUpper_house_compositionFromIdea("missingIdea");
	ASSERT_FALSE(investment);
}

TEST(Mappers_IdeaEffectMapperTests, getUpper_house_compositionFromIdeaDefaultsToNull)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	const mappers::IdeaEffectMapper theMapper(input);

	ASSERT_FALSE(theMapper.getUpper_house_compositionFromIdea("specifiedIdea"));
}

TEST(Mappers_IdeaEffectMapperTests, getUpper_house_compositionFromIdeaCanBeSetForIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tupper_house_composition = 1";
	input << "}";
	const mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(-4, theMapper.getUpper_house_compositionFromIdea("specifiedIdea"));
}

TEST(Mappers_IdeaEffectMapperTests, getVote_franchiseFromIdeaReturnsNullForMissingIdea)
{
	std::stringstream input;
	const mappers::IdeaEffectMapper theMapper(input);

	const auto investment = theMapper.getVote_franchiseFromIdea("missingIdea");
	ASSERT_FALSE(investment);
}

TEST(Mappers_IdeaEffectMapperTests, getVote_franchiseFromIdeaDefaultsToNull)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	const mappers::IdeaEffectMapper theMapper(input);

	ASSERT_FALSE(theMapper.getVote_franchiseFromIdea("specifiedIdea"));
}

TEST(Mappers_IdeaEffectMapperTests, getVote_franchiseFromIdeaCanBeSetForIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tvote_franchise = 1";
	input << "}";
	const mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(-4, theMapper.getVote_franchiseFromIdea("specifiedIdea"));
}

TEST(Mappers_IdeaEffectMapperTests, getVoting_systemFromIdeaReturnsNullForMissingIdea)
{
	std::stringstream input;
	const mappers::IdeaEffectMapper theMapper(input);

	const auto investment = theMapper.getVoting_systemFromIdea("missingIdea");
	ASSERT_FALSE(investment);
}

TEST(Mappers_IdeaEffectMapperTests, getVoting_systemFromIdeaDefaultsToNull)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	const mappers::IdeaEffectMapper theMapper(input);

	ASSERT_FALSE(theMapper.getVoting_systemFromIdea("specifiedIdea"));
}

TEST(Mappers_IdeaEffectMapperTests, getVoting_systemFromIdeaCanBeSetForIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tvoting_system = 1";
	input << "}";
	const mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(-4, theMapper.getVoting_systemFromIdea("specifiedIdea"));
}

TEST(Mappers_IdeaEffectMapperTests, getPublic_meetingsFromIdeaReturnsNullForMissingIdea)
{
	std::stringstream input;
	const mappers::IdeaEffectMapper theMapper(input);

	const auto investment = theMapper.getPublic_meetingsFromIdea("missingIdea");
	ASSERT_FALSE(investment);
}

TEST(Mappers_IdeaEffectMapperTests, getPublic_meetingsFromIdeaDefaultsToNull)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	const mappers::IdeaEffectMapper theMapper(input);

	ASSERT_FALSE(theMapper.getPublic_meetingsFromIdea("specifiedIdea"));
}

TEST(Mappers_IdeaEffectMapperTests, getPublic_meetingsFromIdeaCanBeSetForIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tpublic_meetings = 1";
	input << "}";
	const mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(-4, theMapper.getPublic_meetingsFromIdea("specifiedIdea"));
}

TEST(Mappers_IdeaEffectMapperTests, getPress_rightsFromIdeaReturnsNullForMissingIdea)
{
	std::stringstream input;
	const mappers::IdeaEffectMapper theMapper(input);

	const auto investment = theMapper.getPress_rightsFromIdea("missingIdea");
	ASSERT_FALSE(investment);
}

TEST(Mappers_IdeaEffectMapperTests, getPress_rightsFromIdeaDefaultsToNull)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	const mappers::IdeaEffectMapper theMapper(input);

	ASSERT_FALSE(theMapper.getPress_rightsFromIdea("specifiedIdea"));
}

TEST(Mappers_IdeaEffectMapperTests, getPress_rightsFromIdeaCanBeSetForIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tpress_rights = 1";
	input << "}";
	const mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(-4, theMapper.getPress_rightsFromIdea("specifiedIdea"));
}

TEST(Mappers_IdeaEffectMapperTests, getTrade_unionsFromIdeaReturnsNullForMissingIdea)
{
	std::stringstream input;
	const mappers::IdeaEffectMapper theMapper(input);

	const auto investment = theMapper.getTrade_unionsFromIdea("missingIdea");
	ASSERT_FALSE(investment);
}

TEST(Mappers_IdeaEffectMapperTests, getTrade_unionsFromIdeaDefaultsToNull)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	const mappers::IdeaEffectMapper theMapper(input);

	ASSERT_FALSE(theMapper.getTrade_unionsFromIdea("specifiedIdea"));
}

TEST(Mappers_IdeaEffectMapperTests, getTrade_unionsFromIdeaCanBeSetForIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\ttrade_unions = 1";
	input << "}";
	const mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(-4, theMapper.getTrade_unionsFromIdea("specifiedIdea"));
}

TEST(Mappers_IdeaEffectMapperTests, getPolitical_partiesFromIdeaReturnsNullForMissingIdea)
{
	std::stringstream input;
	const mappers::IdeaEffectMapper theMapper(input);

	const auto investment = theMapper.getPolitical_partiesFromIdea("missingIdea");
	ASSERT_FALSE(investment);
}

TEST(Mappers_IdeaEffectMapperTests, getPolitical_partiesFromIdeaDefaultsToNull)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "}";
	const mappers::IdeaEffectMapper theMapper(input);

	ASSERT_FALSE(theMapper.getPolitical_partiesFromIdea("specifiedIdea"));
}

TEST(Mappers_IdeaEffectMapperTests, getPolitical_partiesFromIdeaCanBeSetForIdea)
{
	std::stringstream input;
	input << "specifiedIdea ={\n";
	input << "\tpolitical_parties = 1";
	input << "}";
	const mappers::IdeaEffectMapper theMapper(input);

	ASSERT_EQ(-4, theMapper.getPolitical_partiesFromIdea("specifiedIdea"));
}
