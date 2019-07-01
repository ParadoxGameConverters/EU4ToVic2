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

		int getArmyInvestmentFromIdea(const std::string& ideaName, int ideaLevel) const;
		int getCommerceInvestmentFromIdea(const std::string& ideaName, int ideaLevel) const;
		int getCultureInvestmentFromIdea(const std::string& ideaName, int ideaLevel) const;
		int getIndustryInvestmentFromIdea(const std::string& ideaName, int ideaLevel) const;
		int getNavyInvestmentFromIdea(const std::string& ideaName, int ideaLevel) const;

		double getUHLiberalFromIdea(const std::string& ideaName, int ideaLevel) const;
		double getUHReactionaryFromIdea(const std::string& ideaName, int ideaLevel) const;

		double getLiteracyFromIdea(const std::string& ideaName, int ideaLevel) const;

		int getOrderInfluenceFromIdea(const std::string& ideaName, int ideaLevel) const;
		int getLibertyInfluenceFromIdea(const std::string& ideaName, int ideaLevel) const;
		int getEqualityInfluenceFromIdea(const std::string& ideaName, int ideaLevel) const;

		double getArmyTechFromIdeas(const std::map<std::string, int>& ideas) const;
		double getCommerceTechFromIdeas(const std::map<std::string, int>& ideas) const;
		double getCultureTechFromIdeas(const std::map<std::string, int>& ideas) const;
		double getIndustryTechFromIdeas(const std::map<std::string, int>& ideas) const;
		double getNavyTechFromIdeas(const std::map<std::string, int>& ideas) const;

	private:
		std::map<std::string, int> armyInvestmentIdeas;
		std::map<std::string, int> commerceInvestmentIdeas;
		std::map<std::string, int> cultureInvestmentIdeas;
		std::map<std::string, int> industryInvestmentIdeas;
		std::map<std::string, int> navyInvestmentIdeas;

		std::map<std::string, double> UHLiberalIdeas;
		std::map<std::string, double> UHReactionaryIdeas;

		std::map<std::string, std::set<int>> literacyIdeas;

		std::map<std::string, int> orderIdeas;
		std::map<std::string, int> libertyIdeas;
		std::map<std::string, int> equalityIdeas;

		std::map<std::string, double> armyTechIdeas;
		std::map<std::string, double> commerceTechIdeas;
		std::map<std::string, double> cultureTechIdeas;
		std::map<std::string, double> industryTechIdeas;
		std::map<std::string, double> navyTechIdeas;
};

}



#endif // IDEA_EFFECT_MAPPER_H
