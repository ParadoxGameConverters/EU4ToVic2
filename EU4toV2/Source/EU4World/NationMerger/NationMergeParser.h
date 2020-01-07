#ifndef EU4_NATION_MERGE_PARSER_H
#define EU4_NATION_MERGE_PARSER_H

#include "newParser.h"
#include "MergeBlock.h"

namespace EU4
{
	class NationMergeParser : commonItems::parser
	{
	public:
		NationMergeParser();
		const std::vector<EU4::MergeBlock>& getMergeBlocks() const { return mergeBlocks; }
		bool getMergeDaimyos() const { return mergeDaimyos; }

	private:
		std::vector<EU4::MergeBlock> mergeBlocks;
		bool mergeDaimyos = false;
	};
}

#endif // EU4_NATION_MERGE_PARSER_H
