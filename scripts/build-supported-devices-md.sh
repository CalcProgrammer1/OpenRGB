#!/bin/bash
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
DATA_TABLE_HEAD='| Controller Name | Connection | Save to Flash | Direct | Hardware Effects |\n'
DATA_TABLE_ALIGN='| :--- | :---: | :---: | :---: | :---: |'
USB_DEVICE_TABLE_HEAD='| Vendor ID | Product ID | Device Name |\n'
GPU_DEVICE_TABLE_HEAD='| Vendor &<br/>Device ID | Sub-Vendor &<br/>Product ID | Device Name |\n'
DEVICE_TABLE_ALIGN='| :---: | :---: | :--- |'
MAIN_FILE='Supported Devices.md'
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
DELIMITER=$'\x01'
## | callback_function | VID | PID | Name |
HID_LIST=$(grep -hR -e "static\ HIDDeviceDetector" . | cut -d '(' -f 2- | awk -F , -v delim="${DELIMITER}" '{ print $2 ":"delim $3 delim $4 delim $1 delim }')
I2C_LIST=$(grep -hR -e "static\ I2CPCIDeviceDetector" . | cut -d '(' -f 2- | awk -F , -v delim="${DELIMITER}" '{ print $2 ":"delim $3 delim $4 delim $5 delim $6 delim $1 delim }')

printf %s\n%s "$HID_LIST" "$I2C_LIST" > "device.list"

## This is the list of RGBController.cpp classes including path
FILE_LIST=$(find ${CONTROLLER_PATH} | grep RGBController_ | grep cpp | sort)

echo -e $FILE_LIST > "file.list"

## Write the heading to the main file prior to iterating over the controller list
echo -e "# Supported Devices\n\n" > "$MAIN_FILE"

## Write quick links to each section heading in an unordered list
echo -e "- [Motherboards](#motherboards)" >> "$MAIN_FILE"
echo -e "- [RAM](#ram)" >> "$MAIN_FILE"
echo -e "- [Graphics Cards](#graphics-cards)" >> "$MAIN_FILE"
echo -e "- [Coolers](#coolers)" >> "$MAIN_FILE"
echo -e "- [LED Strips](#led-strips)" >> "$MAIN_FILE"
echo -e "- [Keyboards](#keyboards)" >> "$MAIN_FILE"
echo -e "- [Microphones](#microphones)" >> "$MAIN_FILE"
echo -e "- [Mice](#mice)" >> "$MAIN_FILE"
echo -e "- [Mouse Mats](#mouse-mats)" >> "$MAIN_FILE"
echo -e "- [Headsets](#headsets)" >> "$MAIN_FILE"
echo -e "- [Headset Stands](#headset-stands)" >> "$MAIN_FILE"
echo -e "- [Gamepads](#gamepads)" >> "$MAIN_FILE"
echo -e "- [Accessories](#Accessories)" >> "$MAIN_FILE"
echo -e "- [Lights](#lights)" >> "$MAIN_FILE"
echo -e "- [Speakers](#speakers)" >> "$MAIN_FILE"
echo -e "- [Virtual Devices](#virtual-devices)" >> "$MAIN_FILE"
echo -e "- [Storage](#storage)" >> "$MAIN_FILE"
echo -e "- [Cases](#cases)" >> "$MAIN_FILE"
echo -e "- [Other Devices](#other-devices)" >> "$MAIN_FILE"

