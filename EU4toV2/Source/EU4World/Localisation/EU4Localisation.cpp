#include "EU4Localisation.h"
#include <fstream>
#include <vector>
#include <set>
#include "OSCompatibilityLayer.h"
#include "../../Configuration.h"
#include <filesystem>
namespace fs = std::filesystem;

EU4::EU4Localisation::EU4Localisation()
{
	readFromAllFilesInFolder(theConfiguration.getEU4Path() + "/localisation");
	for (const auto& itr : theConfiguration.getEU4Mods())
	{
		readFromAllFilesInFolder(itr + "/localisation");
		readFromAllFilesInFolder(itr + "/localisation/replace");
	}
}

void EU4::EU4Localisation::readFromFile(const std::string& fileName)
{
	std::ifstream in(fs::u8path(fileName));

	const int maxLineLength = 10000;	// the maximum line length
	char line[maxLineLength];			// the line being processed

	// First line is the language like "l_english:"
	in.getline(line, maxLineLength);
	const auto& language = determineLanguageForFile(removeUTF8BOM(line));
	if (language.empty())
	{
		return;
	}

	// Subsequent lines are 'KEY: "Text"'
	while (!in.eof())
	{
		in.getline(line, maxLineLength);
		if (!in.eof())
		{
			const auto keyLocalisationPair = determineKeyLocalisationPair(removeUTF8BOM(line));
			const std::string& key = keyLocalisationPair.first;
			const std::string& currentLocalisation = keyLocalisationPair.second;
			if (!key.empty() && !currentLocalisation.empty())
			{
				localisations[key][language] = currentLocalisation;
			}
		}
	}
}

void EU4::EU4Localisation::readFromAllFilesInFolder(const std::string& folderPath)
{
	// Get all files in the folder.
	auto fileNames = commonItems::GetAllFilesInFolder(folderPath);

	// Read all these files.
	for (const auto& fileName : fileNames)
	{
		readFromFile(folderPath + '/' + fileName);
	}
}

const std::string& EU4::EU4Localisation::getText(const std::string& key, const std::string& language) const
{
	static const std::string noLocalisation; // used if there's no localization

	const auto keyFindIter = localisations.find(key);
	if (keyFindIter == localisations.end())
	{
		return noLocalisation;
	}
	const auto& localisationsByLanguage = keyFindIter->second;
	const auto& languageFindIter = localisationsByLanguage.find(language);
	if (languageFindIter == localisationsByLanguage.end())
	{
		return noLocalisation;
	}

	return languageFindIter->second;
}

const std::map<std::string, std::string>& EU4::EU4Localisation::getTextInEachLanguage(const std::string& key) const
{
	static const std::map<std::string, std::string> noLocalisation; // used if there's no localization

	const auto& keyFindIter = localisations.find(key);
	if (keyFindIter == localisations.end())
	{
		return noLocalisation;
	}

	return keyFindIter->second;
}

std::string EU4::EU4Localisation::determineLanguageForFile(const std::string& text)
{
	static const std::string noLanguageIndicated;	// used when no language is indicated

	if (text.size() < 2 || text[0] != 'l' || text[1] != '_')
	{	// Not in the desired format - no "l_"
		return noLanguageIndicated;
	}
	const size_t beginPos = 2;	// Skip l_ for our language name.
	const auto endPos = text.find(':', beginPos);	// the end of the language name
	if (endPos == std::string::npos)
	{	// Not in the desired format - no ":"
		return noLanguageIndicated;
	}

	return text.substr(beginPos, endPos - beginPos);
}

std::pair<std::string, std::string> EU4::EU4Localisation::determineKeyLocalisationPair(const std::string& text)
{
	static const std::pair<std::string, std::string> noLocalisationPair;	// used when there's no localization pair

	const auto keyBeginPos = text.find_first_not_of(' ');	// the first non-space character
	if (keyBeginPos == std::string::npos)
	{
		return noLocalisationPair;
	}
	const auto keyEndPos = text.find_first_of(':', keyBeginPos + 1); // the end of the key
	const auto quotePos = text.find_first_of('"', keyEndPos); // the beginning of the string literal
	if (quotePos == std::string::npos)
	{
		return noLocalisationPair;
	}
	const auto localisationBeginPos = quotePos + 1;	// where the localization begins
	const auto localisationEndPos = text.find_first_of('"', localisationBeginPos); // where the localization ends
	return std::make_pair(text.substr(keyBeginPos, keyEndPos - keyBeginPos), text.substr(localisationBeginPos, localisationEndPos - localisationBeginPos));
}

std::string EU4::EU4Localisation::removeUTF8BOM(const std::string& text)
{
	if (text.size() >= 3 && text[0] == '\xEF' && text[1] == '\xBB' && text[2] == '\xBF')
	{
		return text.substr(3);
	}
	return text;
}
