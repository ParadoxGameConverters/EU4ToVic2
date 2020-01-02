#ifndef COLONIAL_TAG_H
#define COLONIAL_TAG_H

#include "newParser.h"


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
		ColonialTag(std::istream& theStream);
		ColonyStruct getColonyTag() const { return colonyTag; }

	private:
		ColonyStruct colonyTag;
	};
}

#endif // COLONIAL_TAG_H
