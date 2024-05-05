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

REM Retrieve deplibs binary
SET DEPLIBS_TGZ=deplibs_%RUNNER_OS%_%RUNNER_ARCH%.tar.gz
SET DEPLIBS_VERSION=v0.0.2
wget --progress=dot:mega -c ^
     https://github.com/CueMol/build_prerequisites/releases/download/%DEPLIBS_VERSION%/%DEPLIBS_TGZ%
tar xzf %DEPLIBS_TGZ%
dir
SET DEPLIBS_DIR=%TMPDIR%\proj64_deplibs
echo "DEPLIBS_DIR:" %DEPLIBS_DIR%
del %DEPLIBS_DIR%\CGAL-4.14.3\lib\cmake\CGAL\CGALConfig-installation-dirs.cmake

REM Build libcuemol2
SET INSTPATH=%DEPLIBS_DIR%\cuemol2
rd /s /q build

cmake -S %GITHUB_WORKSPACE% -B build ^
 -DCMAKE_INSTALL_PREFIX=%INSTPATH% ^
 -DBoost_ROOT=%DEPLIBS_DIR%\boost_1_84_0 ^
 -DCGAL_ROOT=%DEPLIBS_DIR%\CGAL-4.14.3 ^
 -DFFTW_ROOT=%DEPLIBS_DIR%\fftw-3.3.10 ^
 -DLCMS2_ROOT=%DEPLIBS_DIR%\lcms2-2.16 ^
 -DGLEW_ROOT=%DEPLIBS_DIR%\glew-2.2.0 ^
 -DLibLZMA_ROOT=%DEPLIBS_DIR%\xz-5.2.12 ^
 -DBUILD_PYTHON_BINDINGS=OFF ^
 -DBUILD_PYTHON_MODULE=OFF ^
 -DBUILD_XPCJS_BINDINGS=ON ^
 -DCGAL_DO_NOT_WARN_ABOUT_CMAKE_BUILD_TYPE=TRUE ^
 -DCGAL_DISABLE_GMP=TRUE ^
 -DCGAL_HEADER_ONLY=TRUE ^
 -DCMAKE_BUILD_TYPE=Release

cmake --build build --config Release
cmake --install build --config Release
