#!/bin/sh

dh_auto_configure -- -DARTIFACTS_DIR=${ADT_ARTIFACTS}

dh_auto_build --parallel -- -k xvfballtests
