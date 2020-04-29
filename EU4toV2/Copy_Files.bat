echo on
rem Copy converter data files
copy "Data_Files\log.txt" "..\Release\EU4ToVic2\log.txt"
copy "Data_Files\configuration-example.txt" "..\Release\EU4ToVic2\configuration-example.txt"

mkdir "..\Release\Configuration"
copy "Data_Files\fronter-configuration.txt" "..\Release\Configuration\fronter-configuration.txt"
copy "Data_Files\fronter-options.txt" "..\Release\Configuration\fronter-options.txt"

mkdir "..\Release\Docs"
copy "Data_Files\ReadMe.txt" "..\Release\Docs\readme.txt"
copy "Data_Files\FAQ.txt" "..\Release\Docs\FAQ.txt"
copy "Data_Files\license.txt" "..\Release\Docs\license.txt"
copy "Data_Files\after_converting.txt" "..\Release\Docs\after_converting.txt"

rem Create Configurables
del "..\Release\EU4ToVic2\configurables" /Q
rmdir "..\Release\EU4ToVic2\configurables" /S /Q
xcopy "Data_Files\configurables" "..\Release\EU4ToVic2\configurables" /Y /E /I

rem Create Blank Mod
del "..\Release\EU4ToVic2\blankMod" /Q
rmdir "..\Release\EU4ToVic2\blankMod" /S /Q
xcopy "Data_Files\blankMod" "..\Release\EU4ToVic2\blankMod" /Y /E /I

rem Copy Flags
del "..\Release\EU4ToVic2\flags" /Q
rmdir "..\Release\EU4ToVic2\flags" /S /Q
xcopy "Data_Files\flags" "..\Release\EU4ToVic2\flags" /Y /E /I
