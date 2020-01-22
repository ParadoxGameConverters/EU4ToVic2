#ifndef STATE_H
#define STATE_H

#include <memory>
#include <string>
#include <vector>

namespace V2
{
	class Factory;
	class Province;
	class State
	{
	public:
		State(int newId, std::shared_ptr<Province> firstProvince);

		void addRailroads();
		void addFactory(std::shared_ptr<Factory> factory);

		bool isCoastal() const;
		bool hasLocalSupply(const std::string& product) const;
		double getSuppliedInputs(std::shared_ptr<Factory> factory) const;
		bool provInState(int id) const;
		bool hasLandConnection() const;
		double getMfgRatio() const;

		void addProvince(std::shared_ptr<Province> newProvince) { provinces.push_back(newProvince); }
		void setColonial(bool col) { colonial = col; }

		bool isColonial() const { return colonial; };
		int getFactoryCount() const { return factories.size(); };
		int getID() const { return id; };
		std::vector<std::shared_ptr<Province>> getProvinces() const { return provinces; };
		void rebuildNavalBase();
		
	private:
		int id = 0;
		bool colonial = false;
		std::vector<std::shared_ptr<Province>> provinces;
		std::vector<std::shared_ptr<Factory>> factories;
	};
}

#endif	// STATE_H