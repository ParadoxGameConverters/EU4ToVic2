#ifndef EU4_MERGE_BLOCK_H
#define EU4_MERGE_BLOCK_H

#include "newParser.h"
#include <set>

namespace EU4
{
	class MergeBlock : commonItems::parser
	{
	public:
		MergeBlock(std::istream& theStream);
		std::string getMaster() const { return master; }
		const std::set<std::string>& getSlaves() const { return slaves; }
		bool getMerge() const { return merge; }

	private:
		std::string master;
		std::set<std::string> slaves;
		bool merge = false;
	};
}

#endif // EU4_MERGE_BLOCK_H
