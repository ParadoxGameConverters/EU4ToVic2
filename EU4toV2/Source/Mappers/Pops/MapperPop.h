#ifndef MAPPER_POP_H
#define MAPPER_POP_H

#include "Parser.h"

namespace mappers
{
	class MapperPop: commonItems::parser
	{
	public:
		MapperPop() = default;
		explicit MapperPop(std::istream& theStream);
		
		[[nodiscard]] const auto& getCulture() const { return culture; }
		[[nodiscard]] const auto& getReligion() const { return religion; }
		[[nodiscard]] auto getSize() const { return size; }

	private:
		std::string culture;
		std::string religion;
		int size = 0;
	};
}

#endif // MAPPER_POP_H
