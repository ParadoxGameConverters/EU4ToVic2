#ifndef OUT_AGREEMENT_H
#define OUT_AGREEMENT_H

#include "../Diplomacy/Agreement.h"
#include <ostream>

namespace V2
{
	std::ostream& operator<<(std::ostream& output, Agreement theAgreement);
}

#endif // OUT_AGREEMENT_H