#!/bin/sh

SHELL_QML_PATH=$(pkg-config --variable=plugindir unity-shell-api)
UNITY_SOURCE_DIR=$(readlink -f $(dirname $(readlink -f $0))/../..)

dh_auto_configure -- -DARTIFACTS_DIR=${ADT_ARTIFACTS}

dh_auto_build --parallel -- -k xvfballtests
