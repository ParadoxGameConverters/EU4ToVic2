#include "VanillaWorld.h"
#include "../../Configuration.h"
#include "Log.h"
#include <fstream>

V2::VanillaWorld::VanillaWorld():
	provinceMapper(mappers::ProvinceMapper("configurables/province_mappings.txt")),
	stateMapper(mappers::StateMapper(theConfiguration.getVanillaVic2Path() + "/map/region.txt"))
{
	Log(LogLevel::Info) << "Loading vanilla mappings";
	determineProvinceNumbers();
	verifyMappings();
};

void V2::VanillaWorld::determineProvinceNumbers()
{
	for (const auto& mapping: provinceMapper.getVic2ToEU4ProvinceMap())
	{
		if (!(mapping.second).empty())
			provinces.push_back(mapping.first);
	}
}

void V2::VanillaWorld::verifyMappings()
{
	Log(LogLevel::Info) << "\t-> Verifying vanilla mappings";
	std::ofstream output("output/vanillaMappings.log");
	if (!output.is_open())
		Log(LogLevel::Debug) << "Could not create vanillaMappings.log";

	for (const auto& province: provinces)
	{
		const auto& eu4Provs = provinceMapper.getEU4ProvinceNumbers(province);
		output << "V2: " << province << "\tEU4:";
		for (const auto& eu4Prov: eu4Provs)
		{
			output << " " << eu4Prov;
		}
		output << "\n";
	}
}