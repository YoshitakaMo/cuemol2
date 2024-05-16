#!/bin/sh
set -eux

BASEDIR=$1
RUNNER_OS=$2
RUNNER_ARCH=$3

SCRIPT_DIR=$(cd $(dirname $0); pwd)

# TMPDIR=$BASEDIR/tmp
# mkdir -p $TMPDIR

###########
# Setup bundle software
BUNDLE_DIR=$BASEDIR/cuemol2_bundle_apps
mkdir -p $BUNDLE_DIR
pushd $BUNDLE_DIR

# Retrieve povray prebuild binary
POV_TGZ=povray_${RUNNER_OS}_${RUNNER_ARCH}.tar.bz2
wget --progress=dot:mega -c \
     https://github.com/CueMol/povray_build/releases/download/v0.0.5/$POV_TGZ
if [ $RUNNER_OS = "macOS" ]; then
    xattr -cr $POV_TGZ
fi
tar xjf $POV_TGZ

# Retrieve ffmpeg bin
if [ $RUNNER_ARCH = "ARM64" ]; then
    FFMPEG_DIST=ffmpeg61arm
elif [ $RUNNER_ARCH = "X64" ]; then
    FFMPEG_DIST=ffmpeg61intel
else
    echo "unknown runner arch: $RUNNER_ARCH"
    exit 1
fi

wget --progress=dot:mega -c https://www.osxexperts.net/${FFMPEG_DIST}.zip
if [ $RUNNER_OS = "macOS" ]; then
    xattr -cr ${FFMPEG_DIST}.zip
fi
mkdir -p $BUNDLE_DIR/ffmpeg/bin
cd $BUNDLE_DIR/ffmpeg/bin
unzip -o ../../${FFMPEG_DIST}.zip
popd

# # Clean-up tmpdir
# rm -rf $TMPDIR
