#include "PopTypes.h"
#include "CommonRegexes.h"
#include "ParserHelpers.h"

mappers::PopTypes::PopTypes(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::PopTypes::registerKeys()
{
	registerRegex(commonItems::catchallRegex, [this](const std::string& popType, std::istream& theStream) {
		const PopInstance newPop(theStream, popType);
		popTypes.emplace_back(newPop.getPopDetails());
	});
}
