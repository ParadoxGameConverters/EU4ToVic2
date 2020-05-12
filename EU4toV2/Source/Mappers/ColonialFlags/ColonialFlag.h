#ifndef COLONIAL_FLAG_H
#define COLONIAL_FLAG_H

#include "Parser.h"
#include <string>

namespace mappers
{
	class ColonialFlag: commonItems::parser
	{
	public:
		ColonialFlag() = default;
		ColonialFlag(std::istream& theStream, const std::string& region);

		[[nodiscard]] auto isUnique() const { return unique; }
		[[nodiscard]] const auto& getName() const { return name; }
		[[nodiscard]] const auto& getRegion() const { return region; }
		[[nodiscard]] const auto& getOverlord() const { return overlord; }
		
		void setOverlord(const std::string& newOverlord) { overlord = newOverlord; }

	private:
		bool unique = false;
		std::string name;
		std::string region;
		std::string overlord;
	};
}

#endif // COLONIAL_FLAG_H
