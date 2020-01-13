#ifndef COLONIAL_TAG_MAPPER_H
#define COLONIAL_TAG_MAPPER_H

#include "ColonialTag.h"

namespace mappers
{

	class ColonialTagMapper: commonItems::parser
	{
	public:
		ColonialTagMapper();
		std::vector<ColonyStruct> getColonyList() const { return colonyList; }

	private:
		std::vector<ColonyStruct> colonyList;
	};
}

#endif // COLONIAL_TAG_MAPPER_H
