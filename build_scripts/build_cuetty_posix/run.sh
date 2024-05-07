#!/bin/sh
set -eux

BASEDIR=$1
BOOST_VER=boost_1_84_0

# Install location
INST_PATH=$BASEDIR/cuemol2

# Build
BUILD_DIR=$BASEDIR/build_cuetty
mkdir -p $BUILD_DIR
cd $BUILD_DIR

# BUILD_TYPE=Debug
BUILD_TYPE=Release

ls -la $BASEDIR

cmake -S ${GITHUB_WORKSPACE}/cli -B $BUILD_DIR \
      -DCMAKE_INSTALL_PREFIX=$INST_PATH \
      -DCMAKE_PREFIX_PATH=$BASEDIR \
      -DBoost_ROOT=$BASEDIR/$BOOST_VER/ \
      -DLIBCUEMOL2_ROOT=$BASEDIR\cuemol2 \
      -DCMAKE_BUILD_TYPE=$BUILD_TYPE

make # -j 8
make install

ls -la $BASEDIR/$BOOST_VER/lib/lib*
cp $BASEDIR/$BOOST_VER/lib/lib* $BASEDIR/cuemol2/lib/
