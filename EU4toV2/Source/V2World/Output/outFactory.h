#ifndef OUT_FACTORY_H
#define OUT_FACTORY_H

#include "../Factory/Factory.h"

namespace V2
{
	std::ostream& operator<<(std::ostream& output, const Factory& factory);
};

#endif // OUT_FACTORY_H
