#!/bin/sh
set -eux

BASEDIR=$1
WSDIR=$2
# ARCH=$2
# TMPDIR=$BASEDIR/tmp
SCRIPT_DIR=$(cd $(dirname $0); pwd)

# mkdir -p $TMPDIR
# cd $TMPDIR

# # Retrieve UXP tarball
# UXP_TGZ=RB_20231106.tar.gz
# UXP_VERSION=v0.0.1
# wget --progress=dot:giga -c \
#      https://github.com/CueMol/uxp_release/releases/download/$UXP_VERSION/$UXP_TGZ
# tar xzf $UXP_TGZ
# mv uxp ${WSDIR}/uxp_gui/platform

# # Apply patch
# cd ${WSDIR}/uxp_gui
# patch -p5 < uxp_diff.patch

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

CUEMOL_DIR=$BASEDIR/cuemol2
PROJ_DIR=$BASEDIR
BOOST_DIR=$PROJ_DIR/boost_1_84_0
DEPLIBS_DIR=$PROJ_DIR/boost_1_84_0/lib64-msvc-14.3

cd ${WSDIR}/uxp_gui
sed "s!@CUEMOL_BUNDLE@!$BUNDLE_DIR!g" $SCRIPT_DIR/mozconfig_win \
    | sed "s!@CUEMOL_DIR@!$CUEMOL_DIR!g" \
    | sed "s!@BOOST_DIR@!$BOOST_DIR!g" \
    | sed "s!@DEPLIBS_DIR@!$DEPLIBS_DIR!g" > .mozconfig
./mach build
./mach package

ls -l obj-*/dist/cuemol2-*.dmg
cp obj-*/dist/cuemol2-*.dmg ${WSDIR}/
cd ${WSDIR}
ls -l cuemol2-*.dmg
tar cjvf ${WSDIR}/${ARTIFACT_NAME} *.dmg
