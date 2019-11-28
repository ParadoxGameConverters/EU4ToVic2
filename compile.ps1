#del -Force *.zip 

#del -Force -Recurse Release
#del -Force -Recurse ReleaseIntermediate

& "C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\MSBuild\15.0\Bin\msbuild.exe" EU4ToVic2.sln /p:Configuration=Release /p:Platform=Win32 /m


#cd Release
#del *.pdb
#del *.ipdb
#del *.iobj
#cd ..
