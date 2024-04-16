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
 -DFFTW_ROOT=%DEPLIBS_DIR%\fftw-3.3.10 ^
 -DLCMS2_ROOT=%DEPLIBS_DIR%\lcms2-2.16 ^
 -DGLEW_ROOT=%DEPLIBS_DIR%\glew-2.2.0 ^
 -DLibLZMA_ROOT=%DEPLIBS_DIR%\xz-5.2.12 ^
 -DBUILD_PYTHON_BINDINGS=OFF ^
 -DBUILD_PYTHON_MODULE=OFF ^
 -DBUILD_XPCJS_BINDINGS=ON ^
 -DCGAL_DO_NOT_WARN_ABOUT_CMAKE_BUILD_TYPE=TRUE ^
 -DCMAKE_BUILD_TYPE=Release

# cmake --build build --config Release
# cmake --install build --config Release
