/*Copyright (c) 2019 The Paradox Game Converters Project

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/



#include "Configuration.h"
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include "EU4World/World.h"
#include "Mappers/Ideas/IdeaEffectMapper.h"
#include "Mappers/Ideas/TechGroupsMapper.h"
#include "V2World/V2World.h"
#include <fstream>



void setOutputName(const string& EU4SaveFileName);
void deleteExistingOutputFolder();
void ConvertEU4ToVic2(const string& EU4SaveFileName)
{
	ConfigurationFile configurationFile("configuration.txt");
	setOutputName(EU4SaveFileName);
	deleteExistingOutputFolder();

	std::ifstream ideaEffectsFile("idea_effects.txt");
	std::ifstream reformEffectsFile("reform_effects.txt");
	mappers::IdeaEffectMapper ideaEffectMapper(ideaEffectsFile, reformEffectsFile);
	ideaEffectsFile.close();
	reformEffectsFile.close();

	std::ifstream techGroupsFile("techGroups.txt");
	mappers::TechGroupsMapper techGroupsMapper(techGroupsFile);
	techGroupsFile.close();

	EU4::world sourceWorld(EU4SaveFileName, ideaEffectMapper);
	V2World destWorld(sourceWorld, ideaEffectMapper, techGroupsMapper);

	LOG(LogLevel::Info) << "* V2 construction: " << destWorld.getDuration() << " seconds";
	LOG(LogLevel::Info) << "* Conversion complete *";
}


string trimPath(const string& fileName);
string trimExtension(const string& fileName);
string replaceCharacter(string fileName, char character);
void setOutputName(const string& EU4SaveFileName)
{
	string outputName = trimPath(EU4SaveFileName);
	outputName = trimExtension(outputName);
	outputName = replaceCharacter(outputName, '-');
	outputName = replaceCharacter(outputName, ' ');

	theConfiguration.setOutputName(outputName);
	LOG(LogLevel::Info) << "Using output name " << outputName;
}


string trimPath(const string& fileName)
{
	const int lastSlash = fileName.find_last_of("\\");
	return fileName.substr(lastSlash + 1, fileName.length());
}


string trimExtension(const string& fileName)
{
	const int length = fileName.find_last_of(".");
	return fileName.substr(0, length);
}


string replaceCharacter(string fileName, char character)
{
	int position = fileName.find_first_of(character);
	while (position != string::npos)
	{
		fileName.replace(position, 1, "_");
		position = fileName.find_first_of(character);
	}

	return fileName;
}


void deleteExistingOutputFolder()
{
	string outputFolder = Utils::getCurrentDirectory() + "/output/" + theConfiguration.getOutputName();
	if (Utils::doesFolderExist(outputFolder.c_str()))
	{
		if (!Utils::deleteFolder(outputFolder))
		{
			LOG(LogLevel::Error) << "Could not delete pre-existing output folder " << Utils::getCurrentDirectory() << "/output/" << theConfiguration.getOutputName();
			exit(-1);
		}
	}
}