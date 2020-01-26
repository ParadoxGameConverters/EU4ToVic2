#include "TechValues.h"
#include "../EU4World/Country/EU4Country.h"
#include "../V2World/Country/Country.h"
#include <algorithm>

helpers::TechValues::TechValues(const std::map<std::string, std::shared_ptr<V2::Country>>& countries)
{
	auto numValidCountries = 0;
	double armyTotal = 0;
	double navyTotal = 0;
	double commerceTotal = 0;
	double cultureTotal = 0;
	double industryTotal = 0;

	for (const auto& countryItr: countries)
	{
		const auto& country = countryItr.second;
		if (!isValidCountryForTechConversion(*country)) continue;

		armyMax = std::max(armyMax, getCountryArmyTech(*country->getSourceCountry()));
		armyTotal += getCountryArmyTech(*country->getSourceCountry());
		navyMax = std::max(navyMax, getCountryNavyTech(*country->getSourceCountry()));
		navyTotal += getCountryNavyTech(*country->getSourceCountry());
		commerceMax = std::max(commerceMax, getCountryCommerceTech(*country->getSourceCountry()));
		commerceTotal += getCountryCommerceTech(*country->getSourceCountry());
		cultureMax = std::max(cultureMax, getCountryCultureTech(*country->getSourceCountry()));
		cultureTotal += getCountryCultureTech(*country->getSourceCountry());
		industryMax = std::max(industryMax, getCountryIndustryTech(*country->getSourceCountry()));
		industryTotal += getCountryIndustryTech(*country->getSourceCountry());
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

bool helpers::TechValues::isValidCountryForTechConversion(const V2::Country& country)
{
	return country.isCivilized() && !country.getProvinces().empty() && country.getSourceCountry();
}

double helpers::TechValues::getNormalizedArmyTech(const EU4::Country& country) const
{
	return getNormalizedScore(
		getCountryArmyTech(country), armyMax, armyMean);
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

double helpers::TechValues::getCountryArmyTech(const EU4::Country& country)
{
	return country.getMilTech() +	country.getAdmTech() + country.getArmy();
}

double helpers::TechValues::getCountryNavyTech(const EU4::Country& country)
{
	return country.getMilTech() +	country.getDipTech() + country.getNavy();
}

double helpers::TechValues::getCountryCommerceTech(const EU4::Country& country)
{
	return country.getAdmTech() +	country.getDipTech() + country.getCommerce();
}

double helpers::TechValues::getCountryCultureTech(const EU4::Country& country)
{
	return country.getDipTech() + country.getCulture();
}

double helpers::TechValues::getCountryIndustryTech(const EU4::Country& country)
{
	return country.getAdmTech() +	country.getDipTech() + country.getMilTech() + country.getIndustry();
}

double helpers::TechValues::getNormalizedScore(const double score, const double max, const double mean)
{
	if (mean == max) return 1;
	return (score - mean) / (max - mean);
}