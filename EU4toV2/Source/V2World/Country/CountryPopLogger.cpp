#include "CountryPopLogger.h"
#include <set>
#include "OSCompatibilityLayer.h"
#include "../../Mappers/Pops/PopMapper.h"
#include "../Pop/Pop.h"

void V2::CountryPopLogger::logPopsByCountry(const std::map<int, std::shared_ptr<Province>>& provinces)
{
	 // country, poptype, num
	auto filenames = Utils::GetAllFilesInFolder("./blankMod/output/history/pops/1836.1.1/");
	
	for (const auto& filename : filenames) logPopsFromFile(provinces, filename);
	outputLog(popsByCountry);
}

void V2::CountryPopLogger::logPopsFromFile(const std::map<int, std::shared_ptr<Province>>& provinces, const std::string& filename)
{
	const mappers::PopMapper popMapper("./blankMod/output/history/pops/1836.1.1/" + filename);

	for (const auto& provinceItr : popMapper.getProvincePopTypeMap())
	{
		logPopsInProvince(provinces, provinceItr.first, provinceItr.second);
	}
}

void V2::CountryPopLogger::logPopsInProvince(const std::map<int, std::shared_ptr<Province>>& provinces, const int provinceID, const mappers::PopTypes& popTypes)
{
	const auto& province = provinces.find(provinceID);
	if (province == provinces.end())
	{
		LOG(LogLevel::Warning) << "Could not find province " << provinceID << " for original pops.";
		return;
	}

	const auto& countryPopItr = getCountryForPopLogging(province->second->getOwner());

	for (const auto& popType : popTypes.getPopTypes())
	{
		totalPopulation += popType.second.getSize();
		logPop(popType.first, Pop(popType.first, popType.second), countryPopItr);
	}
}

void V2::CountryPopLogger::logPop(const std::string& popType, const Pop& pop, std::map<std::string, std::map<std::string, long int>>::iterator countryPopItr) const
{
	auto popItr = countryPopItr->second.find(popType);
	if (popItr == countryPopItr->second.end())
	{
		long int newPopSize = 0;
		const auto& newIterator = countryPopItr->second.insert(make_pair(popType, newPopSize));
		popItr = newIterator.first;
	}
	popItr->second += pop.getSize();
}

std::map<std::string, std::map<std::string, long int>>::iterator V2::CountryPopLogger::getCountryForPopLogging(const std::string& country)
{
	auto countryPopItr = popsByCountry.find(country);
	if (countryPopItr == popsByCountry.end())
	{
		std::map<std::string, long int> newCountryPop;
		const auto& newIterator = popsByCountry.insert(make_pair(country, newCountryPop));
		countryPopItr = newIterator.first;
	}

	return countryPopItr;
}

void V2::CountryPopLogger::outputLog(const std::map<std::string, std::map<std::string, long int>>& popsByCountry) const
{
	for (const auto& countryItr : popsByCountry)
	{
		long int total = 0;
		for (const auto& popsItr : countryItr.second) total += popsItr.second;

		for (const auto& popsItr : countryItr.second)
		{
			LOG(LogLevel::Info) << "," << countryItr.first << "," << popsItr.first << "," << popsItr.second << "," << static_cast<double>(popsItr.second) / total;
		}

		LOG(LogLevel::Info) << "," << countryItr.first << "," << "Total," << total << "," << static_cast<double>(total) / totalPopulation;
	}
}
