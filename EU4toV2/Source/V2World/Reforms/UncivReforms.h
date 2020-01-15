#ifndef UNCIV_REFORMS_H
#define UNCIV_REFORMS_H

#include <ostream>

class V2Country;

namespace V2
{	
	class UncivReforms {
	public:
		UncivReforms(int westernizationProgress, double milFocus, double socioEcoFocus, V2Country* country);

		friend std::ostream& operator<<(std::ostream& output, const UncivReforms& reforms);

	private:
		bool reforms[16] = {};
	};
}


#endif // UNCIV_REFORMS_H