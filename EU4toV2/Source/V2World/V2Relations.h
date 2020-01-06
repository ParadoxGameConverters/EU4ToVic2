#ifndef V2RELATIONS_H_
#define V2RELATIONS_H_

#include "Date.h"
#include <ostream>
#include "../EU4World/Relations/EU4Relations.h"

class V2Relations
{
	public:
		V2Relations(std::string& newTag);
		V2Relations(std::string& newTag, EU4::EU4RelationDetails& oldRelations);

		void		setLevel(int level);

		std::string	getTag()			const { return tag; };
		int		getRelations()	const { return value; };
		int		getLevel()		const { return level; };

		friend std::ostream& operator<<(std::ostream& output, const V2Relations& relations);

	private:
		std::string	tag;
		int		value;
		bool		militaryAccess;
		date		lastSendDiplomat;
		date		lastWar;
		int		level;
};


std::ostream& operator<<(std::ostream&, const V2Relations& relations);



#endif // V2RELATIONS_H_