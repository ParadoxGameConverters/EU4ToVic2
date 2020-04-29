#ifndef EU4_TECHNOLOGY_H
#define EU4_TECHNOLOGY_H

#include "Parser.h"

namespace EU4
{
	class EU4Technology: commonItems::parser
	{
	public:
		EU4Technology() = default;
		explicit EU4Technology(std::istream& theStream);
		[[nodiscard]] int getAdm() const { return adm; }
		[[nodiscard]] int getDip() const { return dip; }
		[[nodiscard]] int getMil() const { return mil; }

	private:
		int adm = 0;
		int dip = 0;
		int mil = 0;
	};
}

#endif // EU4_TECHNOLOGY_H