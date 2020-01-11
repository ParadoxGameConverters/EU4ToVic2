#ifndef CULTURE_H
#define CULTURE_H

#include "newParser.h"

namespace mappers
{
	class Culture: commonItems::parser
	{
	public:
		Culture(std::istream& theStream);

	private:
		std::string primaryTag;
		std::string graphicalCulture;
		std::vector<std::string> maleNames;
		std::vector<std::string> femaleNames;
		std::vector<std::string> dynastyNames;
	};
}

#endif // CULTURE_H
