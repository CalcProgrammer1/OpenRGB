#-----------------------------------------------------------------------------------------------#
# OpenRGB 0.x QMake Project                                                                     #
#                                                                                               #
#   Adam Honse (CalcProgrammer1)                        5/25/2020                               #
#-----------------------------------------------------------------------------------------------#

#-----------------------------------------------------------------------------------------------#
# Qt Configuration                                                                              #
#-----------------------------------------------------------------------------------------------#
QT +=                                                                                           \
    core                                                                                        \
    gui                                                                                         \

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#-----------------------------------------------------------------------------------------------#
# Application Configuration                                                                     #
#-----------------------------------------------------------------------------------------------#
VERSION     = 0.41
TARGET      = OpenRGB
TEMPLATE    = app

#-----------------------------------------------------------------------------------------------#
# Automatically generated build information                                                     #
#-----------------------------------------------------------------------------------------------#
win32:BUILDDATE = $$system(date /t)
unix:BUILDDATE  = $$system(date -R -d "@${SOURCE_DATE_EPOCH:-$(date +%s)}")
GIT_COMMIT_ID   = $$system(git --git-dir $$_PRO_FILE_PWD_/.git --work-tree $$_PRO_FILE_PWD_ rev-parse HEAD)
GIT_COMMIT_DATE = $$system(git --git-dir $$_PRO_FILE_PWD_/.git --work-tree $$_PRO_FILE_PWD_ show -s --format=%ci HEAD)
GIT_BRANCH      = $$system(git --git-dir $$_PRO_FILE_PWD_/.git --work-tree $$_PRO_FILE_PWD_ rev-parse --abbrev-ref HEAD)

DEFINES +=                                                                                      \
    VERSION_STRING=\\"\"\"$$VERSION\\"\"\"                                                      \
    BUILDDATE_STRING=\\"\"\"$$BUILDDATE\\"\"\"                                                  \
    GIT_COMMIT_ID=\\"\"\"$$GIT_COMMIT_ID\\"\"\"                                                 \
    GIT_COMMIT_DATE=\\"\"\"$$GIT_COMMIT_DATE\\"\"\"                                             \
    GIT_BRANCH=\\"\"\"$$GIT_BRANCH\\"\"\"

#-----------------------------------------------------------------------------------------------#
# OpenRGB Common                                                                                #
#-----------------------------------------------------------------------------------------------#
INCLUDEPATH +=                                                                                  \
    dependencies/ColorWheel                                                                     \
    dependencies/CRCpp/                                                                         \
    dependencies/hueplusplus/include                                                            \
    dependencies/hueplusplus/include/hueplusplus                                                \
    dependencies/hueplusplus/include/json                                                       \
    dependencies/libe131/src/                                                                   \
    dependencies/mbedtls/include                                                                \
    i2c_smbus/                                                                                  \
    i2c_tools/                                                                                  \
    net_port/                                                                                   \
    pci_ids/                                                                                    \
    serial_port/                                                                                \
    super_io/                                                                                   \
    Controllers/AorusCPUCoolerController/                                                       \
    Controllers/AMDWraithPrismController/                                                       \
    Controllers/AuraUSBController/                                                              \
    Controllers/AuraCoreController/                                                             \
    Controllers/AuraGPUController/                                                              \
    Controllers/AuraSMBusController/                                                            \
    Controllers/CoolerMasterController/                                                         \
    Controllers/CorsairDominatorPlatinumController/                                             \
    Controllers/CorsairHydroController/                                                         \
    Controllers/CorsairPeripheralController/                                                    \
    Controllers/CorsairLightingNodeController/                                                  \
    Controllers/CorsairVengeanceController/                                                     \
    Controllers/CorsairVengeanceProController/                                                  \
    Controllers/CrucialController/                                                              \
    Controllers/DebugController/                                                                \
    Controllers/DuckyKeyboardController/                                                        \
    Controllers/E131Controller/                                                                 \
    Controllers/EKController/                                                                   \
    Controllers/EspurnaController/                                                              \
    Controllers/EVGAGPUController/                                                              \
    Controllers/GalaxGPUController/                                                             \
    Controllers/HoltekController/                                                               \
    Controllers/HuePlusController/                                                              \
    Controllers/HyperXDRAMController/                                                           \
    Controllers/HyperXKeyboardController/                                                       \
    Controllers/HyperXMouseController/                                                          \
    Controllers/HyperXMousematController/                                                       \
    Controllers/LEDStripController/                                                             \
    Controllers/LogitechController/                                                             \
    Controllers/MSI3ZoneController/                                                             \
    Controllers/MSIGPUController/                                                               \
    Controllers/MSIMysticLightController/                                                       \
    Controllers/MSIRGBController/                                                               \
    Controllers/NZXTHue2Controller/                                                             \
    Controllers/NZXTKrakenController/                                                           \
    Controllers/OpenRazerController/                                                            \
    Controllers/PatriotViperController/                                                         \
    Controllers/PhilipsHueController/                                                           \
    Controllers/PolychromeController/                                                           \
    Controllers/PoseidonZRGBController/                                                         \
    Controllers/RedragonController/                                                             \
    Controllers/RGBFusionController/                                                            \
    Controllers/RGBFusion2DRAMController/                                                       \
    Controllers/RGBFusion2SMBusController/                                                      \
    Controllers/RGBFusion2USBController/                                                        \
    Controllers/RGBFusionGPUController/                                                         \
    Controllers/SapphireGPUController/                                                          \
    Controllers/SinowealthController/                                                           \
    Controllers/SonyDS4Controller/                                                              \
    Controllers/SteelSeriesController/                                                          \
    Controllers/TecknetController/                                                              \
    Controllers/ThermaltakeRiingController/                                                     \
    RGBController/                                                                              \
    qt/

