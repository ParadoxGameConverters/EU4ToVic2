#ifndef PROVINCE_DETAILS_H
#define PROVINCE_DETAILS_H

#include "newParser.h"
#include <set>

namespace mappers
{
	class ProvinceDetails : commonItems::parser
	{
	public:
		ProvinceDetails() = default;
		ProvinceDetails(const std::string& filename);
		ProvinceDetails(std::istream& theStream);

		// These values are open to ease management.
		// This is a storage container for V2::Province.
		std::string owner;
		std::string controller;
		std::string rgoType;
		std::string terrain;
		std::string climate;
		std::set<std::string> cores;
		int lifeRating = 0;
		int colonial = 0;
		int colonyLevel = 0;
		int navalBaseLevel = 0;
		int fortLevel = 0;
		int railLevel = 0;
		bool slaveState = false;

	private:
		void registerKeys();
	};
}

#endif // PROVINCE_DETAILS_H
