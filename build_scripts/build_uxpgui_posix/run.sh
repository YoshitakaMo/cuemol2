#!/bin/sh
set -eux

BASEDIR=$1
RUNNER_OS=$2
RUNNER_ARCH=$3

TMPDIR=$BASEDIR/tmp
mkdir -p $TMPDIR
cd $TMPDIR

SCRIPT_DIR=$(cd $(dirname $0); pwd)

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
POV_TGZ=povray_${RUNNER_OS}_${RUNNER_ARCH}.tar.bz2
wget --progress=dot:mega -c \
     https://github.com/CueMol/povray_build/releases/download/v0.0.5/$POV_TGZ
xattr -cr $POV_TGZ
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
xattr -cr ${FFMPEG_DIST}.zip
mkdir -p $BUNDLE_DIR/ffmpeg/bin
cd $BUNDLE_DIR/ffmpeg/bin
unzip -o ../../${FFMPEG_DIST}.zip
popd

# Build UXP
cd ${GITHUB_WORKSPACE}/uxp_gui

CUEMOL_DIR=$BASEDIR/cuemol2
BOOST_DIR=$BASEDIR/boost_1_84_0
DEPLIBS_DIR=$BASEDIR/boost_1_84_0/lib

if [ $RUNNER_OS = "macOS" ]; then
    brew install autoconf@2.13 yasm

    SDK_BASE_PATH=$(dirname $(xcrun --show-sdk-path))
    SDK_PATH=$(echo $SDK_BASE_PATH/MacOSX*.*.sdk | tr ' ' '\n' | sort | tail -1)
    echo SDK_PATH: $SDK_PATH
    ls -la $SDK_PATH

    ADD_MOZCONFIG=""
    if [ $RUNNER_ARCH = "ARM64" ]; then
        BUILD_ARCH="aarch64-apple-darwin"
        SDK_PATH=/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX14.0.sdk
    elif [ $RUNNER_ARCH = "X64" ]; then
        BUILD_ARCH="x86_64-apple-darwin"
        # SDK_PATH=/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX11.1.sdk
        ADD_MOZCONFIG="export CXXFLAGS='-stdlib=libc++'"
    else
        echo "unknown runner arch: $RUNNER_ARCH"
        exit 1
    fi

    sed "s!@CUEMOL_BUNDLE@!$BUNDLE_DIR!g" $SCRIPT_DIR/mozconfig_macOS \
        | sed "s!@CUEMOL_DIR@!$CUEMOL_DIR!g" \
        | sed "s!@BOOST_DIR@!$BOOST_DIR!g" \
        | sed "s!@DEPLIBS_DIR@!$DEPLIBS_DIR!g" \
        | sed "s!@BUILD_ARCH@!$BUILD_ARCH!g" \
        | sed "s!@SDK_PATH@!$SDK_PATH!g" > .mozconfig

    echo $ADD_MOZCONFIG >> .mozconfig

    # elif [ $RUNNER_OS = "Linux" ]; then
else
    echo "unknown runner os: $RUNNER_OS"
    exit 1
fi

./mach build
./mach package

if [ $RUNNER_OS = "macOS" ]; then
    ls -l obj-*/dist/cuemol2-*.dmg
    cp obj-*/dist/cuemol2-*.dmg ${GITHUB_WORKSPACE}/
    cd ${GITHUB_WORKSPACE}
    ls -l cuemol2-*.dmg
    tar cjvf ${GITHUB_WORKSPACE}/${ARTIFACT_NAME} *.dmg
else
    echo "unknown runner os: $RUNNER_OS"
    exit 1
fi