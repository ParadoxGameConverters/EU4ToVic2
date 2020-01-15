#ifndef OUT_PARTY_H
#define OUT_PARTY_H

#include "../Party/Party.h"

namespace V2
{
	std::ostream& operator<<(std::ostream& output, const Party& party);
};

#endif // OUT_PARTY_H