#ifndef EU4WAR_H
#define EU4WAR_H

#include "Date.h"
#include "newParser.h"
#include "EU4WarDetails.h"


namespace EU4
{
	class War : commonItems::parser
	{
	public:
		explicit War(std::istream& theStream);
		[[nodiscard]] const WarDetails& getDetails() const { return details; }
		[[nodiscard]] const std::vector<std::string>& getAttackers() const { return attackers; }
		[[nodiscard]] const std::vector<std::string>& getDefenders() const { return defenders; }
		[[nodiscard]] const std::string& getName() const { return name; }

	private:
		WarDetails details;
		std::string name;
		std::vector<std::string> attackers; // order maters! first is primary.
		std::vector<std::string> defenders; // order maters! first is primary.
	};
}

#endif // EU4WAR_H