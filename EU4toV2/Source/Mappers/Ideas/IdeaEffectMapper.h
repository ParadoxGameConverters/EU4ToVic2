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



#ifndef IDEA_EFFECT_MAPPER_H
#define IDEA_EFFECT_MAPPER_H



#include "newParser.h"
#include <map>
#include <set>
#include <string>




namespace mappers
{

class IdeaEffectMapper: commonItems::parser
{
	public:
		IdeaEffectMapper(std::istream& theStream);

		int getArmyFromIdea(const std::string& ideaName, int ideaLevel) const;
		int getCommerceFromIdea(const std::string& ideaName, int ideaLevel) const;
		int getCultureFromIdea(const std::string& ideaName, int ideaLevel) const;
		int getIndustryFromIdea(const std::string& ideaName, int ideaLevel) const;
		int getNavyFromIdea(const std::string& ideaName, int ideaLevel) const;

		int getSlaveryFromIdea(const std::string& ideaName, int ideaLevel) const;
		int getUpper_house_compositionFromIdea(const std::string& ideaName, int ideaLevel) const;
		int getVote_franchiseFromIdea(const std::string& ideaName, int ideaLevel) const;
		int getVoting_systemFromIdea(const std::string& ideaName, int ideaLevel) const;
		int getPublic_meetingsFromIdea(const std::string& ideaName, int ideaLevel) const;
		int getPress_rightsFromIdea(const std::string& ideaName, int ideaLevel) const;
		int getTrade_unionsFromIdea(const std::string& ideaName, int ideaLevel) const;
		int getPolitical_partiesFromIdea(const std::string& ideaName, int ideaLevel) const;

		int getLibertyFromIdea(const std::string& ideaName, int ideaLevel) const;
		int getEqualityFromIdea(const std::string& ideaName, int ideaLevel) const;
		int getOrderFromIdea(const std::string& ideaName, int ideaLevel) const;
		int getLiteracyFromIdea(const std::string& ideaName, int ideaLevel) const;

		int getReactionaryFromIdea(const std::string& ideaName, int ideaLevel) const;
		int getLiberalFromIdea(const std::string& ideaName, int ideaLevel) const;

	private:
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