HEADERS +=                                                                                      \
    dependencies/ColorWheel/ColorWheel.h                                                        \
    NetworkClient.h                                                                             \
    NetworkProtocol.h                                                                           \
    NetworkServer.h                                                                             \
    ProfileManager.h                                                                            \
    ResourceManager.h                                                                           \
    Detector.h                                                                                  \
    DeviceDetector.h                                                                            \
    qt/OpenRGBClientInfoPage.h                                                                  \
    qt/OpenRGBDeviceInfoPage.h                                                                  \
    qt/OpenRGBDevicePage.h                                                                      \
    qt/OpenRGBDialog.h                                                                          \
    i2c_smbus/i2c_smbus.h                                                                       \
    i2c_tools/i2c_tools.h                                                                       \
    net_port/net_port.h                                                                         \
    pci_ids/pci_ids.h                                                                           \
    qt/DeviceView.h                                                                             \
    qt/OpenRGBDialog2.h                                                                         \
    qt/OpenRGBProfileSaveDialog.h                                                               \
    qt/OpenRGBServerInfoPage.h                                                                  \
    qt/OpenRGBSoftwareInfoPage.h                                                                \
    qt/OpenRGBSystemInfoPage.h                                                                  \
    qt/OpenRGBZoneResizeDialog.h                                                                \
    serial_port/find_usb_serial_port.h                                                          \
    serial_port/serial_port.h                                                                   \
    super_io/super_io.h                                                                         \
    Controllers/AMDWraithPrismController/AMDWraithPrismController.h                             \
    Controllers/AMDWraithPrismController/RGBController_AMDWraithPrism.h                         \
    Controllers/AorusCPUCoolerController/ATC800Controller.h                                     \
    Controllers/AorusCPUCoolerController/RGBController_AorusATC800.h                            \
    Controllers/AuraUSBController/AuraUSBController.h                                           \
    Controllers/AuraUSBController/AuraAddressableController.h                                   \
    Controllers/AuraUSBController/AuraMainboardController.h                                     \
    Controllers/AuraUSBController/AuraMouseController.h                                         \
    Controllers/AuraUSBController/RGBController_AuraMouse.h                                     \
    Controllers/AuraUSBController/RGBController_AuraUSB.h                                       \
    Controllers/AuraCoreController/AuraCoreController.h                                         \
    Controllers/AuraCoreController/RGBController_AuraCore.h                                     \
    Controllers/AuraGPUController/AuraGPUController.h                                           \
    Controllers/AuraGPUController/RGBController_AuraGPU.h                                       \
    Controllers/AuraSMBusController/AuraSMBusController.h                                       \
    Controllers/AuraSMBusController/RGBController_AuraSMBus.h                                   \
    Controllers/CoolerMasterController/CMMP750Controller.h                                      \
    Controllers/CoolerMasterController/RGBController_CMMP750Controller.h                        \
    Controllers/CorsairDominatorPlatinumController/CorsairDominatorPlatinumController.h         \
    Controllers/CorsairDominatorPlatinumController/RGBController_CorsairDominatorPlatinum.h     \
    Controllers/CorsairHydroController/CorsairHydroController.h                                 \
    Controllers/CorsairHydroController/RGBController_CorsairHydro.h                             \
    Controllers/CorsairLightingNodeController/CorsairLightingNodeController.h                   \
    Controllers/CorsairLightingNodeController/RGBController_CorsairLightingNode.h               \
    Controllers/CorsairPeripheralController/CorsairPeripheralController.h                       \
    Controllers/CorsairPeripheralController/RGBController_CorsairPeripheral.h                   \
    Controllers/CorsairVengeanceController/CorsairVengeanceController.h                         \
    Controllers/CorsairVengeanceController/RGBController_CorsairVengeance.h                     \
    Controllers/CorsairVengeanceProController/CorsairVengeanceProController.h                   \
    Controllers/CorsairVengeanceProController/RGBController_CorsairVengeancePro.h               \
    Controllers/CrucialController/CrucialController.h                                           \
    Controllers/CrucialController/RGBController_Crucial.h                                       \
    Controllers/DuckyKeyboardController/DuckyKeyboardController.h                               \
    Controllers/DuckyKeyboardController/RGBController_DuckyKeyboard.h                           \
    Controllers/E131Controller/RGBController_E131.h                                             \
    Controllers/EKController/EKController.h                                                     \
    Controllers/EKController/RGBController_EKController.h                                       \
    Controllers/EspurnaController/EspurnaController.h                                           \
    Controllers/EspurnaController/RGBController_Espurna.h                                       \
    Controllers/EVGAGPUController/EVGAGPUv1Controller.h                                         \
    Controllers/EVGAGPUController/EVGAGPUv2Controller.h                                         \
    Controllers/EVGAGPUController/RGBController_EVGAGPUv1.h                                     \
    Controllers/EVGAGPUController/RGBController_EVGAGPUv2.h                                     \
    Controllers/GalaxGPUController/GalaxGPUController.h                                         \
    Controllers/GalaxGPUController/RGBController_GalaxGPU.h                                     \
    Controllers/HoltekController/HoltekA070Controller.h                                         \
    Controllers/HoltekController/RGBController_HoltekA070.h                                     \
    Controllers/HuePlusController/HuePlusController.h                                           \
    Controllers/HuePlusController/RGBController_HuePlus.h                                       \
    Controllers/HyperXDRAMController/HyperXDRAMController.h                                     \
    Controllers/HyperXDRAMController/RGBController_HyperXDRAM.h                                 \
    Controllers/HyperXKeyboardController/HyperXAlloyOriginsController.h                         \
    Controllers/HyperXKeyboardController/HyperXKeyboardController.h                             \
    Controllers/HyperXKeyboardController/RGBController_HyperXAlloyOrigins.h                     \
    Controllers/HyperXKeyboardController/RGBController_HyperXKeyboard.h                         \
    Controllers/HyperXMouseController/HyperXPulsefireSurgeController.h                          \
    Controllers/HyperXMouseController/RGBController_HyperXPulsefireSurge.h                      \
    Controllers/HyperXMousematController/HyperXMousematController.h                             \
    Controllers/HyperXMousematController/RGBController_HyperXMousemat.h                         \
    Controllers/LEDStripController/LEDStripController.h                                         \
    Controllers/LEDStripController/RGBController_LEDStrip.h                                     \
    Controllers/LogitechController/LogitechG203Controller.h                                     \
    Controllers/LogitechController/LogitechG203LController.h                                    \
    Controllers/LogitechController/LogitechG403Controller.h                                     \
    Controllers/LogitechController/LogitechG502PSController.h                                   \
    Controllers/LogitechController/LogitechG810Controller.h                                     \
    Controllers/LogitechController/LogitechG213Controller.h                                     \
    Controllers/LogitechController/LogitechGPowerPlayController.h                               \
    Controllers/LogitechController/LogitechGProWirelessController.h                             \
    Controllers/LogitechController/RGBController_LogitechG203.h                                 \
    Controllers/LogitechController/RGBController_LogitechG203L.h                                \
    Controllers/LogitechController/RGBController_LogitechG403.h                                 \
    Controllers/LogitechController/RGBController_LogitechG502PS.h                               \
    Controllers/LogitechController/RGBController_LogitechG810.h                                 \
    Controllers/LogitechController/RGBController_LogitechG213.h                                 \
    Controllers/LogitechController/RGBController_LogitechGPowerPlay.h                           \
    Controllers/LogitechController/RGBController_LogitechGProWireless.h                         \
    Controllers/MSI3ZoneController/MSI3ZoneController.h                                         \
    Controllers/MSI3ZoneController/RGBController_MSI3Zone.h                                     \
    Controllers/MSIGPUController/MSIGPUController.h                                             \
    Controllers/MSIGPUController/RGBController_MSIGPU.h                                         \
    Controllers/MSIMysticLightController/MSIMysticLightController.h                             \
    Controllers/MSIMysticLightController/RGBController_MSIMysticLight.h                         \
    Controllers/MSIRGBController/MSIRGBController.h                                             \
    Controllers/MSIRGBController/RGBController_MSIRGB.h                                         \
    Controllers/NZXTHue2Controller/NZXTHue2Controller.h                                         \
    Controllers/NZXTHue2Controller/RGBController_NZXTHue2.h                                     \
    Controllers/NZXTKrakenController/NZXTKrakenController.h                                     \
    Controllers/NZXTKrakenController/RGBController_NZXTKraken.h                                 \
    Controllers/OpenRazerController/OpenRazerDevices.h                                          \
    Controllers/PatriotViperController/PatriotViperController.h                                 \
    Controllers/PatriotViperController/RGBController_PatriotViper.h                             \
    Controllers/PhilipsHueController/PhilipsHueController.h                                     \
    Controllers/PhilipsHueController/RGBController_PhilipsHue.h                                 \
    Controllers/PolychromeController/PolychromeController.h                                     \
    Controllers/PolychromeController/RGBController_Polychrome.h                                 \
    Controllers/PoseidonZRGBController/PoseidonZRGBController.h                                 \
    Controllers/PoseidonZRGBController/RGBController_PoseidonZRGB.h                             \
    Controllers/RedragonController/RedragonK556Controller.h                                     \
    Controllers/RedragonController/RedragonM711Controller.h                                     \
    Controllers/RedragonController/RGBController_RedragonK556.h                                 \
    Controllers/RedragonController/RGBController_RedragonM711.h                                 \
    Controllers/RGBFusionController/RGBFusionController.h                                       \
    Controllers/RGBFusionController/RGBController_RGBFusion.h                                   \
    Controllers/RGBFusion2DRAMController/RGBFusion2DRAMController.h                             \
    Controllers/RGBFusion2DRAMController/RGBController_RGBFusion2DRAM.h                         \
    Controllers/RGBFusion2SMBusController/RGBFusion2SMBusController.h                           \
    Controllers/RGBFusion2SMBusController/RGBController_RGBFusion2SMBus.h                       \
    Controllers/RGBFusion2USBController/RGBFusion2USBController.h                               \
    Controllers/RGBFusion2USBController/RGBController_RGBFusion2USB.h                           \
    Controllers/RGBFusionGPUController/RGBFusionGPUController.h                                 \
    Controllers/RGBFusionGPUController/RGBController_RGBFusionGPU.h                             \
    Controllers/SapphireGPUController/SapphireGPUController.h                                   \
    Controllers/SapphireGPUController/RGBController_SapphireGPU.h                               \
    Controllers/SinowealthController/SinowealthController.h                                     \
    Controllers/SinowealthController/RGBController_Sinowealth.h                                 \
    Controllers/SonyDS4Controller/SonyDS4Controller.h                                           \
    Controllers/SonyDS4Controller/RGBController_SonyDS4.h                                       \
    Controllers/SteelSeriesController/SteelSeriesApexBaseController.h                           \
    Controllers/SteelSeriesController/SteelSeriesApexController.h                               \
    Controllers/SteelSeriesController/SteelSeriesApexMController.h                              \
    Controllers/SteelSeriesController/SteelSeriesRivalController.h                              \
    Controllers/SteelSeriesController/SteelSeriesSiberiaController.h                            \
    Controllers/SteelSeriesController/RGBController_SteelSeriesApex.h                           \
    Controllers/SteelSeriesController/RGBController_SteelSeriesRival.h                          \
    Controllers/SteelSeriesController/RGBController_SteelSeriesSiberia.h                        \
    Controllers/TecknetController/TecknetController.h                                           \
    Controllers/TecknetController/RGBController_Tecknet.h                                       \
    Controllers/ThermaltakeRiingController/ThermaltakeRiingController.h                         \
    Controllers/ThermaltakeRiingController/RGBController_ThermaltakeRiing.h                     \
    RGBController/RGBController.h                                                               \
    RGBController/RGBController_Dummy.h                                                         \
    RGBController/RGBController_Network.h                                                       \


