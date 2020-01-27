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
		
		[[nodiscard]] const auto& getMergeBlocks() const { return mergeBlocks; }
		[[nodiscard]] auto getMergeDaimyos() const { return mergeDaimyos; }

	private:
		std::vector<MergeBlock> mergeBlocks;
		bool mergeDaimyos = false;
	};
}

#endif // EU4_NATION_MERGE_PARSER_H
