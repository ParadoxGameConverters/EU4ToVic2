/*Copyright (c) 2018 The Paradox Game Converters Project

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



#ifndef V2Province_H_
#define V2Province_H_



#include "../EU4World/World.h"
#include "Factory/Factory.h"
#include <vector>


namespace V2
{
	class Province;
}

namespace EU4
{
	class world;
}



class V2State
{
	public:
		V2State(int newId, std::shared_ptr<V2::Province> firstProvince);

		void		addRailroads();
		void		addFactory(const V2::Factory& factory);

		bool		isCoastal() const;
		bool		hasLocalSupply(const std::string& product) const;
		double getSuppliedInputs(const V2::Factory& factory) const;
		bool		provInState(int id)	const;
		bool		hasLandConnection()	const;
		double	getManuRatio() const;

		void		addProvince(std::shared_ptr<V2::Province> newProvince) { provinces.push_back(newProvince); }
		void		setColonial(bool isIt) { colonial = isIt; }

		bool						isColonial()		const { return colonial; };
		int						getFactoryCount()	const { return factories.size(); };
		int						getID()				const { return id; };
		std::vector<std::shared_ptr<V2::Province>> getProvinces() const { return provinces; };
		void	colloectNavalBase();
	private:
		int								id;
		bool								colonial;
		std::vector<std::shared_ptr<V2::Province>> provinces;
		std::vector<V2::Factory> factories;
};



#endif	// V2Province_H_