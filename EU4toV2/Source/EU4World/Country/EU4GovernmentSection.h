#ifndef EU4_GOVERNMENTSECTION_H
#define EU4_GOVERNMENTSECTION_H

#include "newParser.h"
#include <set>

namespace EU4
{
	class GovernmentSection : commonItems::parser
	{
	public:
		GovernmentSection(std::istream& theStream);
		std::string getGovernment() const { return government; }
		const std::set<std::string>& getGovernmentReforms() const { return reformStack; }

	private:
		std::string government;
		std::set<std::string> reformStack;
	};
}

#endif // EU4_GOVERNMENTSECTION_H
