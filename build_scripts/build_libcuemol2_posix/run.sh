#!/bin/bash
#
# build script for libcuemol2 in posix
# usage: run.sh deplibs_dir [Debug]
#

set -eux

BASEDIR=$1
# RUNNER_OS=$2
# RUNNER_ARCH=$3

REPOS_DIR=$(cd $(dirname $0)/../..; pwd)
WORKSPACE=${GITHUB_WORKSPACE:-$REPOS_DIR}
DEBUG_FLAG=${2:-}
NCPU=8

# Build
BUILD_DIR=$BASEDIR/build_libcuemol2
mkdir -p $BUILD_DIR
cd $BUILD_DIR

if [ $DEBUG_FLAG = "Debug" ]; then
    BUILD_TYPE=Debug
else
    BUILD_TYPE=Release
fi

PYTHON=python3
PYTHON_ROOT=$($PYTHON -c 'import sys;import pathlib; print(pathlib.Path(sys.executable).parent.parent)')
echo "PYTHON_ROOT=$PYTHON_ROOT"

BUILD_PYTHON_BINDINGS=OFF
BUILD_PYTHON_MODULE=OFF

# Install location
CMAKE_INSTALL_PREFIX=$BASEDIR/cuemol2

CMAKE_PREFIX_PATH="$BASEDIR"
ls -la $BASEDIR

cmake -S ${WORKSPACE} -B $BUILD_DIR \
      -DCMAKE_C_COMPILER_LAUNCHER=sccache \
      -DCMAKE_CXX_COMPILER_LAUNCHER=sccache \
      -DCMAKE_INSTALL_PREFIX=$CMAKE_INSTALL_PREFIX \
      -DCMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH \
      -DBoost_ROOT=$BASEDIR/boost_1_84_0 \
      -DFFTW_ROOT=$BASEDIR/fftw-3.3.10 \
      -DLCMS2_ROOT=$BASEDIR/lcms2-2.15 \
      -DGLEW_ROOT=$BASEDIR/glew-2.1.0 \
      -DBUILD_PYTHON_BINDINGS=$BUILD_PYTHON_BINDINGS \
      -DBUILD_PYTHON_MODULE=$BUILD_PYTHON_MODULE \
      -DPython3_ROOT_DIR=$PYTHON_ROOT \
      -DBUILD_XPCJS_BINDINGS=ON \
      -DCGAL_DO_NOT_WARN_ABOUT_CMAKE_BUILD_TYPE=TRUE \
      -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
      -DCGAL_DISABLE_GMP=TRUE \
      -DCGAL_HEADER_ONLY=TRUE

make -j $NCPU
make install
