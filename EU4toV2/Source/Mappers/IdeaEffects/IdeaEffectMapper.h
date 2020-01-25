#ifndef IDEA_EFFECT_MAPPER_H
#define IDEA_EFFECT_MAPPER_H

#include "newParser.h"
#include <map>

namespace mappers
{
	class IdeaEffectMapper: commonItems::parser
	{
	public:
		IdeaEffectMapper();
		explicit IdeaEffectMapper(std::istream& theStream);

		[[nodiscard]] std::string getEnforceFromIdea(const std::string& ideaName) const;

		[[nodiscard]] int getArmyFromIdea(const std::string& ideaName) const;
		[[nodiscard]] int getCommerceFromIdea(const std::string& ideaName) const;
		[[nodiscard]] int getCultureFromIdea(const std::string& ideaName) const;
		[[nodiscard]] int getIndustryFromIdea(const std::string& ideaName) const;
		[[nodiscard]] int getNavyFromIdea(const std::string& ideaName) const;

		[[nodiscard]] int getSlaveryFromIdea(const std::string& ideaName) const;
		[[nodiscard]] int getUpper_house_compositionFromIdea(const std::string& ideaName) const;
		[[nodiscard]] int getVote_franchiseFromIdea(const std::string& ideaName) const;
		[[nodiscard]] int getVoting_systemFromIdea(const std::string& ideaName) const;
		[[nodiscard]] int getPublic_meetingsFromIdea(const std::string& ideaName) const;
		[[nodiscard]] int getPress_rightsFromIdea(const std::string& ideaName) const;
		[[nodiscard]] int getTrade_unionsFromIdea(const std::string& ideaName) const;
		[[nodiscard]] int getPolitical_partiesFromIdea(const std::string& ideaName) const;

		[[nodiscard]] int getLibertyFromIdea(const std::string& ideaName) const;
		[[nodiscard]] int getEqualityFromIdea(const std::string& ideaName) const;
		[[nodiscard]] int getOrderFromIdea(const std::string& ideaName) const;
		[[nodiscard]] int getLiteracyFromIdea(const std::string& ideaName) const;

		[[nodiscard]] int getReactionaryFromIdea(const std::string& ideaName) const;
		[[nodiscard]] int getLiberalFromIdea(const std::string& ideaName) const;

		void registerKeys();

	private:
		std::map<std::string, std::string> enforceIdeas;

		std::map<std::string, int> armyIdeas;
		std::map<std::string, int> commerceIdeas;
		std::map<std::string, int> cultureIdeas;
		std::map<std::string, int> industryIdeas;
		std::map<std::string, int> navyIdeas;

		std::map<std::string, int> libertyIdeas;
		std::map<std::string, int> equalityIdeas;
		std::map<std::string, int> orderIdeas;
		std::map<std::string, int> literacyIdeas;

		std::map<std::string, int> slaveryIdeas;
		std::map<std::string, int> upper_house_compositionIdeas;
		std::map<std::string, int> vote_franchiseIdeas;
		std::map<std::string, int> voting_systemIdeas;
		std::map<std::string, int> public_meetingsIdeas;
		std::map<std::string, int> press_rightsIdeas;
		std::map<std::string, int> trade_unionsIdeas;
		std::map<std::string, int> political_partiesIdeas;

		std::map<std::string, int> reactionaryIdeas;
		std::map<std::string, int> liberalIdeas;
	};
}

#endif // IDEA_EFFECT_MAPPER_H
