#ifndef COLONIAL_FLAG_REGION_H
#define COLONIAL_FLAG_REGION_H

#include "newParser.h"
#include "ColonialFlag.h"
#include <map>

namespace mappers
{
	class ColonialFlagRegion: commonItems::parser
	{
	public:
		ColonialFlagRegion(std::istream& theStream, const std::string& region);
		std::map<std::string, ColonialFlag> getRegionalFlags() const { return regionalFlags; }

	private:
		std::map<std::string, ColonialFlag> regionalFlags;
	};
}

#endif // COLONIAL_FLAG_REGION_H