SOURCES +=                                                                                      \
    dependencies/dmiinfo.cpp                                                                    \
    dependencies/ColorWheel/ColorWheel.cpp                                                      \
    dependencies/hueplusplus/src/APICache.cpp                                                   \
    dependencies/hueplusplus/src/BaseDevice.cpp                                                 \
    dependencies/hueplusplus/src/BaseHttpHandler.cpp                                            \
    dependencies/hueplusplus/src/Bridge.cpp                                                     \
    dependencies/hueplusplus/src/BridgeConfig.cpp                                               \
    dependencies/hueplusplus/src/CLIPSensors.cpp                                                \
    dependencies/hueplusplus/src/ColorUnits.cpp                                                 \
    dependencies/hueplusplus/src/ExtendedColorHueStrategy.cpp                                   \
    dependencies/hueplusplus/src/ExtendedColorTemperatureStrategy.cpp                           \
    dependencies/hueplusplus/src/Group.cpp                                                      \
    dependencies/hueplusplus/src/HueCommandAPI.cpp                                              \
    dependencies/hueplusplus/src/HueDeviceTypes.cpp                                             \
    dependencies/hueplusplus/src/HueException.cpp                                               \
    dependencies/hueplusplus/src/Light.cpp                                                      \
    dependencies/hueplusplus/src/ModelPictures.cpp                                              \
    dependencies/hueplusplus/src/NewDeviceList.cpp                                              \
    dependencies/hueplusplus/src/Scene.cpp                                                      \
    dependencies/hueplusplus/src/Schedule.cpp                                                   \
    dependencies/hueplusplus/src/Sensor.cpp                                                     \
    dependencies/hueplusplus/src/SimpleBrightnessStrategy.cpp                                   \
    dependencies/hueplusplus/src/SimpleColorHueStrategy.cpp                                     \
    dependencies/hueplusplus/src/SimpleColorTemperatureStrategy.cpp                             \
    dependencies/hueplusplus/src/StateTransaction.cpp                                           \
    dependencies/hueplusplus/src/TimePattern.cpp                                                \
    dependencies/hueplusplus/src/UPnP.cpp                                                       \
    dependencies/hueplusplus/src/Utils.cpp                                                      \
    dependencies/hueplusplus/src/ZLLSensors.cpp                                                 \
    dependencies/libe131/src/e131.c                                                             \
    dependencies/mbedtls/library/aes.c                                                          \
    dependencies/mbedtls/library/aesni.c                                                        \
    dependencies/mbedtls/library/arc4.c                                                         \
    dependencies/mbedtls/library/aria.c                                                         \
    dependencies/mbedtls/library/asn1parse.c                                                    \
    dependencies/mbedtls/library/asn1write.c                                                    \
    dependencies/mbedtls/library/base64.c                                                       \
    dependencies/mbedtls/library/bignum.c                                                       \
    dependencies/mbedtls/library/blowfish.c                                                     \
    dependencies/mbedtls/library/camellia.c                                                     \
    dependencies/mbedtls/library/ccm.c                                                          \
    dependencies/mbedtls/library/certs.c                                                        \
    dependencies/mbedtls/library/chacha20.c                                                     \
    dependencies/mbedtls/library/chachapoly.c                                                   \
    dependencies/mbedtls/library/cipher.c                                                       \
    dependencies/mbedtls/library/cipher_wrap.c                                                  \
    dependencies/mbedtls/library/cmac.c                                                         \
    dependencies/mbedtls/library/ctr_drbg.c                                                     \
    dependencies/mbedtls/library/debug.c                                                        \
    dependencies/mbedtls/library/des.c                                                          \
    dependencies/mbedtls/library/dhm.c                                                          \
    dependencies/mbedtls/library/ecdh.c                                                         \
    dependencies/mbedtls/library/ecdsa.c                                                        \
    dependencies/mbedtls/library/ecjpake.c                                                      \
    dependencies/mbedtls/library/ecp.c                                                          \
    dependencies/mbedtls/library/ecp_curves.c                                                   \
    dependencies/mbedtls/library/entropy.c                                                      \
    dependencies/mbedtls/library/entropy_poll.c                                                 \
    dependencies/mbedtls/library/error.c                                                        \
    dependencies/mbedtls/library/gcm.c                                                          \
    dependencies/mbedtls/library/havege.c                                                       \
    dependencies/mbedtls/library/hkdf.c                                                         \
    dependencies/mbedtls/library/hmac_drbg.c                                                    \
    dependencies/mbedtls/library/md.c                                                           \
    dependencies/mbedtls/library/md2.c                                                          \
    dependencies/mbedtls/library/md4.c                                                          \
    dependencies/mbedtls/library/md5.c                                                          \
    dependencies/mbedtls/library/memory_buffer_alloc.c                                          \
    dependencies/mbedtls/library/net_sockets.c                                                  \
    dependencies/mbedtls/library/nist_kw.c                                                      \
    dependencies/mbedtls/library/oid.c                                                          \
    dependencies/mbedtls/library/padlock.c                                                      \
    dependencies/mbedtls/library/pem.c                                                          \
    dependencies/mbedtls/library/pk.c                                                           \
    dependencies/mbedtls/library/pk_wrap.c                                                      \
    dependencies/mbedtls/library/pkcs5.c                                                        \
    dependencies/mbedtls/library/pkcs11.c                                                       \
    dependencies/mbedtls/library/pkcs12.c                                                       \
    dependencies/mbedtls/library/pkparse.c                                                      \
    dependencies/mbedtls/library/pkwrite.c                                                      \
    dependencies/mbedtls/library/platform.c                                                     \
    dependencies/mbedtls/library/platform_util.c                                                \
    dependencies/mbedtls/library/poly1305.c                                                     \
    dependencies/mbedtls/library/psa_crypto.c                                                   \
    dependencies/mbedtls/library/psa_crypto_driver_wrappers.c                                   \
    dependencies/mbedtls/library/psa_crypto_se.c                                                \
    dependencies/mbedtls/library/psa_crypto_slot_management.c                                   \
    dependencies/mbedtls/library/psa_crypto_storage.c                                           \
    dependencies/mbedtls/library/psa_its_file.c                                                 \
    dependencies/mbedtls/library/ripemd160.c                                                    \
    dependencies/mbedtls/library/rsa.c                                                          \
    dependencies/mbedtls/library/rsa_internal.c                                                 \
    dependencies/mbedtls/library/sha1.c                                                         \
    dependencies/mbedtls/library/sha256.c                                                       \
    dependencies/mbedtls/library/sha512.c                                                       \
    dependencies/mbedtls/library/ssl_cache.c                                                    \
    dependencies/mbedtls/library/ssl_ciphersuites.c                                             \
    dependencies/mbedtls/library/ssl_cli.c                                                      \
    dependencies/mbedtls/library/ssl_cookie.c                                                   \
    dependencies/mbedtls/library/ssl_msg.c                                                      \
    dependencies/mbedtls/library/ssl_srv.c                                                      \
    dependencies/mbedtls/library/ssl_ticket.c                                                   \
    dependencies/mbedtls/library/ssl_tls.c                                                      \
    dependencies/mbedtls/library/ssl_tls13_keys.c                                               \
    dependencies/mbedtls/library/threading.c                                                    \
    dependencies/mbedtls/library/timing.c                                                       \
    dependencies/mbedtls/library/version.c                                                      \
    dependencies/mbedtls/library/version_features.c                                             \
    dependencies/mbedtls/library/x509.c                                                         \
    dependencies/mbedtls/library/x509_create.c                                                  \
    dependencies/mbedtls/library/x509_crl.c                                                     \
    dependencies/mbedtls/library/x509_crt.c                                                     \
    dependencies/mbedtls/library/x509_csr.c                                                     \
    dependencies/mbedtls/library/x509write_crt.c                                                \
    dependencies/mbedtls/library/x509write_csr.c                                                \
    dependencies/mbedtls/library/xtea.c                                                         \
    main.cpp                                                                                    \
    cli.cpp                                                                                     \
    NetworkClient.cpp                                                                           \
    NetworkServer.cpp                                                                           \
    ProfileManager.cpp                                                                          \
    ResourceManager.cpp                                                                         \
    qt/OpenRGBClientInfoPage.cpp                                                                \
    qt/OpenRGBDeviceInfoPage.cpp                                                                \
    qt/OpenRGBDevicePage.cpp                                                                    \
    qt/OpenRGBDialog.cpp                                                                        \
    i2c_smbus/i2c_smbus.cpp                                                                     \
    i2c_tools/i2c_tools.cpp                                                                     \
    net_port/net_port.cpp                                                                       \
    qt/DeviceView.cpp                                                                           \
    qt/OpenRGBDialog2.cpp                                                                       \
    qt/OpenRGBProfileSaveDialog.cpp                                                             \
    qt/OpenRGBServerInfoPage.cpp                                                                \
    qt/OpenRGBSoftwareInfoPage.cpp                                                              \
    qt/OpenRGBSystemInfoPage.cpp                                                                \
    qt/OpenRGBZoneResizeDialog.cpp                                                              \
    qt/hsv.cpp                                                                                  \
    serial_port/serial_port.cpp                                                                 \
    super_io/super_io.cpp                                                                       \
    Controllers/AMDWraithPrismController/AMDWraithPrismController.cpp                           \
    Controllers/AMDWraithPrismController/AMDWraithPrismControllerDetect.cpp                     \
    Controllers/AMDWraithPrismController/RGBController_AMDWraithPrism.cpp                       \
    Controllers/AorusCPUCoolerController/ATC800Controller.cpp                                   \
    Controllers/AorusCPUCoolerController/AorusCPUCoolerControllerDetect.cpp                     \
    Controllers/AorusCPUCoolerController/RGBController_AorusATC800.cpp                          \
    Controllers/AuraUSBController/AuraUSBController.cpp                                         \
    Controllers/AuraUSBController/AuraAddressableController.cpp                                 \
    Controllers/AuraUSBController/AuraMainboardController.cpp                                   \
    Controllers/AuraUSBController/AuraMouseController.cpp                                       \
    Controllers/AuraUSBController/AuraUSBControllerDetect.cpp                                   \
    Controllers/AuraUSBController/RGBController_AuraMouse.cpp                                   \
    Controllers/AuraUSBController/RGBController_AuraUSB.cpp                                     \
    Controllers/AuraCoreController/AuraCoreController.cpp                                       \
    Controllers/AuraCoreController/AuraCoreControllerDetect.cpp                                 \
    Controllers/AuraCoreController/RGBController_AuraCore.cpp                                   \
    Controllers/AuraGPUController/AuraGPUController.cpp                                         \
    Controllers/AuraGPUController/AuraGPUControllerDetect.cpp                                   \
    Controllers/AuraGPUController/RGBController_AuraGPU.cpp                                     \
    Controllers/AuraSMBusController/AuraSMBusController.cpp                                     \
    Controllers/AuraSMBusController/AuraSMBusControllerDetect.cpp                               \
    Controllers/AuraSMBusController/RGBController_AuraSMBus.cpp                                 \
    Controllers/CoolerMasterController/CMMP750Controller.cpp                                    \
    Controllers/CoolerMasterController/CoolerMasterControllerDetect.cpp                         \
    Controllers/CoolerMasterController/RGBController_CMMP750Controller.cpp                      \
    Controllers/CorsairDominatorPlatinumController/CorsairDominatorPlatinumController.cpp       \
    Controllers/CorsairDominatorPlatinumController/CorsairDominatorPlatinumControllerDetect.cpp \
    Controllers/CorsairDominatorPlatinumController/RGBController_CorsairDominatorPlatinum.cpp   \
    Controllers/CorsairHydroController/CorsairHydroController.cpp                               \
    Controllers/CorsairHydroController/CorsairHydroControllerDetect.cpp                         \
    Controllers/CorsairHydroController/RGBController_CorsairHydro.cpp                           \
    Controllers/CorsairLightingNodeController/CorsairLightingNodeController.cpp                 \
    Controllers/CorsairLightingNodeController/CorsairLightingNodeControllerDetect.cpp           \
    Controllers/CorsairLightingNodeController/RGBController_CorsairLightingNode.cpp             \
    Controllers/CorsairPeripheralController/CorsairPeripheralController.cpp                     \
    Controllers/CorsairPeripheralController/CorsairPeripheralControllerDetect.cpp               \
    Controllers/CorsairPeripheralController/RGBController_CorsairPeripheral.cpp                 \
    Controllers/CorsairVengeanceController/CorsairVengeanceController.cpp                       \
    Controllers/CorsairVengeanceController/CorsairVengeanceControllerDetect.cpp                 \
    Controllers/CorsairVengeanceController/RGBController_CorsairVengeance.cpp                   \
    Controllers/CorsairVengeanceProController/CorsairVengeanceProController.cpp                 \
    Controllers/CorsairVengeanceProController/CorsairVengeanceProControllerDetect.cpp           \
    Controllers/CorsairVengeanceProController/RGBController_CorsairVengeancePro.cpp             \
    Controllers/CrucialController/CrucialController.cpp                                         \
    Controllers/CrucialController/CrucialControllerDetect.cpp                                   \
    Controllers/CrucialController/RGBController_Crucial.cpp                                     \
    Controllers/DebugController/DebugControllerDetect.cpp                                       \
    Controllers/DuckyKeyboardController/DuckyKeyboardController.cpp                             \
    Controllers/DuckyKeyboardController/DuckyKeyboardControllerDetect.cpp                       \
    Controllers/DuckyKeyboardController/RGBController_DuckyKeyboard.cpp                         \
    Controllers/E131Controller/E131ControllerDetect.cpp                                         \
    Controllers/E131Controller/RGBController_E131.cpp                                           \
    Controllers/EKController/EKControllerDetect.cpp                                             \
    Controllers/EKController/EKController.cpp                                                   \
    Controllers/EKController/RGBController_EKController.cpp                                     \
    Controllers/EspurnaController/EspurnaController.cpp                                         \
    Controllers/EspurnaController/EspurnaControllerDetect.cpp                                   \
    Controllers/EspurnaController/RGBController_Espurna.cpp                                     \
    Controllers/EVGAGPUController/EVGAGPUv1Controller.cpp                                       \
    Controllers/EVGAGPUController/EVGAGPUv2Controller.cpp                                       \
    Controllers/EVGAGPUController/EVGAGPUControllerDetect.cpp                                   \
    Controllers/EVGAGPUController/RGBController_EVGAGPUv1.cpp                                   \
    Controllers/EVGAGPUController/RGBController_EVGAGPUv2.cpp                                   \
    Controllers/GalaxGPUController/GalaxGPUController.cpp                                       \
    Controllers/GalaxGPUController/GalaxGPUControllerDetect.cpp                                 \
    Controllers/GalaxGPUController/RGBController_GalaxGPU.cpp                                   \
    Controllers/HoltekController/HoltekA070Controller.cpp                                       \
    Controllers/HoltekController/HoltekControllerDetect.cpp                                     \
    Controllers/HoltekController/RGBController_HoltekA070.cpp                                   \
    Controllers/HuePlusController/HuePlusController.cpp                                         \
    Controllers/HuePlusController/HuePlusControllerDetect.cpp                                   \
    Controllers/HuePlusController/RGBController_HuePlus.cpp                                     \
    Controllers/HyperXDRAMController/HyperXDRAMController.cpp                                   \
    Controllers/HyperXDRAMController/HyperXDRAMControllerDetect.cpp                             \
    Controllers/HyperXDRAMController/RGBController_HyperXDRAM.cpp                               \
    Controllers/HyperXKeyboardController/HyperXAlloyOriginsController.cpp                       \
    Controllers/HyperXKeyboardController/HyperXKeyboardController.cpp                           \
    Controllers/HyperXKeyboardController/HyperXKeyboardControllerDetect.cpp                     \
    Controllers/HyperXKeyboardController/RGBController_HyperXAlloyOrigins.cpp                   \
    Controllers/HyperXKeyboardController/RGBController_HyperXKeyboard.cpp                       \
    Controllers/HyperXMouseController/HyperXMouseControllerDetect.cpp                           \
    Controllers/HyperXMouseController/HyperXPulsefireSurgeController.cpp                        \
    Controllers/HyperXMouseController/RGBController_HyperXPulsefireSurge.cpp                    \
    Controllers/HyperXMousematController/HyperXMousematController.cpp                           \
    Controllers/HyperXMousematController/HyperXMousematControllerDetect.cpp                     \
    Controllers/HyperXMousematController/RGBController_HyperXMousemat.cpp                       \
    Controllers/LEDStripController/LEDStripController.cpp                                       \
    Controllers/LEDStripController/LEDStripControllerDetect.cpp                                 \
    Controllers/LEDStripController/RGBController_LEDStrip.cpp                                   \
    Controllers/LogitechController/LogitechControllerDetect.cpp                                 \
    Controllers/LogitechController/LogitechG203Controller.cpp                                   \
    Controllers/LogitechController/LogitechG203LController.cpp                                  \
    Controllers/LogitechController/LogitechG403Controller.cpp                                   \
    Controllers/LogitechController/LogitechG502PSController.cpp                                 \
    Controllers/LogitechController/LogitechG810Controller.cpp                                   \
    Controllers/LogitechController/LogitechG213Controller.cpp                                   \
    Controllers/LogitechController/LogitechGPowerPlayController.cpp                             \
    Controllers/LogitechController/LogitechGProWirelessController.cpp                           \
    Controllers/LogitechController/RGBController_LogitechG203.cpp                               \
    Controllers/LogitechController/RGBController_LogitechG203L.cpp                              \
    Controllers/LogitechController/RGBController_LogitechG403.cpp                               \
    Controllers/LogitechController/RGBController_LogitechG502PS.cpp                             \
    Controllers/LogitechController/RGBController_LogitechG810.cpp                               \
    Controllers/LogitechController/RGBController_LogitechG213.cpp                               \
    Controllers/LogitechController/RGBController_LogitechGPowerPlay.cpp                         \
    Controllers/LogitechController/RGBController_LogitechGProWireless.cpp                       \
    Controllers/MSI3ZoneController/MSI3ZoneController.cpp                                       \
    Controllers/MSI3ZoneController/MSI3ZoneControllerDetect.cpp                                 \
    Controllers/MSI3ZoneController/RGBController_MSI3Zone.cpp                                   \
    Controllers/MSIGPUController/MSIGPUController.cpp                                           \
    Controllers/MSIGPUController/MSIGPUControllerDetect.cpp                                     \
    Controllers/MSIGPUController/RGBController_MSIGPU.cpp                                       \
    Controllers/MSIMysticLightController/MSIMysticLightController.cpp                           \
    Controllers/MSIMysticLightController/MSIMysticLightControllerDetect.cpp                     \
    Controllers/MSIMysticLightController/RGBController_MSIMysticLight.cpp                       \
    Controllers/MSIRGBController/MSIRGBController.cpp                                           \
    Controllers/MSIRGBController/MSIRGBControllerDetect.cpp                                     \
    Controllers/MSIRGBController/RGBController_MSIRGB.cpp                                       \
    Controllers/NZXTHue2Controller/NZXTHue2Controller.cpp                                       \
    Controllers/NZXTHue2Controller/NZXTHue2ControllerDetect.cpp                                 \
    Controllers/NZXTHue2Controller/RGBController_NZXTHue2.cpp                                   \
    Controllers/NZXTKrakenController/NZXTKrakenController.cpp                                   \
    Controllers/NZXTKrakenController/NZXTKrakenControllerDetect.cpp                             \
    Controllers/NZXTKrakenController/RGBController_NZXTKraken.cpp                               \
    Controllers/PatriotViperController/PatriotViperController.cpp                               \
    Controllers/PatriotViperController/PatriotViperControllerDetect.cpp                         \
    Controllers/PatriotViperController/RGBController_PatriotViper.cpp                           \
    Controllers/PhilipsHueController/PhilipsHueController.cpp                                   \
    Controllers/PhilipsHueController/PhilipsHueControllerDetect.cpp                             \
    Controllers/PhilipsHueController/PhilipsHueEntertainmentController.cpp                      \
    Controllers/PhilipsHueController/RGBController_PhilipsHue.cpp                               \
    Controllers/PhilipsHueController/RGBController_PhilipsHueEntertainment.cpp                  \
    Controllers/PolychromeController/PolychromeController.cpp                                   \
    Controllers/PolychromeController/PolychromeControllerDetect.cpp                             \
    Controllers/PolychromeController/RGBController_Polychrome.cpp                               \
    Controllers/PoseidonZRGBController/PoseidonZRGBController.cpp                               \
    Controllers/PoseidonZRGBController/PoseidonZRGBControllerDetect.cpp                         \
    Controllers/PoseidonZRGBController/RGBController_PoseidonZRGB.cpp                           \
    Controllers/RedragonController/RedragonK556Controller.cpp                                   \
    Controllers/RedragonController/RedragonM711Controller.cpp                                   \
    Controllers/RedragonController/RedragonControllerDetect.cpp                                 \
    Controllers/RedragonController/RGBController_RedragonK556.cpp                               \
    Controllers/RedragonController/RGBController_RedragonM711.cpp                               \
    Controllers/RGBFusionController/RGBFusionController.cpp                                     \
    Controllers/RGBFusionController/RGBFusionControllerDetect.cpp                               \
    Controllers/RGBFusionController/RGBController_RGBFusion.cpp                                 \
    Controllers/RGBFusion2DRAMController/RGBFusion2DRAMController.cpp                           \
    Controllers/RGBFusion2DRAMController/RGBFusion2DRAMControllerDetect.cpp                     \
    Controllers/RGBFusion2DRAMController/RGBController_RGBFusion2DRAM.cpp                       \
    Controllers/RGBFusion2SMBusController/RGBFusion2SMBusController.cpp                         \
    Controllers/RGBFusion2SMBusController/RGBFusion2SMBusControllerDetect.cpp                   \
    Controllers/RGBFusion2SMBusController/RGBController_RGBFusion2SMBus.cpp                     \
    Controllers/RGBFusion2USBController/RGBFusion2USBController.cpp                             \
    Controllers/RGBFusion2USBController/RGBFusion2USBControllerDetect.cpp                       \
    Controllers/RGBFusion2USBController/RGBController_RGBFusion2USB.cpp                         \
    Controllers/RGBFusionGPUController/RGBFusionGPUController.cpp                               \
    Controllers/RGBFusionGPUController/RGBFusionGPUControllerDetect.cpp                         \
    Controllers/RGBFusionGPUController/RGBController_RGBFusionGPU.cpp                           \
    Controllers/SapphireGPUController/SapphireGPUController.cpp                                 \
    Controllers/SapphireGPUController/SapphireGPUControllerDetect.cpp                           \
    Controllers/SapphireGPUController/RGBController_SapphireGPU.cpp                             \
    Controllers/SinowealthController/SinowealthController.cpp                                   \
    Controllers/SinowealthController/SinowealthControllerDetect.cpp                             \
    Controllers/SinowealthController/RGBController_Sinowealth.cpp                               \
    Controllers/SonyDS4Controller/SonyDS4Controller.cpp                                         \
    Controllers/SonyDS4Controller/SonyDS4ControllerDetect.cpp                                   \
    Controllers/SonyDS4Controller/RGBController_SonyDS4.cpp                                     \
    Controllers/SteelSeriesController/SteelSeriesApexController.cpp                             \
    Controllers/SteelSeriesController/SteelSeriesApexMController.cpp                            \
    Controllers/SteelSeriesController/SteelSeriesRivalController.cpp                            \
    Controllers/SteelSeriesController/SteelSeriesSiberiaController.cpp                          \
    Controllers/SteelSeriesController/SteelSeriesControllerDetect.cpp                           \
    Controllers/SteelSeriesController/RGBController_SteelSeriesApex.cpp                         \
    Controllers/SteelSeriesController/RGBController_SteelSeriesRival.cpp                        \
    Controllers/SteelSeriesController/RGBController_SteelSeriesSiberia.cpp                      \
    Controllers/TecknetController/TecknetController.cpp                                         \
    Controllers/TecknetController/TecknetControllerDetect.cpp                                   \
    Controllers/TecknetController/RGBController_Tecknet.cpp                                     \
    Controllers/ThermaltakeRiingController/ThermaltakeRiingController.cpp                       \
    Controllers/ThermaltakeRiingController/ThermaltakeRiingControllerDetect.cpp                 \
    Controllers/ThermaltakeRiingController/RGBController_ThermaltakeRiing.cpp                   \
    RGBController/RGBController.cpp                                                             \
    RGBController/RGBController_Dummy.cpp                                                       \
    RGBController/RGBController_Network.cpp                                                     \

