#include "TechValues.h"
#include "../EU4World/Country/EU4Country.h"
#include "../V2World/V2Country.h"
#include <algorithm>

helpers::TechValues::TechValues( const std::map<std::string, V2Country*>& countries ) 
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
		if (!isValidCountryForTechConversion(country))
		{
			continue;
		}

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

bool helpers::TechValues::isValidCountryForTechConversion(const V2Country* country) const
{
	return (country->isCivilized() && (country->getProvinces().size() > 0) && country->getSourceCountry());
}

double helpers::TechValues::getNormalizedArmyTech( const EU4::Country& country) const
{
	return getNormalizedScore(
		getCountryArmyTech(country), armyMax, armyMean);
}

double helpers::TechValues::getNormalizedNavyTech(	const EU4::Country& country ) const
{
	return getNormalizedScore(getCountryNavyTech(country), navyMax, navyMean);
}

double helpers::TechValues::getNormalizedCommerceTech( const EU4::Country& country ) const
{
	return getNormalizedScore(getCountryCommerceTech(country), commerceMax, commerceMean);
}

double helpers::TechValues::getNormalizedCultureTech( const EU4::Country& country ) const
{
	return getNormalizedScore(getCountryCultureTech(country), cultureMax, cultureMean);
}

double helpers::TechValues::getNormalizedIndustryTech( const EU4::Country& country ) const
{
	return getNormalizedScore(getCountryIndustryTech(country), industryMax, industryMean);
}

double helpers::TechValues::getCountryArmyTech( const EU4::Country& country ) const
{
	return country.getMilTech() +	country.getAdmTech() + country.getArmyInvestment() - 5;
}


double helpers::TechValues::getCountryNavyTech( const EU4::Country& country) const
{
	return country.getMilTech() +	country.getDipTech() + country.getNavyInvestment() - 5;
}


double helpers::TechValues::getCountryCommerceTech( const EU4::Country& country ) const
{
	return country.getAdmTech() +	country.getDipTech() + country.getCommerceInvestment() - 5;
}


double helpers::TechValues::getCountryCultureTech( const EU4::Country& country ) const
{
	return country.getDipTech() + country.getCultureInvestment() - 5;
}


double helpers::TechValues::getCountryIndustryTech( const EU4::Country& country ) const
{
	return country.getAdmTech() +	country.getDipTech() + country.getMilTech() + country.getIndustryInvestment() - 5;
}

double helpers::TechValues::getNormalizedScore(double score, double max, double mean) const
{
	if (mean == max)
	{
		return 1.0;
	}

	return (score - mean) / (max - mean);
}