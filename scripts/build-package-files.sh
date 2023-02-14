#!/bin/bash
PROJECT_FILE="OpenRGB.pro"
VERSION_PATTERN="__VERSION__"
INFILE_SUFFIX=".in"

if [ -z "$1" ]; then
    echo "ERROR! No file give to parse."
    exit 1
fi

INFILE_PATH=${1}${INFILE_SUFFIX}

if [ -e ${INFILE_PATH} ]; then
    echo "ERROR! Source file ${INFILE_PATH} missing."
fi

if [ -e ${1} ]; then
    echo "Warning: File ${1} exists and will be overwritten"
fi

MAJOR=$(grep MAJOR\  ${PROJECT_FILE} | cut -d= -f 2 | tr -d [:space:])
MINOR=$(grep MINOR\  ${PROJECT_FILE} | cut -d= -f 2 | tr -d [:space:])
REVISION=$(grep REVISION\  ${PROJECT_FILE} | cut -d= -f 2 | tr -d [:space:])

#Convert Revision to a nummber in case it is blank in the project file
REVISION=$(( ${REVISION} + 0 ))

PACKAGE_VERSION="${MAJOR}.${MINOR}.${REVISION}"

sed -e "s/${VERSION_PATTERN}/${PACKAGE_VERSION}/g" ${INFILE_PATH} > ${1}
