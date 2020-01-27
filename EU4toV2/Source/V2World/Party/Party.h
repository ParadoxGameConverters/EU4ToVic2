#ifndef PARTY_H
#define PARTY_H

#include "Date.h"
#include "../../Mappers/PartyTypes/PartyType.h"

namespace V2
{
	class Party
	{
	public:
		explicit Party(const mappers::PartyType& partyType);
		
		[[nodiscard]] bool isActiveOn(date query) const;
		[[nodiscard]] const auto& getName() const { return name; }
		[[nodiscard]] const auto& getIdeology() const { return partyDetails.getIdeology(); }

		friend std::ostream& operator<<(std::ostream& output, const Party& party);

	private:
		mappers::PartyType partyDetails;
		std::string name;
	};
}

#endif // PARTY_H