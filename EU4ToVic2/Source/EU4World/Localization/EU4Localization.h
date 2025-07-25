#ifndef EU4_LOCALIZATION_H
#define EU4_LOCALIZATION_H
#include <map>
#include <optional>
#include <string>
#include <utility>

namespace EU4
{
class EU4Localization
{
  public:
	void initializeFromEU4Installation();

	void readFromAllFilesInFolder(const std::filesystem::path& folderPath);
	void readFromFile(const std::filesystem::path& fileName);
	void readFromStream(std::istream& theStream);

	[[nodiscard]] std::optional<std::string> getText(const std::string& key, const std::string& language) const;
	[[nodiscard]] std::optional<std::map<std::string, std::string>> getTextInEachLanguage(const std::string& key) const;

  private:
	[[nodiscard]] static std::optional<std::string> determineLanguageForFile(const std::string& text);
	[[nodiscard]] static std::pair<std::string, std::string> determineKeyLocalisationPair(const std::string& text);
	[[nodiscard]] static std::string removeUTF8BOM(const std::string& text);

	std::map<std::string, std::map<std::string, std::string>> localizations; // key, map<language, value>
};
} // namespace EU4

#endif // EU4_LOCALISATION_H