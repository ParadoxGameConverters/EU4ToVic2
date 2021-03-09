#ifndef ID_CONVERTER_H
#include "../Mappers/ProvinceMappings/ProvinceMapper.h"
#include "../Mappers/StateMapper/StateMapper.h"

namespace V2
{
class IdConverter
{
  public:
	IdConverter(const std::vector<int>& vanillaProvinces, const std::map<int, std::string>& provinceLocalisation);

	[[nodiscard]] const auto& getStateMap() const { return stateMap; }
	[[nodiscard]] const auto& getProvinceMap() const { return provinceMap; }

  private:
	void drawProvinceMap();
	void drawStateMap();
	void mapUnlocalized(std::ofstream& output);
	void mapUnchanged(std::ofstream& output);
	void mapLeftovers(std::ofstream& output);
	void addStateMapping(std::string origID, std::string modID) { stateMap.insert(std::make_pair(origID, modID)); }
	std::string findBestMatch(const std::pair<std::string, std::map<std::string, int>>& state);
	void sortStateMap(std::map<std::string, std::map<std::string, int>>& freqMap, std::vector<std::string>& sortedStates);
	void updateStateMap(std::map<std::string, std::map<std::string, int>>& freqMap, std::string modStateID);
	void addProvinceMapping(int origID, int modID) { provinceMap.insert(std::make_pair(origID, modID)); }
	std::string getModStateId(int province, std::ofstream& output);
	bool verifyMap(std::map<int, int> provinceMap) const;
	bool verifyMap(std::map<std::string, std::string> stateMap) const;
	void outStates(std::string srcFile, std::string outFile) const;
	void outStateMap(std::string outFile) const;
	void outProvinceMap(std::string outFile) const;

	std::string mod;

	std::map<std::string, std::string> stateMap;
	std::map<int, int> provinceMap;

	std::vector<int> vanillaProvs;
	std::map<int, std::string> locProvs;
	mappers::ProvinceMapper vanillaMapper;
	mappers::ProvinceMapper provinceMapper;
	mappers::StateMapper vanillaStateMapper;
	mappers::StateMapper stateMapper;
	std::map<std::string, std::set<int>> vanillaStates;
	std::map<std::string, std::set<int>> modStates;
};
} // namespace V2
#define ID_CONVERTER_H value
#endif