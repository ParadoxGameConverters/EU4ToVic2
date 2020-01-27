#ifndef EU4TOVIC2_CONVERTER_H
#define EU4TOVIC2_CONVERTER_H

#include <string>

namespace mappers {
	class VersionParser;
}

void convertEU4ToVic2(const std::string& eu4SaveFileName, const mappers::VersionParser& versionParser);
void setOutputName(const std::string& eu4SaveFileName);
void deleteExistingOutputFolder();
std::string trimPath(const std::string& fileName);
std::string trimExtension(const std::string& fileName);
std::string replaceCharacter(std::string fileName, char character);

#endif // EU4TOVIC2_CONVERTER_H
