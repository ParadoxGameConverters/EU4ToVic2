#include "MappingChecker.h"
#include "Log.h"
#include "../../Mappers/ProvinceMappings/ProvinceMapper.h"
#include "../../Mappers/ReligionMapper/ReligionMapper.h"
#include "../../Mappers/CultureMapper/CultureMapper.h"
#include "../../EU4World/World.h"

void V2::MappingChecker::check(const EU4::World& sourceWorld,
	const mappers::ProvinceMapper& provinceMapper,
	const mappers::ReligionMapper& religionMapper,
	const mappers::CultureMapper& cultureMapper) const
{
	LOG(LogLevel::Info) << "-> Checking all Land Provinces Mapped (and may kraken take the rest)";
	checkAllEU4ProvincesMapped(sourceWorld, provinceMapper);
	LOG(LogLevel::Info) << "-> Checking all Cultures Mapped";
	checkAllEU4CulturesMapped(sourceWorld, cultureMapper);
	LOG(LogLevel::Info) << "-> Checking all Religions Mapped";
	checkAllEU4ReligionsMapped(sourceWorld, religionMapper);
}


void V2::MappingChecker::checkAllEU4ProvincesMapped(const EU4::World& sourceWorld, const mappers::ProvinceMapper& provinceMapper)
{
	for (const auto& eu4province : sourceWorld.getProvinces())
	{
		auto Vic2Provinces = provinceMapper.getVic2ProvinceNumbers(eu4province.first);
		if (Vic2Provinces.empty() && provinceMapper.isValidProvince(eu4province.first))
		{
			LOG(LogLevel::Warning) << "No mapping for EU4 province " << eu4province.first << ", check province_mappings.txt!";
		}
	}
}

void V2::MappingChecker::checkAllEU4ReligionsMapped(const EU4::World& sourceWorld, const mappers::ReligionMapper& religionMapper)
{
	for (const auto& EU4Religion : sourceWorld.getReligions().getAllReligions())
	{
		if (!religionMapper.getVic2Religion(EU4Religion.first))
		{
			Log(LogLevel::Warning) << "No religion mapping for EU4 religion " << EU4Religion.first;
		}
	}
}

void V2::MappingChecker::checkAllEU4CulturesMapped(const EU4::World& sourceWorld, const mappers::CultureMapper& cultureMapper)
{
	for (const auto& cultureGroupItr : sourceWorld.getCultureGroupsMapper().getCultureGroupsMap())
	{
		for (const auto& cultureItr: cultureGroupItr.second.getCultures())
		{
			const auto& matched = cultureMapper.cultureMatch(sourceWorld.getRegions(), cultureItr.first, "", 0, "");
			if (!matched)
			{
				LOG(LogLevel::Warning) << "No Vic2 culture mapping for EU4 culture " << cultureItr.first;
			}			
		}
	}
}
