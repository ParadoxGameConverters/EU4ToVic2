#include "Configuration.h"
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include "EU4World/World.h"
#include "Mappers/IdeaEffects/IdeaEffectMapper.h"
#include "Mappers/TechGroups/TechGroupsMapper.h"
#include "V2World/V2World.h"
#include "Mappers/VersionParser/VersionParser.h"
#include "EU4ToVic2Converter.h"

void convertEU4ToVic2(const mappers::VersionParser& versionParser)
{
	ConfigurationFile configurationFile("configuration.txt");
	deleteExistingOutputFolder();

	const mappers::IdeaEffectMapper ideaEffectMapper;
	const mappers::TechGroupsMapper techGroupsMapper;

	const EU4::World sourceWorld(ideaEffectMapper);
	V2::World destWorld(sourceWorld, ideaEffectMapper, techGroupsMapper, versionParser);

	LOG(LogLevel::Info) << "* Conversion complete *";
}


void deleteExistingOutputFolder()
{
	const auto outputFolder = Utils::GetCurrentDirectory() + "/output/" + theConfiguration.getOutputName();
	if (Utils::DoesFolderExist(outputFolder))
	{
		if (!Utils::DeleteFolder(outputFolder))
		{
			LOG(LogLevel::Error) << "Could not delete pre-existing output folder " << Utils::GetCurrentDirectory() << "/output/" << theConfiguration.getOutputName();
			exit(-1);
		}
	}
}