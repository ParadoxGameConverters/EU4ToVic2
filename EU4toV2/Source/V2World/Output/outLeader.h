#ifndef OUT_LEADER_H
#define OUT_LEADER_H

#include "../Leader/Leader.h"

namespace V2
{
	std::ostream& operator<<(std::ostream& output, const Leader& leader);
}

#endif // OUT_LEADER_H