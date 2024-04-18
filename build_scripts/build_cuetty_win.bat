if "%1"=="" (
   echo "arg1 not specified"
   exit /b
)

REM SET DEPLIBS_DIR=c:\proj64_deplibs
SET DEPLIBS_DIR=%1

if not exist %DEPLIBS_DIR% (
   echo "DEPLIBS_DIR not exists " %DEPLIBS_DIR%
   exit /b
)

echo "DEPLIBS_DIR: " %DEPLIBS_DIR%
SET INSTPATH=%DEPLIBS_DIR%\cuemol2
rd /s /q build

cmake -S . -B build ^
 -DCMAKE_INSTALL_PREFIX=%INSTPATH% ^
 -DCMAKE_PREFIX_PATH=%DEPLIBS_DIR% ^
 -DBoost_ROOT=%DEPLIBS_DIR%\boost_1_84_0 ^
 -DLIBCUEMOL2_ROOT=%DEPLIBS_DIR%\cuemol2 ^
 -DCMAKE_BUILD_TYPE=Release
