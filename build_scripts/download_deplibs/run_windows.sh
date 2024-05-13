#!/bin/bash
set -eux

BASEDIR=$1
RUNNER_OS=Windows
RUNNER_ARCH=X64
TMPDIR=$BASEDIR/tmp

mkdir -p $TMPDIR
cd $TMPDIR

# Retrieve deplibs binary
DEPLIBS_TGZ=deplibs_${RUNNER_OS}_${RUNNER_ARCH}.tar.gz
DEPLIBS_VERSION=v0.0.2
DEPLIBS_URL=https://github.com/CueMol/build_prerequisites/releases/download
wget --progress=dot:mega -c \
     $DEPLIBS_URL/$DEPLIBS_VERSION/$DEPLIBS_TGZ

tar xzf $DEPLIBS_TGZ
mv proj64_deplibs $BASEDIR/

