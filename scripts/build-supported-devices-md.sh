#!/usr/bin/env bash
#-----------------------------------------------------------------------------#
#  This script relies on the preprocessed *.ii and *.s files from             #
#    all "RGBController_" .cpp files and the detectors that call them         #
#    so that all #define macros have been replaced and can be text processed  #
#                                                                             #
#  This script produces a set of markdown documents as defined by the         #
#    supported_devs target in the Linux specific section of OpenRGB.pro       #
#-----------------------------------------------------------------------------#

## Modular Variables
OPENRGB_PATH=$1
CONTROLLER_PATH=${OPENRGB_PATH}'/Controllers'

CSV_TABLE_HEAD='Name,Category,Type,RGBController,VID,PID,SVID,SPID,Save,Direct,Effects,Comments'
CSV_FILE='Supported Devices.csv'

## Symbol Declarations
WHITE_CHECK_MARK_STRING=':white_check_mark:'
WHITE_CHECK_MARK='✔️'
ROTATING_LIGHT_STRING=':rotating_light:'
ROTATING_LIGHT='🚨'
ROBOT_STRING=':robot:'
ROBOT='🤖'
TOOLS_STRING=':tools:'
TOOLS='⚒️'
NO_ENTRY_STRING=':o:'
NO_ENTRY='🚫'
CROSS_STRING=':x:'
CROSS='❌'

declare -a STRINGS=($WHITE_CHECK_MARK_STRING $ROTATING_LIGHT_STRING $ROBOT_STRING $TOOLS_STRING $NO_ENTRY_STRING $CROSS_STRING)
declare -a UNICODE=($WHITE_CHECK_MARK $ROTATING_LIGHT $ROBOT $TOOLS $NO_ENTRY $CROSS)

## We first need to build OpenRGB with the preprocessed files saved
#qmake ${OPENRGB_PATH}/OpenRGB.pro "QMAKE_CXXFLAGS+=-save-temps"
#make -j$(nproc)

