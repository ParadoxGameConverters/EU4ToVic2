#ifndef V2ARMY_H_
#define V2ARMY_H_

#include "../EU4World/Army/EU4Army.h"
#include <ostream>


struct V2ArmyID
{
	public:
		V2ArmyID();
		void output(FILE* out, int indentlevel) const;

		int id;
		int type;
};


class V2Regiment // also Ship
{
	public:
		V2Regiment(EU4::REGIMENTCATEGORY rc);

		void setName(std::string _name) { name = _name; };
		void setHome(int newHome) { home = newHome; };

		bool getShip() const { return isShip; };
		EU4::REGIMENTCATEGORY getCategory()	const { return category; };

		friend std::ostream& operator<<(std::ostream& output, const V2Regiment& regiment);

	private:
		V2ArmyID id;
		std::string name;
		std::string type;
		int home;
		bool isShip;
		EU4::REGIMENTCATEGORY category;
};


std::ostream& operator<<(std::ostream& output, const V2Regiment& regiment);


class V2Army // also Navy
{
	public:
		V2Army(const EU4::EU4Army& oldArmy);
		void addRegiment(V2Regiment reg);

		void setLocation(int provinceID) { location = provinceID; };
		void setNavy(bool navy) { isNavy = navy; };
		void setArmyRemainders(EU4::REGIMENTCATEGORY category, double remainder) { armyRemainders[static_cast<int>(category)] = remainder; };

		std::string getName() const { return name; };
		void getRegimentCounts(int counts[static_cast<int>(EU4::REGIMENTCATEGORY::num_reg_categories)]) const;
		double getArmyRemainder(EU4::REGIMENTCATEGORY category) const { return armyRemainders[static_cast<int>(category)]; };
		EU4::EU4Army getSourceArmy() const { return sourceArmy; };
		bool getNavy() const { return isNavy; };

		static V2Army* makeTestNavy(int location);

		friend std::ostream& operator<<(std::ostream& output, const V2Army& army);

	private:
		V2Army() {}; // used by makeTestNavy
		V2ArmyID id;
		std::string name;
		int location = -1;
		std::vector<V2Regiment>	regiments;
		double armyRemainders[static_cast<int>(EU4::REGIMENTCATEGORY::num_reg_categories)];
		EU4::EU4Army sourceArmy; // only valid during conversion
		bool isNavy = false;
};

std::ostream& operator<<(std::ostream& output, const V2Army& army);

#endif // V2ARMY_H_