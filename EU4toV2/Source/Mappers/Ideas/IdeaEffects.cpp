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



#include "IdeaEffects.h"
#include "ParserHelpers.h"



mappers::IdeaEffects::IdeaEffects(std::istream& theStream)
{
	registerKeyword(std::regex("army_investment"), [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleInt armyInvestmentInt(theStream);
		armyInvestmentValue = armyInvestmentInt.getInt();
	});
	registerKeyword(std::regex("navy_investment"), [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleInt navyInvestmentInt(theStream);
		navyInvestmentValue = navyInvestmentInt.getInt();
	});
	registerKeyword(std::regex("commerce_investment"), [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleInt commerceInvestmentInt(theStream);
		commerceInvestmentValue = commerceInvestmentInt.getInt();
	});
	registerKeyword(std::regex("culture_investment"), [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleInt cultureInvestmentInt(theStream);
		cultureInvestmentValue = cultureInvestmentInt.getInt();
	});
	registerKeyword(std::regex("industry_investment"), [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleInt industryInvestmentInt(theStream);
		industryInvestmentValue = industryInvestmentInt.getInt();
	});

	registerKeyword(std::regex("army_tech_score"), [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleDouble armyTechScoreDouble(theStream);
		armyTechScoreValue = armyTechScoreDouble.getDouble();
	});
	registerKeyword(std::regex("navy_tech_score"), [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleDouble navyTechScoreDouble(theStream);
		navyTechScoreValue = navyTechScoreDouble.getDouble();
	});
	registerKeyword(std::regex("commerce_tech_score"), [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleDouble commerceTechScoreDouble(theStream);
		commerceTechScoreValue = commerceTechScoreDouble.getDouble();
	});
	registerKeyword(std::regex("culture_tech_score"), [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleDouble cultureTechScoreDouble(theStream);
		cultureTechScoreValue = cultureTechScoreDouble.getDouble();
	});
	registerKeyword(std::regex("industry_tech_score"), [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleDouble industryTechScoreDouble(theStream);
		industryTechScoreValue = industryTechScoreDouble.getDouble();
	});

	registerKeyword(std::regex("upper_house_liberal"), [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleDouble upperHouseLiberalDouble(theStream);
		upperHouseLiberalValue = upperHouseLiberalDouble.getDouble();
	});
	registerKeyword(std::regex("upper_house_reactionary"), [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleDouble upperHouseReactionaryDouble(theStream);
		upperHouseReactionaryValue = upperHouseReactionaryDouble.getDouble();
	});

	registerKeyword(std::regex("NV_order"), [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleInt orderInfluenceInt(theStream);
		orderInfluenceValue = orderInfluenceInt.getInt();
	});
	registerKeyword(std::regex("NV_liberty"), [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleInt libertyInfluenceInt(theStream);
		libertyInfluenceValue = libertyInfluenceInt.getInt();
	});
	registerKeyword(std::regex("NV_equality"), [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleInt equalityInfluenceInt(theStream);
		equalityInfluenceValue = equalityInfluenceInt.getInt();
	});

	registerKeyword(std::regex("literacy"), [this](const std::string& unused, std::istream& theStream) {
		commonItems::intList literacyLevelInts(theStream);
		for (auto level: literacyLevelInts.getInts())
		{
			literacyLevels.insert(level);
		}
	});

	parseStream(theStream);
}