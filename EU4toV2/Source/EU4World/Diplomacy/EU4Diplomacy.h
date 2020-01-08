#ifndef EU4_DIPLOMACY_H
#define EU4_DIPLOMACY_H

#include "newParser.h"
#include "EU4Agreement.h"

namespace EU4
{
	class EU4Diplomacy : commonItems::parser
	{
	public:
		EU4Diplomacy() = default;
		EU4Diplomacy(std::istream& theStream);
		const std::vector<EU4Agreement>& getAgreements() const { return agreements; }

	private:
		std::vector<EU4Agreement> agreements;
	};
}


#endif // EU4DIPLOMACY_H