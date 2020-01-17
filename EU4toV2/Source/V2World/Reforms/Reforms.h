#ifndef REFORMS_H
#define REFORMS_H

#include <memory>
#include "../../EU4World/Country/EU4Country.h"


namespace V2
{	
	class V2Country;
	class Reforms
	{
	public:
		Reforms(const V2Country*, const std::shared_ptr<EU4::Country>);
		
		friend std::ostream& operator<<(std::ostream& output, const Reforms& reforms);

	private:
		bool abolishSlavery = false;
		double slavery = 5.0;
		double vote_franchise = 5.0;
		double upper_house_composition = 5.0;
		double voting_system = 5.0;
		double public_meetings = 5.0;
		double press_rights = 5.0;
		double trade_unions = 5.0;
		double political_parties = 5.0;
	};
}

#endif // REFORMS_H