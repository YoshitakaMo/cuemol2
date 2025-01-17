#!/bin/sh
set -eux

BASEDIR=$1
RUNNER_OS="Windows"
RUNNER_ARCH="X64"

SCRIPT_DIR=$(cd $(dirname $0); pwd)
WSDIR=$(cd $(dirname $0)/..; pwd)
cd ${WSDIR}/uxp_gui

if [ ! -d ${WSDIR}/uxp_gui/platform ]; then
    # Retrieve UXP tarball
    UXP_TGZ=RB_20231106.tar.gz
    UXP_VERSION=v0.0.1
    wget --progress=dot:giga -c \
         https://github.com/CueMol/uxp_release/releases/download/$UXP_VERSION/$UXP_TGZ
    rm -rf uxp
    set +e
    tar xzf $UXP_TGZ
    set -e
    mv uxp platform

    # Apply patch
    patch -p5 < uxp_diff.patch
fi


# Setup bundle software
BUNDLE_DIR=$BASEDIR/cuemol2_bundle

# mkdir -p $BUNDLE_DIR
# pushd $BUNDLE_DIR
# # Retrieve povray prebuild binary
# POV_TGZ=povray_macOS_${ARCH}.tar.bz2
# wget --progress=dot:mega -c \
#      https://github.com/CueMol/povray_build/releases/download/v0.0.5/$POV_TGZ
# xattr -cr $POV_TGZ
# tar xjvf $POV_TGZ

# # Retrieve ffmpeg bin
# FFMPEG_DIST=ffmpeg61arm
# wget --progress=dot:mega -c https://www.osxexperts.net/${FFMPEG_DIST}.zip
# xattr -cr ${FFMPEG_DIST}.zip
# mkdir -p $BUNDLE_DIR/ffmpeg/bin
# cd $BUNDLE_DIR/ffmpeg/bin
# unzip -o ../../${FFMPEG_DIST}.zip
# popd

# Build UXP
cd ${WSDIR}/uxp_gui
./mach build
./mach package
