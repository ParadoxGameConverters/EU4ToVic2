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
		void setRelations(int value) { relations = value; }
		void increaseRelations(int value) { relations += value; }
		void setInfluence(int value) { influence = value; }
		void setAccess(bool access) { militaryAccess = access; }
		std::string getTarget() const { return target; }
		int getRelations() const { return relations; }
		int getInfluence() const { return influence; }
		int getLevel() const { return level; }

		friend std::ostream& operator<<(std::ostream& output, const Relation& relations);

	private:
		std::string target;
		int relations = 0;
		int influence = 0;
		bool militaryAccess = false;
		date lastSendDiplomat;
		date lastWar;
		int level = 2;
	};
}

#endif // RELATIONS_H