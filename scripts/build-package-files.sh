#!/usr/bin/env bash
PROJECT_FILE="OpenRGB.pro"
VERSION_PATTERN="__VERSION__"
INFILE_SUFFIX=".in"

if [ -z "$1" ]; then
    echo "ERROR! No file give to parse."
    exit 1
fi

INFILE_PATH=${1}${INFILE_SUFFIX}

if [ ! -e ${INFILE_PATH} ]; then
    echo "ERROR! Source file ${INFILE_PATH} missing."
    exit 1
fi

if [ -e ${1} ]; then
    echo "Warning: File ${1} exists and will be overwritten"
fi

VERSION_VAR="VERSION_NUM"
QMAKE_EXE="qmake"

if [[ ${1} == *"debian"* ]]; then
  VERSION_VAR="VERSION_DEB"
  QMAKE_EXE="qmake"
fi

if [[ ${1} == *"fedora"* ]]; then
  VERSION_VAR="VERSION_RPM"
  QMAKE_EXE="qmake-qt5"
fi

PACKAGE_VERSION=$(${QMAKE_EXE} ${PROJECT_FILE} 2>&1 | grep ${VERSION_VAR} | cut -d ':' -f 3 | tr -d ' ')
echo $PACKAGE_VERSION

sed -e "s/${VERSION_PATTERN}/${PACKAGE_VERSION}/g" ${INFILE_PATH} > ${1}
