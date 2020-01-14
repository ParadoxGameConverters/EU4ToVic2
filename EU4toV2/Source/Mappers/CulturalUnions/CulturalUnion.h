#ifndef CULTURAL_UNION_H
#define CULTURAL_UNION_H

#include "newParser.h"

namespace mappers
{
	class CulturalUnion: commonItems::parser
	{
	public:
		CulturalUnion(std::istream& theStream);
		std::pair<std::string, std::vector<std::string>> getUnion() const { return theUnion; }

	private:
		std::string culture;
		std::vector<std::string> tags;
		std::pair<std::string, std::vector<std::string>> theUnion;
	};
}

#endif // CULTURAL_UNION_H
