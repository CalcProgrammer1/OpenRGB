# OpenRGB MSI Builder for Windows
# PowerShell conversion of build-msi.sh

# Add Wix to path
$env:Path += ";C:\Program Files (x86)\WiX Toolset v3.14\bin"

# Add Qt to path
$env:Path += ";C:\Qt\6.8.3\msvc2022_64\bin"

# Function to generate random UUID v4
function Generate-UUIDv4
{
    return [guid]::NewGuid().ToString().ToUpper()
}

# Function to generate namespace-based UUID (SHA1)
function Generate-NamespaceUUID
{
    param
    (
        [guid]$Namespace,
        [string]$Name
    )

    $nameBytes      = [System.Text.Encoding]::UTF8.GetBytes($Name)
    $combined       = $Namespace.ToByteArray() + $nameBytes
    $hash           = [System.Security.Cryptography.SHA1]::Create().ComputeHash($combined)

    # Set version to 5 (SHA1-based namespace UUID)
    $hash[6]        = ($hash[6] -band 0x0f) -bor 0x50
    $hash[8]        = ($hash[8] -band 0x3f) -bor 0x80

    $uuidBytes      = [byte[]]($hash[0..15])
    $uuid           = [guid]$uuidBytes

    return $uuid.ToString().ToUpper()
}

# Configuration
$PRODUCTNAME        = "OpenRGB"
$VENDOR             = $PRODUCTNAME

$TLD                = "org"
$WEBSITE            = "https://$($PRODUCTNAME.ToLower()).$TLD"

# URL namespace UUID (RFC 4122, version 4 namespace)
$URLNamespace       = [guid]'6ba7b810-9dad-11d1-80b4-00c04fd430c8'
$NAMESPACE          = Generate-NamespaceUUID $URLNamespace $WEBSITE

$VENDOR_ID          = "$TLD.$VENDOR"
$APP_ID             = "$VENDOR_ID.$($PRODUCTNAME.ToLower())"

$GITURL             = "https://gitlab.com/CalcProgrammer1/OpenRGB/-"
$GITPARAM           = "?inline=false"
$ICONFILE           = "qt/OpenRGB.ico"
$LICENSEFILE        = "scripts/License.rtf"
$BANNERIMAGE        = "scripts/banner.bmp"
$DIALOGBACKGROUND   = "scripts/dialog_background.bmp"
$PROJECT_FILE       = "OpenRGB.pro"
$XMLOUTFILE         = "$($PRODUCTNAME.ToLower()).wxs"

# Extract version from project file
$VERSION            = & qmake $PROJECT_FILE 2>&1 | Select-String 'VERSION_WIX' | Select-Object -First 1 | ForEach-Object {
    $_.Line -split ':' | Select-Object -Last 1 | ForEach-Object { $_.Trim() }
}

# Check for suffix to determine channel
$CHANNEL            = ""
$SUFFIX             = Select-String -Path $PROJECT_FILE -Pattern 'SUFFIX\s' | ForEach-Object {
    $_.Line -split '=' | Select-Object -Last 1 | ForEach-Object { $_.Trim() }
}

if($SUFFIX)
{
    $CHANNEL        = "-git"
}

# Calculate upgrade code
$UPGRADECODE        = Generate-NamespaceUUID ([guid]$NAMESPACE) $APP_ID

$PRODUCTCOMMENT     = "Open source RGB lighting control that doesn't depend on manufacturer software."

# Print Metadata to the log
Write-Host "Icon URL:`t" $GITURL$ICONFILE
Write-Host "License URL:`t" $GITURL$LICENSEFILE
Write-Host "AppID - Channel:`t" $APP_ID " - " $CHANNEL
Write-Host "Upgrade UUID:`t" $UPGRADECODE
Write-Host "Product Name:`t" $PRODUCTNAME
Write-Host "Vendor - VendorID:`t`t" $VENDOR " - " $VENDOR_ID
Write-Host "Version:`t" $VERSION

# Copy build directory to working path
$BUILT_PATH         = "OpenRGB Windows 64-bit\*"
$WORKING_PATH       = "OpenRGB Windows MSI Installer\"

if(Test-Path $BUILT_PATH)
{
    Copy-Item -Path $BUILT_PATH -Destination $WORKING_PATH -Recurse -Force
}

