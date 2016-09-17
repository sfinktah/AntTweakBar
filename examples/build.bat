call "%VS140COMNTOOLS%\..\..\VC\vcvarsall.bat" x64
msbuild /p:Configuration=Release /m /clp:Summary /nologo Examples_VS2012.sln
