call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" amd64
cd modules
cl /EHsc *.cpp /link /DLL /OUT:../modules.dll