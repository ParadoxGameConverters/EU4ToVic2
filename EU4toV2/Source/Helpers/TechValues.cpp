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



#include "TechValues.h"
#include "../EU4World/EU4Country.h"
#include "../Mappers/Ideas/IdeaEffectMapper.h"
#include "../V2World/V2Country.h"
#include "../Configuration.h"
#include <algorithm>



helpers::TechValues::TechValues(
	const std::map<std::string, V2Country*>& countries,
	const mappers::IdeaEffectMapper& ideaEffectMapper
) {
	int numValidCountries = 0;
	double armyTotal = 0.0;
	double navyTotal = 0.0;
	double commerceTotal = 0.0;
	double cultureTotal = 0.0;
	double industryTotal = 0.0;

	for (auto countryItr: countries)
	{
		auto country = countryItr.second;
		if (!isValidCountryForTechConversion(country))
		{
			continue;
		}

		armyMax = std::max(armyMax, getCountryArmyTech(*country->getSourceCountry(), ideaEffectMapper));
		armyTotal += getCountryArmyTech(*country->getSourceCountry(), ideaEffectMapper);
		navyMax = std::max(navyMax, getCountryNavyTech(*country->getSourceCountry(), ideaEffectMapper));
		navyTotal += getCountryNavyTech(*country->getSourceCountry(), ideaEffectMapper);
		commerceMax = std::max(commerceMax, getCountryCommerceTech(*country->getSourceCountry(), ideaEffectMapper));
		commerceTotal += getCountryCommerceTech(*country->getSourceCountry(), ideaEffectMapper);
		cultureMax = std::max(cultureMax, getCountryCultureTech(*country->getSourceCountry(), ideaEffectMapper));
		cultureTotal += getCountryCultureTech(*country->getSourceCountry(), ideaEffectMapper);
		industryMax = std::max(industryMax, getCountryIndustryTech(*country->getSourceCountry(), ideaEffectMapper));
		industryTotal += getCountryIndustryTech(*country->getSourceCountry(), ideaEffectMapper);
		numValidCountries++;
	}

	if (numValidCountries > 0)
	{
		armyMean = armyTotal / numValidCountries;
		navyMean = navyTotal / numValidCountries;
		commerceMean = commerceTotal / numValidCountries;
		cultureMean = cultureTotal / numValidCountries;
		industryMean = industryTotal / numValidCountries;
	}
}


bool helpers::TechValues::isValidCountryForTechConversion(const V2Country* country) const
{
	return (country->isCivilized() && (country->getProvinces().size() > 0) && country->getSourceCountry());
}


double helpers::TechValues::getNormalizedArmyTech(
	const EU4::Country& country,
	const mappers::IdeaEffectMapper& ideaEffectMapper
) const
{
	return getNormalizedScore(
		getCountryArmyTech(country, ideaEffectMapper), armyMax, armyMean);
}


double helpers::TechValues::getNormalizedNavyTech(
	const EU4::Country& country,
	const mappers::IdeaEffectMapper& ideaEffectMapper
) const
{
	return getNormalizedScore(getCountryNavyTech(country, ideaEffectMapper), navyMax, navyMean);
}


double helpers::TechValues::getNormalizedCommerceTech(
	const EU4::Country& country,
	const mappers::IdeaEffectMapper& ideaEffectMapper
) const
{
	return getNormalizedScore(getCountryCommerceTech(country, ideaEffectMapper), commerceMax, commerceMean);
}


double helpers::TechValues::getNormalizedCultureTech(
	const EU4::Country& country,
	const mappers::IdeaEffectMapper& ideaEffectMapper
) const
{
	return getNormalizedScore(getCountryCultureTech(country, ideaEffectMapper), cultureMax, cultureMean);
}


double helpers::TechValues::getNormalizedIndustryTech(
	const EU4::Country& country,
	const mappers::IdeaEffectMapper& ideaEffectMapper
) const
{
	return getNormalizedScore(getCountryIndustryTech(country, ideaEffectMapper), industryMax, industryMean);
}


double helpers::TechValues::getCountryArmyTech(
	const EU4::Country& country,
	const mappers::IdeaEffectMapper& ideaEffectMapper
) const
{
	return country.getMilTech() +
		country.getAdmTech() +
		ideaEffectMapper.getArmyTechFromIdeas(country.getNationalIdeas());
}


double helpers::TechValues::getCountryNavyTech(
	const EU4::Country& country,
	const mappers::IdeaEffectMapper& ideaEffectMapper
) const
{
	return country.getMilTech() +
		country.getDipTech() +
		ideaEffectMapper.getNavyTechFromIdeas(country.getNationalIdeas());
}


double helpers::TechValues::getCountryCommerceTech(
	const EU4::Country& country,
	const mappers::IdeaEffectMapper& ideaEffectMapper
) const
{
	return country.getAdmTech() +
		country.getDipTech() +
		ideaEffectMapper.getCommerceTechFromIdeas(country.getNationalIdeas());
}


double helpers::TechValues::getCountryCultureTech(
	const EU4::Country& country,
	const mappers::IdeaEffectMapper& ideaEffectMapper
) const
{
	return country.getDipTech() + ideaEffectMapper.getCultureTechFromIdeas(country.getNationalIdeas());
}


double helpers::TechValues::getCountryIndustryTech(
	const EU4::Country& country,
	const mappers::IdeaEffectMapper& ideaEffectMapper
) const
{
	return country.getAdmTech() +
		country.getDipTech() +
		country.getMilTech() +
		ideaEffectMapper.getIndustryTechFromIdeas(country.getNationalIdeas());
}


double helpers::TechValues::getNormalizedScore(double score, double max, double mean) const
{
	if (mean == max)
	{
		return 1.0;
	}

	return (score - mean) / (max - mean);
}