#ifndef COLONIAL_TAG_H
#define COLONIAL_TAG_H

#include "Parser.h"

namespace mappers
{
	typedef struct
	{
		std::string tag;
		std::string EU4Region;
		std::string V2Region;
		std::string cultureGroup;
	} ColonyStruct;

	class ColonialTag: commonItems::parser
	{
	public:
		explicit ColonialTag(std::istream& theStream);
		
		[[nodiscard]] const auto& getColonyTag() const { return colonyTag; }

	private:
		ColonyStruct colonyTag;
	};
}

#endif // COLONIAL_TAG_H