echo -e "\n## Legend\n\n" >> "$MAIN_FILE"
echo -e "| Symbol | Meaning |" >> "$MAIN_FILE"
echo -e "| :---: | :--- |" >> "$MAIN_FILE"
echo -e "| ${WHITE_CHECK_MARK} | Fully supported by OpenRGB |" >> "$MAIN_FILE"
echo -e "| ${ROTATING_LIGHT} | Support is problematic<br/>See device page for details |" >> "$MAIN_FILE"
echo -e "| ${ROBOT} | Feature is automatic and can not be turned off |" >> "$MAIN_FILE"
echo -e "| ${TOOLS} | Partially supported by OpenRGB<br/>See device page for details |" >> "$MAIN_FILE"
echo -e "| ${NO_ENTRY} | Not currently supported by OpenRGB |" >> "$MAIN_FILE"
echo -e "| ${CROSS} | Not applicable for this device |" >> "$MAIN_FILE"

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

    if [[ $name = *[![:blank:]]* ]]; then    #Check that the name is not blank
        case $save in
            ${CROSS})
                save_title="Not supported by controller"
                ;;
            ${NO_ENTRY})
                save_title="Not currently supported by OpenRGB"
                ;;
            ${ROBOT})
                save_title="Controller saves automatically on every update"
                ;;
            ${WHITE_CHECK_MARK})
                save_title="Saving is supported by this controller"
                ;;
        esac

        case $direct in
            ${CROSS})
                direct_title="Not supported by controller"
                ;;
            ${NO_ENTRY})
                direct_title="Not currently supported by OpenRGB"
                ;;
            ${ROTATING_LIGHT})
                direct_title="Direct control is problematic (See device page for details)"
                ;;
            ${WHITE_CHECK_MARK})
                direct_title="Direct control is supported for Software Effects"
                ;;
        esac

        case $effects in
            ${CROSS})
                effects_title="Hardware effects are not supported by controller"
                ;;
            ${NO_ENTRY})
                effects_title="Not currently supported by OpenRGB"
                ;;
            ${ROTATING_LIGHT})
                effects_title="Hardware effects implementation is problematic"
                ;;
            ${TOOLS})
                effects_title="Hardware effects are not fully implemented by controller (See device page for details)"
                ;;
            ${WHITE_CHECK_MARK})
                effects_title="Hardware effects are supported"
                ;;
        esac

        link=$(echo -e ${name//[[:blank:]]/-} | sed -e 's/(/%28/' -e 's/)/%29/')
        ## Output this controllers entry into the main file's tables
        current_controller='|['${name}']('${link}'.md)|'${type}'|<span title="'$save_title'">'${save}'</span>|<span title="'$direct_title'">'${direct}'</span>|<span title="'$effects_title'">'${effects}'</span>|\n'

        ## Add entry to all relevant categories listed in @category
        while read -r category
        do
            case $category in
                Motherboard)
                    motherboard+=$current_controller
                    ;;
                RAM)
                    ram+=$current_controller
                    ;;
                GPU)
                    gpu+=$current_controller
                    ;;
                Cooler)
                    cooler+=$current_controller
                    ;;
                LEDStrip)
                    ledstrip+=$current_controller
                    ;;
                Keyboard)
                    keyboard+=$current_controller
                    ;;
                Microphone)
                    microphone+=$current_controller
                    ;;
                Mouse)
                    mouse+=$current_controller
                    ;;
                Mousemat)
                    mousemat+=$current_controller
                    ;;
                Headset)
                    headset+=$current_controller
                    ;;
                HeadsetStand)
                    headset_stand+=$current_controller
                    ;;
                Gamepad)
                    gamepad+=$current_controller
                    ;;
                Accessory)
                    accessory+=$current_controller
                    ;;
                Light)
                    light+=$current_controller
                    ;;
                Speaker)
                    speaker+=$current_controller
                    ;;
                Virtual)
                    virtual+=$current_controller
                    ;;
                Storage)
                    storage+=$current_controller
                    ;;
                Case)
                    case+=$current_controller
                    ;;
                Dummy)
                    ## Do nothing for the Dummy controller
                    ;;
                *)
                    unknown+=$current_controller
                    ;;
            esac
        done <<< "$categories"

        ## Create a controller page and fill it with header details and the start of the device table
        outfile=${name//[[:blank:]]/-}.md
        echo -e "# ${name}\n\n ${comment}\n" >"$outfile"
        echo -e "## Connection Type\n ${type}\n" >> "$outfile"
        echo -e "## Saving\n ${save_title}\n" >> "$outfile"
        echo -e "## Direct Mode\n ${direct_title}\n" >> "$outfile"
        echo -e "## Hardware Effects\n ${effects_title}\n" >> "$outfile"
        if [[ $categories = GPU ]]; then
            echo -e "## Device List\n\n${GPU_DEVICE_TABLE_HEAD}${DEVICE_TABLE_ALIGN}" >> "$outfile"
        else
            echo -e "## Device List\n\n${USB_DEVICE_TABLE_HEAD}${DEVICE_TABLE_ALIGN}" >> "$outfile"
        fi

        ## Iterate over the comma seperated detector function list
        while read -r detector
        do
            #Filter the list for all devices that use this detector
            text=$(printf "%s\n%s" "$HID_LIST" "$I2C_LIST" | grep ${detector} | cut -d: -f 2- | sed -e 's/"//g')

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
                        IFS="${DELIMITER}" read null vid pid device_name null <<< "${device}"
                        #Remove leading hex signifier from $vid and $pid
                        vid=${vid/0x/}
                        pid=${pid/0x/}
                        device_name=${device_name//[^[:alnum:][:punct:][:blank:]]/}
                        escaped_comment=$(LC_ALL=C sed 's/["]/""/g' <<<"$comment")
                        
                        csv_row=$(printf '"%s","%s","%s","%s","%s","%s","","","%s","%s","%s","%s"\n' "${device_name//|/\\|}" "${categories//|/\\|}" "${type//|/\\|}" "${name//|/\\|}" "${vid/ /}" "${pid/ /}" "${save}" "${direct}" "${effects}" "${escaped_comment}")
                        table_row=$(printf '| `%s` | `%s` | %s |' "${vid/ /}" "${pid/ /}" "${device_name//|/\\|}")
                        ;;
                    I2C | SMBus)
                        IFS="${DELIMITER}" read null vid pid svid spid device_name null <<<"$device"
                        #Remove leading hex signifiers
                        vid=${vid/0x/}
                        pid=${pid/0x/}
                        svid=${svid/0x/}
                        spid=${spid/0x/}
                        device_name=${device_name//[^[:alnum:][:punct:][:blank:]]/}
                        escaped_comment=$(LC_ALL=C sed 's/["]/""/g' <<<"$comment")

                        csv_row=$(printf '"%s","%s","%s","%s","%s","%s","%s","%s","%s","%s","%s","%s"\n' "${device_name//|/\\|}" "${categories//|/\\|}" "${type//|/\\|}" "${name//|/\\|}" "${vid/ /}" "${pid/ /}" "${svid/ /}" "${spid/ /}" "${save}" "${direct}" "${effects}" "${escaped_comment}")
                        table_row=$(printf '| `%s:%s` | `%s:%s` | %s |' "${vid/ /}" "${pid/ /}" "${svid/ /}" "${spid/ /}" "${device_name//|/\\|}")
                        ;;
                    *)
                        table_row=""
                        ;;
                esac

                if [[ $table_row = *[![:blank:]]* ]]; then
                    echo -e "$table_row" >>"$outfile"
                    echo -e "$csv_row" >>"$CSV_FILE"
                fi
            done <<< "$text"
        done <<< "$detectors"
    fi
done <<< "$FILE_LIST"

printf "\n## Motherboards\n${DATA_TABLE_HEAD}${DATA_TABLE_ALIGN}%s\n" "$(echo -e ${motherboard}| sort)">> "$MAIN_FILE"
printf "\n## RAM\n${DATA_TABLE_HEAD}${DATA_TABLE_ALIGN}%s\n" "$(echo -e ${ram}| sort)">> "$MAIN_FILE"
printf "\n## Graphics Cards\n${DATA_TABLE_HEAD}${DATA_TABLE_ALIGN}%s\n" "$(echo -e ${gpu}| sort)" >> "$MAIN_FILE"
printf "\n## Coolers\n${DATA_TABLE_HEAD}${DATA_TABLE_ALIGN}%s\n" "$(echo -e ${cooler}| sort)" >> "$MAIN_FILE"
printf "\n## LED Strips\n${DATA_TABLE_HEAD}${DATA_TABLE_ALIGN}%s\n" "$(echo -e ${ledstrip}| sort)" >> "$MAIN_FILE"
printf "\n## Keyboards\n${DATA_TABLE_HEAD}${DATA_TABLE_ALIGN}%s\n" "$(echo -e ${keyboard}| sort)" >> "$MAIN_FILE"
printf "\n## Microphones\n${DATA_TABLE_HEAD}${DATA_TABLE_ALIGN}%s\n" "$(echo -e ${microphone}| sort)" >> "$MAIN_FILE"
printf "\n## Mice\n${DATA_TABLE_HEAD}${DATA_TABLE_ALIGN}%s\n" "$(echo -e ${mouse}| sort)" >> "$MAIN_FILE"
printf "\n## Mouse Mats\n${DATA_TABLE_HEAD}${DATA_TABLE_ALIGN}%s\n" "$(echo -e ${mousemat}| sort)" >> "$MAIN_FILE"
printf "\n## Headsets\n${DATA_TABLE_HEAD}${DATA_TABLE_ALIGN}%s\n" "$(echo -e ${headset}| sort)" >> "$MAIN_FILE"
printf "\n## Headset Stands\n${DATA_TABLE_HEAD}${DATA_TABLE_ALIGN}%s\n" "$(echo -e ${headset_stand}| sort)" >> "$MAIN_FILE"
printf "\n## Gamepads\n${DATA_TABLE_HEAD}${DATA_TABLE_ALIGN}%s\n" "$(echo -e ${gamepad}| sort)" >> "$MAIN_FILE"
printf "\n## Accessories\n${DATA_TABLE_HEAD}${DATA_TABLE_ALIGN}%s\n" "$(echo -e ${accessory}| sort)" >> "$MAIN_FILE"
printf "\n## Lights\n${DATA_TABLE_HEAD}${DATA_TABLE_ALIGN}%s\n" "$(echo -e ${light}| sort)" >> "$MAIN_FILE"
printf "\n## Speakers\n${DATA_TABLE_HEAD}${DATA_TABLE_ALIGN}%s\n" "$(echo -e ${speaker}| sort)" >> "$MAIN_FILE"
printf "\n## Virtual Devices\n${DATA_TABLE_HEAD}${DATA_TABLE_ALIGN}%s\n" "$(echo -e ${virtual}| sort)" >> "$MAIN_FILE"
printf "\n## Storage\n${DATA_TABLE_HEAD}${DATA_TABLE_ALIGN}%s\n" "$(echo -e ${storage}| sort)" >> "$MAIN_FILE"
printf "\n## Cases\n${DATA_TABLE_HEAD}${DATA_TABLE_ALIGN}%s\n" "$(echo -e ${case}| sort)" >> "$MAIN_FILE"
printf "\n## Other Devices\n${DATA_TABLE_HEAD}${DATA_TABLE_ALIGN}%s\n" "$(echo -e ${unknown}| sort)" >> "$MAIN_FILE"

