echo on

REM Common Setup
if "%1"=="" (
   echo "arg1 not specified"
   exit /b   
)
SET BASEDIR=%1
SET RUNNER_OS=Windows
SET RUNNER_ARCH=X64
SET TMPDIR=%BASEDIR%\tmp

mkdir %TMPDIR%
%~d1
cd %TMPDIR%

SET DEPLIBS_DIR=%BASEDIR%\proj64_deplibs
echo "DEPLIBS_DIR:" %DEPLIBS_DIR%

REM Build cuetty
SET INSTPATH=%DEPLIBS_DIR%\cuemol2
rd /s /q build

cmake -S %GITHUB_WORKSPACE%\cli -B build_cli ^
 -DCMAKE_INSTALL_PREFIX=%INSTPATH% ^
 -DCMAKE_PREFIX_PATH=%DEPLIBS_DIR% ^
 -DBoost_ROOT=%DEPLIBS_DIR%\boost_1_84_0 ^
 -DLIBCUEMOL2_ROOT=%DEPLIBS_DIR%\cuemol2 ^
 -DCMAKE_BUILD_TYPE=Release

cmake --build build_cli --config Release
cmake --install build_cli --config Release

copy %DEPLIBS_DIR%\boost_1_84_0\lib\*mt-x64*.dll %INSTPATH%\bin\

REM check run
%INSTPATH%\bin\cuetty.exe

