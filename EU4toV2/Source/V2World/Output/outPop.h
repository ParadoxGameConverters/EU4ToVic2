#ifndef OUT_POP_H
#define OUT_POP_H

#include "../Pop/Pop.h"
#include <ostream>

namespace V2
{
	std::ostream& operator<<(std::ostream& output, const Pop& pop);
}

#endif // OUT_POP_H