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



#include "V2TechSchools.h"
#include "Log.h"
#include "../Configuration.h"
#include "ParserHelpers.h"
#include "ParadoxParserUTF8.h"
#include <algorithm>
#include <memory>



namespace Vic2
{

class blockedTechSchools: commonItems::parser
{
	public:
		blockedTechSchools();

		bool isTechSchoolBlocked(const std::string& techSchool) const;

	private:
		std::vector<std::string> theBlockedTechSchools;
};


class Vic2TechSchoolsSchools: commonItems::parser
{
	public:
		Vic2TechSchoolsSchools(const blockedTechSchools& blockedTechSchools, std::istream& theStream);

		std::vector<Vic2TechSchool> getTechSchools() const { return techSchools; }

	private:
		std::vector<Vic2TechSchool> techSchools;
};

}


Vic2::blockedTechSchools::blockedTechSchools()
{
	registerKeyword(std::regex("[a-zA-Z0-9\\_]+"), [this](const std::string& techSchool, std::istream& theStream){
		theBlockedTechSchools.push_back(techSchool);
	});

	parseFile("blocked_tech_schools.txt");
}


bool Vic2::blockedTechSchools::isTechSchoolBlocked(const std::string& techSchool) const
{
	return std::any_of(theBlockedTechSchools.begin(), theBlockedTechSchools.end(),
							 [techSchool](std::string blockedTechSchool){ return techSchool == blockedTechSchool; });
}


Vic2::Vic2TechSchoolsSchools::Vic2TechSchoolsSchools(const blockedTechSchools& theBlockedTechSchools, std::istream& theStream)
{
	registerKeyword(std::regex("[a-zA-Z0-9\\_]+"), [this, theBlockedTechSchools](const std::string& name, std::istream& theStream){
		Vic2TechSchool newTechSchool(name, theStream);
		if (!theBlockedTechSchools.isTechSchoolBlocked(name))
		{
			techSchools.push_back(newTechSchool);
		}
	});

	parseStream(theStream);
}


Vic2::Vic2TechSchools::Vic2TechSchools()
{
	blockedTechSchools theBlockedTechSchools;

	registerKeyword(std::regex("schools"), [this, theBlockedTechSchools](const std::string& unused, std::istream& theStream){
		Vic2TechSchoolsSchools theSchoolsSection(theBlockedTechSchools, theStream);
		techSchools = theSchoolsSection.getTechSchools();
	});
	registerKeyword(std::regex("folders"), commonItems::ignoreItem);

	parseFile(theConfiguration.getVic2Path() + "/common/technology.txt");
}


std::string Vic2::Vic2TechSchools::findBestTechSchool(double armyInvestment, double commerceInvestment, double cultureInvestment, double industryInvestment, double navyInvestment) const
{
	double totalInvestment = armyInvestment + navyInvestment + commerceInvestment + industryInvestment + cultureInvestment;
	armyInvestment /= totalInvestment;
	navyInvestment /= totalInvestment;
	commerceInvestment /= totalInvestment;
	industryInvestment /= totalInvestment;
	cultureInvestment /= totalInvestment;

	double lowestScore = 1.0;
	std::string bestSchool = "traditional_academic";

	for (unsigned int j = 0; j < techSchools.size(); j++)
	{
		double newScore = techSchools[j].calculateComparisonScore(armyInvestment, commerceInvestment, cultureInvestment, industryInvestment, navyInvestment);
		if (newScore < lowestScore)
		{
			bestSchool = techSchools[j].getName();
			lowestScore = newScore;
		}
	}

	return bestSchool;
}