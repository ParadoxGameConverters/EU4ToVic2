#include "PopMapper.h"
#include "ParserHelpers.h"

mappers::PopMapper::PopMapper(std::istream& theStream)
{
	registerKeyword(std::regex("[0-9]+"), [this](const std::string& provinceID, std::istream& theStream)
		{
			mappers::PopTypes newPopType(theStream);
			provincePopTypeMap.insert(std::make_pair(stoi(provinceID), newPopType));
		});
	registerKeyword(std::regex("[a-zA-Z0-9\\_.:]+"), commonItems::ignoreItem);

	parseStream(theStream);
}
