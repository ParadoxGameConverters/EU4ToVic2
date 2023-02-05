#!/bin/bash

# Copy converter globals
rm -rf ../Release-Linux/Docs/
mkdir -p ../Release-Linux/Docs/
cp Data_Files/ReadMe.txt ../Release-Linux/Docs/
cp Data_Files/FAQ.txt ../Release-Linux/Docs/
cp Data_Files/after_converting.txt ../Release-Linux/Docs/
cp Data_Files/license.txt ../Release-Linux/Docs/
cp Data_Files/log.txt ../Release-Linux/EU4ToVic2/
cp Data_Files/configuration-example.txt ../Release-Linux/EU4ToVic2/
cp Resources/librakaly.so ../Release-Linux/EU4ToVic2

# Copy Fronter configuration
rm -rf ../Release-Linux/Configuration
mkdir -p ../Release-Linux/Configuration
cp Data_Files/fronter-configuration.txt ../Release-Linux/Configuration/
cp Data_Files/fronter-options.txt ../Release-Linux/Configuration/
cp Data_Files/*yml ../Release-Linux/Configuration/

# Copy Fronter resources
cp ../Fronter/Fronter/Resources/converter.ico ../Release-Linux/
cp ../Fronter/Fronter/Resources/GeneralFAQ-READ.ME.FIRST.txt ../Release-Linux/
cp ../Fronter/Fronter/Resources/*.yml ../Release-Linux/Configuration/

# Copy configurables
rm -rf Release-Linux/EU4ToVic2/configurables
cp -r Data_Files/configurables ../Release-Linux/EU4ToVic2

# Copy blankmod
rm -rf Release-Linux/EU4ToVic2/blankMod
cp -r Data_Files/blankMod ../Release-Linux/EU4ToVic2

# Copy Flags
rm -rf Release-Linux/EU4ToVic2/flags
cp -r Data_Files/flags ../Release-Linux/EU4ToVic2
