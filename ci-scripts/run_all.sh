#!/bin/sh
set -e
set -x

BASEDIR=$(dirname "$0")

# Install the required tool chain
chmod +x $BASEDIR/install_toolchain.sh
sh $BASEDIR/install_toolchain.sh
