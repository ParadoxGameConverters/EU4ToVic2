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



#ifndef EU4_WORLD_H_
#define EU4_WORLD_H_



#include "EU4Army.h"
#include "EU4Diplomacy.h"
#include "EU4Version.h"
#include "Regions/Regions.h"
#include "newParser.h"
#include <istream>
#include <memory>



class EU4Province;



namespace EU4
{

class Country;


class world: private commonItems::parser
{
	public:
		world(const std::string& EU4SaveFileName);

		EU4Province* getProvince(int provNum) const;

		const EU4::Version& getVersion() const { return *version; };
		std::map<std::string, std::shared_ptr<EU4::Country>> getCountries() const { return theCountries; };
		vector<EU4Agreement> getDiplomaticAgreements() const { return diplomacy->getAgreements(); };
		double getWorldWeightSum() const { return worldWeightSum; };
		const Regions& getRegions() const { return *regions; }

		bool isRandomWorld() const;

	private:
		void verifySave(const string& EU4SaveFileName);

		void loadUsedMods(const shared_ptr<Object> EU4SaveObj);
		map<string, string> loadPossibleMods();
		void loadEU4ModDirectory(map<string, string>& possibleMods);
		void loadCK2ExportDirectory(map<string, string>& possibleMods);

		void loadEU4Version(const shared_ptr<Object> EU4SaveObj);
		void loadActiveDLC(const shared_ptr<Object> EU4SaveObj);
		void loadEmpires(const shared_ptr<Object> EU4SaveObj);
		void loadHolyRomanEmperor(vector<shared_ptr<Object>> empireObj);
		void loadCelestialEmperor(vector<shared_ptr<Object>> celestialEmpireObj);


		void loadProvinces(const shared_ptr<Object> EU4SaveObj);
		map<int, int> determineValidProvinces();

		void loadCountries(istream& theStream);
		void loadRevolutionTargetString(const shared_ptr<Object> EU4SaveObj);
		void loadRevolutionTarget();
		void addProvinceInfoToCountries();
		void loadDiplomacy(const shared_ptr<Object> EU4SaveObj);
		void determineProvinceWeights();

		void loadRegions();
		void loadEU4RegionsNewVersion();
		void loadEU4RegionsOldVersion();

		void checkAllEU4CulturesMapped() const;

		void readCommonCountries();
		void readCommonCountriesFile(istream&, const std::string& rootPath);

		void setLocalisations();
		void resolveRegimentTypes();

		void mergeNations();
		void uniteJapan();

		void checkAllProvincesMapped() const;
		void setNumbersOfDestinationProvinces();
		void checkAllEU4ReligionsMapped() const;

		void removeEmptyNations();
		void removeDeadLandlessNations();
		void removeLandlessNations();

		void setEmpires();

		std::shared_ptr<EU4::Country> getCountry(string tag) const;

		string holyRomanEmperor;
		string celestialEmperor;
		std::unique_ptr<Regions> regions;
		map<int, EU4Province*> provinces;
		std::map<std::string, std::shared_ptr<EU4::Country>> theCountries;
		EU4Diplomacy* diplomacy;
		std::unique_ptr<EU4::Version> version;
		double worldWeightSum;

		std::string revolutionTargetString;
};

}



#endif // EU4_WORLD_H_
