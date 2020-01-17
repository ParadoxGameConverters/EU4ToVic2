#ifndef MAPPER_POP_H
#define MAPPER_POP_H

#include "newParser.h"

namespace mappers
{
	class MapperPop: commonItems::parser
	{
	public:
		MapperPop() = default;
		MapperPop(std::istream& theStream);
		std::string getCulture() const { return culture; }
		std::string getReligion() const { return religion; }
		int getSize() const { return size; }

	private:
		std::string culture;
		std::string religion;
		int size = 0;
	};
}

#endif // MAPPER_POP_H
