#ifndef V2_PARTY_H_
#define V2_PARTY_H_

#include "Date.h"
#include <memory>

#include "newParser.h"

class V2Party : commonItems::parser
{
public:
	V2Party() = default;
	V2Party(std::istream& theStream);	
	V2Party(const std::string& name, const std::string& ideology); // Populates all data with pregenerated values satisfying the required ideology.
	bool isActiveOn(date query) const { return ((query > start_date) && (query < end_date)); };
	std::string getName() const { return name; }
	std::string getIdeology() const { return ideology; }

	friend std::ostream& operator<<(std::ostream& output, const V2Party& party);

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

#endif // V2_PARTY_H_