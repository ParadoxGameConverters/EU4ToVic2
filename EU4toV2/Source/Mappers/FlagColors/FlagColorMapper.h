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
		explicit FlagColorMapper(std::istream& theStream);

		[[nodiscard]] auto getNumFlagColors() const { return flagColorMapping.size(); }
		[[nodiscard]] std::optional<commonItems::Color> getFlagColorByIndex(int index) const;
		
	private:
		void registerKeys();
		
		std::vector<commonItems::Color> flagColorMapping;
	};
};

#endif // FLAG_COLOR_MAPPER_H
