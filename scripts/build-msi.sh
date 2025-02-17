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

#############################################################
# Build file list for files in the working path             #
#############################################################

for file in "$WORKING_PATH"/*;
do
    filename=$(basename "$file")
    if [ $filename == "${PRODUCTNAME}.exe" ] ; then
        #If this is the executable treat as a special case as we need the reference for later
        EXE_ID=${PRODUCTNAME}00
        EXE_FILE=${filename}
        #Add special entry to files list
        FILES="$FILES                        <File Id='${EXE_ID}' Source='${WORKING_PATH}${filename}'/>\n"
    elif [ -d "$file" ] ; then
            #If this is a directory then we need to add another component
            COMPONENTS="${COMPONENTS}                <ComponentRef Id='${filename}Files'/>\n"
            TEMP="                    <Directory Id='${filename}' Name='${filename}'>\n                        <Component Id='${filename}Files' Guid='"$(uuidgen -t | awk '{ print toupper($0) }')"'>\n"
            for file2 in "$file"/*;
            do
                filename2=$(basename "$file2")
                TEMP="$TEMP                            <File Id='${PRODUCTNAME}${count}' Source='${WORKING_PATH}${filename}/${filename2}'/>\n"
                count=$((count+1))
            done
            DIRECTORIES="$DIRECTORIES$TEMP                        </Component>\n                    </Directory>\n"
    else
            #Any other file to files list
            FILES="$FILES                        <File Id='${PRODUCTNAME}${count}' Source='${WORKING_PATH}${filename}'/>\n"
            count=$((count+1))
    fi
done

#############################################################
# Create the Wix XML file                                   #
#   Set IFS to the empty string to allow arbitrary spacing  #
#   within the XML_FILE variable, then set it back to a     #
#   space character after we print the XML file             #
#############################################################
IFS=""
XML_FILE=""

XML_FILE+="<?xml version='1.0' encoding='windows-1252'?>\r\n"
XML_FILE+="<Wix xmlns='http://schemas.microsoft.com/wix/2006/wi'>\r\n"
XML_FILE+="    <Product Name='${PRODUCTNAME}' Manufacturer='${VENDOR}'\r\n"
XML_FILE+="        Id='*'\r\n"
XML_FILE+="        UpgradeCode='"${UPGRADECODE}"'\r\n"
XML_FILE+="        Language='1033' Codepage='1252' Version='${VERSION}'>\r\n"
XML_FILE+="        <Package Keywords='Installer' Description='${PRODUCTNAME} Installer'\r\n"
XML_FILE+="            Comments=\"${PRODUCTCOMMENT}\" Manufacturer='OpenRGB'\r\n"
XML_FILE+="            InstallerVersion='200' Languages='1033' Compressed='yes' SummaryCodepage='1252' Platform='x64'/>\r\n"
XML_FILE+="        <Media Id='1' Cabinet='${PRODUCTNAME,,}.cab' EmbedCab='yes'/>\r\n"
XML_FILE+="        <Condition Message='This package supports Windows 64bit Only'>VersionNT64</Condition>\r\n"
XML_FILE+="        <MajorUpgrade Schedule='afterInstallInitialize' AllowDowngrades='yes'/>\r\n"
XML_FILE+="        <CustomAction Id='LaunchOpenRGBAdmin' FileKey='${EXE_ID}' ExeCommand='${EXE_FILE} --loglevel 1 --nodetect' Execute='deferred' Return='asyncNoWait' Impersonate='no'/>\r\n"
XML_FILE+="        <CustomAction Id='LaunchOpenRGB' FileKey='${EXE_ID}' ExeCommand='--gui --loglevel 6' Execute='immediate' Return='asyncNoWait' Impersonate='yes'/>\r\n"
XML_FILE+="        <Icon Id='OpenRGBIcon' SourceFile='${ICONFILE}'/>\r\n"
XML_FILE+="        <Property Id='ARPPRODUCTICON' Value='OpenRGBIcon'/>\r\n"
XML_FILE+="        <Property Id='ARPURLINFOABOUT' Value='https://www.openrgb.org'/>\r\n"
XML_FILE+="        <InstallExecuteSequence>\r\n"
XML_FILE+="            <Custom Action='LaunchOpenRGBAdmin' After='InstallFiles'>NOT Installed</Custom>\r\n"
XML_FILE+="            <Custom Action='LaunchOpenRGB' After='InstallFinalize'>NOT Installed</Custom>\r\n"
XML_FILE+="        </InstallExecuteSequence>\r\n"
XML_FILE+="        <Property Id='WIXUI_INSTALLDIR' Value='INSTALLDIR'/>\r\n"
XML_FILE+="        <UIRef Id='WixUI_InstallDir'/>\r\n"
XML_FILE+="        <UIRef Id='WixUI_ErrorProgressText'/>\r\n"
XML_FILE+="        <WixVariable Id='WixUILicenseRtf' Value='${LICENSEFILE}'/>\r\n"
XML_FILE+="        <WixVariable Id='WixUIBannerBmp' Value='${BANNERIMAGE}'/>\r\n"
XML_FILE+="        <WixVariable Id='WixUIDialogBmp' Value='${DIALOGBACKGROUND}'/>\r\n"
XML_FILE+="\r\n"
XML_FILE+="        <Directory Id='TARGETDIR' Name='SourceDir'>\r\n"
XML_FILE+="            <Directory Id='ProgramFiles64Folder'>\r\n"
XML_FILE+="                <Directory Id='INSTALLDIR' Name='${PRODUCTNAME}'>\r\n"
XML_FILE+="                    <Component Id='${PRODUCTNAME}Files' Guid='"$(uuidgen -t | awk '{ print toupper($0) }')"'>\r\n"
XML_FILE+="${FILES}\r\n"
XML_FILE+="                        <ProgId Id='${SAVE_FILE}' Description='${PRODUCTNAME} Profile'>\r\n"
XML_FILE+="                            <Extension Id='${EXTENSION}' ContentType='application/${EXTENSION}'>\r\n"
XML_FILE+="                                <Verb Id='open' Command='Open' TargetFile='${EXE_ID}' Argument='-p \"%1\"' />\r\n"
XML_FILE+="                            </Extension>\r\n"
XML_FILE+="                        </ProgId>\r\n"
XML_FILE+="                    </Component>\r\n"
XML_FILE+="${DIRECTORIES}\r\n"
XML_FILE+="                </Directory>\r\n"
XML_FILE+="            </Directory>\r\n"
XML_FILE+="            <Directory Id='ProgramMenuFolder' Name='Programs'>\r\n"
XML_FILE+="                <Component Id='ProgramMenuShortcut' Guid='"$(uuidgen -t | awk '{ print toupper($0) }')"'>\r\n"
XML_FILE+="                    <Shortcut Id='startmenu${PRODUCTNAME}' Directory='ProgramMenuFolder' Name='${PRODUCTNAME}' Target='[#${EXE_ID}]' WorkingDirectory='INSTALLDIR' Advertise='no'/>\r\n"
XML_FILE+="                    <RemoveFolder Id='ProgramMenuShortcut' On='uninstall'/>\r\n"
XML_FILE+="                    <RegistryValue Root='HKCU' Key='Software\[Manufacturer]\[ProductName]' Type='string' Value='' KeyPath='yes'/>\r\n"
XML_FILE+="                </Component>\r\n"
XML_FILE+="            </Directory>\r\n"
XML_FILE+="        </Directory>\r\n"
XML_FILE+="        <Feature Id='Complete' Title='${PRODUCTNAME}' Description='Install all ${PRODUCTNAME} files.' Display='expand' Level='1' ConfigurableDirectory='INSTALLDIR'>\r\n"
XML_FILE+="            <Feature Id='${PRODUCTNAME}Complete' Title='${PRODUCTNAME}' Description='The complete package.' Level='1' AllowAdvertise='no' InstallDefault='local'>\r\n"
XML_FILE+="                <ComponentRef Id='${PRODUCTNAME}Files'/>\r\n"
XML_FILE+="${COMPONENTS}\r\n"
XML_FILE+="                <ComponentRef Id='ProgramMenuShortcut'/>\r\n"
XML_FILE+="            </Feature>\r\n"
XML_FILE+="        </Feature>\r\n"
XML_FILE+="    </Product>\r\n"
XML_FILE+="</Wix>"

echo -e $XML_FILE > $XMLOUTFILE
IFS=" "

#############################################################
# Print the XML for debugging                               #
#############################################################
cat $XMLOUTFILE

#############################################################
# Once the XML file manifest is created, create the package #
#############################################################
candle -arch x64 ${PRODUCTNAME,,}.wxs
light -sval -ext WixUIExtension ${PRODUCTNAME,,}.wixobj -out ${PRODUCTNAME}_Windows_64.msi
