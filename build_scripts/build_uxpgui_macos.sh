#!/bin/sh
set -eux

BASEDIR=$1
ARCH=$2
TMPDIR=$BASEDIR/tmp
SCRIPT_DIR=$(cd $(dirname $0); pwd)

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

# Setup bundle software
BUNDLE_DIR=$TMPDIR/cuemol2_bundle
mkdir -p $BUNDLE_DIR
pushd $BUNDLE_DIR

# Retrieve povray prebuild binary
POV_TGZ=povray_macOS_${ARCH}.tar.bz2
wget --progress=dot:mega -c \
     https://github.com/CueMol/povray_build/releases/download/v0.0.5/$POV_TGZ
xattr -cr $POV_TGZ
tar xjvf $POV_TGZ

# Retrieve ffmpeg bin
FFMPEG_DIST=ffmpeg61arm
wget --progress=dot:mega -c https://www.osxexperts.net/${FFMPEG_DIST}.zip
xattr -cr ${FFMPEG_DIST}.zip
mkdir -p $BUNDLE_DIR/ffmpeg/bin
cd $BUNDLE_DIR/ffmpeg/bin
unzip -o ../../${FFMPEG_DIST}.zip
popd

# Build UXP

xcrun --show-sdk-path
SDK_PATH=/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX14.0.sdk

CUEMOL_DIR=$BASEDIR/cuemol2
PROJ_DIR=$BASEDIR/deplibs
BOOST_DIR=$PROJ_DIR/boost_1_84
DEPLIBS_DIR=$PROJ_DIR/boost_1_84/lib

cd ${GITHUB_WORKSPACE}/uxp_gui
sed "s!@CUEMOL_BUNDLE@!$BUNDLE_DIR!g" $SCRIPT_DIR/mozconfig_${ARCH} \
    | sed "s!@CUEMOL_DIR@!$CUEMOL_DIR!g" \
    | sed "s!@BOOST_DIR@!$BOOST_DIR!g" \
    | sed "s!@DEPLIBS_DIR@!$DEPLIBS_DIR!g" \
    | sed "s!@SDK_PATH@!$SDK_PATH!g" > .mozconfig
./mach build
./mach package
popd
