echo on
rem Copy converter data files
copy "Data_Files\configuration.txt" "..\Release\EU4toVic2\configuration.txt"
copy "Data_Files\ReadMe.txt" "..\Release\EU4toVic2\readme.txt"
copy "Data_Files\ReadMe.txt" "..\Release\readme.txt"
copy "Data_Files\FAQ.txt" "..\Release\EU4toVic2\FAQ.txt"
copy "Data_Files\Eu4ToVic2DefaultConfiguration.xml" "..\Release\Configuration\Eu4ToVic2DefaultConfiguration.xml"
copy "Data_Files\SupportedConvertersDefault.xml" "..\Release\Configuration\SupportedConvertersDefault.xml"
copy "Data_Files\license.txt" "..\Release\EU4toVic2\license.txt"
copy "Data_Files\after_converting.txt" "..\Release\EU4toVic2\after_converting.txt"

rem Create Configurables
del "..\Release\EU4toVic2\configurables" /Q
rmdir "..\Release\EU4toVic2\configurables" /S /Q
xcopy "Data_Files\configurables" "..\Release\EU4toVic2\configurables" /Y /E /I

rem Create Blank Mod
del "..\Release\EU4toVic2\blankMod" /Q
rmdir "..\Release\EU4toVic2\blankMod" /S /Q
xcopy "Data_Files\blankMod" "..\Release\EU4toVic2\blankMod" /Y /E /I

rem Copy Flags
del "..\Release\EU4toVic2\flags" /Q
rmdir "..\Release\EU4toVic2\flags" /S /Q
xcopy "Data_Files\flags" "..\Release\EU4toVic2\flags" /Y /E /I
