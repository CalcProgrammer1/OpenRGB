#!/usr/bin/env bash
PRODUCTNAME="WinRing0"
VENDOR="GermanAizek"

TLD="com.github"
WEBSITE="https://github.com/GermanAizek/WinRing0"
NAMESPACE=$(uuidgen -n @url -N ${WEBSITE} --sha1 | awk '{ print toupper($0) }')
VENDOR_ID="${TLD}.${VENDOR,,}"
APP_ID="${VENDOR_ID,,}.${PRODUCTNAME,,}"
XMLOUTFILE=${PRODUCTNAME,,}".wxs"

#The Upgrade code has to be consistent to allow upgrades between channels
#This value is roughly equivalent to "provides" in Linux packaging
UPGRADECODE=$(uuidgen -n ${NAMESPACE} -N ${APP_ID} --sha1 | awk '{ print toupper($0) }')
#The ProductID will be unique per build
PRODUCTCOMMENT="WinRing0 is a hardware access library for Windows."
VERSION="1.0"

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
XML_FILE+="        <Property Id='WIXUI_INSTALLDIR' Value='INSTALLDIR'/>\r\n"
XML_FILE+="        <UIRef Id='WixUI_InstallDir'/>\r\n"
XML_FILE+="        <UIRef Id='WixUI_ErrorProgressText'/>\r\n"
XML_FILE+="        <CustomAction Id='ExecStartDriver' Directory='INSTALLDIR' Execute='deferred' Impersonate='no' ExeCommand='cmd.exe /c sc start WinRing0x64' Return='ignore' />\r\n"
XML_FILE+="        <CustomAction Id='ExecStopDriver' Directory='INSTALLDIR' Execute='deferred' Impersonate='no' ExeCommand='cmd.exe /c sc stop WinRing0x64' Return='ignore' />\r\n"
XML_FILE+="        <InstallExecuteSequence>\r\n"
XML_FILE+="            <Custom Action='ExecStartDriver' After='InstallFiles'>NOT Installed</Custom>\r\n"
XML_FILE+="            <Custom Action='ExecStopDriver' After='InstallInitialize'>Installed AND NOT REINSTALL</Custom>\r\n"
XML_FILE+="        </InstallExecuteSequence>\r\n"
XML_FILE+="        <Directory Id='TARGETDIR' Name='SourceDir'>\r\n"
XML_FILE+="            <Directory Id='WindowsDir' Name='Windows'>\r\n"
XML_FILE+="                <Directory Id='System32Dir' Name='System32'>\r\n"
XML_FILE+="                    <Directory Id='INSTALLDIR' Name='drivers'>\r\n"
XML_FILE+="                        <Component Id='${PRODUCTNAME}Files' Guid='"$(uuidgen -t | awk '{ print toupper($0) }')"'>\r\n"
XML_FILE+="                            <File Id='OpenRGB00' Source='dependencies/winring0/x64/WinRing0x64.sys'/>\r\n"
XML_FILE+="                        </Component>\r\n"
XML_FILE+="                    </Directory>\r\n"
XML_FILE+="                </Directory>\r\n"
XML_FILE+="            </Directory>\r\n"
XML_FILE+="        </Directory>\r\n"
XML_FILE+="        <Feature Id='Complete' Title='WinRing0' Description='Install all WinRing0 files.' Display='expand' Level='1' ConfigurableDirectory='INSTALLDIR'>\r\n"
XML_FILE+="            <Feature Id='WinRing0Complete' Title='WinRing0' Description='The complete package.' Level='1' AllowAdvertise='no' InstallDefault='local'>\r\n"
XML_FILE+="                <ComponentRef Id='WinRing0Files'/>\r\n"
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
light -sval -ext WixUIExtension ${PRODUCTNAME,,}.wixobj -out ${PRODUCTNAME}.msi
