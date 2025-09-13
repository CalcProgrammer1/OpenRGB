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
        SVC_ID=${PRODUCTNAME}00_SVC
        #Add special entry to files list
        EXE_FILE="                        <File Id='${EXE_ID}' Source='${WORKING_PATH}${filename}' KeyPath='yes'/>\n"
        SVC_FILE="                        <File Id='${SVC_ID}' Source='${WORKING_PATH}${filename}' KeyPath='yes'/>\n"
    elif [ -d "$file" ] ; then
            #If this is a directory then we need to add another component
            COMPONENTS="${COMPONENTS}            <ComponentRef Id='${filename}Files'/>\n"
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
XML_FILE+="        <Icon Id='OpenRGBIcon' SourceFile='${ICONFILE}'/>\r\n"
XML_FILE+="        <Property Id='ARPPRODUCTICON' Value='OpenRGBIcon'/>\r\n"
XML_FILE+="        <Property Id='ARPURLINFOABOUT' Value='https://www.openrgb.org'/>\r\n"
XML_FILE+="        <Property Id='WIXUI_INSTALLDIR' Value='INSTALLDIR'/>\r\n"
XML_FILE+="        <WixVariable Id='WixUILicenseRtf' Value='${LICENSEFILE}'/>\r\n"
XML_FILE+="        <WixVariable Id='WixUIBannerBmp' Value='${BANNERIMAGE}'/>\r\n"
XML_FILE+="        <WixVariable Id='WixUIDialogBmp' Value='${DIALOGBACKGROUND}'/>\r\n"
XML_FILE+="\r\n"
XML_FILE+="        <UI>\r\n"
XML_FILE+="            <UIRef Id='WixUI_InstallDir'/>\r\n"
XML_FILE+="            <UIRef Id='WixUI_ErrorProgressText'/>\r\n"
XML_FILE+="\r\n"
XML_FILE+="            <Dialog Id='ServiceDialog' Width='370' Height='270' Title='!(loc.InstallDirDlg_Title)'>\r\n"
XML_FILE+="                <Control Id='Next' Type='PushButton' X='236' Y='243' Width='56' Height='17' Default='yes' Text='!(loc.WixUINext)' />\r\n"
XML_FILE+="                <Control Id='Back' Type='PushButton' X='180' Y='243' Width='56' Height='17' Text='!(loc.WixUIBack)' />\r\n"
XML_FILE+="                <Control Id='Cancel' Type='PushButton' X='304' Y='243' Width='56' Height='17' Cancel='yes' Text='!(loc.WixUICancel)'>\r\n"
XML_FILE+="                    <Publish Event='SpawnDialog' Value='CancelDlg' />\r\n"
XML_FILE+="                </Control>\r\n"
XML_FILE+="\r\n"
XML_FILE+="                <Control Id='Description' Type='Text' X='25' Y='23' Width='280' Height='15' Transparent='yes' NoPrefix='yes' Text='Choose whether to register ${PRODUCTNAME} as a system service.' />\r\n"
XML_FILE+="                <Control Id='Title' Type='Text' X='15' Y='6' Width='200' Height='15' Transparent='yes' NoPrefix='yes' Text='${PRODUCTNAME} Service' />\r\n"
XML_FILE+="                <Control Id='BannerBitmap' Type='Bitmap' X='0' Y='0' Width='370' Height='44' TabSkip='no' Text='!(loc.InstallDirDlgBannerBitmap)' />\r\n"
XML_FILE+="                <Control Id='BannerLine' Type='Line' X='0' Y='44' Width='373' Height='0' />\r\n"
XML_FILE+="                <Control Id='BottomLine' Type='Line' X='0' Y='234' Width='373' Height='0' />\r\n"
XML_FILE+="\r\n"
XML_FILE+="                <Control Id='TypicalButton' Type='PushButton' X='40' Y='65' Width='200' Height='17' ToolTip='Does not register OpenRGB as a system service.  Accessing certain devices will require running the application as administrator.' Default='yes' Text='Do not Install System Service (Default)'>\r\n"
XML_FILE+="                    <Publish Property='WixUI_InstallMode' Value='InstallTypical'>1</Publish>\r\n"
XML_FILE+="                    <Publish Event='SetInstallLevel' Value='1'>1</Publish>\r\n"
XML_FILE+="                    <Publish Event='NewDialog' Value='VerifyReadyDlg'>1</Publish>\r\n"
XML_FILE+="                </Control>\r\n"
XML_FILE+="                <Control Id='CompleteButton' Type='PushButton' X='40' Y='171' Width='200' Height='17' ToolTip='Registers OpenRGB system service, allowing access to devices which require administrator access without having to run the application as administrator.' Text='Install System Service'>\r\n"
XML_FILE+="                    <Publish Property='WixUI_InstallMode' Value='InstallComplete'>1</Publish>\r\n"
XML_FILE+="                    <Publish Event='SetInstallLevel' Value='2'>1</Publish>\r\n"
XML_FILE+="                    <Publish Event='NewDialog' Value='VerifyReadyDlg'>1</Publish>\r\n"
XML_FILE+="                </Control>\r\n"
XML_FILE+="            </Dialog>\r\n"
XML_FILE+="\r\n"
XML_FILE+="            <Publish Dialog='InstallDirDlg' Control='Next' Event='NewDialog' Value='ServiceDialog' Order='5'>WIXUI_DONTVALIDATEPATH OR WIXUI_INSTALLDIR_VALID=\"1\"</Publish>\r\n"
XML_FILE+="            <Publish Dialog='VerifyReadyDlg' Control='Back' Event='NewDialog' Value='ServiceDialog' Order='2'>1</Publish>\r\n"
XML_FILE+="            <Publish Dialog='ServiceDialog' Control='Next' Event='NewDialog' Value='VerifyReadyDlg' Order='2'>1</Publish>\r\n"
XML_FILE+="            <Publish Dialog='ServiceDialog' Control='Back' Event='NewDialog' Value='InstallDirDlg' Order='2'>1</Publish>\r\n"
XML_FILE+="        </UI>\r\n"
XML_FILE+="\r\n"
XML_FILE+="        <Directory Id='TARGETDIR' Name='SourceDir'>\r\n"
XML_FILE+="            <Directory Id='ProgramFiles64Folder'>\r\n"
XML_FILE+="                <Directory Id='INSTALLDIR' Name='${PRODUCTNAME}'>\r\n"
XML_FILE+="                    <Component Id='${PRODUCTNAME}Files' Guid='"$(uuidgen -t | awk '{ print toupper($0) }')"'>\r\n"
XML_FILE+="${EXE_FILE}\r\n"
XML_FILE+="${FILES}\r\n"
XML_FILE+="                    </Component>\r\n"
XML_FILE+="                    <Component Id='${PRODUCTNAME}Service' Guid='"$(uuidgen -t | awk '{ print toupper($0) }')"'>\r\n"
XML_FILE+="${SVC_FILE}\r\n"
XML_FILE+="                        <ServiceInstall Id='ServiceInstaller' Type='ownProcess' Name='OpenRGB' DisplayName='OpenRGB' Description='OpenRGB SDK Server' Start='auto' Account='[SERVICEACCOUNT]' Password='[SERVICEPASSWORD]' ErrorControl='normal'/>\r\n"
XML_FILE+="                        <ServiceControl Id='StartService' Start='install' Stop='both' Remove='uninstall' Name='OpenRGB' Wait='yes'/>\r\n"
XML_FILE+="                    </Component>\r\n"
XML_FILE+="${DIRECTORIES}\r\n"
XML_FILE+="                </Directory>\r\n"
XML_FILE+="            </Directory>\r\n"
XML_FILE+="            <Directory Id='ProgramMenuFolder' Name='Programs'>\r\n"
XML_FILE+="                <Component Id='ProgramMenuShortcut' Guid='"$(uuidgen -t | awk '{ print toupper($0) }')"'>\r\n"
XML_FILE+="                    <Shortcut Id='startmenu${PRODUCTNAME}' Directory='ProgramMenuFolder' Name='${PRODUCTNAME}' Target='[#${EXE_ID}]' WorkingDirectory='INSTALLDIR' Advertise='no'/>\r\n"
XML_FILE+="                    <RemoveFile Id='ProgramMenuShortcut' Name='${PRODUCTNAME}.lnk' Directory='ProgramMenuFolder' On='uninstall'/>\r\n"
XML_FILE+="                    <RegistryValue Root='HKCU' Key='Software\[Manufacturer]\[ProductName]' Type='string' Value='' KeyPath='yes'/>\r\n"
XML_FILE+="                </Component>\r\n"
XML_FILE+="            </Directory>\r\n"
XML_FILE+="        </Directory>\r\n"
XML_FILE+="        <Feature Id='${PRODUCTNAME}Application' Title='${PRODUCTNAME}' Description='Install the ${PRODUCTNAME} application files.' Level='1' AllowAdvertise='no' ConfigurableDirectory='INSTALLDIR' InstallDefault='local'>\r\n"
XML_FILE+="            <ComponentRef Id='${PRODUCTNAME}Files'/>\r\n"
XML_FILE+="${COMPONENTS}\r\n"
XML_FILE+="            <ComponentRef Id='ProgramMenuShortcut'/>\r\n"
XML_FILE+="        </Feature>\r\n"
XML_FILE+="        <Feature Id='${PRODUCTNAME}RegisterService' Title='${PRODUCTNAME}' Description='Register ${PRODUCTNAME} as a system service.' Display='expand' Level='2' ConfigurableDirectory='INSTALLDIR'>\r\n"
XML_FILE+="            <ComponentRef Id='${PRODUCTNAME}Service'/>\r\n"
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
