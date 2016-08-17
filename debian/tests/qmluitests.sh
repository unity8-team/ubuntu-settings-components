#!/bin/sh

export QT_SELECT=5

dh_auto_configure -- -DARTIFACTS_DIR=${ADT_ARTIFACTS}

dh_auto_build --parallel -- -C tests/qmltests/mocks

dh_auto_build --parallel -- -k xvfballtests