RESOURCES +=                                                                                    \
    qt/resources.qrc

FORMS +=                                                                                        \
    qt/OpenRGBClientInfoPage.ui                                                                 \
    qt/OpenRGBDeviceInfoPage.ui                                                                 \
    qt/OpenRGBDevicePage.ui                                                                     \
    qt/OpenRGBDialog.ui                                                                         \
    qt/OpenRGBDialog2.ui                                                                        \
    qt/OpenRGBProfileSaveDialog.ui                                                              \
    qt/OpenRGBServerInfoPage.ui                                                                 \
    qt/OpenRGBSoftwareInfoPage.ui                                                               \
    qt/OpenRGBSystemInfoPage.ui                                                                 \
    qt/OpenRGBZoneResizeDialog.ui                                                               \

#-----------------------------------------------------------------------------------------------#
# Windows-specific Configuration                                                                #
#-----------------------------------------------------------------------------------------------#
win32:INCLUDEPATH +=                                                                            \
    dependencies/display-library/include                                                        \
    dependencies/hidapi                                                                         \
    dependencies/inpout32_1501/Win32/                                                           \
    dependencies/libusb-1.0.22/include                                                          \
    dependencies/NVFC                                                                           \
    dependencies/openrazer-win32                                                                \
    wmi/                                                                                        \

