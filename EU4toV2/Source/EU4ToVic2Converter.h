#ifndef EU4TOVIC2_CONVERTER_H
#define EU4TOVIC2_CONVERTER_H

namespace mappers
{
class ConverterVersion;
}

void convertEU4ToVic2(const mappers::ConverterVersion& converterVersion);
void deleteExistingOutputFolder();

#endif // EU4TOVIC2_CONVERTER_H
