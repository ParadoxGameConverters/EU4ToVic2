#include "TechValues.h"
#include "../EU4World/Country/EU4Country.h"
#include "../V2World/Country/Country.h"
#include <algorithm>
#include "Log.h"

helpers::TechValues::TechValues(const std::map<std::string, std::shared_ptr<V2::Country>>& countries)
{
	std::vector<double> armyScores;
	std::vector<double> navyScores;
	std::vector<double> commerceScores;
	std::vector<double> cultureScores;
	std::vector<double> industryScores;
	int totalCountries = 0;
	
	for (const auto& countryItr: countries)
	{
		const auto& country = countryItr.second;
		if (!isValidCountryForTechConversion(*country)) continue;

		auto armyScore = getCountryArmyTech(*country->getSourceCountry());
		armyScores.emplace_back(armyScore);
		armyMean += armyScore;

		auto navyScore = getCountryNavyTech(*country->getSourceCountry());
		navyScores.emplace_back(navyScore);
		navyMean += navyScore;

		auto commerceScore = getCountryCommerceTech(*country->getSourceCountry());
		commerceScores.emplace_back(commerceScore);
		commerceMean += commerceScore;

		auto cultureScore = getCountryCultureTech(*country->getSourceCountry());
		cultureScores.emplace_back(cultureScore);
		cultureMean += cultureScore;

		auto industryScore = getCountryIndustryTech(*country->getSourceCountry());
		industryScores.emplace_back(industryScore);
		industryMean += industryScore;
		
		++totalCountries;
	}

	if (!armyScores.empty()) armyMean /= totalCountries;
	if (!navyScores.empty()) navyMean /= totalCountries;
	if (!commerceScores.empty()) commerceMean /= totalCountries;
	if (!cultureScores.empty()) cultureMean /= totalCountries;
	if (!industryScores.empty()) industryMean /= totalCountries;

	armyMean = dropOutliers(armyScores, armyMean);
	navyMean = dropOutliers(navyScores, navyMean);
	commerceMean = dropOutliers(commerceScores, commerceMean);
	cultureMean = dropOutliers(cultureScores, cultureMean);
	industryMean = dropOutliers(industryScores, industryMean);
	
	if (!armyScores.empty()) armyMax = armyScores.back();
	if (!navyScores.empty()) navyMax = navyScores.back();
	if (!commerceScores.empty()) commerceMax = commerceScores.back();
	if (!cultureScores.empty()) cultureMax = cultureScores.back();
	if (!industryScores.empty()) industryMax = industryScores.back();

	Log(LogLevel::Debug) << "Industry median : " << industryMedian << "Industry mean : " << industryMean << "industry min : " << industryMin << " industry max: " << industryMax;

}

bool helpers::TechValues::isValidCountryForTechConversion(const V2::Country& country)
{
	return country.isCivilized() && !country.getProvinces().empty() && country.getSourceCountry();
}

double helpers::TechValues::getNormalizedArmyTech(const EU4::Country& country) const
{
	return getNormalizedScore(getCountryArmyTech(country), armyMax, armyMedian);
}

double helpers::TechValues::getNormalizedNavyTech(const EU4::Country& country) const
{
	return getNormalizedScore(getCountryNavyTech(country), navyMax, navyMedian);
}

double helpers::TechValues::getNormalizedCommerceTech(const EU4::Country& country) const
{
	return getNormalizedScore(getCountryCommerceTech(country), commerceMax, commerceMedian);
}

double helpers::TechValues::getNormalizedCultureTech(const EU4::Country& country) const
{
	return getNormalizedScore(getCountryCultureTech(country), cultureMax, cultureMedian);
}

double helpers::TechValues::getNormalizedIndustryTech(const EU4::Country& country) const
{
	Log(LogLevel::Debug) << country.getTag() << " " << country.getName() << " normalized score: " << getNormalizedScore(getCountryIndustryTech(country), industryMax, industryMedian);
	return getNormalizedScore(getCountryIndustryTech(country), industryMax, industryMedian);
}

double helpers::TechValues::getCountryArmyTech(const EU4::Country& country)
{
	return (country.getMilTech() + country.getAdmTech()) * (1 + country.getArmy() / 10);
}

double helpers::TechValues::getCountryNavyTech(const EU4::Country& country)
{
	return (country.getMilTech() + country.getDipTech()) * (1 + country.getNavy() / 10);
}

double helpers::TechValues::getCountryCommerceTech(const EU4::Country& country)
{
	return (country.getAdmTech() + country.getDipTech()) * (1 + country.getCommerce() / 10);
}

double helpers::TechValues::getCountryCultureTech(const EU4::Country& country)
{
	return country.getDipTech() * (1 + country.getCulture() / 10);
}

double helpers::TechValues::getCountryIndustryTech(const EU4::Country& country)
{
	Log(LogLevel::Debug) << country.getName() << " ind score: " << (country.getAdmTech() + country.getDipTech() + country.getMilTech()) * ( 1.0 + country.getIndustry() / 10.0) << " indscore " << country.getIndustry();
	return (country.getAdmTech() + country.getDipTech() + country.getMilTech()) * ( 1.0 + country.getIndustry() / 10.0);
}

double helpers::TechValues::getNormalizedScore(const double score, const double max, const double median)
{
	if (median == max) return 1;
	return (score - median) / (max - median);
}

double helpers::TechValues::dropOutliers(std::vector<double>& scores, const double mean)
{
	auto size = scores.size();
	if (size == 0) return 0;
	std::sort(scores.begin(), scores.end());
	auto mid = size/2;

	const auto medianRaw = size % 2 == 0 ? (scores[mid] + scores[mid-1]) / 2 : scores[mid];

	// Drop outliers.
	std::vector<double> filteredScores;
	for (const auto& score: scores) if (std::abs(score - medianRaw) < 0.5 * medianRaw) filteredScores.emplace_back(score);
	scores.swap(filteredScores);
	
	size = scores.size();
	if (size == 0) return 0;
	mid = size/2;
	return size % 2 == 0 ? (scores[mid] + scores[mid-1]) / 2 : scores[mid];
}