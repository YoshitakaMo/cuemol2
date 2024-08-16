#!/bin/bash

SRC_DIR=$HOME/tmp/proj64_deplibs/tmp/uxp

diff -x "*.pyc" -x "*~" -r -u $SRC_DIR platform > uxp_diff.patch
