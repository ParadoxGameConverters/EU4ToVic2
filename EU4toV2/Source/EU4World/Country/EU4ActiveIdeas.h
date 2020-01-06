#ifndef EU4ACTIVE_IDEAS_H
#define EU4ACTIVE_IDEAS_H

#include "newParser.h"
#include <map>

namespace EU4
{
	class EU4ActiveIdeas : commonItems::parser
	{
	public:
		EU4ActiveIdeas(std::istream& theStream);
		std::map<std::string, int> getActiveIdeas() const { return activeIdeas; }

	private:
		std::map<std::string, int> activeIdeas;
	};
}

#endif // EU4ACTIVE_IDEAS_H