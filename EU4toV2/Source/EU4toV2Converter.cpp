#include "Configuration.h"
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include "EU4World/World.h"
#include "Mappers/IdeaEffects/IdeaEffectMapper.h"
#include "Mappers/TechGroups/TechGroupsMapper.h"
#include "V2World/V2World.h"
#include <fstream>
#include "Mappers/VersionParser/VersionParser.h"
#include "EU4ToVic2Converter.h"

void convertEU4ToVic2(const std::string& eu4SaveFileName, const mappers::VersionParser& versionParser)
{
	ConfigurationFile configurationFile("configuration.txt");
	setOutputName(eu4SaveFileName);
	deleteExistingOutputFolder();

	const mappers::IdeaEffectMapper ideaEffectMapper;
	const mappers::TechGroupsMapper techGroupsMapper;

	const EU4::World sourceWorld(eu4SaveFileName, ideaEffectMapper);
	V2::World destWorld(sourceWorld, ideaEffectMapper, techGroupsMapper, versionParser);

	LOG(LogLevel::Info) << "* Conversion complete *";
}

void setOutputName(const std::string& eu4SaveFileName)
{
	std::string outputName = trimPath(eu4SaveFileName);
	outputName = trimExtension(outputName);
	outputName = replaceCharacter(outputName, '-');
	outputName = replaceCharacter(outputName, ' ');

	theConfiguration.setOutputName(outputName);
	LOG(LogLevel::Info) << "Using output name " << outputName;
}

std::string trimPath(const std::string& fileName)
{
	const int lastSlash = fileName.find_last_of("\\");
	return fileName.substr(lastSlash + 1, fileName.length());
}

std::string trimExtension(const std::string& fileName)
{
	const int length = fileName.find_last_of(".");
	return fileName.substr(0, length);
}

std::string replaceCharacter(std::string fileName, char character)
{
	auto position = fileName.find_first_of(character);
	while (position != std::string::npos)
	{
		fileName.replace(position, 1, "_");
		position = fileName.find_first_of(character);
	}

	return fileName;
}

void deleteExistingOutputFolder()
{
	const auto outputFolder = Utils::getCurrentDirectory() + "/output/" + theConfiguration.getOutputName();
	if (Utils::doesFolderExist(outputFolder))
	{
		if (!Utils::deleteFolder(outputFolder))
		{
			LOG(LogLevel::Error) << "Could not delete pre-existing output folder " << Utils::getCurrentDirectory() << "/output/" << theConfiguration.getOutputName();
			exit(-1);
		}
	}
}