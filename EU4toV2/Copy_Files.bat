echo on
rem Copy converter data files
copy "Data_Files\configuration.txt" "..\Release\EU4toVic2\configuration.txt"
copy "Data_Files\readme.txt" "..\Release\EU4toVic2\readme.txt"
copy "Data_Files\readme.txt" "..\Release\readme.txt"
copy "Data_Files\merge_nations.txt" "..\Release\EU4toVic2\merge_nations.txt"
copy "Data_Files\starting_factories.txt" "..\Release\EU4toVic2\starting_factories.txt"
copy "Data_Files\province_mappings.txt" "..\Release\EU4toVic2\province_mappings.txt"
copy "Data_Files\country_mappings.txt" "..\Release\EU4toVic2\country_mappings.txt"
copy "Data_Files\cultureMap.txt" "..\Release\EU4toVic2\cultureMap.txt"
copy "Data_Files\slaveCultureMap.txt" "..\Release\EU4toVic2\slaveCultureMap.txt"
copy "Data_Files\religionMap.txt" "..\Release\EU4toVic2\religionMap.txt"
copy "Data_Files\unions.txt" "..\Release\EU4toVic2\unions.txt"
copy "Data_Files\nationals.txt" "..\Release\EU4toVic2\nationals.txt"
copy "Data_Files\unreleasable_tags.txt" "..\Release\EU4toVic2\unreleasable_tags.txt"
copy "Data_Files\governmentMapping.txt" "..\Release\EU4toVic2\governmentMapping.txt"
copy "Data_Files\techGroups.txt" "..\Release\EU4toVic2\techGroups.txt"
copy "Data_Files\reform_effects.txt" "..\Release\EU4toVic2\reform_effects.txt"
copy "Data_Files\blocked_tech_schools.txt" "..\Release\EU4toVic2\blocked_tech_schools.txt"
copy "Data_Files\leader_traits.txt" "..\Release\EU4toVic2\leader_traits.txt"
copy "Data_Files\regiment_costs.txt" "..\Release\EU4toVic2\regiment_costs.txt"
copy "Data_Files\license.txt" "..\Release\EU4toVic2\license.txt"
copy "Data_Files\idea_effects.txt" "..\Release\EU4toVic2\idea_effects.txt"
copy "Data_Files\colonial_flags.txt" "..\Release\EU4toVic2\colonial_flags.txt"
copy "Data_Files\colonial_tags.txt" "..\Release\EU4toVic2\colonial_tags.txt"
copy "Data_Files\ck2titlemap.txt" "..\Release\EU4toVic2\ck2titlemap.txt"
copy "Data_Files\port_blacklist.txt" "..\Release\EU4toVic2\port_blacklist.txt"
copy "Data_Files\port_whitelist.txt" "..\Release\EU4toVic2\port_whitelist.txt"
copy "Data_Files\minorityPops.txt" "..\Release\EU4toVic2\minorityPops.txt"
copy "Data_Files\terrainData.txt" "..\Release\EU4toVic2\terrainData.txt"
copy "Data_Files\FAQ.txt" "..\Release\EU4toVic2\FAQ.txt"
copy "Data_Files\after_converting.txt" "..\Release\EU4toVic2\after_converting.txt"
copy "Data_Files\Eu4ToVic2DefaultConfiguration.xml" "..\Release\Configuration\Eu4ToVic2DefaultConfiguration.xml"
copy "Data_Files\SupportedConvertersDefault.xml" "..\Release\Configuration\SupportedConvertersDefault.xml"

rem Create Blank Mod
del "..\Release\EU4toVic2\blankMod" /Q
rmdir "..\Release\EU4toVic2\blankMod" /S /Q
xcopy "Data_Files\blankMod" "..\Release\EU4toVic2\blankMod" /Y /E /I

rem Copy Flags
del "..\Release\EU4toVic2\flags" /Q
rmdir "..\Release\EU4toVic2\flags" /S /Q
xcopy "Data_Files\flags" "..\Release\EU4toVic2\flags" /Y /E /I
