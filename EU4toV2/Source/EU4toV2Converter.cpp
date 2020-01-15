#include "Configuration.h"
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include "EU4World/World.h"
#include "Mappers/IdeaEffects/IdeaEffectMapper.h"
#include "Mappers/TechGroups/TechGroupsMapper.h"
#include "V2World/V2World.h"
#include <fstream>


void setOutputName(const std::string& EU4SaveFileName);
void deleteExistingOutputFolder();
void ConvertEU4ToVic2(const std::string& EU4SaveFileName)
{
	ConfigurationFile configurationFile("configuration.txt");
	setOutputName(EU4SaveFileName);
	deleteExistingOutputFolder();

	mappers::IdeaEffectMapper ideaEffectMapper;
	mappers::TechGroupsMapper techGroupsMapper;

	EU4::World sourceWorld(EU4SaveFileName, ideaEffectMapper);
	V2World destWorld(sourceWorld, ideaEffectMapper, techGroupsMapper);

	LOG(LogLevel::Info) << "* V2 construction: " << destWorld.getDuration() << " seconds";
	LOG(LogLevel::Info) << "* Conversion complete *";
}


std::string trimPath(const std::string& fileName);
std::string trimExtension(const std::string& fileName);
std::string replaceCharacter(std::string fileName, char character);
void setOutputName(const std::string& EU4SaveFileName)
{
	std::string outputName = trimPath(EU4SaveFileName);
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
	int position = fileName.find_first_of(character);
	while (position != std::string::npos)
	{
		fileName.replace(position, 1, "_");
		position = fileName.find_first_of(character);
	}

	return fileName;
}


void deleteExistingOutputFolder()
{
	std::string outputFolder = Utils::getCurrentDirectory() + "/output/" + theConfiguration.getOutputName();
	if (Utils::doesFolderExist(outputFolder.c_str()))
	{
		if (!Utils::deleteFolder(outputFolder))
		{
			LOG(LogLevel::Error) << "Could not delete pre-existing output folder " << Utils::getCurrentDirectory() << "/output/" << theConfiguration.getOutputName();
			exit(-1);
		}
	}
}