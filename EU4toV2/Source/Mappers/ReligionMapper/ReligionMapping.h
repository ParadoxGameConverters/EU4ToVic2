#ifndef RELIGION_MAPPING_H
#define RELIGION_MAPPING_H

#include "newParser.h"

namespace mappers
{
	class ReligionMapping: commonItems::parser
	{
	public:
		ReligionMapping(std::istream& theStream);
		std::string getVic2Religion() const { return vic2Religion; }
		std::vector<std::string> getEU4Religions() const { return eu4Religions; }

	private:
		std::string vic2Religion;
		std::vector<std::string> eu4Religions;
	};
}

#endif // RELIGION_MAPPING_H