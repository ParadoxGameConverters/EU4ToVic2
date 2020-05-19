#!/bin/sh

export CC=/usr/bin/gcc-9
export CXX=/usr/bin/g++-9

rm -rf Release/
cd Fronter
./build_linux.sh
mv Release ../
cd ../EU4toV2
rm -rf build
rm -rf Release-Linux
cmake -H. -Bbuild
cmake --build build -- -j3 
mv Release-Linux ../Release/EU4ToVic2
cd ..

cp EU4toV2/Data_Files/*yml Release/Configuration/
cp EU4toV2/Data_Files/fronter*txt Release/Configuration/

tar -cjf EU4ToVic2-dev-release.tar.bz2 Release
