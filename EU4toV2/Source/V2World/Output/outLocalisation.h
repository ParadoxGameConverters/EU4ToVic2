#ifndef OUT_LOCALISATION_H
#define OUT_LOCALISATION_H

#include "../Localisation/Localisation.h"

namespace V2
{
	std::ostream& operator<<(std::ostream& out, const Localisation& localisation);
}

#endif //OUT_LOCALISATION_H