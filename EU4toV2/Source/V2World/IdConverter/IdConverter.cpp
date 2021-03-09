#include "IdConverter.h"
#include "Configuration.h"
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include "State/StateNameParser.h"
#include <fstream>
#include <set>

V2::IdConverter::IdConverter(const std::vector<int>& vanillaProvinces, const std::map<int, std::string>& provinceLocalisation):
	 mod(theConfiguration.getVic2ModName()), vanillaProvs(vanillaProvinces), locProvs(provinceLocalisation),
	 vanillaMapper(mappers::ProvinceMapper("configurables/province_mappings.txt")),
	 vanillaStateMapper(mappers::StateMapper(theConfiguration.getVanillaVic2Path() + "/map/region.txt")),
	 provinceMapper(mappers::ProvinceMapper("configurables/" + theConfiguration.getVic2ModName() + "/province_mappings.txt")),
	 stateMapper(mappers::StateMapper(theConfiguration.getVic2ModPath() + "/" + theConfiguration.getVic2ModName() + "/map/region.txt"))
{
	vanillaStates = vanillaStateMapper.getStateMap();
	modStates = stateMapper.getStateMap();

	commonItems::TryCreateFolder("debug");

	drawProvinceMap();
	drawStateMap();
	outStateMap("debug/state_map.txt");
	outProvinceMap("debug/province_map.txt");
	outStates(theConfiguration.getVic2ModPath() + "/" + mod + "/map/region.txt", "debug/region.txt");
}

void V2::IdConverter::drawProvinceMap()
{
	std::ofstream debug("debug/provinceMap.log");
	if (!debug.is_open())
		Log(LogLevel::Debug) << "Could not create provinceMap.log";

	LOG(LogLevel::Info) << "->> Drawing province map (V2 -> " << mod << ")";

	// All that are not localised in the mod map to themselves
	mapUnlocalized(debug);
	// Provinces mapped to the same eu4 province
	mapUnchanged(debug);
	// Process all remaining
	mapLeftovers(debug);

	for (const auto& province: vanillaProvs)
	{
		int origProvinceID = province;
		if (provinceMap.find(origProvinceID) != provinceMap.end())
			continue;
		debug << "Original province:\t" << province << "\tUnmapped\n";
	}
}

