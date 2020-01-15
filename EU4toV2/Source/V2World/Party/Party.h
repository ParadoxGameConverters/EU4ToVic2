#ifndef PARTY_H
#define PARTY_H

#include "Date.h"
#include <memory>

#include "newParser.h"

namespace V2
{
	class Party : commonItems::parser
	{
	public:
		Party() = default;
		Party(std::istream& theStream);
		Party(const std::string& name, const std::string& ideology); // Populates all data with pregenerated values satisfying the required ideology.
		bool isActiveOn(date query) const { return ((query > start_date) && (query < end_date)); };
		std::string getName() const { return name; }
		std::string getIdeology() const { return ideology; }

		friend std::ostream& operator<<(std::ostream& output, const Party& party);

	private:
		std::string name;
		std::string ideology;

		date start_date;
		date end_date;

		std::string economic_policy;
		std::string trade_policy;
		std::string religious_policy;
		std::string citizenship_policy;
		std::string war_policy;

	};
}

#endif // PARTY_H