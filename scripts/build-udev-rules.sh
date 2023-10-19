#!/bin/bash
#-----------------------------------------------------------------------------#
#  This script relies on the preprocessed *.ii and *.s files from             #
#    all "RGBController_" .cpp files and the detectors that call them         #
#    so that all #define macros have been replaced and can be text processed  #
#                                                                             #
#  This script produces the outfile of the 60-openrgb.rules target defined    #
#    in the Linux specific section of OpenRGB.pro                             #
#-----------------------------------------------------------------------------#

## Modular Variables
OPENRGB_PATH=$1
CONTROLLER_PATH=${OPENRGB_PATH}'/Controllers'
GIT_SHORT_HASH=$2
UDEV_FILE='60-openrgb.rules'
UDEV_LINE='#---------------------------------------------------------------#\n'

#Check the output of the makefile
# grep -A3 install_udev_rules: Makefile

#-----------------------------------------------------------------------------#
#  Add the static header of the udev rules                                    #
#-----------------------------------------------------------------------------#
echo -e "Adding Static Headers"
UDEV_HEADER=${UDEV_LINE}'#  OpenRGB udev rules - Git Commit: '${GIT_SHORT_HASH:0:8}'                    #\n'${UDEV_LINE}'\n'
UDEV_HEADER+=${UDEV_LINE}'#  User I2C/SMBus Access                                        #\n'${UDEV_LINE}'KERNEL=="i2c-[0-99]*", TAG+="uaccess"\n\n'
UDEV_HEADER+=${UDEV_LINE}'#  Super I/O Access                                             #\n'${UDEV_LINE}'KERNEL=="port", TAG+="uaccess"\n\n'

# Faustus rules
ASUS_TUF_DEVICES=('blue' 'flags' 'green' 'mode' 'red' 'set' 'speed')
UDEV_HEADER+=${UDEV_LINE}'#  ASUS TUF Laptops (faustus)                                   #\n'${UDEV_LINE}
for DEV in ${ASUS_TUF_DEVICES[@]}; do
  UDEV_HEADER+='ACTION=="add", SUBSYSTEM=="platform", KERNEL=="faustus", RUN+="/bin/chmod a+w /sys/bus/platform/devices/%k/kbbl/kbbl_'${DEV}'"\n'
done

# asus-wmi rules
ASUS_WMI_DEVICES=('kbd_rgb_mode' 'brightness')
UDEV_HEADER+='\n'${UDEV_LINE}'#  ASUS TUF Laptops (asus-wmi)                                  #\n'${UDEV_LINE}
for DEV in ${ASUS_WMI_DEVICES[@]}; do
  UDEV_HEADER+='ACTION=="add", SUBSYSTEM=="leds", KERNEL=="asus::kbd_backlight", RUN+="/bin/chmod a+w /sys%p/'${DEV}'"\n'
done

echo -e "$UDEV_HEADER" > "$UDEV_FILE"

#-----------------------------------------------------------------------------#
#  Create a list of USB HID PIDs                                              #
#    The HID list is produced from each "REGISTER_DETECTOR" macro replacement #
#                                                                             #
#    | callback_function | VID | PID | Name |                                 #
#-----------------------------------------------------------------------------#
echo -e "Creating device list"
HID_LIST=$(grep -hR -e "static HIDDeviceDetector" . | cut -d '(' -f 2- | awk -F , '{ print $2 ":|" $3 "|" $4 "|" $1 "|" }')
HID_WRAPPER_LIST=$(grep -hR -e "static HIDWrappedDeviceDetector" . | cut -d '(' -f 2- | awk -F , '{ print $2 ":|" $3 "|" $4 "|" $1 "|" }')
DUMMY_LIST=$( grep -hR -e DUMMY_DEVICE_DETECTOR ${CONTROLLER_PATH} | cut -d '(' -f 2- | cut -d ')' -f 1 | awk -F , '{ print $2 ":|" $3 "|" $4 "|" $1 "|" }')

#Check the output of the hid_list
# echo -e "$HID_LIST\n$DUMMY_LIST" >> "hid_list.txt"

#-----------------------------------------------------------------------------#
#  Create a list of RGBController.cpp classes including path                  #
#-----------------------------------------------------------------------------#
echo -e "Creating file list to parse metadata"
FILE_LIST=$(find ${CONTROLLER_PATH} -name "RGBController_*.cpp" | sort)

#Check the output of the file_list
# echo -e "$FILE_LIST" >> "file_list.txt"

#-----------------------------------------------------------------------------#
#  Iterate over the Controller file list and repplace the detectors           #
#    for each controller with the list of PIDs                                #
#-----------------------------------------------------------------------------#
while read -r controller
do
    ## 's/\r$//' - Convert DOS text to Unix text
    DATA=$(sed -e 's/\r$//' ${controller} | awk -v RS='' '/\/\*\*/')

    ## 's/^ *//g' - Remove all leading whitespace
    ## 's/\,/\n/g' - Convert a comma separated list to lines
    name=$(printf %s "$DATA" | grep @name |  sed -e 's/@name//g' -e 's/^ *//g')
    type=$(printf %s "$DATA" | grep @type |  sed -e 's/@type//g' -e 's/^ *//g')
    detectors=$(printf %s "$DATA" | grep @detectors |  sed -e 's/@detectors *//g' -e 's/^ *//g' -e 's/\,/\n/g')

    if [[ -n "$detectors" ]] && [[ $type = USB || $type = Serial ]]; then    #Check that the type is USB
        ## Iterate over the comma seperated detector function list
        while read -r detector
        do
            #Filter the list for all devices that use this detector
            text=$(printf '%s\n%s\n%s' "$HID_LIST" "$HID_WRAPPER_LIST" "$DUMMY_LIST" | grep ${detector} | cut -d: -f 2- | sed -e 's/"//g')

            #Replace the detector string with the list of devices
            detectors=${detectors/${detector}/${text}}

            #Add a section heading for each RGBcontroller_
            printf "#---------------------------------------------------------------#\n# %s - %s\n#---------------------------------------------------------------#\n" "$name" "$detector" >> "$UDEV_FILE"
            while read -r device
            do
                IFS='|' read null vid pid device_name null <<<"$device"

                #Remove leading hex signifier from $vid and $pid
                vid=${vid/0x/}
                pid=${pid/0x/}
                device_name=${device_name//[^[:alnum:][:blank:]]/}

                udev_line=$(printf 'SUBSYSTEMS=="%s|hidraw", ATTRS{idVendor}=="%s", ATTRS{idProduct}=="%s", TAG+="uaccess", TAG+="%s"\n' ${type,,} ${vid,,} ${pid,,} ${device_name// /_})

                #Check to ensure that the vid and pid are not blank
                if [[ $vid = "" || $pid = "" ]]; then
                    echo -e "Blank VID or PID Skipping: ${name} ${detector} ${udev_line}"
                else
                    echo -e "$udev_line" >>"$UDEV_FILE"
                fi
            done <<< "$text"
        done <<< "$detectors"
    fi
done <<< "$FILE_LIST"

if [ -f "$UDEV_FILE" ]; then
    outpath=$(readlink -f "$UDEV_FILE")
    echo -e "Udev rules built at: $outpath"

    ## Clean up the preprocessor files if the rules file was created
    # rm *.{ii,s}
else
    echo -e "Something went wrong. No Udev file was found"
fi
