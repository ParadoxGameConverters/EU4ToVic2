#ifndef RELATION_H
#define RELATION_H

#include "Date.h"
#include "../../EU4World/Relations/EU4Relations.h"

namespace V2
{
	class Relation
	{
	public:
		Relation(const std::string& _target);
		Relation(const std::string& _target, EU4::EU4RelationDetails& oldRelations);

		void setLevel(int level);
		std::string	getTarget() const { return target; }
		int getRelations() const { return value; }
		int getLevel() const { return level; }

		friend std::ostream& operator<<(std::ostream& output, const Relation& relations);

	private:
		std::string	target;
		int value = 0;
		bool militaryAccess = false;
		date lastSendDiplomat;
		date lastWar;
		int level = 2;
	};
}

#endif // RELATIONS_H