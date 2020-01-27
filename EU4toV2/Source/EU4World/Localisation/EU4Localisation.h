#ifndef EU4_LOCALISATION_H
#define EU4_LOCALISATION_H

#include <map>
#include <string>
#include <unordered_map>
#include <utility>

namespace EU4
{
	// Holds a map from key to localized text for all languages in which 
	// the localization is provided.
	class EU4Localisation
	{
	public:
		EU4Localisation();

		// Adds all localizations found in the specified file. The file should begin with
		// a line like "l_english:" to indicate what language the texts are in.
		void readFromFile(const std::string& fileName);
		// Adds all localizations found in files in the specified folder as per ReadFromFile().
		void readFromAllFilesInFolder(const std::string& folderPath);

		// Returns the localized text for the given key in the specified language. Returns
		// an empty string if no such localization is available.
		[[nodiscard]] const std::string& getText(const std::string& key, const std::string& language) const;
		// Returns the localized text for the given key in each language - the returned map is from
		// language to localized text.
		[[nodiscard]] const std::map<std::string, std::string>& getTextInEachLanguage(const std::string& key) const;

	private:
		// Returns the language name from text in the form "l_english:". Returns an empty string
		// if the text doesn't fit this format.
		static std::string determineLanguageForFile(const std::string& text);
		
		// Returns the localization from text in the form 'KEY: "Localization"'. Returns a pair
		// with empty strings if the text doesn't fit this format. Additional spaces around the
		// elements can be included and are ignored.
		static std::pair<std::string, std::string> determineKeyLocalisationPair(const std::string& text);

		// Removes a UTF-8 BOM from the beginning of the text, if present. (These are added by the
		// CK2-EU4 converter.)
		static std::string removeUTF8BOM(const std::string& text);

		typedef std::map<std::string, std::string> LanguageToLocalisationMap;
		typedef std::unordered_map<std::string, LanguageToLocalisationMap> KeyToLocalisationsMap;
		KeyToLocalisationsMap localisations;	// a map between keys and localizations
	};
}

#endif // EU4_LOCALISATION_H