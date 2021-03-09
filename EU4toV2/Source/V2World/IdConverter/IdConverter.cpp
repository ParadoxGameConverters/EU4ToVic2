#include "IdConverter.h"
#include "Configuration.h"
#include "Log.h"
#include "State/StateNameParser.h"
#include <fstream>
#include <set>

V2::IdConverter::IdConverter(std::vector<int> vanillaProvinces,
	std::map<int, std::string> provinceLocalisation):
	vanillaProvs(vanillaProvinces),
	locProvs(provinceLocalisation),
	vanillaMapper(mappers::ProvinceMapper("configurables/province_mappings.txt")),
	vanillaStateMapper(mappers::StateMapper(theConfiguration.getVanillaVic2Path() + "/map/region.txt")),
	provinceMapper(mappers::ProvinceMapper("configurables/" + theConfiguration.getVic2ModName() + "/province_mappings.txt")),
	stateMapper(mappers::StateMapper(theConfiguration.getVic2ModPath() + "/" + theConfiguration.getVic2ModName() + "/map/region.txt"))
{
	vanillaStates = vanillaStateMapper.getStateMap();
	modStates = stateMapper.getStateMap();
	
	drawProvinceMap();
	drawStateMap(vanillaStates, modStates);
	outStateMap("output/state_map.txt");
	outProvinceMap("output/province_map.txt");

	if (const auto& mod = theConfiguration.getVic2ModName(); !mod.empty())
	{
		outputStateMap(theConfiguration.getVic2ModPath() + "/" + mod + "/map/region.txt", "output/region.txt");
	}
}

void V2::IdConverter::drawProvinceMap()
{
	std::ofstream output("output/provinceMap.log");
	if (!output.is_open())
		Log(LogLevel::Debug) << "Could not provinceMap.log";

	if (const auto& mod = theConfiguration.getVic2ModName(); !mod.empty())
	{
		LOG(LogLevel::Debug) << "Drawing province map (V2 -> " << mod << ")";

	//All that are not localised in the mod map to themselves
	mapUnlocalized(output);
	//Provinces mapped to the same eu4 province
	mapUnchanged(output);
	//Process all remaining
	mapLeftovers(output);

		for (const auto& province: vanillaProvs)
		{
			int origProvinceID = province;
			if (provinceMap.find(origProvinceID) != provinceMap.end())
				continue;
			output << "Original province:\t" << province << "\tUnmapped\n";
		}
	}
}

void V2::IdConverter::drawStateMap(std::map<std::string, std::set<int>> vanillaStates,
	std::map<std::string, std::set<int>> modStates)
{
	std::ofstream output("output/stateMap.log");
	if (!output.is_open())
		Log(LogLevel::Debug) << "Could not create stateMap.log";

	if (const auto& mod = theConfiguration.getVic2ModName(); !mod.empty())
	{
		LOG(LogLevel::Debug) << "Drawing state map (V2 -> " << mod << ")";

		// Namesakes map to each other
		std::set<std::string> unchangedStates;
		StateNameParser vanillaStatesMapper(theConfiguration.getVanillaVic2Path() + "/localisation");
		StateNameParser modStatesMapper(theConfiguration.getVic2ModPath() + "/" + theConfiguration.getVic2ModName() + "/localisation");
		const auto& vanillaLoc = vanillaStatesMapper.getStateNames();
		const auto& modLoc = modStatesMapper.getStateNames();
		for (const auto& [vanillaID, vanillaName]: vanillaLoc)
		{
			if (const auto& modStateItr = std::find_if(modLoc.begin(), modLoc.end(), [vanillaName](const std::pair<std::string, std::string>& state) {
				return state.second == vanillaName;
			}); modStateItr != modLoc.end())
			{
				const auto& modID = modStateItr->first;
				addStateMapping(vanillaID, modID);
				unchangedStates.insert(vanillaID);
			}
		}

		std::map<std::string, std::map<std::string, int>> freqMap;

		for (const auto& [stateID, stateProvinces]: vanillaStates)
		{
			output << "Original state:\t" << stateID << "\n";
			if (unchangedStates.contains(stateID))
			{
				output << " -> Mapped to a mod namesake (" << vanillaLoc.at(stateID) << ")\n";
				continue;
			}
			
			std::map<std::string, int> freqCounter;

			if (!stateProvinces.empty())
			{
				for (const auto& province: stateProvinces)
				{
					const auto& modStateID = getModStateId(province, output, modStates);
					freqCounter[modStateID]++;
					output << " -> " << modStateID << "\t{" << freqCounter[modStateID] << "}\n";
				}
			}
			freqMap[stateID] = freqCounter;
		}

		std::vector<std::string> sortedStates;
		sortStateMap(freqMap, sortedStates);

		auto idItr = sortedStates.begin();
		while (idItr != sortedStates.end())
		{
			const auto& stateItr = freqMap.find(*idItr);
			const auto& modStateID = findBestMatch(*stateItr);
			addStateMapping(stateItr->first, modStateID);
			freqMap.erase(stateItr->first);
			updateStateMap(freqMap, modStateID);
			sortStateMap(freqMap, sortedStates);
			idItr = sortedStates.begin();
		}
	}
}

