#ifndef OUT_ARMY_H
#define OUT_ARMY_H

#include "../Army/Army.h"
#include <ostream>

namespace V2
{
	std::ostream& operator<<(std::ostream& output, const Army& army);
}

#endif // OUT_ARMY_H
