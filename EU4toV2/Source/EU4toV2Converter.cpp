#include "Configuration.h"
#include "EU4ToVic2Converter.h"
#include "EU4World/World.h"
#include "Log.h"
#include "Mappers/IdeaEffects/IdeaEffectMapper.h"
#include "Mappers/TechGroups/TechGroupsMapper.h"
#include "OSCompatibilityLayer.h"
#include "V2World/V2World.h"

void convertEU4ToVic2(const commonItems::ConverterVersion& converterVersion)
{
	Log(LogLevel::Progress) << "0 %";
	ConfigurationFile configurationFile("configuration.txt", converterVersion);
	deleteExistingOutputFolder();
	Log(LogLevel::Progress) << "4 %";

	const mappers::IdeaEffectMapper ideaEffectMapper;
	const mappers::TechGroupsMapper techGroupsMapper;
	Log(LogLevel::Progress) << "5 %";

	const EU4::World sourceWorld(ideaEffectMapper, converterVersion);
	V2::World destWorld(sourceWorld, ideaEffectMapper, techGroupsMapper, converterVersion);

	Log(LogLevel::Info) << "* Conversion complete *";
	Log(LogLevel::Progress) << "100 %";
}


void deleteExistingOutputFolder()
{
	const auto outputFolder = "output/" + theConfiguration.getOutputName();
	if (commonItems::DoesFolderExist(outputFolder))
	{
		if (!commonItems::DeleteFolder(outputFolder))
		{
			throw std::runtime_error("Could not delete pre-existing output folder: output/" + theConfiguration.getOutputName());
		}
	}
}