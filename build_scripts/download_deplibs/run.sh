#!/bin/bash
set -eux

BASEDIR=$1
RUNNER_ARCH=$2
RUNNER_OS=$3
TMPDIR=$BASEDIR/tmp

mkdir -p $TMPDIR
cd $TMPDIR

# Retrieve deplibs binary
DEPLIBS_TGZ=deplibs_${RUNNER_OS}_${RUNNER_ARCH}.tar.bz2
DEPLIBS_VERSION=v0.0.2
wget --progress=dot:mega -c \
     https://github.com/CueMol/build_scripts/releases/download/$DEPLIBS_VERSION/$DEPLIBS_TGZ
xattr -cr $DEPLIBS_TGZ
tar xjf $DEPLIBS_TGZ
mv target/* $BASEDIR/
