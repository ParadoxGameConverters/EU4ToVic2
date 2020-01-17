#ifndef OUT_RELATION_H
#define OUT_RELATION_H

#include "Date.h"
#include <ostream>
#include "../Diplomacy/Relation.h"

namespace V2
{
	std::ostream& operator<<(std::ostream&, const Relation& relation);
}

#endif // OUT_RELATION_H