void V2::IdConverter::drawStateMap()
{
	std::ofstream debug("debug/stateMap.log");
	if (!debug.is_open())
		Log(LogLevel::Debug) << "Could not create stateMap.log";

	LOG(LogLevel::Info) << "->> Drawing state map (V2 -> " << mod << ")";

	// Namesakes map to each other
	std::map<std::string, std::string> namesakes;
	StateNameParser vanillaStatesMapper(theConfiguration.getVanillaVic2Path() + "/localisation");
	StateNameParser modStatesMapper(theConfiguration.getVic2ModPath() + "/" + theConfiguration.getVic2ModName() + "/localisation");
	const auto& vanillaLoc = vanillaStatesMapper.getStateNames();
	const auto& modLoc = modStatesMapper.getStateNames();
	for (const auto& [vanillaID, vanillaName]: vanillaLoc)
	{
		if (const auto& modStateItr = std::find_if(modLoc.begin(),
				  modLoc.end(),
				  [vanillaName](const std::pair<std::string, std::string>& state) {
					  return state.second == vanillaName;
				  });
			 modStateItr != modLoc.end())
		{
			const auto& modID = modStateItr->first;
			addStateMapping(vanillaID, modID);
			namesakes.insert(std::make_pair(vanillaID, modID));
		}
	}

	std::map<std::string, std::map<std::string, int>> freqMap;

	for (const auto& [stateID, stateProvinces]: vanillaStates)
	{
		std::string name;
		if (vanillaLoc.contains(stateID)) name = vanillaLoc.at(stateID);
		debug << "Original state:\t" << stateID << " (" << name << ")\n";
		if (namesakes.contains(stateID))
		{
			const auto& id = namesakes.at(stateID);
			const auto& name = modLoc.at(id);
			debug << " -> Mapped to a mod namesake " << id << " (" << name << ")\n\n";
			continue;
		}

		std::map<std::string, int> freqCounter;

		if (!stateProvinces.empty())
		{
			for (const auto& province: stateProvinces)
			{
				const auto& modStateID = getModStateId(province, debug);
				freqCounter[modStateID]++;

				// Debug
				std::string modStateName;
				if (modLoc.contains(modStateID)) modStateName = modLoc.at(modStateID);
				else if (vanillaLoc.contains(modStateID)) modStateName = vanillaLoc.at(modStateID);
				debug << " -> " << modStateID << " (" << modStateName << ")\t{" << freqCounter[modStateID] << "}\n";
			}
		}
		debug << "\n";
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

std::string V2::IdConverter::findBestMatch(const std::pair<std::string, std::map<std::string, int>>& state)
{
	const auto& matchItr =
		 std::max_element(state.second.begin(), state.second.end(), [](const std::pair<std::string, int>& state1, const std::pair<std::string, int>& state2) {
			 return state1.second < state2.second;
		 });
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

std::string V2::IdConverter::getModStateId(int province, std::ofstream& debug)
{
	debug << "( " << province;
	int modProvince = provinceMap.find(province)->second;
	debug << " -> " << modProvince << " )";

	const auto& modStateItr = std::find_if(modStates.begin(), modStates.end(), [modProvince](const std::pair<std::string, std::set<int>> modState) {
		return modState.second.contains(modProvince);
	});
	return modStateItr->first;
}

void V2::IdConverter::mapUnlocalized(std::ofstream& debug)
{
	for (const auto& province: vanillaProvs)
	{
		int origProvinceID = province;
		debug << "Original province:\t" << origProvinceID << " ->";
		int modProvinceID = 0;

		if (locProvs.find(origProvinceID) == locProvs.end())
		{
			modProvinceID = origProvinceID;
			addProvinceMapping(origProvinceID, modProvinceID);
		}
		if (modProvinceID > 0)
		{
			debug << " " << modProvinceID << " Unlocalized province";
		}
		debug << "\n";
	}
}

void V2::IdConverter::mapUnchanged(std::ofstream& debug)
{
	for (const auto& province: vanillaProvs)
	{
		int origProvinceID = province;
		if (provinceMap.find(origProvinceID) != provinceMap.end())
			continue;

		debug << "Original province:\t" << origProvinceID << " ->";
		int modProvinceID = 0;

		// Get all eu4 provinces that map to this one
		const auto& eu4Provs = vanillaMapper.getEU4ProvinceNumbers(origProvinceID);

		// For each one of them
		for (int eu4Prov: eu4Provs)
		{
			// Find all mod provinces that they map to
			if (const auto& modProvs = provinceMapper.getVic2ProvinceNumbers(eu4Prov); !modProvs.empty())
			{
				debug << " " << eu4Prov << " (";
				for (const auto& modProv: modProvs)
				{
					debug << " " << modProv;
				}
				debug << " )";

				// If original ID is there, map it to itself
				if (const auto& modProvsItr = std::find(modProvs.begin(), modProvs.end(), origProvinceID); modProvsItr != modProvs.end())
				{
					modProvinceID = *modProvsItr;
					addProvinceMapping(origProvinceID, modProvinceID);
				}
			}
		}
		if (modProvinceID > 0)
		{
			debug << " -> " << modProvinceID << " Unchanged ID";
		}
		debug << "\n";
	}
}

void V2::IdConverter::mapLeftovers(std::ofstream& debug)
{
	// Cycle through all the provinces imported from Vanilla V2
	for (const auto& province: vanillaProvs)
	{
		int origProvinceID = province;
		if (provinceMap.find(origProvinceID) != provinceMap.end())
			continue;

		debug << "Original province:\t" << origProvinceID << " ->";
		int modProvinceID = 0;
		// Get all eu4 provinces that map to this one
		const auto& eu4Provs = vanillaMapper.getEU4ProvinceNumbers(origProvinceID);

		int fallback;
		// If original province can't be mapped, map it to the first one found in first mapping
		for (int eu4Prov: eu4Provs)
		{
			if (eu4Prov > 0)
			{
				for (int vic2Prov: provinceMapper.getVic2ProvinceNumbers(eu4Prov))
				{
					if (vic2Prov > 0)
						fallback = vic2Prov;
				}
			}
		}

		// For each one of them
		for (int eu4Prov: eu4Provs)
		{
			// Find all mod provinces that they map to
			if (const auto& modProvs = provinceMapper.getVic2ProvinceNumbers(eu4Prov); !modProvs.empty())
			{
				debug << " " << eu4Prov << " (";
				for (const auto& modProv: modProvs)
				{
					debug << " " << modProv;
				}
				debug << " )";

				bool isMapped = false;
				auto modProvsItr = modProvs.begin();
				const auto& e = modProvs.end();

				// Check all mod provinces mapped to eu4 province
				for (const auto& modProv: modProvs)
				{
					// Check if it's mapped
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
						debug << " -> " << modProvinceID << " Unmapped province\n";
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
			debug << " -> " << fallback << " Fallback\n";
		}
	}
}

void V2::IdConverter::outStates(std::string srcFile, std::string outFile) const
{
	const mappers::StateMapper map(srcFile);

	std::ofstream debug(outFile);
	if (!debug.is_open())
		Log(LogLevel::Debug) << "Could not create " + outFile;

	for (const auto& state: map.getStateMap())
	{
		debug << "ID:\t" << state.first << "\tProvinces: ";
		for (const auto& province: state.second)
		{
			debug << " " << province;
		}
		debug << "\n";
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
		if (!mapChecksOut)
			break;
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
		if (!mapChecksOut)
			break;
	}
	return mapChecksOut;
}

void V2::IdConverter::outStateMap(std::string outFile) const
{
	if (!verifyMap(stateMap))
		LOG(LogLevel::Debug) << "State map did not check out. Check " << outFile << " for duplicate mappings";

	std::ofstream debug(outFile);
	if (!debug.is_open())
		Log(LogLevel::Debug) << "Could not create " + outFile;
	for (const auto& mapping: stateMap)
	{
		debug << "V2 state: " << mapping.first << "\t\tMod state: " << mapping.second << "\n";
	}
}

void V2::IdConverter::outProvinceMap(std::string outFile) const
{
	if (!verifyMap(provinceMap))
		LOG(LogLevel::Debug) << "Province map did not check out. Check " << outFile << " for duplicate mappings";

	std::ofstream debug(outFile);
	if (!debug.is_open())
		Log(LogLevel::Debug) << "Could not create " + outFile;
	for (const auto& mapping: provinceMap)
	{
		debug << "V2 province:\t" << mapping.first << "\tMod province:\t" << mapping.second << "\n";
	}
}
