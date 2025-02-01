#!/usr/bin/env bash
PRODUCTNAME="OpenRGB"
VENDOR=${PRODUCTNAME}

TLD="org"
WEBSITE="https://${PRODUCTNAME,,}.${TLD}"
NAMESPACE=$(uuidgen -n @url -N ${WEBSITE} --sha1 | awk '{ print toupper($0) }')
VENDOR_ID="${TLD}.${VENDOR}"
APP_ID="${VENDOR_ID}.${PRODUCTNAME,,}"

GITURL="https://gitlab.com/CalcProgrammer1/OpenRGB/-"
GITPARAM="?inline=false"
ICONFILE="qt/OpenRGB.ico"
LICENSEFILE="scripts/License.rtf"
BANNERIMAGE="scripts/banner.bmp"
DIALOGBACKGROUND="scripts/dialog_background.bmp"
PROJECT_FILE="OpenRGB.pro"
XMLOUTFILE=${PRODUCTNAME,,}".wxs"


VERSION=$(qmake ${PROJECT_FILE} 2>&1 | grep VERSION_WIX | cut -d ':' -f 3 | tr -d ' ')
#If the suffix is not empty set the channel to "-git"
CHANNEL=$(grep SUFFIX\  ${PROJECT_FILE} | cut -d= -f 2)
if [[ -n $CHANNEL ]];
then
    CHANNEL="-git"
fi


#The Upgrade code has to be consistent to allow upgrades between channels
#This value is roughly equivalent to "provides" in Linux packaging
UPGRADECODE=$(uuidgen -n ${NAMESPACE} -N ${APP_ID} --sha1 | awk '{ print toupper($0) }')
#The ProductID will be unique per build
PRODUCTCOMMENT="Open source RGB lighting control that doesn't depend on manufacturer software."

#Print Metadata to the log
echo -e "Icon URL:\t" $GITURL$ICONFILE
echo -e "License URL:\t" $GITURL$LICENSEFILE
echo -e "AppID - Channel:\t" ${APP_ID} " - " ${CHANNEL}
echo -e "Upgrade UUID:\t" ${UPGRADECODE}
echo -e "Product Name:\t" ${PRODUCTNAME}
echo -e "Vendor - VendorID:\t\t" ${VENDOR} " - " ${VENDOR_ID}
echo -e "Version:\t" ${VERSION}


#Wix and / or Wine have issues with the mixed upper and lower case letters
WORKING_PATH="orgb/"
BUILT_PATH="OpenRGB Windows 64-bit/"
mv -T "${BUILT_PATH}" ${WORKING_PATH}

EXTENSION="orp"
SAVE_FILE="${PRODUCTNAME}.${EXTENSION}"
count=1

