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



#include "TechConversionHelpers.h"
#include "../EU4World/EU4Country.h"
#include "../Mappers/IdeaEffectMapper.h"
#include "../V2World/V2Country.h"
#include "../Configuration.h"



double helpers::getCountryArmyTech(const std::shared_ptr<EU4::Country>& country)
{
	return country->getMilTech() + country->getAdmTech() + ideaEffectMapper::getArmyTechFromIdeas(country->getNationalIdeas());
}


double helpers::getCountryNavyTech(const std::shared_ptr<EU4::Country>& country)
{
	return country->getMilTech() + country->getDipTech() + ideaEffectMapper::getNavyTechFromIdeas(country->getNationalIdeas());
}


double helpers::getCountryCommerceTech(const std::shared_ptr<EU4::Country>& country)
{
	return country->getAdmTech() + country->getDipTech() + ideaEffectMapper::getCommerceTechFromIdeas(country->getNationalIdeas());
}


double helpers::getCountryCultureTech(const std::shared_ptr<EU4::Country>& country)
{
	return country->getDipTech() + ideaEffectMapper::getCultureTechFromIdeas(country->getNationalIdeas());
}


double helpers::getCountryIndustryTech(const std::shared_ptr<EU4::Country>& country)
{
	return country->getAdmTech() + country->getDipTech() + country->getMilTech() + ideaEffectMapper::getIndustryTechFromIdeas(country->getNationalIdeas());
}


helpers::TechValues::TechValues(const std::map<std::string, V2Country*>& countries)
{
	int numValidCountries = 0;
	double armyTotal = 0.0;
	double navyTotal = 0.0;
	double commerceTotal = 0.0;
	double cultureTotal = 0.0;
	double industryTotal = 0.0;
	for (auto countryItr: countries)
	{
		auto country = countryItr.second;
		if ((theConfiguration.getVic2Gametype() != "vanilla") && !country->isCivilized())
			continue;

		if (country->getProvinces().size() == 0)
			continue;

		helpers::updateMaxAndTotal(armyMax, armyTotal, helpers::getCountryArmyTech(country->getSourceCountry()));
		helpers::updateMaxAndTotal(navyMax, navyTotal, helpers::getCountryNavyTech(country->getSourceCountry()));
		helpers::updateMaxAndTotal(commerceMax, commerceTotal, helpers::getCountryCommerceTech(country->getSourceCountry()));
		helpers::updateMaxAndTotal(cultureMax, cultureTotal, helpers::getCountryCultureTech(country->getSourceCountry()));
		helpers::updateMaxAndTotal(industryMax, industryTotal, helpers::getCountryIndustryTech(country->getSourceCountry()));
		numValidCountries++;
	}

	armyMean = armyTotal / numValidCountries;
	navyMean = navyTotal / numValidCountries;
	commerceMean = commerceTotal / numValidCountries;
	cultureMean = cultureTotal / numValidCountries;
	industryMean = industryTotal / numValidCountries;
}


void helpers::updateMaxAndTotal(double& max, double& total, double techLevel)
{
	if (techLevel > max)
	{
		max = techLevel;
	}

	total += techLevel;
}


double helpers::getNormalizedScore(double score, double max, double mean)
{
	if (mean == max)
		return max;
	return (score - mean) / (max - mean);
};