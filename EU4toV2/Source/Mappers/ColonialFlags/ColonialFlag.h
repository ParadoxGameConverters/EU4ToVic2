#ifndef COLONIAL_FLAG_H
#define COLONIAL_FLAG_H

#include "newParser.h"
#include <string>

namespace mappers
{
	class ColonialFlag: commonItems::parser
	{
	public:
		ColonialFlag() = default;
		ColonialFlag(std::istream& theStream, const std::string& region);

		std::string getName() const { return name; }
		std::string getRegion() const { return region; }
		bool isUnique() const { return unique; }
		std::string getOverlord() const { return overlord; }
		void setOverlord(const std::string& newOverlord) { overlord = newOverlord; }

	private:
		std::string name;
		std::string region;
		bool unique = false;
		std::string overlord;
	};
}

#endif // COLONIAL_FLAG_H