$count              = 1
$EXE_FILE           = ""
$SVC_FILE           = ""
$FILES              = ""
$COMPONENTS         = ""
$DIRECTORIES        = ""

# Build file list for files in the working path
Get-ChildItem -Path $WORKING_PATH -Force | ForEach-Object {
    $file           = $_
    $filename       = $_.Name

    if($filename -eq "$PRODUCTNAME.exe")
    {
        # If this is the executable, treat as special case
        $script:EXE_ID = "$($PRODUCTNAME)00"
        $script:SVC_ID = "$($PRODUCTNAME)00_SVC"
        $script:EXE_FILE = "                        <File Id='$($script:EXE_ID)' Source='$WORKING_PATH$filename' KeyPath='yes'/>`r`n"
        $script:SVC_FILE = "                        <File Id='$($script:SVC_ID)' Source='$WORKING_PATH$filename' KeyPath='yes'/>`r`n"
    }
    elseif($_.PSIsContainer)
    {
        # If this is a directory, add another component
        $script:COMPONENTS += "            <ComponentRef Id='$($filename)Files'/>`r`n"
        $guid = Generate-UUIDv4
        $TEMP = "                    <Directory Id='$filename' Name='$filename'>`r`n"
        $TEMP += "                        <Component Id='$($filename)Files' Guid='$guid'>`r`n"

        Get-ChildItem -Path $file.FullName -Force | ForEach-Object {
            $filename2 = $_.Name
            $TEMP += "                            <File Id='$($PRODUCTNAME)$($script:count)' Source='$WORKING_PATH$filename/$filename2'/>`r`n"
            $script:count++
        }

        $TEMP += "                        </Component>`r`n                    </Directory>`r`n"
        $script:DIRECTORIES += $TEMP
    }
    else
    {
        # Any other file to files list
        $script:FILES += "                        <File Id='$($PRODUCTNAME)$($script:count)' Source='$WORKING_PATH$filename'/>`r`n"
        $script:count++
    }
}

# Create the Wix XML file
$XML_FILE = ""

