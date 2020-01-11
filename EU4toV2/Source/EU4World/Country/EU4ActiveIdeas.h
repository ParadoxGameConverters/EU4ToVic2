#ifndef EU4ACTIVE_IDEAS_H
#define EU4ACTIVE_IDEAS_H

#include "newParser.h"
#include <set>

namespace EU4
{
	class EU4ActiveIdeas : commonItems::parser
	{
	public:
		EU4ActiveIdeas(std::istream& theStream);
		const std::set<std::string>& getActiveIdeas() const { return activeIdeas; }

	private:
		std::set<std::string> activeIdeas;
	};
}

#endif // EU4ACTIVE_IDEAS_H