#!/bin/sh
set -eux

BASEDIR=$1
ARCH=$2
TMPDIR=$BASEDIR/tmp

mkdir -p $TMPDIR
cd $TMPDIR

# Retrieve UXP tarball
UXP_TGZ=RB_20231106.tar.gz
UXP_VERSION=v0.0.1
wget --progress=dot:giga -c \
     https://github.com/CueMol/uxp_release/releases/download/$UXP_VERSION/$UXP_TGZ
tar xjf $UXP_TGZ
mv uxp ${GITHUB_WORKSPACE}/uxp_gui/platform

# Apply patch
cd ${GITHUB_WORKSPACE}/uxp_gui
patch -p5 < uxp_diff.patch

