#include "CultureMapping.h"

mappers::CultureMapping::CultureMapping(
	const std::string& _sourceCulture,
	const std::string& _destinationCulture,
	const std::map<DISTINGUISHER_TYPES, std::string>& _distinguishers
):
	sourceCulture(_sourceCulture),
	destinationCulture(_destinationCulture),
	distinguishers(_distinguishers)
{}

std::optional<std::string> mappers::CultureMapping::cultureMatch(
	const EU4::Regions& EU4Regions,
	const std::string& culture,
	const std::string& religion,
	int EU4Province,
	const std::string& ownerTag
) const
{
	if (sourceCulture == culture)
	{
		if (distinguishersMatch(EU4Regions, distinguishers, religion, EU4Province, ownerTag))
		{
			return destinationCulture;
		}
	}
	return std::nullopt;
}

bool mappers::CultureMapping::distinguishersMatch(
	const EU4::Regions& EU4Regions,
	const std::map<DISTINGUISHER_TYPES, std::string>& distinguishers,
	const std::string& religion,
	int EU4Province,
	const std::string& ownerTag) const
{
	for (auto currentDistinguisher: distinguishers)
	{
		if (currentDistinguisher.first == DISTINGUISHER_TYPES::OWNER)
		{
			if (ownerTag != currentDistinguisher.second)
			{
				return false;
			}
		}
		else if (currentDistinguisher.first == DISTINGUISHER_TYPES::RELIGION)
		{
			if (religion != currentDistinguisher.second)
			{
				return false;
			}
		}
		else if (currentDistinguisher.first == DISTINGUISHER_TYPES::PROVINCE)
		{
			if (stoi(currentDistinguisher.second) != EU4Province)
			{
				return false;
			}
		}
		else if (currentDistinguisher.first == DISTINGUISHER_TYPES::REGION)
		{
			if (!EU4Regions.provinceInRegion(EU4Province, currentDistinguisher.second))
			{
				return false;
			}
		}
	}

	return true;
}