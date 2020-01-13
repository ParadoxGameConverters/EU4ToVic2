#ifndef FLAG_COLOR_MAPPER_H
#define FLAG_COLOR_MAPPER_H

#include "newParser.h"
#include "Color.h"

namespace mappers
{
	class FlagColorMapper : commonItems::parser
	{
	public:
		FlagColorMapper();
		FlagColorMapper(std::istream& theStream);

		int getNumFlagColors() const { return flagColorMapping.size(); }
		std::optional<commonItems::Color> getFlagColorByIndex(int index) const;
		
	private:
		void registerKeys();
		std::vector<commonItems::Color> flagColorMapping;
	};
};

#endif // FLAG_COLOR_MAPPER_H
