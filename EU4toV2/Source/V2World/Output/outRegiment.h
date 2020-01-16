#ifndef OUT_REGIMENT_H
#define OUT_REGIMENT_H

#include "../Army/Regiment.h"
#include <ostream>

namespace V2
{
	std::ostream& operator<<(std::ostream& output, const Regiment& regiment);
}

#endif // OUT_REGIMENT_H
