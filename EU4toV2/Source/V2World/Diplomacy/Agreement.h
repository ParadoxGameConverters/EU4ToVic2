#ifndef AGREEMENT_H
#define AGREEMENT_H

#include "Date.h"
#include <ostream>

namespace V2
{
	class Agreement
	{
	public:
		Agreement(const std::string& _first, const std::string& _second, const std::string& _type, const date& _start_date);
		std::string getType() const { return type; }
		friend std::ostream& operator<<(std::ostream& output, Agreement theAgreement);

	private:
		std::string type;
		std::string first;
		std::string second;
		date start_date;
	};
}

#endif // AGREEMENT_H