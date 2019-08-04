/*Copyright (c) 2019 The Paradox Game Converters Project

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/



#ifndef V2ARMY_H_
#define V2ARMY_H_



#include "../EU4World/EU4Army.h"
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
		V2Regiment(RegimentCategory rc);

		void setName(string _name)		{ name = _name; };
		void setHome(int newHome)		{ home = newHome; };

		bool					getShip()		const { return isShip; };
		RegimentCategory	getCategory()	const { return category; };

		friend std::ostream& operator<<(std::ostream& output, const V2Regiment& regiment);
	private:
		V2ArmyID				id;
		string				name;
		string				type;
		int					home;
		bool					isShip;
		RegimentCategory	category;
};


std::ostream& operator<<(std::ostream& output, const V2Regiment& regiment);


class V2Army // also Navy
{
	public:
		V2Army(EU4Army* oldArmy, map<int, int> leaderIDMap);
		void					addRegiment(V2Regiment reg);

		void					setLocation(int provinceID)												{ location = provinceID; };
		void					setNavy(bool navy)															{ isNavy = navy; };
		void					setArmyRemainders(RegimentCategory category, double remainder)	{ armyRemainders[category] = remainder; };

		string				getName() const { return name; };
		void					getRegimentCounts(int counts[num_reg_categories]) const;
		double				getArmyRemainder(RegimentCategory category) const { return armyRemainders[category]; };
		EU4Army*				getSourceArmy() const { return sourceArmy; };
		bool					getNavy() const { return isNavy; };

		static V2Army*			makeTestNavy(int location);

		friend std::ostream& operator<<(std::ostream& output, const V2Army& army);

	private:
		V2Army() {}; // used by makeTestNavy
		V2ArmyID					id;
		string					name;
		int						location;
		std::vector<V2Regiment>	regiments;
		double					armyRemainders[num_reg_categories];
		EU4Army*					sourceArmy; // only valid during conversion
		bool						isNavy;
};


std::ostream& operator<<(std::ostream& output, const V2Army& army);



#endif