std::string V2::IdConverter::findBestMatch(const std::pair<std::string, std::map<std::string, int>>& state)
{
	const auto& matchItr = std::max_element(
		state.second.begin(), state.second.end(),
		[](const std::pair<std::string, int>& state1, const std::pair<std::string, int>& state2){
			return state1.second < state2.second;
		}
	);
	return matchItr->first;
}

void V2::IdConverter::sortStateMap(std::map<std::string, std::map<std::string, int>>& freqMap, std::vector<std::string>& sortedStates)
{
	sortedStates.clear();
	for (const auto& state: freqMap)
	{
		if (state.second.size() == 1)
			sortedStates.push_back(state.first);
	}
	for (const auto& state: freqMap)
	{
		if (state.second.size() > 1)
			sortedStates.push_back(state.first);
	}
}

void V2::IdConverter::updateStateMap(std::map<std::string, std::map<std::string, int>>& freqMap, std::string modStateID)
{
	for (auto& remainingState: freqMap)
	{
		auto& modStates = remainingState.second;
		if (modStates.contains(modStateID) && modStates.size() > 1)
		{
			modStates.erase(modStateID);
		}
	}
}

std::string V2::IdConverter::getModStateId(int province, std::ofstream& output, std::map<std::string, std::set<int>> modStates)
{
	output << "( " << province;
	int modProvince = provinceMap.find(province)->second;
	output << " -> " << modProvince << " )";

	const auto& modStateItr = std::find_if(modStates.begin(), modStates.end(), [modProvince](const std::pair<std::string, std::set<int>> modState) {
		return modState.second.contains(modProvince);
	});
	return modStateItr->first;
}

void V2::IdConverter::mapUnlocalized(std::ofstream& output)
{
	for (const auto& province: vanillaProvs)
	{
		int origProvinceID = province;
		output << "Original province:\t" << origProvinceID << " ->";
		int modProvinceID = 0;

		if (locProvs.find(origProvinceID) == locProvs.end())
		{
			modProvinceID = origProvinceID;
			addProvinceMapping(origProvinceID, modProvinceID);
		}
		if (modProvinceID > 0)
		{
			output << " " << modProvinceID << " Unlocalized province";
		}
		output << "\n";
	}
}

void V2::IdConverter::mapUnchanged(std::ofstream& output)
{
	for (const auto& province: vanillaProvs)
	{
		int origProvinceID = province;
		if (provinceMap.find(origProvinceID) != provinceMap.end())
			continue;

		output << "Original province:\t" << origProvinceID << " ->";
		int modProvinceID = 0;

		//Get all eu4 provinces that map to this one
		const auto& eu4Provs = vanillaMapper.getEU4ProvinceNumbers(origProvinceID);

		//For each one of them
		for (int eu4Prov: eu4Provs)
		{
			//Find all mod provinces that they map to
			if (const auto& modProvs = modMapper.getVic2ProvinceNumbers(eu4Prov); 
				!modProvs.empty())
			{
				output << " " << eu4Prov << " (";
				for (const auto& modProv: modProvs)
				{
					output << " " << modProv;
				}
				output << " )";

				//If original ID is there, map it to itself
				if (const auto& modProvsItr = std::find(modProvs.begin(), modProvs.end(), origProvinceID);
					 modProvsItr != modProvs.end())
				{
					modProvinceID = *modProvsItr;
					addProvinceMapping(origProvinceID, modProvinceID);
				}
			}
		}
		if (modProvinceID > 0)
		{
			output << " -> " << modProvinceID << " Unchanged ID";
		}
		output << "\n";
	}
}

