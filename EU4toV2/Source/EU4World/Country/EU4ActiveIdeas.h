#ifndef EU4ACTIVE_IDEAS_H
#define EU4ACTIVE_IDEAS_H

#include "Parser.h"
#include <set>

namespace EU4
{
	class EU4ActiveIdeas : commonItems::parser
	{
	public:
		explicit EU4ActiveIdeas(std::istream& theStream);
		[[nodiscard]] const auto& getActiveIdeas() const { return activeIdeas; }

	private:
		std::set<std::string> activeIdeas;
	};
}

#endif // EU4ACTIVE_IDEAS_H