for file in "$WORKING_PATH"/*;
do
    filename=$(basename "$file")
    if [ $filename == "${PRODUCTNAME}.exe" ] ; then
        #If this is the executable treat as a special case as we need the reference for later
        EXE_ID=${PRODUCTNAME}00
        EXE_FILE=${filename}
        #Add special entry to files list
        FILES="$FILES\t\t\t\t\t<File Id='${EXE_ID}' Source='${WORKING_PATH}${filename}'/>\n"
    elif [ -d "$file" ] ; then
            #If this is a directory then we need to add another component
            COMPONENTS="${COMPONENTS}\t\t\t<ComponentRef Id='${filename}Files'/>\n"
            TEMP="\t\t\t\t<Directory Id='${filename}' Name='${filename}'>\n\t\t\t\t\t<Component Id='${filename}Files' Guid='"$(uuidgen -t | awk '{ print toupper($0) }')"'>\n"
            for file2 in "$file"/*;
            do
                filename2=$(basename "$file2")
                TEMP="$TEMP\t\t\t\t\t\t<File Id='${PRODUCTNAME}${count}' Source='${WORKING_PATH}${filename}/${filename2}'/>\n"
                count=$((count+1))
            done
            DIRECTORIES="$DIRECTORIES$TEMP\t\t\t\t\t</Component>\n\t\t\t\t</Directory>\n"
    else
            #Any other file to files list
            FILES="$FILES\t\t\t\t\t<File Id='${PRODUCTNAME}${count}' Source='${WORKING_PATH}${filename}'/>\n"
            count=$((count+1))
    fi
done

echo -e "Building XML:\t" $XMLOUTFILE

XML_PACKAGE="\t<Package Keywords='Installer' Description='${PRODUCTNAME} Installer'\n\t\tComments=\"${PRODUCTCOMMENT}\" Manufacturer='OpenRGB'\n\t\tInstallerVersion='200' Languages='1033' Compressed='yes' SummaryCodepage='1252' Platform='x64'/>\n"
XML_MEDIA="\t<Media Id='1' Cabinet='${PRODUCTNAME,,}.cab' EmbedCab='yes'/>\n"
XML_CONDITIONS="\t<Condition Message='This package supports Windows 64bit Only'>VersionNT64</Condition>\n"
XML_ACTION_RUNAS_ADMIN="\t<CustomAction Id='LaunchOpenRGBAdmin' FileKey='${EXE_ID}' ExeCommand='${EXE_FILE} --loglevel 1 --nodetect' Execute='deferred' Return='asyncNoWait' Impersonate='no'/>\n"
XML_ACTION_FIRSTRUN="\t<CustomAction Id='LaunchOpenRGB' FileKey='${EXE_ID}' ExeCommand='--gui --loglevel 6' Execute='immediate' Return='asyncNoWait' Impersonate='yes'/>\n"
XML_ICON="\t<Icon Id='OpenRGBIcon' SourceFile='${ICONFILE}'/>\n"
XML_PROPERTY="\t<Property Id='ARPPRODUCTICON' Value='OpenRGBIcon'/>\n\t<Property Id='ARPURLINFOABOUT' Value='https://www.openrgb.org'/>\n"
XML_ACTIONS_EXECUTE="\t<InstallExecuteSequence>\n\t\t<Custom Action='LaunchOpenRGBAdmin' After='InstallFiles'>NOT Installed</Custom>\n\t\t<Custom Action='LaunchOpenRGB' After='InstallFinalize'>NOT Installed</Custom>\n\t</InstallExecuteSequence>\n"
XML_WIX_UI="\t<Property Id='WIXUI_INSTALLDIR' Value='INSTALLDIR' />\n\t<UIRef Id='WixUI_InstallDir'/>\n\t<UIRef Id='WixUI_ErrorProgressText'/>\n\t<WixVariable Id='WixUILicenseRtf' Value='${LICENSEFILE}'/>\n\t<WixVariable Id='WixUIBannerBmp' Value='${BANNERIMAGE}'/>\n\t<WixVariable Id='WixUIDialogBmp' Value='${DIALOGBACKGROUND}'/>\n"
XML_MAJOR_UPGRADE="\t<MajorUpgrade Schedule='afterInstallInitialize' AllowDowngrades='yes'/>\n"
XML_METADATA="$XML_PACKAGE $XML_MEDIA $XML_CONDITIONS $XML_MAJOR_UPGRADE $XML_ACTION_RUNAS_ADMIN $XML_ACTION_FIRSTRUN $XML_ICON $XML_PROPERTY $XML_ACTIONS_EXECUTE $XML_WIX_UI"

XML_STARTMENU="\t\t<Directory Id='ProgramMenuFolder' Name='Programs'>\n\t\t\t<Component Id='ProgramMenuShortcut' Guid='"$(uuidgen -t | awk '{ print toupper($0) }')"'>\n\t\t\t\t<Shortcut Id='startmenu${PRODUCTNAME}' Directory='ProgramMenuFolder' Name='${PRODUCTNAME}' Target='[#${EXE_ID}]' WorkingDirectory='INSTALLDIR' Advertise='no'/>\n\t\t\t\t<RemoveFolder Id='ProgramMenuShortcut' On='uninstall'/>\n\t\t\t\t<RegistryValue Root='HKCU' Key='Software\[Manufacturer]\[ProductName]' Type='string' Value='' KeyPath='yes'/>\n\t\t\t</Component>\n\t\t</Directory>\n"
XML_ASSOCIATE_FILE="\t\t\t\t\t<ProgId Id='${SAVE_FILE}' Description='${PRODUCTNAME} Profile'>\n\t\t\t\t\t\t<Extension Id='${EXTENSION}' ContentType='application/${EXTENSION}'>\n\t\t\t\t\t\t<Verb Id='open' Command='Open' TargetFile='${EXE_ID}' Argument='-p \"%1\"' />\n\t\t\t\t\t\t</Extension>\n\t\t\t\t\t</ProgId>\n"
XML_DIRECTORIES="\t<Directory Id='TARGETDIR' Name='SourceDir'>\n\t\t<Directory Id='ProgramFiles64Folder'>\n\t\t\t<Directory Id='INSTALLDIR' Name='${PRODUCTNAME}'>\n\t\t\t\t<Component Id='${PRODUCTNAME}Files' Guid='"$(uuidgen -t | awk '{ print toupper($0) }')"'>\n$FILES\n$XML_SHORTCUT\n$XML_ASSOCIATE_FILE\t\t\t\t</Component>\n$DIRECTORIES\t\t\t</Directory>\n\t\t</Directory>\n$XML_STARTMENU\t</Directory>\n"

XML_COMPONENTS="\t<Feature Id='Complete' Title='${PRODUCTNAME}' Description='Install all ${PRODUCTNAME} files.' Display='expand' Level='1' ConfigurableDirectory='INSTALLDIR'>\n\t\t<Feature Id='${PRODUCTNAME}Complete' Title='${PRODUCTNAME}' Description='The complete package.' Level='1' AllowAdvertise='no' InstallDefault='local'>\n\t\t\t<ComponentRef Id='${PRODUCTNAME}Files'/>\n$COMPONENTS\t\t\t<ComponentRef Id='ProgramMenuShortcut'/>\n\t\t</Feature>\n\t</Feature>\n"
XML_DATA="$XML_DIRECTORIES $XML_COMPONENTS"

#Wipe out any previous XMLOUTFILE and add the header
XML_HEADER="<?xml version='1.0' encoding='windows-1252'?>\n<Wix xmlns='http://schemas.microsoft.com/wix/2006/wi'>\n"
XML_PRODUCT="\t<Product Name='${PRODUCTNAME}' Manufacturer='${VENDOR}'\n\t\tId='*'\n\t\tUpgradeCode='"${UPGRADECODE}"'\n\t\tLanguage='1033' Codepage='1252' Version='${VERSION}'>\n$XML_METADATA\n$XML_DATA\n\t</Product>\n</Wix>"

echo -e $XML_HEADER $XML_PRODUCT > $XMLOUTFILE
echo -e "\t...Done!\n\n"


#Once the XML file manifest is created create the package
candle -arch x64 ${PRODUCTNAME,,}.wxs
light -sval -ext WixUIExtension ${PRODUCTNAME,,}.wixobj -out ${PRODUCTNAME}_Windows_64.msi
