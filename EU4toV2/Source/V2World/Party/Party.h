#ifndef PARTY_H
#define PARTY_H

#include "Date.h"
#include "../../Mappers/PartyTypes/PartyType.h"

namespace V2
{
	class Party
	{
	public:
		Party(const mappers::PartyType& partyType);
		bool isActiveOn(date query) const;
		std::string getName() const { return name; }
		std::string getIdeology() const { return partyDetails.getIdeology(); }

		friend std::ostream& operator<<(std::ostream& output, const Party& party);

	private:
		mappers::PartyType partyDetails;
		std::string name;
	};
}

#endif // PARTY_H