win32:SOURCES +=                                                                                \
#   dependencies/hidapi/hidapi.c                                                                \
    dependencies/hueplusplus/src/WinHttpHandler.cpp                                             \
    dependencies/NVFC/nvapi.cpp                                                                 \
    i2c_smbus/i2c_smbus_amdadl.cpp                                                              \
    i2c_smbus/i2c_smbus_i801.cpp                                                                \
    i2c_smbus/i2c_smbus_nct6775.cpp                                                             \
    i2c_smbus/i2c_smbus_nvapi.cpp                                                               \
    i2c_smbus/i2c_smbus_piix4.cpp                                                               \
    serial_port/find_usb_serial_port_win.cpp                                                    \
    wmi/wmi.cpp                                                                                 \
    Controllers/OpenRazerController/OpenRazerWindowsDetect.cpp                                  \
    Controllers/OpenRazerController/RGBController_OpenRazerWindows.cpp                          \

win32:HEADERS +=                                                                                \
    dependencies/display-library/include/adl_defines.h                                          \
    dependencies/display-library/include/adl_sdk.h                                              \
    dependencies/display-library/include/adl_structures.h                                       \
    dependencies/inpout32_1501/Win32/inpout32.h                                                 \
    dependencies/NVFC/nvapi.h                                                                   \
    i2c_smbus/i2c_smbus_i801.h                                                                  \
    i2c_smbus/i2c_smbus_nct6775.h                                                               \
    i2c_smbus/i2c_smbus_nvapi.h                                                                 \
    i2c_smbus/i2c_smbus_piix4.h                                                                 \
    wmi/wmi.h                                                                                   \
    Controllers/OpenRazerController/RGBController_OpenRazerWindows.h                            \

