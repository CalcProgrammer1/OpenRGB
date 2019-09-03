
//! \file RzChromaSDK.h
//! \brief Exported APIs.

#ifndef _RZCHROMASDK_H_
#define _RZCHROMASDK_H_

#pragma once

#include "RzErrors.h"
#include "RzChromaSDKDefines.h"
#include "RzChromaSDKTypes.h"

using namespace ChromaSDK;
using namespace ChromaSDK::Keyboard;
using namespace ChromaSDK::Mouse;
using namespace ChromaSDK::Headset;
using namespace ChromaSDK::Mousepad;
using namespace ChromaSDK::Keypad;
using namespace ChromaSDK::ChromaLink;

// Exported functions
#ifdef __cplusplus
extern "C" 
{
#endif

    /*!
     * Initialize Chroma SDK.
     * @return <b>RZRESULT_SERVICE_NOT_ACTIVE</b>: Chroma SDK Service not running.
     * @return <b>RZRESULT_ALREADY_INITIALIZED</b>: Chroma SDK has already been initialized.
     * @return For other error codes, please refer to [Windows System Error Codes](https://msdn.microsoft.com/en-us/library/windows/desktop/ms681381(v=vs.85).aspx) from Windows SDK (WinError.h).
     */
    RZRESULT Init(void);

    /*!
     * UnInitialize Chroma SDK.
     * @return <b>RZRESULT_NOT_VALID_STATE</b>: Chroma SDK not in a valid state. For every Init() call there should be a corresponding UnInit().
     * @return For other error codes, please refer to [Windows System Error Codes](https://msdn.microsoft.com/en-us/library/windows/desktop/ms681381(v=vs.85).aspx) from Windows SDK (WinError.h).
     */
    RZRESULT UnInit(void);

    /*! 
     * Create generic effect for specific or all devices.
     * @param[in] DeviceId Device Id defined in RzChromaSDKDefines.h
     * @param[in] Effect Standard effect type.
     * @param[in] pParam Pointer to a parameter type specified by Effect.
     * @param[in,out] pEffectId Valid effect Id if successful. Set it to NULL if not required.
     * @return <b>RZRESULT_NOT_VALID_STATE</b>: Chroma SDK not in a valid state. Did you forgot to call Init()?
     * @return <b>RZRESULT_INVALID_PARAMETER</b>: Invalid parameter.
     * @return <b>RZRESULT_DEVICE_NOT_AVAILABLE</b>: Device not supported.
     * @return <b>RZRESULT_NOT_SUPPORTED</b>: Effect not supported for this device.
     * @return For other error codes, please refer to [Windows System Error Codes](https://msdn.microsoft.com/en-us/library/windows/desktop/ms681381(v=vs.85).aspx) from Windows SDK (WinError.h).
     */
    RZRESULT CreateEffect(RZDEVICEID DeviceId, ChromaSDK::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);

    /*! 
     * Create keyboard effect.
     * @param[in] Effect Keyboard effect type.
     * @param[in] pParam Pointer to a parameter type specified by Effect.
     * @param[in,out] pEffectId Valid effect Id if successful. Set it to NULL if not required.
     * @return <b>RZRESULT_NOT_VALID_STATE</b>: Chroma SDK not in a valid state. Did you forgot to call Init()?
     * @return <b>RZRESULT_INVALID_PARAMETER</b>: Invalid parameter.
     * @return <b>RZRESULT_NOT_SUPPORTED</b>: Effect not supported for this device.
     * @return For other error codes, please refer to [Windows System Error Codes](https://msdn.microsoft.com/en-us/library/windows/desktop/ms681381(v=vs.85).aspx) from Windows SDK (WinError.h).
     */
    RZRESULT CreateKeyboardEffect(ChromaSDK::Keyboard::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);

    /*! 
     * Create mouse effect.
     * @param[in] Effect Mouse effect type.
     * @param[in] pParam Pointer to a parameter type specified by Effect.
     * @param[in,out] pEffectId Valid effect Id if successful. Set it to NULL if not required.
     * @return <b>RZRESULT_NOT_VALID_STATE</b>: Chroma SDK not in a valid state. Did you forgot to call Init()?
     * @return <b>RZRESULT_INVALID_PARAMETER</b>: Invalid parameter.
     * @return <b>RZRESULT_NOT_SUPPORTED</b>: Effect not supported for this device.
     * @return For other error codes, please refer to [Windows System Error Codes](https://msdn.microsoft.com/en-us/library/windows/desktop/ms681381(v=vs.85).aspx) from Windows SDK (WinError.h).
     */
    RZRESULT CreateMouseEffect(ChromaSDK::Mouse::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);

    /*! 
     * Create headset effect.
     * @param[in] Effect Headset effect type.
     * @param[in] pParam Pointer to a parameter type specified by Effect defined in RzChromaSDKTypes.h
     * @param[in,out] pEffectId Valid effect Id if successful. Set it to NULL if not required.
     * @return <b>RZRESULT_NOT_VALID_STATE</b>: Chroma SDK not in a valid state. Did you forgot to call Init()?
     * @return <b>RZRESULT_INVALID_PARAMETER</b>: Invalid parameter.
     * @return <b>RZRESULT_NOT_SUPPORTED</b>: Effect not supported for this device.
     * @return For other error codes, please refer to [Windows System Error Codes](https://msdn.microsoft.com/en-us/library/windows/desktop/ms681381(v=vs.85).aspx) from Windows SDK (WinError.h).
     */
    RZRESULT CreateHeadsetEffect(ChromaSDK::Headset::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);

    /*! 
     * Create mousepad effect.
     * @param[in] Effect Mousemat effect type.
     * @param[in] pParam Pointer to a parameter type specified by Effect defined in RzChromaSDKTypes.h
     * @param[in,out] pEffectId Valid effect Id if successful. Set it to NULL if not required.
     * @return <b>RZRESULT_NOT_VALID_STATE</b>: Chroma SDK not in a valid state. Did you forgot to call Init()?
     * @return <b>RZRESULT_INVALID_PARAMETER</b>: Invalid parameter.
     * @return <b>RZRESULT_NOT_SUPPORTED</b>: Effect not supported for this device.
     * @return For other error codes, please refer to [Windows System Error Codes](https://msdn.microsoft.com/en-us/library/windows/desktop/ms681381(v=vs.85).aspx) from Windows SDK (WinError.h).
     */
    RZRESULT CreateMousepadEffect(ChromaSDK::Mousepad::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);

    /*! 
     * Create keypad effect.
     * @param[in] Effect Keypad effect type.
     * @param[in] pParam Pointer to a parameter type specified by Effect defined in RzChromaSDKTypes.h
     * @param[in,out] pEffectId Valid effect Id if successful. Set it to NULL if not required.
     * @return <b>RZRESULT_NOT_VALID_STATE</b>: Chroma SDK not in a valid state. Did you forgot to call Init()?
     * @return <b>RZRESULT_INVALID_PARAMETER</b>: Invalid parameter.
     * @return <b>RZRESULT_NOT_SUPPORTED</b>: Effect not supported for this device.
     * @return For other error codes, please refer to [Windows System Error Codes](https://msdn.microsoft.com/en-us/library/windows/desktop/ms681381(v=vs.85).aspx) from Windows SDK (WinError.h).
     */
    RZRESULT CreateKeypadEffect(ChromaSDK::Keypad::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);

    /*!
    * Create effects on Chroma Linked devices.
    * @param[in] Effect Chroma Link effect type.
    * @param[in] pParam Pointer to a parameter type specified by Effect defined in RzChromaSDKTypes.h
    * @param[in,out] pEffectId Valid effect Id if successful. Set it to NULL if not required.
    * @return <b>RZRESULT_NOT_VALID_STATE</b>: Chroma SDK not in a valid state. Did you forgot to call Init()?
    * @return <b>RZRESULT_INVALID_PARAMETER</b>: Invalid parameter.
    * @return <b>RZRESULT_NOT_SUPPORTED</b>: Effect not supported for this device.
    * @return For other error codes, please refer to [Windows System Error Codes](https://msdn.microsoft.com/en-us/library/windows/desktop/ms681381(v=vs.85).aspx) from Windows SDK (WinError.h).
    */
    RZRESULT CreateChromaLinkEffect(ChromaSDK::ChromaLink::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);

    /*! 
     * Delete effect
     * @param[in] EffectId Id of the effect that needs to be deleted.
     * @return <b>RZRESULT_NOT_VALID_STATE</b>: Chroma SDK not in a valid state. Did you forgot to call Init()?
     * @return <b>RZRESULT_NOT_FOUND</b>: Effect Id not found.
     * @return For other error codes, please refer to [Windows System Error Codes](https://msdn.microsoft.com/en-us/library/windows/desktop/ms681381(v=vs.85).aspx) from Windows SDK (WinError.h).
     */
    RZRESULT DeleteEffect(RZEFFECTID EffectId);

    /*! 
     * Set effect.
     * @param[in] EffectId Id of the effect that needs to be set.
     * @return <b>RZRESULT_NOT_VALID_STATE</b>: Chroma SDK not in a valid state. Did you forgot to call Init()?
     * @return <b>RZRESULT_ACCESS_DENIED</b>: No permision to access device.
     * @return <b>RZRESULT_NOT_FOUND</b>: Effect Id not found.
     * @return For other error codes, please refer to [Windows System Error Codes](https://msdn.microsoft.com/en-us/library/windows/desktop/ms681381(v=vs.85).aspx) from Windows SDK (WinError.h).
     */
    RZRESULT SetEffect(RZEFFECTID EffectId);

    /*! 
     * Register for event notification.
     * @param[in] hWnd Application window handle.
     * @return <b>RZRESULT_NOT_VALID_STATE</b>: Chroma SDK not in a valid state. Did you forgot to call Init()?
     * @return <b>RZRESULT_ALREADY_INITIALIZED</b>: Invalid Windows handle.
     * @return <b>RZRESULT_INVALID_PARAMETER</b>: Invalid parameter.
     * @return For other error codes, please refer to [Windows System Error Codes](https://msdn.microsoft.com/en-us/library/windows/desktop/ms681381(v=vs.85).aspx) from Windows SDK (WinError.h).
     * @remark <b>WM_CHROMA_EVENT</b> will be sent if there is an event.
     * @remark 
     * - \c <b>wParam</b> = 1 : Chroma SDK support. 
     *  - \c <b>lParam</b> = 1 : Enabled.
     *  - \c <b>lParam</b> = 0 : Disabled. 
     * - \c <b>wParam</b> = 2 : Access to device.
     *  - \c <b>lParam</b> = 1 : Granted accessible.
     *  - \c <b>lParam</b> = 0 = Access revoked.
     * - \c <b>wParam</b> = 3 : Application state. 
     *  - \c <b>lParam</b> = 1 : Enabled.
     *  - \c <b>lParam</b> = 0 = Disabled.
     */
    RZRESULT RegisterEventNotification(HWND hWnd);

    /*!
     * Un-register for event notification.
     * @return <b>RZRESULT_NOT_VALID_STATE</b>: Internal state is not valid. There was no event registered.
     * @return For other error codes, please refer to [Windows System Error Codes](https://msdn.microsoft.com/en-us/library/windows/desktop/ms681381(v=vs.85).aspx) from Windows SDK (WinError.h).
     */
    RZRESULT UnregisterEventNotification();

    /*!
     * Query for device information.
     * @param[in] DeviceId Device id found in RzChromaSDKDefines.h
     * @param[out] DeviceInfo Contains device information specified by DeviceId.
     * @return <b>RZRESULT_DEVICE_NOT_AVAILABLE</b>: Device not supported.
     * @return For other error codes, please refer to [Windows System Error Codes](https://msdn.microsoft.com/en-us/library/windows/desktop/ms681381(v=vs.85).aspx) from Windows SDK (WinError.h).
     */
    RZRESULT QueryDevice(RZDEVICEID DeviceId, DEVICE_INFO_TYPE &DeviceInfo);

#ifdef __cplusplus
}
#endif

#endif
