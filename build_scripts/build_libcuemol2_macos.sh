#!/bin/sh
set -eux

BASEDIR=$1
ARCH=$2
TMPDIR=$BASEDIR/tmp

mkdir -p $TMPDIR
cd $TMPDIR

# Retrieve deplibs binary
DEPLIBS_TGZ=deplibs_macOS_${ARCH}.tar.bz2
DEPLIBS_VERSION=v0.0.1
wget --progress=dot:mega -c \
     https://github.com/CueMol/build_scripts/releases/download/$DEPLIBS_VERSION/$DEPLIBS_TGZ
xattr -cr $DEPLIBS_TGZ
tar xjf $DEPLIBS_TGZ
mv builds $BASEDIR/deplibs
PROJ_DIR=$BASEDIR/deplibs

# Build
BUILD_DIR=$BASEDIR/build
mkdir -p $BUILD_DIR
cd $BUILD_DIR

# BUILD_TYPE=Debug
BUILD_TYPE=Release

PYTHON=python3
PYTHON_ROOT=$($PYTHON -c 'import sys;import pathlib; print(pathlib.Path(sys.executable).parent.parent)')
echo "PYTHON_ROOT=$PYTHON_ROOT"

BUILD_PYTHON_BINDINGS=OFF
BUILD_PYTHON_MODULE=OFF

# Install location
CMAKE_INSTALL_PREFIX=$BASEDIR/cuemol2

CMAKE_PREFIX_PATH="$PROJ_DIR"
ls -la $PROJ_DIR

cmake ${GITHUB_WORKSPACE} \
      -DCMAKE_C_COMPILER_LAUNCHER=sccache \
      -DCMAKE_CXX_COMPILER_LAUNCHER=sccache \
      -DCMAKE_INSTALL_PREFIX=$CMAKE_INSTALL_PREFIX \
      -DCMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH \
      -DBoost_ROOT=$PROJ_DIR/boost_1_84/ \
      -DFFTW_ROOT=$PROJ_DIR/fftw-3.3.10 \
      -DLCMS2_ROOT=$PROJ_DIR/lcms2-2.15 \
      -DGLEW_ROOT=$PROJ_DIR/glew-2.1.0 \
      -DBUILD_PYTHON_BINDINGS=$BUILD_PYTHON_BINDINGS \
      -DBUILD_PYTHON_MODULE=$BUILD_PYTHON_MODULE \
      -DPython3_ROOT_DIR=$PYTHON_ROOT \
      -DBUILD_XPCJS_BINDINGS=ON \
      -DCGAL_DO_NOT_WARN_ABOUT_CMAKE_BUILD_TYPE=TRUE \
      -DCMAKE_BUILD_TYPE=$BUILD_TYPE

make -j 8
make install