void V2::IdConverter::mapLeftovers(std::ofstream& output)
{
	//Cycle through all the provinces imported from Vanilla V2
	for (const auto& province: vanillaProvs)
	{
		int origProvinceID = province;
		if (provinceMap.find(origProvinceID) != provinceMap.end())
			continue;

		output << "Original province:\t" << origProvinceID << " ->";
		int modProvinceID = 0;
		//Get all eu4 provinces that map to this one
		const auto& eu4Provs = vanillaMapper.getEU4ProvinceNumbers(origProvinceID);

		int fallback;
		//If original province can't be mapped, map it to the first one found in first mapping
		for (int eu4Prov: eu4Provs)
		{
			if (eu4Prov > 0)
			{
				for (int vic2Prov: modMapper.getVic2ProvinceNumbers(eu4Prov))
				{
					if (vic2Prov > 0)
						fallback = vic2Prov;
				}
			}
		}

		//For each one of them
		for (int eu4Prov: eu4Provs)
		{
			//Find all mod provinces that they map to
			if (const auto& modProvs = modMapper.getVic2ProvinceNumbers(eu4Prov);
				 !modProvs.empty())
			{
				output << " " << eu4Prov << " (";
				for (const auto& modProv: modProvs)
				{
					output << " " << modProv;
				}
				output << " )";

				bool isMapped = false;
				auto modProvsItr = modProvs.begin();
				const auto& e = modProvs.end();

				//Check all mod provinces mapped to eu4 province
				for (const auto& modProv: modProvs)
				{
					//Check if it's mapped
					for (const auto& mapping: provinceMap)
					{
						if (mapping.second == modProv)
						{
							isMapped = true;
						}
					}
					if (!isMapped)
					{
						modProvinceID = modProv;
						addProvinceMapping(origProvinceID, modProvinceID);
						output << " -> " << modProvinceID << " Unmapped province\n";
						break;
					}
				}
				if (!isMapped)
					break;
			}
		}

		if (provinceMap.find(origProvinceID) == provinceMap.end())
		{
			addProvinceMapping(origProvinceID, fallback);
			output << " -> " << fallback << " Fallback\n";
		}
	}
}

void V2::IdConverter::outputStateMap(std::string srcFile, std::string outFile) const
{
	const mappers::StateMapper map(srcFile);

	LOG(LogLevel::Debug) << "Outputting states to " + outFile;
	std::ofstream output(outFile);
	if (!output.is_open())
		Log(LogLevel::Debug) << "Could not create " + outFile;

	for (const auto& state: map.getStateMap())
	{
		output << "ID:\t" << state.first << "\tProvinces: ";
		for (const auto& province: state.second)
		{
			output << " " << province;
		}
		output << "\n";
	}
}

bool V2::IdConverter::verifyMap(std::map<int, int> theMap) const
{
	bool mapChecksOut = true;
	auto mapItr = theMap.begin();
	const auto& mapEnd = theMap.end();

	for (mapItr; mapItr != mapEnd; ++mapItr)
	{
		if (theMap.count(mapItr->first) > 1)
		{
			mapChecksOut = false;
			break;
		}
		if (!mapChecksOut) break;
	}
	return mapChecksOut;
}

bool V2::IdConverter::verifyMap(std::map<std::string, std::string> theMap) const
{
	bool mapChecksOut = true;
	auto mapItr = theMap.begin();
	const auto& mapEnd = theMap.end();

	for (mapItr; mapItr != mapEnd; ++mapItr)
	{
		if (theMap.count(mapItr->first) > 1)
		{
			mapChecksOut = false;
			break;
		}
		if (!mapChecksOut) break;
	}
	return mapChecksOut;
}

void V2::IdConverter::outStateMap(std::string outFile) const
{
	if (!verifyMap(stateMap))
		LOG(LogLevel::Debug) << "State map did not check out. Check " << outFile << " for duplicate mappings";

	Log(LogLevel::Debug) << "Outputting state map";
	std::ofstream output(outFile);
	if (!output.is_open())
		Log(LogLevel::Debug) << "Could not create " + outFile;
	for (const auto& mapping: stateMap)
	{
		output << "V2 state:\t" << mapping.first << "\tMod state:\t" << mapping.second << "\n";
	}
}

void V2::IdConverter::outProvinceMap(std::string outFile) const
{
	if (!verifyMap(provinceMap))
		LOG(LogLevel::Debug) << "Province map did not check out. Check " << outFile << " for duplicate mappings";

	Log(LogLevel::Debug) << "Outputting province map";
	std::ofstream output(outFile);
	if (!output.is_open())
		Log(LogLevel::Debug) << "Could not create " + outFile;
	for (const auto& mapping: provinceMap)
	{
		output << "V2 province:\t" << mapping.first << "\tMod province:\t" << mapping.second << "\n";
	}
}

