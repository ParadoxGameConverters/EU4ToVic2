#!/bin/bash

# Copy converter globals
cp "Data_Files/configuration.txt" "Release-Linux/configuration.txt"
cp "Data_Files/ReadMe.txt" "Release-Linux/ReadMe.txt"
cp "Data_Files/FAQ.txt" "Release-Linux/FAQ.txt"
cp "Data_Files/after_converting.txt" "Release-Linux/after_converting.txt"
cp "Data_Files/license.txt" "Release-Linux/license.txt"

# Copy configurables
rm -rf "Release-Linux/configurables"
cp -r "Data_Files/configurables" "Release-Linux/configurables"

# Create Blank Mod
rm -rf "Release-Linux/blankMod"
cp -r "Data_Files/blankMod" "Release-Linux/blankMod"
mkdir "Release-Linux/blankMod/output/history/provinces"
mkdir "Release-Linux/blankMod/output/history/provinces/africa"
mkdir "Release-Linux/blankMod/output/history/provinces/asia"
mkdir "Release-Linux/blankMod/output/history/provinces/australia"
mkdir "Release-Linux/blankMod/output/history/provinces/austria"
mkdir "Release-Linux/blankMod/output/history/provinces/balkan"
mkdir "Release-Linux/blankMod/output/history/provinces/canada"
mkdir "Release-Linux/blankMod/output/history/provinces/carribean"
mkdir "Release-Linux/blankMod/output/history/provinces/central asia"
mkdir "Release-Linux/blankMod/output/history/provinces/china"
mkdir "Release-Linux/blankMod/output/history/provinces/france"
mkdir "Release-Linux/blankMod/output/history/provinces/germany"
mkdir "Release-Linux/blankMod/output/history/provinces/india"
mkdir "Release-Linux/blankMod/output/history/provinces/indonesia"
mkdir "Release-Linux/blankMod/output/history/provinces/italy"
mkdir "Release-Linux/blankMod/output/history/provinces/japan"
mkdir "Release-Linux/blankMod/output/history/provinces/low countries"
mkdir "Release-Linux/blankMod/output/history/provinces/mexico"
mkdir "Release-Linux/blankMod/output/history/provinces/pacific island"
mkdir "Release-Linux/blankMod/output/history/provinces/portugal"
mkdir "Release-Linux/blankMod/output/history/provinces/scandinavia"
mkdir "Release-Linux/blankMod/output/history/provinces/south america"
mkdir "Release-Linux/blankMod/output/history/provinces/soviet"
mkdir "Release-Linux/blankMod/output/history/provinces/spain"
mkdir "Release-Linux/blankMod/output/history/provinces/united kingdom"
mkdir "Release-Linux/blankMod/output/history/provinces/usa"
mkdir "Release-Linux/blankMod/output/history/countries"
mkdir "Release-Linux/blankMod/output/history/diplomacy"
mkdir "Release-Linux/blankMod/output/history/units"

# Copy Flags
rm -rf "Release-Linux/flags"
cp -r "Data_Files/flags" "Release-Linux/flags"

# Create changelog
#del Release-Linux/changelog.txt
#git log --oneline --decorate >> Release-Linux/log.txt
#(for /f "delims=" %%i in (Release-Linux/log.txt) do @echo %%i)>Release-Linux/changelog.txt
#del Release-Linux/log.txt