win32:contains(QMAKE_TARGET.arch, x86_64) {
    LIBS +=                                                                                     \
        -lws2_32                                                                                \
        -L"$$PWD/dependencies/inpout32_1501/x64/" -linpoutx64                                   \
        -L"$$PWD/dependencies/libusb-1.0.22/MS64/dll" -llibusb-1.0                              \
        -L"$$PWD/dependencies/hidapi-win/x64/" -lhidapi                                         \
}

win32:contains(QMAKE_TARGET.arch, x86) {
    LIBS +=                                                                                     \
        -lws2_32                                                                                \
        -L"$$PWD/dependencies/inpout32_1501/Win32/" -linpout32                                  \
        -L"$$PWD/dependencies/libusb-1.0.22/MS32/dll" -llibusb-1.0                              \
        -L"$$PWD/dependencies/hidapi-win/x86/" -lhidapi                                         \
}

win32:DEFINES -=                                                                                \
    UNICODE

win32:DEFINES +=                                                                                \
    USE_HID_USAGE                                                                               \
    _MBCS                                                                                       \
    WIN32                                                                                       \
    _CRT_SECURE_NO_WARNINGS                                                                     \
    _WINSOCK_DEPRECATED_NO_WARNINGS                                                             \
    WIN32_LEAN_AND_MEAN                                                                         \

