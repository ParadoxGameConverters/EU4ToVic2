#include "TechSchoolMapper.h"
#include "../../Configuration.h"
#include "Log.h"
#include "ParserHelpers.h"
#include <algorithm>
#include <memory>

mappers::TechSchoolMapper::TechSchoolMapper()
{
	registerKeys();
	parseFile(theConfiguration.getVic2Path() + "/common/technology.txt");
	clearRegisteredKeywords();
}

mappers::TechSchoolMapper::TechSchoolMapper(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::TechSchoolMapper::registerKeys()
{
	registerKeyword("schools", [this](const std::string& unused, std::istream& theStream) 
		{
			TechSchools theTechSchools(theStream);
			techSchools = theTechSchools.getTechSchools();
		});
	registerRegex("[a-zA-Z0-9\\_.:]+", commonItems::ignoreItem);
}

std::string mappers::TechSchoolMapper::findBestTechSchool(
	double armyInvestment,
	double commerceInvestment,
	double cultureInvestment,
	double industryInvestment,
	double navyInvestment) const
{
	double totalInvestment = armyInvestment + navyInvestment + commerceInvestment + industryInvestment + cultureInvestment;
	armyInvestment /= totalInvestment;
	navyInvestment /= totalInvestment;
	commerceInvestment /= totalInvestment;
	industryInvestment /= totalInvestment;
	cultureInvestment /= totalInvestment;

	double lowestScore = 1.0;
	std::string bestSchool = "traditional_academic";

	for (const auto& techSchool : techSchools)
	{
		double newScore = techSchool.second.calculateComparisonScore(armyInvestment, commerceInvestment, cultureInvestment, industryInvestment, navyInvestment);
		if (newScore < lowestScore)
		{
			bestSchool = techSchool.first;
			lowestScore = newScore;
		}
	}
	return bestSchool;
}
