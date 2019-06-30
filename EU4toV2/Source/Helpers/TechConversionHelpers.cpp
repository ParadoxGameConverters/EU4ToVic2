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

		updateMaxAndTotal(armyMax, armyTotal, getCountryArmyTech(*country->getSourceCountry()));
		updateMaxAndTotal(navyMax, navyTotal, getCountryNavyTech(*country->getSourceCountry()));
		updateMaxAndTotal(commerceMax, commerceTotal, getCountryCommerceTech(*country->getSourceCountry()));
		updateMaxAndTotal(cultureMax, cultureTotal, getCountryCultureTech(*country->getSourceCountry()));
		updateMaxAndTotal(industryMax, industryTotal, getCountryIndustryTech(*country->getSourceCountry()));
		numValidCountries++;
	}

	armyMean = armyTotal / numValidCountries;
	navyMean = navyTotal / numValidCountries;
	commerceMean = commerceTotal / numValidCountries;
	cultureMean = cultureTotal / numValidCountries;
	industryMean = industryTotal / numValidCountries;
}


double helpers::TechValues::getNormalizedArmyTech(const EU4::Country& country) const
{
	return getNormalizedScore(getCountryArmyTech(country), armyMax, armyMean);
}


double helpers::TechValues::getNormalizedNavyTech(const EU4::Country& country) const
{
	return getNormalizedScore(getCountryNavyTech(country), navyMax, navyMean);
}


double helpers::TechValues::getNormalizedCommerceTech(const EU4::Country& country) const
{
	return getNormalizedScore(getCountryCommerceTech(country), commerceMax, commerceMean);
}


double helpers::TechValues::getNormalizedCultureTech(const EU4::Country& country) const
{
	return getNormalizedScore(getCountryCultureTech(country), cultureMax, cultureMean);
}


double helpers::TechValues::getNormalizedIndustryTech(const EU4::Country& country) const
{
	return getNormalizedScore(getCountryIndustryTech(country), industryMax, industryMean);
}


double helpers::TechValues::getCountryArmyTech(const EU4::Country& country) const
{
	return country.getMilTech() + country.getAdmTech() + ideaEffectMapper::getArmyTechFromIdeas(country.getNationalIdeas());
}


double helpers::TechValues::getCountryNavyTech(const EU4::Country& country) const
{
	return country.getMilTech() + country.getDipTech() + ideaEffectMapper::getNavyTechFromIdeas(country.getNationalIdeas());
}


double helpers::TechValues::getCountryCommerceTech(const EU4::Country& country) const
{
	return country.getAdmTech() + country.getDipTech() + ideaEffectMapper::getCommerceTechFromIdeas(country.getNationalIdeas());
}


double helpers::TechValues::getCountryCultureTech(const EU4::Country& country) const
{
	return country.getDipTech() + ideaEffectMapper::getCultureTechFromIdeas(country.getNationalIdeas());
}


double helpers::TechValues::getCountryIndustryTech(const EU4::Country& country) const
{
	return country.getAdmTech() + country.getDipTech() + country.getMilTech() + ideaEffectMapper::getIndustryTechFromIdeas(country.getNationalIdeas());
}


void helpers::TechValues::updateMaxAndTotal(double& max, double& total, double techLevel)
{
	if (techLevel > max)
	{
		max = techLevel;
	}

	total += techLevel;
}


double helpers::TechValues::getNormalizedScore(double score, double max, double mean) const
{
	if (mean == max)
	{
		return max;
	}

	return (score - mean) / (max - mean);
};