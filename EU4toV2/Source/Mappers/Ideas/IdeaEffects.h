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



#ifndef IDEA_EFFECTS_H
#define IDEA_EFFECTS_H



#include "newParser.h"
#include <set>



namespace mappers
{

class IdeaEffects: commonItems::parser
{
	public:
		IdeaEffects(std::istream& theStream);

		int getArmyInvestmentValue() const { return armyInvestmentValue; }
		int getNavyInvestmentValue() const { return navyInvestmentValue; }
		int getCommerceInvestmentValue() const { return commerceInvestmentValue; }
		int getCultureInvestmentValue() const { return cultureInvestmentValue; }
		int getIndustryInvestmentValue() const { return industryInvestmentValue; }

		double getArmyTechScoreValue() const { return armyTechScoreValue; }
		double getNavyTechScoreValue() const { return navyTechScoreValue; }
		double getCommerceTechScoreValue() const { return commerceTechScoreValue; }
		double getCultureTechScoreValue() const { return cultureTechScoreValue; }
		double getIndustryTechScoreValue() const { return industryTechScoreValue; }

		double getUpperHouseLiberalValue() const { return upperHouseLiberalValue; }
		double getUpperHouseReactionaryValue() const { return upperHouseReactionaryValue; }

		double getOrderInfluenceValue() const { return orderInfluenceValue; }
		double getLibertyInfluenceValue() const { return libertyInfluenceValue; }
		double getEqualityInfluenceValue() const { return equalityInfluenceValue; }

		std::set<int> getLiteracyLevels() const { return literacyLevels; }

	private:
		int armyInvestmentValue = 0;
		int navyInvestmentValue = 0;
		int commerceInvestmentValue = 0;
		int cultureInvestmentValue = 0;
		int industryInvestmentValue = 0;

		double armyTechScoreValue = 0.0;
		double navyTechScoreValue = 0.0;
		double commerceTechScoreValue = 0.0;
		double cultureTechScoreValue = 0.0;
		double industryTechScoreValue = 0.0;

		double upperHouseLiberalValue = 0.0;
		double upperHouseReactionaryValue = 0.0;

		double orderInfluenceValue = 0.0;
		double libertyInfluenceValue = 0.0;
		double equalityInfluenceValue = 0.0;

		std::set<int> literacyLevels;
};

}



#endif // IDEA_EFFECTS_H