win32:RC_ICONS +=                                                                               \
    qt/OpenRGB.ico

#-----------------------------------------------------------------------------------------------#
# Windows GitLab CI Configuration                                                               #
#-----------------------------------------------------------------------------------------------#
win32:CONFIG(debug, debug|release) {
    win32:DESTDIR = debug
}

win32:CONFIG(release, debug|release) {
    win32:DESTDIR = release
}

win32:OBJECTS_DIR = _intermediate_$$DESTDIR/.obj
win32:MOC_DIR     = _intermediate_$$DESTDIR/.moc
win32:RCC_DIR     = _intermediate_$$DESTDIR/.qrc
win32:UI_DIR      = _intermediate_$$DESTDIR/.ui

#-----------------------------------------------------------------------------------------------#
# Copy dependencies to output directory                                                         #
#-----------------------------------------------------------------------------------------------#

win32:contains(QMAKE_TARGET.arch, x86_64) {
    copydata.commands  = $(COPY_FILE) \"$$shell_path($$PWD/dependencies/openrazer-win32/OpenRazer64.dll      )\" \"$$shell_path($$DESTDIR)\" $$escape_expand(\n\t)
    copydata.commands += $(COPY_FILE) \"$$shell_path($$PWD/dependencies/inpout32_1501/x64/inpoutx64.dll      )\" \"$$shell_path($$DESTDIR)\" $$escape_expand(\n\t)
    copydata.commands += $(COPY_FILE) \"$$shell_path($$PWD/dependencies/libusb-1.0.22/MS64/dll/libusb-1.0.dll)\" \"$$shell_path($$DESTDIR)\" $$escape_expand(\n\t)
    copydata.commands += $(COPY_FILE) \"$$shell_path($$PWD/dependencies/hidapi-win/x64/hidapi.dll            )\" \"$$shell_path($$DESTDIR)\" $$escape_expand(\n\t)
    first.depends = $(first) copydata
    export(first.depends)
    export(copydata.commands)
    QMAKE_EXTRA_TARGETS += first copydata
}

