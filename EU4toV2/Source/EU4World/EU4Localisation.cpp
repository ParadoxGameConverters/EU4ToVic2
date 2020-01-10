#include "EU4Localisation.h"
#include <fstream>
#include <vector>
#include <set>
#include "OSCompatibilityLayer.h"

void EU4::EU4Localisation::ReadFromFile(const std::string& fileName)
{
	std::ifstream in(fileName);

	const int maxLineLength = 10000;	// the maximum line length
	char line[maxLineLength];			// the line being processed

	// First line is the language like "l_english:"
	in.getline(line, maxLineLength);
	std::string language = DetermineLanguageForFile(RemoveUTF8BOM(line));
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
			const auto keyLocalisationPair = DetermineKeyLocalisationPair(RemoveUTF8BOM(line));		// the localisation pair
			const std::string& key = keyLocalisationPair.first;												// the key from the pair
			const std::string& currentLocalisation = keyLocalisationPair.second;							// the localisation from the pair
			if (!key.empty() && !currentLocalisation.empty())
			{
				localisations[key][language] = currentLocalisation;
			}
		}
	}
}

void EU4::EU4Localisation::ReadFromAllFilesInFolder(const std::string& folderPath)
{
	// Get all files in the folder.
	std::set<std::string> fileNames;
	Utils::GetAllFilesInFolder(folderPath, fileNames);

	// Read all these files.
	for (const auto& fileName : fileNames)
	{
		ReadFromFile(folderPath + '/' + fileName);
	}
}

const std::string& EU4::EU4Localisation::GetText(const std::string& key, const std::string& language) const
{
	static const std::string noLocalisation = "";	// used if there's no localisation

	const auto keyFindIter = localisations.find(key);	// the localisations for this key
	if (keyFindIter == localisations.end())
	{
		return noLocalisation;
	}
	const auto& localisationsByLanguage = keyFindIter->second;	// the localisations for this language
	const auto languageFindIter = localisationsByLanguage.find(language);	// the localisation we want
	if (languageFindIter == localisationsByLanguage.end())
	{
		return noLocalisation;
	}

	return languageFindIter->second;
}

const std::map<std::string, std::string>& EU4::EU4Localisation::GetTextInEachLanguage(const std::string& key) const
{
	static const std::map<std::string, std::string> noLocalisation;	// used if there's no localisation

	const auto keyFindIter = localisations.find(key);	// the localisation we want
	if (keyFindIter == localisations.end())
	{
		return noLocalisation;
	}

	return keyFindIter->second;
}

std::string EU4::EU4Localisation::DetermineLanguageForFile(const std::string& text)
{
	static const std::string noLanguageIndicated = "";	// used when no language is indicated

	if (text.size() < 2 || text[0] != 'l' || text[1] != '_')
	{	// Not in the desired format - no "l_"
		return noLanguageIndicated;
	}
	size_t beginPos = 2;	// Skip l_ for our language name.
	size_t endPos = text.find(':', beginPos);	// the end of the language name
	if (endPos == std::string::npos)
	{	// Not in the desired format - no ":"
		return noLanguageIndicated;
	}

	return text.substr(beginPos, endPos - beginPos);
}

std::pair<std::string, std::string> EU4::EU4Localisation::DetermineKeyLocalisationPair(const std::string& text)
{
	static const std::pair<std::string, std::string> noLocalisationPair;	// used when there's no localisation pair

	size_t keyBeginPos = text.find_first_not_of(' ');	// the first non-space character
	if (keyBeginPos == std::string::npos)
	{
		return noLocalisationPair;
	}
	size_t keyEndPos = text.find_first_of(':', keyBeginPos + 1);	// the end of the key
	size_t quotePos = text.find_first_of('"', keyEndPos);				// the begining of the string literal
	if (quotePos == std::string::npos)
	{
		return noLocalisationPair;
	}
	size_t localisationBeginPos = quotePos + 1;	// where the localisation begins
	size_t localisationEndPos = text.find_first_of('"', localisationBeginPos);	// where the localisation ends
	return std::make_pair(text.substr(keyBeginPos, keyEndPos - keyBeginPos), text.substr(localisationBeginPos, localisationEndPos - localisationBeginPos));
}

std::string EU4::EU4Localisation::RemoveUTF8BOM(const std::string& text)
{
	if (text.size() >= 3 && text[0] == '\xEF' && text[1] == '\xBB' && text[2] == '\xBF')
	{
		return text.substr(3);
	}
	else
	{
		return text;
	}
}
