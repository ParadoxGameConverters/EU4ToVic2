#ifndef EU4TOVIC2_CONVERTER_H
#define EU4TOVIC2_CONVERTER_H

#include <string>

namespace mappers {
	class VersionParser;
}

void ConvertEU4ToVic2(const std::string& EU4SaveFileName, const mappers::VersionParser& versionParser);

#endif // EU4TOVIC2_CONVERTER_H
