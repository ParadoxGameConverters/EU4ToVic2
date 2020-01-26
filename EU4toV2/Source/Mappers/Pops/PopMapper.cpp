#include "PopMapper.h"
#include "ParserHelpers.h"

mappers::PopMapper::PopMapper(const std::string& filename)
{
	registerKeys();
	parseFile(filename);
	clearRegisteredKeywords();
}

mappers::PopMapper::PopMapper(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::PopMapper::registerKeys()
{
	registerRegex("[0-9]+", [this](const std::string& provinceID, std::istream& theStream)
		{
			PopTypes newPopType(theStream);
			provincePopTypeMap.insert(std::make_pair(stoi(provinceID), newPopType));
		});
	registerRegex("[a-zA-Z0-9\\_.:]+", commonItems::ignoreItem);
}
