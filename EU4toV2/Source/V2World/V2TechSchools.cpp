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
#include "BlockedTechSchools.h"
#include "../Configuration.h"
#include "Log.h"
#include "ParserHelpers.h"
#include <algorithm>
#include <memory>



Vic2::TechSchools::TechSchools(std::istream& theStream, std::unique_ptr<blockedTechSchools>& theBlockedTechSchools)
{
	registerKeyword(std::regex("[a-zA-Z0-9\\_]+"), [this, &theBlockedTechSchools](const std::string& name, std::istream& theStream){
		Vic2TechSchool newTechSchool(name, theStream);
		if (!theBlockedTechSchools->isTechSchoolBlocked(name))
		{
			techSchools.push_back(newTechSchool);
		}
	});

	parseStream(theStream);
}


std::string Vic2::TechSchools::findBestTechSchool(double armyInvestment, double commerceInvestment, double cultureInvestment, double industryInvestment, double navyInvestment) const
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


Vic2::TechSchoolsFile::TechSchoolsFile(std::unique_ptr<blockedTechSchools> suppliedBlockedTechSchools)
{
	registerKeyword(std::regex("schools"), [this, &suppliedBlockedTechSchools](const std::string& unused, std::istream& theStream){
		theTechSchools = std::make_unique<TechSchools>(theStream, suppliedBlockedTechSchools);
	});
	registerKeyword(std::regex("folders"), commonItems::ignoreItem);

	parseFile(theConfiguration.getVic2Path() + "/common/technology.txt");
}