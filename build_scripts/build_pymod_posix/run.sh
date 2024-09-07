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

# Install location
INST_PATH=$BASEDIR/cuemol2

cd $WORKSPACE/pymod
$PYTHON -m pip install \
     --config-settings=cmake.define.LIBCUEMOL2_ROOT=$INST_PATH \
     -v . 

# Copy dependent libs (boost)
ls -la $BASEDIR/$BOOST_VER/lib/lib*
cp $BASEDIR/$BOOST_VER/lib/lib* $BASEDIR/cuemol2/lib/
