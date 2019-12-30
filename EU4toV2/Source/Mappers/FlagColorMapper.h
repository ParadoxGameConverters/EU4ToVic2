#ifndef FLAG_COLOR_MAPPER_H_
#define FLAG_COLOR_MAPPER_H_

#include <memory>
#include <vector>
#include "newParser.h"
#include "Color.h"



namespace mappers
{
	class FlagColorMapper : commonItems::parser
	{
	public:
		FlagColorMapper(std::istream& theStream);
		int getNumFlagColors() const { return flagColorMapping.size(); }
		commonItems::Color getFlagColorByIndex(int index) const { return flagColorMapping[index]; }
		

	private:
		std::vector<commonItems::Color> flagColorMapping;
	};
};

#endif // FLAG_COLOR_MAPPER_H_
