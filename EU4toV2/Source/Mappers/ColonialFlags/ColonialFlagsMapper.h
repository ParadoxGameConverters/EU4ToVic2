#ifndef COLONIAL_FLAGS_MAPPER
#define COLONIAL_FLAGS_MAPPER

#include "newParser.h"
#include "ColonialFlagRegion.h"
#include <map>
#include <string>

namespace mappers
{
	class ColonialFlagsMapper: commonItems::parser
	{
	public:
		ColonialFlagsMapper();
		ColonialFlagsMapper(std::istream& theStream);

		std::optional<mappers::ColonialFlag> getFlag(const std::string& name) const;
		std::vector<std::string> getNames() const;
		void removeFlag(const std::string& name);

	private:
		void registerKeys();
		std::map<std::string, ColonialFlag> colonialFlags;
	};
}

#endif // COLONIAL_FLAGS_MAPPER
