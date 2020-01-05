#include "Pop.h"
#include "ParserHelpers.h"

mappers::Pop::Pop(std::istream& theStream)
{
	registerKeyword(std::regex("culture"), [this](const std::string& unused, std::istream& theStream)
	{
			commonItems::singleString cultureStr(theStream);
			culture = cultureStr.getString();
	});
	registerKeyword(std::regex("religion"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleString religionStr(theStream);
			religion = religionStr.getString();
		});
	registerKeyword(std::regex("size"), [this](const std::string& unused, std::istream& theStream)
		{
			commonItems::singleInt sizeInt(theStream);
			size = sizeInt.getInt();
		});
	registerKeyword(std::regex("[a-zA-Z0-9\\_.:]+"), commonItems::ignoreItem);

	parseStream(theStream);
}