win32:contains(QMAKE_TARGET.arch, x86) {
    copydata.commands  = $(COPY_FILE) \"$$shell_path($$PWD/dependencies/openrazer-win32/OpenRazer.dll        )\" \"$$shell_path($$DESTDIR)\" $$escape_expand(\n\t)
    copydata.commands += $(COPY_FILE) \"$$shell_path($$PWD/dependencies/inpout32_1501/Win32/inpout32.dll     )\" \"$$shell_path($$DESTDIR)\" $$escape_expand(\n\t)
    copydata.commands += $(COPY_FILE) \"$$shell_path($$PWD/dependencies/libusb-1.0.22/MS32/dll/libusb-1.0.dll)\" \"$$shell_path($$DESTDIR)\" $$escape_expand(\n\t)
    copydata.commands += $(COPY_FILE) \"$$shell_path($$PWD/dependencies/hidapi-win/x86/hidapi.dll            )\" \"$$shell_path($$DESTDIR)\" $$escape_expand(\n\t)

    first.depends = $(first) copydata
    export(first.depends)
    export(copydata.commands)
    QMAKE_EXTRA_TARGETS += first copydata
}

#-----------------------------------------------------------------------------------------------#
# Linux-specific Configuration                                                                  #
#-----------------------------------------------------------------------------------------------#
unix:!macx {
    INCLUDEPATH +=                                                                              \
    Controllers/FaustusController                                                               \
    Controllers/LinuxLEDController                                                              \

    HEADERS +=                                                                                  \
    i2c_smbus/i2c_smbus_linux.h                                                                 \
    Controllers/FaustusController/RGBController_Faustus.h                                       \
    Controllers/LinuxLEDController/LinuxLEDController.h                                         \
    Controllers/LinuxLEDController/RGBController_LinuxLED.h                                     \
    Controllers/OpenRazerController/RGBController_OpenRazer.h                                   \


    LIBS +=                                                                                     \
    -lusb-1.0                                                                                   \
    -lstdc++fs                                                                                  \

    #-------------------------------------------------------------------------------------------#
    # Determine which hidapi to use based on availability                                       #
    #   Prefer hidraw backend, then libusb                                                      #
    #-------------------------------------------------------------------------------------------#
    packagesExist(hidapi-hidraw) {
        LIBS += -lhidapi-hidraw
    } else {
        packagesExist(hidapi-libusb) {
            LIBS += -lhidapi-libusb
        } else {
            LIBS += -lhidapi
        }
    }

    SOURCES +=                                                                                  \
    dependencies/hueplusplus/src/LinHttpHandler.cpp                                             \
    i2c_smbus/i2c_smbus_linux.cpp                                                               \
    serial_port/find_usb_serial_port_linux.cpp                                                  \
    Controllers/FaustusController/RGBController_Faustus.cpp                                     \
    Controllers/LinuxLEDController/LinuxLEDController.cpp                                       \
    Controllers/LinuxLEDController/LinuxLEDControllerDetect.cpp                                 \
    Controllers/LinuxLEDController/RGBController_LinuxLED.cpp                                   \
    Controllers/OpenRazerController/OpenRazerDetect.cpp                                         \
    Controllers/OpenRazerController/RGBController_OpenRazer.cpp                                 \

    #-------------------------------------------------------------------------------------------#
    # Set up install paths                                                                      #
    # These install paths are used for AppImage and .deb packaging                              #
    #-------------------------------------------------------------------------------------------#
    isEmpty(PREFIX) {
        PREFIX = /usr
    }

    target.path=$$PREFIX/bin/
    desktop.path=$$PREFIX/share/applications/
    desktop.files+=qt/OpenRGB.desktop
    pixmap.path=$$PREFIX/share/pixmaps/
    pixmap.files+=qt/OpenRGB.png
    INSTALLS += target desktop pixmap
}

unix:!macx:CONFIG(asan) {
    message("ASan Mode")
    QMAKE_CFLAGS=-fsanitize=address
    QMAKE_CXXFLAGS=-fsanitize=address
    QMAKE_LFLAGS=-fsanitize=address
}

#-----------------------------------------------------------------------------------------------#
# MacOS-specific Configuration                                                                  #
#-----------------------------------------------------------------------------------------------#
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.15

macx:ICON = qt/OpenRGB.icns

unix:macx {
    SOURCES +=                                                                                  \
    serial_port/find_usb_serial_port_linux.cpp                                                  \

    INCLUDEPATH +=                                                                              \
    /usr/local/include                                                                          \

    LIBS +=                                                                                     \
    -L/usr/local/lib -lusb-1.0 -lhidapi                                                         \

    CONFIG +=                                                                                   \
    c++14                                                                                       \
}
