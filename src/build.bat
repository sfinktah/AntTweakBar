call "%VS140COMNTOOLS%\..\..\VC\vcvarsall.bat" x64
msbuild /p:Configuration=Release /clp:Summary /nologo AntTweakBar_VS2015.sln
