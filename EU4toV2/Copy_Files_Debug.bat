echo on
rem Copy converter data files
copy "Data_Files\configuration.txt" "..\Debug\EU4toVic2\configuration.txt"
copy "Data_Files\ReadMe.txt" "..\Debug\EU4toVic2\readme.txt"
copy "Data_Files\ReadMe.txt" "..\Debug\readme.txt"
copy "Data_Files\FAQ.txt" "..\Debug\EU4toVic2\FAQ.txt"
copy "Data_Files\Eu4ToVic2DefaultConfiguration.xml" "..\Debug\Configuration\Eu4ToVic2DefaultConfiguration.xml"
copy "Data_Files\SupportedConvertersDefault.xml" "..\Debug\Configuration\SupportedConvertersDefault.xml"
copy "Data_Files\license.txt" "..\Debug\EU4toVic2\license.txt"
copy "Data_Files\after_converting.txt" "..\Debug\EU4toVic2\after_converting.txt"

rem Create Configurables
del "..\Debug\EU4toVic2\configurables" /Q
rmdir "..\Debug\EU4toVic2\configurables" /S /Q
xcopy "Data_Files\configurables" "..\Debug\EU4toVic2\configurables" /Y /E /I

rem Create Blank Mod
del "..\Debug\EU4toVic2\blankMod" /Q
rmdir "..\Debug\EU4toVic2\blankMod" /S /Q
xcopy "Data_Files\blankMod" "..\Debug\EU4toVic2\blankMod" /Y /E /I

rem Copy Flags
del "..\Debug\EU4toVic2\flags" /Q
rmdir "..\Debug\EU4toVic2\flags" /S /Q
xcopy "Data_Files\flags" "..\Debug\EU4toVic2\flags" /Y /E /I
