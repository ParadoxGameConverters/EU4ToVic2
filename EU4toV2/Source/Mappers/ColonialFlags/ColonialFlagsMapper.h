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
		explicit ColonialFlagsMapper(std::istream& theStream);

		[[nodiscard]] std::optional<ColonialFlag> getFlag(const std::string& name) const;
		[[nodiscard]] std::vector<std::string> getNames() const;
		
		void removeFlag(const std::string& name);

	private:
		void registerKeys();
		
		std::map<std::string, ColonialFlag> colonialFlags;
	};
}

#endif // COLONIAL_FLAGS_MAPPER
