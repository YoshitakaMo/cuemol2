#!/bin/sh
#
# build script for pymod in posix
# usage: run.sh deplibs_dir
#

set -eux

BASEDIR=$1

PYTHON="python3.12"
REPOS_DIR=$(cd $(dirname $0)/../..; pwd)
WORKSPACE=${GITHUB_WORKSPACE:-$REPOS_DIR}

BOOST_VER=boost_1_84_0
BOOST_DIR=$BASEDIR/$BOOST_VER

# Install location
INST_PATH=$BASEDIR/cuemol2

cd $WORKSPACE/pymod
$PYTHON -m pip install \
     --config-settings=cmake.define.LIBCUEMOL2_ROOT=$INST_PATH \
     --config-settings=cmake.define.Boost_ROOT=$BOOST_DIR \
     -v . 

# Copy dependent libs (boost)
ls -la $BOOST_DIR/lib/lib*
cp $BOOST_DIR/lib/lib* $BASEDIR/cuemol2/lib/

# run python tests
cd $WORKSPACE
$PYTHON -m pip install pytest
env LD_LIBRARY_PATH=$INST_PATH/lib \
    pytest tests