$XML_FILE += "<?xml version='1.0' encoding='windows-1252'?>`r`n"
$XML_FILE += "<Wix xmlns='http://schemas.microsoft.com/wix/2006/wi'>`r`n"
$XML_FILE += "    <Product Name='$PRODUCTNAME' Manufacturer='$VENDOR'`r`n"
$XML_FILE += "        Id='*'`r`n"
$XML_FILE += "        UpgradeCode='$UPGRADECODE'`r`n"
$XML_FILE += "        Language='1033' Codepage='1252' Version='$VERSION'>`r`n"
$XML_FILE += "        <Package Keywords='Installer' Description='$PRODUCTNAME Installer'`r`n"
$XML_FILE += "            Comments=`"$PRODUCTCOMMENT`" Manufacturer='OpenRGB'`r`n"
$XML_FILE += "            InstallerVersion='200' Languages='1033' Compressed='yes' SummaryCodepage='1252' Platform='x64'/>`r`n"
$XML_FILE += "        <Media Id='1' Cabinet='$($PRODUCTNAME.ToLower()).cab' EmbedCab='yes'/>`r`n"
$XML_FILE += "        <Condition Message='This package supports Windows 64bit Only'>VersionNT64</Condition>`r`n"
$XML_FILE += "        <MajorUpgrade Schedule='afterInstallInitialize' AllowDowngrades='yes'/>`r`n"
$XML_FILE += "        <Icon Id='OpenRGBIcon' SourceFile='$ICONFILE'/>`r`n"
$XML_FILE += "        <Property Id='ARPPRODUCTICON' Value='OpenRGBIcon'/>`r`n"
$XML_FILE += "        <Property Id='ARPURLINFOABOUT' Value='https://www.openrgb.org'/>`r`n"
$XML_FILE += "        <Property Id='WIXUI_INSTALLDIR' Value='INSTALLDIR'/>`r`n"
$XML_FILE += "        <WixVariable Id='WixUILicenseRtf' Value='$LICENSEFILE'/>`r`n"
$XML_FILE += "        <WixVariable Id='WixUIBannerBmp' Value='$BANNERIMAGE'/>`r`n"
$XML_FILE += "        <WixVariable Id='WixUIDialogBmp' Value='$DIALOGBACKGROUND'/>`r`n"
$XML_FILE += "`r`n"
$XML_FILE += "        <UI>`r`n"
$XML_FILE += "            <UIRef Id='WixUI_InstallDir'/>`r`n"
$XML_FILE += "            <UIRef Id='WixUI_ErrorProgressText'/>`r`n"
$XML_FILE += "`r`n"
$XML_FILE += "            <Dialog Id='PawnIODialog' Width='370' Height='270' Title='!(loc.InstallDirDlg_Title)'>`r`n"
$XML_FILE += "                <Control Id='Next' Type='PushButton' X='236' Y='243' Width='56' Height='17' Default='yes' Text='!(loc.WixUINext)' />`r`n"
$XML_FILE += "                <Control Id='Back' Type='PushButton' X='180' Y='243' Width='56' Height='17' Text='!(loc.WixUIBack)' />`r`n"
$XML_FILE += "                <Control Id='Cancel' Type='PushButton' X='304' Y='243' Width='56' Height='17' Cancel='yes' Text='!(loc.WixUICancel)'>`r`n"
$XML_FILE += "                    <Publish Event='SpawnDialog' Value='CancelDlg' />`r`n"
$XML_FILE += "                </Control>`r`n"
$XML_FILE += "                <Control Id='Description' Type='Text' X='25' Y='23' Width='280' Height='15' Transparent='yes' NoPrefix='yes' Text='Install PawnIO for RGB RAM and Motherboard control.' />`r`n"
$XML_FILE += "                <Control Id='Title' Type='Text' X='15' Y='6' Width='200' Height='15' Transparent='yes' NoPrefix='yes' Text='PawnIO' />`r`n"
$XML_FILE += "                <Control Id='BannerBitmap' Type='Bitmap' X='0' Y='0' Width='370' Height='44' TabSkip='no' Text='!(loc.InstallDirDlgBannerBitmap)' />`r`n"
$XML_FILE += "                <Control Id='BannerLine' Type='Line' X='0' Y='44' Width='373' Height='0' />`r`n"
$XML_FILE += "                <Control Id='BottomLine' Type='Line' X='0' Y='234' Width='373' Height='0' />`r`n"
$XML_FILE += "                <Control Id='MainText' Type='Text' X='40' Y='60' Width='300' Height='40' Text='OpenRGB uses the PawnIO driver to control RGB lighting on DRAM modules and some older motherboards.  For these devices to work, you must manually install PawnIO by visiting the link below.' />`r`n"
$XML_FILE += "                <Control Id='SecondText' Type='Text' X='40' Y='100' Width='300' Height='40' Text='If you do not need support for these devices, you can safely skip this step and continue with the installation.' />`r`n"
$XML_FILE += "                <Control Id='MainLink' Type='Hyperlink' X='40' Y='140' Width='300' Height='20' Text='&lt;a href=`"https://pawnio.eu`"&gt;https://pawnio.eu&lt;/a&gt;' />`r`n"
$XML_FILE += "                <Control Id='ThirdText' Type='Text' X='40' Y='180' Width='300' Height='40' Text='With PawnIO installed, OpenRGB must be run as administrator to access these devices.' />`r`n"
$XML_FILE += "            </Dialog>`r`n"
$XML_FILE += "`r`n"
$XML_FILE += "            <Dialog Id='ServiceDialog' Width='370' Height='270' Title='!(loc.InstallDirDlg_Title)'>`r`n"
$XML_FILE += "                <Control Id='Next' Type='PushButton' X='236' Y='243' Width='56' Height='17' Default='yes' Text='!(loc.WixUINext)' />`r`n"
$XML_FILE += "                <Control Id='Back' Type='PushButton' X='180' Y='243' Width='56' Height='17' Text='!(loc.WixUIBack)' />`r`n"
$XML_FILE += "                <Control Id='Cancel' Type='PushButton' X='304' Y='243' Width='56' Height='17' Cancel='yes' Text='!(loc.WixUICancel)'>`r`n"
$XML_FILE += "                    <Publish Event='SpawnDialog' Value='CancelDlg' />`r`n"
$XML_FILE += "                </Control>`r`n"
$XML_FILE += "`r`n"
$XML_FILE += "                <Control Id='Description' Type='Text' X='25' Y='23' Width='280' Height='15' Transparent='yes' NoPrefix='yes' Text='Choose whether to register $PRODUCTNAME as a system service.' />`r`n"
$XML_FILE += "                <Control Id='Title' Type='Text' X='15' Y='6' Width='200' Height='15' Transparent='yes' NoPrefix='yes' Text='$PRODUCTNAME Service' />`r`n"
$XML_FILE += "                <Control Id='BannerBitmap' Type='Bitmap' X='0' Y='0' Width='370' Height='44' TabSkip='no' Text='!(loc.InstallDirDlgBannerBitmap)' />`r`n"
$XML_FILE += "                <Control Id='BannerLine' Type='Line' X='0' Y='44' Width='373' Height='0' />`r`n"
$XML_FILE += "                <Control Id='BottomLine' Type='Line' X='0' Y='234' Width='373' Height='0' />`r`n"
$XML_FILE += "`r`n"
$XML_FILE += "                <Control Id='TypicalText' Type='Text' X='40' Y='60' Width='300' Height='40' Text='Installing OpenRGB without registering as a system service means nothing runs automatically in the background by default.  Accessing devices through PawnIO will require running OpenRGB as administrator.' />`r`n"
$XML_FILE += "                <Control Id='TypicalButton' Type='PushButton' X='40' Y='100' Width='200' Height='17' ToolTip='Does not register OpenRGB as a system service.  Accessing certain devices will require running the application as administrator.' Default='yes' Text='Do not Install System Service (Default)'>`r`n"
$XML_FILE += "                    <Publish Property='WixUI_InstallMode' Value='InstallTypical'>1</Publish>`r`n"
$XML_FILE += "                    <Publish Event='SetInstallLevel' Value='1'>1</Publish>`r`n"
$XML_FILE += "                    <Publish Event='NewDialog' Value='VerifyReadyDlg'>1</Publish>`r`n"
$XML_FILE += "                </Control>`r`n"
$XML_FILE += "                <Control Id='CompleteText' Type='Text' X='40' Y='120' Width='300' Height='40' Text='Installing OpenRGB as a system service means the application will run automatically in the background.  Accessing devices through PawnIO will not require running OpenRGB as administrator.' />`r`n"
$XML_FILE += "                <Control Id='CompleteWarning' Type='Text' X='40' Y='160' Width='300' Height='40' Text='Warning: Installing OpenRGB as a system service is experimental in the current release.  Settings do not synchronize between the user interface and the service.  If you encounter issues with the service, you can uninstall it and use the non-service installation instead.' />`r`n"
$XML_FILE += "                <Control Id='CompleteButton' Type='PushButton' X='40' Y='210' Width='200' Height='17' ToolTip='Registers OpenRGB system service, allowing access to devices which require administrator access without having to run the application as administrator.' Text='Install System Service'>`r`n"
$XML_FILE += "                    <Publish Property='WixUI_InstallMode' Value='InstallComplete'>1</Publish>`r`n"
$XML_FILE += "                    <Publish Event='SetInstallLevel' Value='2'>2</Publish>`r`n"
$XML_FILE += "                    <Publish Event='NewDialog' Value='VerifyReadyDlg'>1</Publish>`r`n"
$XML_FILE += "                </Control>`r`n"
$XML_FILE += "            </Dialog>`r`n"
$XML_FILE += "`r`n"
$XML_FILE += "            <Publish Dialog='InstallDirDlg' Control='Next' Event='NewDialog' Value='PawnIODialog' Order='5'>WIXUI_DONTVALIDATEPATH OR WIXUI_INSTALLDIR_VALID=`"1`"</Publish>`r`n"
$XML_FILE += "            <Publish Dialog='VerifyReadyDlg' Control='Back' Event='NewDialog' Value='ServiceDialog' Order='2'>1</Publish>`r`n"
$XML_FILE += "            <Publish Dialog='PawnIODialog' Control='Next' Event='NewDialog' Value='ServiceDialog' Order='2'>1</Publish>`r`n"
$XML_FILE += "            <Publish Dialog='PawnIODialog' Control='Back' Event='NewDialog' Value='InstallDirDlg' Order='2'>1</Publish>`r`n"
$XML_FILE += "            <Publish Dialog='ServiceDialog' Control='Next' Event='NewDialog' Value='VerifyReadyDlg' Order='2'>1</Publish>`r`n"
$XML_FILE += "            <Publish Dialog='ServiceDialog' Control='Back' Event='NewDialog' Value='PawnIODialog' Order='2'>1</Publish>`r`n"
$XML_FILE += "        </UI>`r`n"
$XML_FILE += "`r`n"
$XML_FILE += "        <Directory Id='TARGETDIR' Name='SourceDir'>`r`n"
$XML_FILE += "            <Directory Id='ProgramFiles64Folder'>`r`n"
$XML_FILE += "                <Directory Id='INSTALLDIR' Name='$PRODUCTNAME'>`r`n"

$guid1 = Generate-UUIDv4
$XML_FILE += "                    <Component Id='$($PRODUCTNAME)Files' Guid='$guid1'>`r`n"
$XML_FILE += $EXE_FILE
$XML_FILE += $FILES
$XML_FILE += "                    </Component>`r`n"

$guid2 = Generate-UUIDv4
$XML_FILE += "                    <Component Id='$($PRODUCTNAME)Service' Guid='$guid2'>`r`n"
$XML_FILE += $SVC_FILE
$XML_FILE += "                        <ServiceInstall Id='ServiceInstaller' Type='ownProcess' Name='OpenRGB' DisplayName='OpenRGB' Description='OpenRGB SDK Server' Start='auto' Account='[SERVICEACCOUNT]' Password='[SERVICEPASSWORD]' ErrorControl='normal'/>`r`n"
$XML_FILE += "                        <ServiceControl Id='StartService' Start='install' Stop='both' Remove='uninstall' Name='OpenRGB' Wait='yes'/>`r`n"
$XML_FILE += "                    </Component>`r`n"
$XML_FILE += $DIRECTORIES
$XML_FILE += "                </Directory>`r`n"
$XML_FILE += "            </Directory>`r`n"
$XML_FILE += "            <Directory Id='ProgramMenuFolder' Name='Programs'>`r`n"

$guid3 = Generate-UUIDv4
$XML_FILE += "                <Component Id='ProgramMenuShortcut' Guid='$guid3'>`r`n"
$XML_FILE += "                    <Shortcut Id='startmenu$PRODUCTNAME' Directory='ProgramMenuFolder' Name='$PRODUCTNAME' Target='[#$EXE_ID]' WorkingDirectory='INSTALLDIR' Advertise='no'/>`r`n"
$XML_FILE += "                    <RemoveFile Id='ProgramMenuShortcut' Name='$PRODUCTNAME.lnk' Directory='ProgramMenuFolder' On='uninstall'/>`r`n"
$XML_FILE += "                    <RegistryValue Root='HKCU' Key='Software\[Manufacturer]\[ProductName]' Type='string' Value='' KeyPath='yes'/>`r`n"
$XML_FILE += "                </Component>`r`n"
$XML_FILE += "            </Directory>`r`n"
$XML_FILE += "        </Directory>`r`n"
$XML_FILE += "        <Feature Id='$($PRODUCTNAME)Application' Title='$PRODUCTNAME' Description='Install the $PRODUCTNAME application files.' Level='1' AllowAdvertise='no' ConfigurableDirectory='INSTALLDIR' InstallDefault='local'>`r`n"
$XML_FILE += "            <ComponentRef Id='$($PRODUCTNAME)Files'/>`r`n"
$XML_FILE += $COMPONENTS
$XML_FILE += "            <ComponentRef Id='ProgramMenuShortcut'/>`r`n"
$XML_FILE += "        </Feature>`r`n"
$XML_FILE += "        <Feature Id='$($PRODUCTNAME)RegisterService' Title='$PRODUCTNAME' Description='Register $PRODUCTNAME as a system service.' Display='expand' Level='2' ConfigurableDirectory='INSTALLDIR'>`r`n"
$XML_FILE += "            <ComponentRef Id='$($PRODUCTNAME)Service'/>`r`n"
$XML_FILE += "        </Feature>`r`n"
$XML_FILE += "    </Product>`r`n"
$XML_FILE += "</Wix>"

# Write XML file
$XML_FILE | Out-File -FilePath $XMLOUTFILE -Encoding UTF8

# Print the XML for debugging
Write-Host (Get-Content $XMLOUTFILE)

# Build the MSI package
Write-Host "`nBuilding MSI package..."

$wixObjFile         = "$($PRODUCTNAME.ToLower()).wixobj"
$msiFile            = "$($PRODUCTNAME)_Windows_64.msi"

& candle -arch x64 $XMLOUTFILE
& light -sval -ext WixUIExtension $wixObjFile -out $msiFile
