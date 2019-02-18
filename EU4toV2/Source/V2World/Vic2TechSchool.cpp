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



#include "Vic2TechSchool.h"
#include "ParserHelpers.h"
#include <cmath>



Vic2::Vic2TechSchool::Vic2TechSchool(const std::string& _name, std::istream& theStream): name(_name)
{
	registerKeyword(std::regex("army_tech_research_bonus"), [this](const std::string& unused, std::istream& theStream){
		armyBonus = commonItems::singleDouble(theStream).getDouble();
	});
	registerKeyword(std::regex("commerce_tech_research_bonus"), [this](const std::string& unused, std::istream& theStream){
		commerceBonus = commonItems::singleDouble(theStream).getDouble();
	});
	registerKeyword(std::regex("culture_tech_research_bonus"), [this](const std::string& unused, std::istream& theStream){
		cultureBonus = commonItems::singleDouble(theStream).getDouble();
	});
	registerKeyword(std::regex("industry_tech_research_bonus"), [this](const std::string& unused, std::istream& theStream){
		industryBonus = commonItems::singleDouble(theStream).getDouble();
	});
	registerKeyword(std::regex("navy_tech_research_bonus"), [this](const std::string& unused, std::istream& theStream){
		navyBonus = commonItems::singleDouble(theStream).getDouble();
	});
	registerKeyword(std::regex("[a-zA-Z0-9\\_]+"), commonItems::ignoreItem);

	parseStream(theStream);
}


double Vic2::Vic2TechSchool::calculateComparisonScore(double armyInvestment, double commerceInvestment, double cultureInvestment, double industryInvestment, double navyInvestment) const
{
	return std::abs(armyInvestment - armyBonus - 0.2) +
		std::abs(navyInvestment - navyBonus - 0.2) +
		std::abs(commerceInvestment - commerceBonus - 0.2) +
		std::abs(industryInvestment - industryBonus - 0.2) +
		std::abs(cultureInvestment - cultureBonus - 0.2);
}