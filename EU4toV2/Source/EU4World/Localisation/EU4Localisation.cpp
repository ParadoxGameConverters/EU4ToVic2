#include "EU4Localisation.h"
#include "Configuration.h"
#include "OSCompatibilityLayer.h"
#include <filesystem>
#include <fstream>
#include <set>
#include <vector>
namespace fs = std::filesystem;

EU4::EU4Localisation::EU4Localisation()
{
	readFromAllFilesInFolder(theConfiguration.getEU4Path() + "/localisation");
	for (const auto& itr: theConfiguration.getEU4Mods())
	{
		readFromAllFilesInFolder(itr + "/localisation");
		readFromAllFilesInFolder(itr + "/localisation/replace");
	}
}

void EU4::EU4Localisation::readFromAllFilesInFolder(const std::string& folderPath)
{
	// There is no need to recurse as EU4 doesn't support subfolders in loc dir except for "replace"
	for (const auto& fileName: commonItems::GetAllFilesInFolder(folderPath))
		readFromFile(folderPath + '/' + fileName);
}

void EU4::EU4Localisation::readFromFile(const std::string& fileName)
{
	std::ifstream locFile(fs::u8path(fileName));
	readFromStream(locFile);
	locFile.close();
}

void EU4::EU4Localisation::readFromStream(std::istream& theStream)
{
	std::string line;
	std::getline(theStream, line); // First line is the language like "l_english:"

	const auto& language = determineLanguageForFile(removeUTF8BOM(line));
	if (!language)
		return;

	while (!theStream.eof())
	{
		std::getline(theStream, line); // Subsequent lines are ' KEY: "Text"'
		const auto [key, value] = determineKeyLocalisationPair(line);
		if (!key.empty() && !value.empty())
			localisations[key][*language] = value;
	}
}

std::optional<std::string> EU4::EU4Localisation::getText(const std::string& key, const std::string& language) const
{
	const auto keyFindIter = localisations.find(key);
	if (keyFindIter == localisations.end())
		return std::nullopt;

	const auto& localisationsByLanguage = keyFindIter->second;
	const auto& languageFindIter = localisationsByLanguage.find(language);
	if (languageFindIter == localisationsByLanguage.end())
		return std::nullopt;

	return languageFindIter->second;
}

std::optional<std::map<std::string, std::string>> EU4::EU4Localisation::getTextInEachLanguage(const std::string& key) const
{
	const auto& keyFindIter = localisations.find(key);
	if (keyFindIter == localisations.end())
		return std::nullopt;
	else
		return keyFindIter->second;
}

std::optional<std::string> EU4::EU4Localisation::determineLanguageForFile(const std::string& text)
{
	if (text.size() < 2 || text[0] != 'l' || text[1] != '_')
		return std::nullopt; // Not in the desired format - no "l_"

	const size_t beginPos = 2; // Skip l_ for our language name.
	const auto endPos = text.find(':', beginPos);
	if (endPos == std::string::npos)
		return std::nullopt;

	return text.substr(beginPos, endPos - beginPos);
}

std::pair<std::string, std::string> EU4::EU4Localisation::determineKeyLocalisationPair(const std::string& text)
{
	std::pair<std::string, std::string> blankReturn;

	if (text.size() > 2 && (text[0] == '#' || text[1] == '#'))
		return blankReturn;

	const auto keyBeginPos = text.find_first_not_of(' '); // the first non-space character
	if (keyBeginPos == std::string::npos)
		return blankReturn;

	const auto keyEndPos = text.find_first_of(':', keyBeginPos + 1); // the end of the key
	const auto quotePos = text.find_first_of('"', keyEndPos);		  // the beginning of the string literal
	if (quotePos == std::string::npos)
		return blankReturn;

	const auto localisationBeginPos = quotePos + 1;								// where the localization begins
	const auto localisationEndPos = text.find_last_of('"', text.size()); // where the localization ends
	return std::make_pair(text.substr(keyBeginPos, keyEndPos - keyBeginPos), text.substr(localisationBeginPos, localisationEndPos - localisationBeginPos));
}

std::string EU4::EU4Localisation::removeUTF8BOM(const std::string& text)
{
	if (text.size() >= 3 && text[0] == '\xEF' && text[1] == '\xBB' && text[2] == '\xBF')
		return text.substr(3);
	else
		return text;
}
