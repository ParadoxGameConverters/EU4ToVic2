#include "IdeaEffectMapper.h"
#include "IdeaEffects.h"
#include "Log.h"

mappers::IdeaEffectMapper::IdeaEffectMapper()
{
	LOG(LogLevel::Info) << "Parsing idea effects";
	registerKeys();
	parseFile("idea_effects.txt");
	parseFile("reform_effects.txt");
}

mappers::IdeaEffectMapper::IdeaEffectMapper(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
}

void mappers::IdeaEffectMapper::registerKeys()
{
	registerRegex("[a-zA-Z0-9_]+", [this](const std::string& idea, std::istream& theStream) {
		IdeaEffects ideaEffects(theStream);

		enforceIdeas[idea] = ideaEffects.getEnforce();

		armyIdeas[idea] = ideaEffects.getArmy();
		navyIdeas[idea] = ideaEffects.getNavy();
		commerceIdeas[idea] = ideaEffects.getCommerce();
		cultureIdeas[idea] = ideaEffects.getCulture();
		industryIdeas[idea] = ideaEffects.getIndustry();

		liberalIdeas[idea] = ideaEffects.getLiberal();
		reactionaryIdeas[idea] = ideaEffects.getReactionary();

		orderIdeas[idea] = ideaEffects.getOrder();
		libertyIdeas[idea] = ideaEffects.getLiberty();
		equalityIdeas[idea] = ideaEffects.getEquality();
		literacyIdeas[idea] = ideaEffects.getLiteracy();

		slaveryIdeas[idea] = ideaEffects.getSlavery();
		upper_house_compositionIdeas[idea] = ideaEffects.getUpper_house_composition();
		vote_franchiseIdeas[idea] = ideaEffects.getVote_franchise();
		voting_systemIdeas[idea] = ideaEffects.getVoting_system();
		public_meetingsIdeas[idea] = ideaEffects.getPublic_meetings();
		press_rightsIdeas[idea] = ideaEffects.getPress_rights();
		trade_unionsIdeas[idea] = ideaEffects.getTrade_unions();
		political_partiesIdeas[idea] = ideaEffects.getPolitical_parties();
	});
}

std::string mappers::IdeaEffectMapper::getEnforceFromIdea(const std::string& ideaName) const
{
	auto idea = enforceIdeas.find(ideaName);
	if (idea != enforceIdeas.end()) return idea->second;
	return "";
}

int mappers::IdeaEffectMapper::getArmyFromIdea(const std::string& ideaName) const
{
	auto idea = armyIdeas.find(ideaName);
	if (idea != armyIdeas.end()) return idea->second;
	return 5;
}

int mappers::IdeaEffectMapper::getCommerceFromIdea(const std::string& ideaName) const
{
	auto idea = commerceIdeas.find(ideaName);
	if (idea != commerceIdeas.end()) return idea->second;
	return 5;
}

int mappers::IdeaEffectMapper::getCultureFromIdea(const std::string& ideaName) const
{
	auto idea = cultureIdeas.find(ideaName);
	if (idea != cultureIdeas.end()) return idea->second;
	return 5;
}

int mappers::IdeaEffectMapper::getIndustryFromIdea(const std::string& ideaName) const
{
	auto idea = industryIdeas.find(ideaName);
	if (idea != industryIdeas.end()) return idea->second;
	return 5;
}

int mappers::IdeaEffectMapper::getNavyFromIdea(const std::string& ideaName) const
{
	auto idea = navyIdeas.find(ideaName);
	if (idea != navyIdeas.end()) return idea->second;
	return 5;
}

int mappers::IdeaEffectMapper::getLiberalFromIdea(const std::string& ideaName) const
{
	auto idea = liberalIdeas.find(ideaName);
	if (idea != liberalIdeas.end()) return idea->second;
	return 5;
}

int mappers::IdeaEffectMapper::getReactionaryFromIdea(const std::string& ideaName) const
{
	auto idea = reactionaryIdeas.find(ideaName);
	if (idea != reactionaryIdeas.end()) return idea->second;
	return 5;
}

int mappers::IdeaEffectMapper::getLiteracyFromIdea(const std::string& ideaName) const
{
	auto idea = literacyIdeas.find(ideaName);
	if (idea != literacyIdeas.end()) return idea->second;
	return 5;
}

int mappers::IdeaEffectMapper::getSlaveryFromIdea(const std::string& ideaName) const
{
	auto idea = slaveryIdeas.find(ideaName);
	if (idea != slaveryIdeas.end()) return idea->second;
	return 5;
}

int mappers::IdeaEffectMapper::getUpper_house_compositionFromIdea(const std::string& ideaName) const
{
	auto idea = upper_house_compositionIdeas.find(ideaName);
	if (idea != upper_house_compositionIdeas.end()) return idea->second;
	return 5;
}

int mappers::IdeaEffectMapper::getVote_franchiseFromIdea(const std::string& ideaName) const
{
	auto idea = vote_franchiseIdeas.find(ideaName);
	if (idea != vote_franchiseIdeas.end()) return idea->second;
	return 5;
}

int mappers::IdeaEffectMapper::getVoting_systemFromIdea(const std::string& ideaName) const
{
	auto idea = voting_systemIdeas.find(ideaName);
	if (idea != voting_systemIdeas.end()) return idea->second;
	return 5;
}

int mappers::IdeaEffectMapper::getPublic_meetingsFromIdea(const std::string& ideaName) const
{
	auto idea = public_meetingsIdeas.find(ideaName);
	if (idea != public_meetingsIdeas.end()) return idea->second;
	return 5;
}

int mappers::IdeaEffectMapper::getPress_rightsFromIdea(const std::string& ideaName) const
{
	auto idea = press_rightsIdeas.find(ideaName);
	if (idea != press_rightsIdeas.end()) return idea->second;
	return 5;
}

int mappers::IdeaEffectMapper::getTrade_unionsFromIdea(const std::string& ideaName) const
{
	auto idea = trade_unionsIdeas.find(ideaName);
	if (idea != trade_unionsIdeas.end()) return idea->second;
	return 5;
}

int mappers::IdeaEffectMapper::getPolitical_partiesFromIdea(const std::string& ideaName) const
{
	auto idea = political_partiesIdeas.find(ideaName);
	if (idea != political_partiesIdeas.end()) return idea->second;
	return 5;
}

int mappers::IdeaEffectMapper::getOrderFromIdea(const std::string& ideaName) const
{
	auto idea = orderIdeas.find(ideaName);
	if (idea != orderIdeas.end()) return idea->second;
	return 5;
}

int mappers::IdeaEffectMapper::getLibertyFromIdea(const std::string& ideaName) const
{
	auto idea = libertyIdeas.find(ideaName);
	if (idea != libertyIdeas.end()) return idea->second;
	return 5;
}

int mappers::IdeaEffectMapper::getEqualityFromIdea(const std::string& ideaName) const
{
	auto idea = equalityIdeas.find(ideaName);
	if (idea != equalityIdeas.end()) return idea->second;
	return 5;
}