## The HID list is produced from each "REGISTER_DETECTOR" macro replacement.
DLM=$'\x01'
## | callback_function | VID | PID | Name |
HID_LIST=$(grep -hR -e "static\ HIDDeviceDetector" . | sed -e "s/^.*\(\".*\"\), \(.*\), \([0-9ABCDEFx]*\), \([0-9ABCDEFx]*\).*,.*,.*,.*;$/\2${DLM}\3${DLM}\4${DLM}\1/g")
I2C_LIST=$(grep -hR -e "static\ I2CPCIDeviceDetector" . | sed -e "s/^.*\(\".*\"\), \(.*\), \([0-9ABCDEFx]*\), \([0-9ABCDEFx]*\), \([0-9ABCDEFx]*\), \([0-9ABCDEFx]*\),.*;$/\2${DLM}\3${DLM}\4${DLM}\5${DLM}\6${DLM}\1/")
DUMMY_LIST=$(grep -hR -e DUMMY_DEVICE_DETECTOR\( ${CONTROLLER_PATH} | sed -e "s/^.*\(\".*\"\), \(.*\), \([0-9ABCDEFx]*\), \([0-9ABCDEFx]*\) ).*/\2${DLM}\3${DLM}\4${DLM}\1/")

printf "%s\n%s\n%s" "$HID_LIST" "$I2C_LIST" "$DUMMY_LIST" > "device.list"

## This is the list of RGBController.cpp classes including path
FILE_LIST=$(find ${CONTROLLER_PATH} | grep RGBController_ | grep cpp | sort)

echo -e $FILE_LIST > "file.list"
echo -e "${CSV_TABLE_HEAD}" > "${CSV_FILE}"

while read -r controller
do
    ## 's/\r$//' - Convert DOS text to Unix text
    DATA=$(sed -e 's/\r$//' ${controller} | awk '/\/\*\*/{flag=1;next}/\\\*/{flag=0}flag')

    ## 's/^ *//g' - Remove all leading whitespace
    ## 's/\,/\n/g' - Convert a comma separated list to lines
    name=$(printf %s "$DATA" | grep @name |  sed -e 's/@name//g' -e 's/^ *//g')
    categories=$(printf %s "$DATA" | grep @category |  sed -e 's/@category//g' -e 's/^ *//g' -e 's/\,/\n/g')
    type=$(printf %s "$DATA" | grep @type |  sed -e 's/@type//g' -e 's/^ *//g')
    save=$(printf %s "$DATA" | grep @save |  sed -e 's/@save//g' -e 's/^ *//g')
    direct=$(printf %s "$DATA" | grep @direct |  sed -e 's/@direct//g' -e 's/^ *//g')
    effects=$(printf %s "$DATA" | grep @effects |  sed -e 's/@effects//g' -e 's/^ *//g')
    detectors=$(printf %s "$DATA" | grep @detectors |  sed -e 's/@detectors *//g' -e 's/^ *//g' -e 's/\,/\n/g')
    comment=$(printf %s "$DATA" | awk -v RS='    @' '/comment/' | sed -e 's/comment//g' -e 's/^ *//g')

    # Replace any of the wiki strings with the unicode equivalents
    replace_size=${#STRINGS[@]}
    for (( i=0; i<${replace_size}; i++ ));
    do
      save=${save/${STRINGS[$i]}/${UNICODE[$i]}}
      direct=${direct/${STRINGS[$i]}/${UNICODE[$i]}}
      effects=${effects/${STRINGS[$i]}/${UNICODE[$i]}}
    done

    ## Iterate over the comma seperated detector function list
    while read -r detector
    do
        #Filter the list for all devices that use this detector
        text=$(printf "%s\n%s\n%s" "$HID_LIST" "$I2C_LIST" "$DUMMY_LIST" | grep ${detector} | cut -d: -f 2- | sed -e 's/"//g')
        if [[ -z ${text} ]]; then
            echo "${detector} was not found in the list"
        fi

        #Replace the detector string with the list of devices
        detectors=${detectors/${detector}/${text}}

        vid=""
        pid=""
        svid=""
        spid=""
        device_name=""

        while read -r device
        do
            case $type in
                USB)
                    IFS="${DLM}" read null vid pid device_name null <<< "${device}"
                    #Remove leading hex signifier from $vid and $pid
                    vid=${vid/0x/}
                    pid=${pid/0x/}
                    device_name=${device_name//[^[:alnum:][:punct:][:blank:]]/}
                    escaped_comment=$(LC_ALL=C sed 's/["]/""/g' <<<"$comment")

                    csv_row=$(printf '"%s","%s","%s","%s","%s","%s","","","%s","%s","%s","%s"\n' "${device_name//|/\\|}" "${categories//|/\\|}" "${type//|/\\|}" "${name//|/\\|}" "${vid/ /}" "${pid/ /}" "${save}" "${direct}" "${effects}" "${escaped_comment}")
                    ;;
                I2C | SMBus)
                    IFS="${DLM}" read null vid pid svid spid device_name null <<<"$device"
                    #Remove leading hex signifiers
                    vid=${vid/0x/}
                    pid=${pid/0x/}
                    svid=${svid/0x/}
                    spid=${spid/0x/}
                    device_name=${device_name//[^[:alnum:][:punct:][:blank:]]/}
                    escaped_comment=$(LC_ALL=C sed 's/["]/""/g' <<<"$comment")

                    csv_row=$(printf '"%s","%s","%s","%s","%s","%s","%s","%s","%s","%s","%s","%s"\n' "${device_name//|/\\|}" "${categories//|/\\|}" "${type//|/\\|}" "${name//|/\\|}" "${vid/ /}" "${pid/ /}" "${svid/ /}" "${spid/ /}" "${save}" "${direct}" "${effects}" "${escaped_comment}")
                    ;;
                *)
                    csv_row=""
                    ;;
            esac

            if [[ $csv_row = *[![:blank:]]* ]]; then
                echo -e "$csv_row" >>"$CSV_FILE"
            fi
        done <<< "$text"
    done <<< "$detectors"
done <<< "$FILE_LIST"
