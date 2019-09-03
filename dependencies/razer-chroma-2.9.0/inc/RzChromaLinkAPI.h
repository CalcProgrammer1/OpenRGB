//! \file RzChromaLinkAPI.h
//! \brief Exported APIs.

#ifndef _RZCHROMALINKAPI_H_
#define _RZCHROMALINKAPI_H_

#pragma once

#include "RzErrors.h"
#include "RzChromaSDKTypes.h"

using namespace ChromaSDK;

// Callback funtion
typedef LONG(*LPCHROMALINKPROC)(ChromaLink::EFFECT_TYPE Effect, PRZPARAM pData);

// Exported functions
#ifdef __cplusplus
extern "C"
{
#endif
    /*!
    * Initialize Chroma Link API.
    * @return <b>RZRESULT_ALREADY_INITIALIZED</b>: Chroma Link API has already been initialized.
    * @return For other error codes, please refer to [Windows System Error Codes](https://msdn.microsoft.com/en-us/library/windows/desktop/ms681381(v=vs.85).aspx) from Windows SDK (WinError.h).
    */
    RZRESULT Init(ChromaSDK::APPINFOTYPE AppInfo);

    /*!
    * UnInitialize Chroma API.
    * @return For other error codes, please refer to [Windows System Error Codes](https://msdn.microsoft.com/en-us/library/windows/desktop/ms681381(v=vs.85).aspx) from Windows SDK (WinError.h).
    */
    RZRESULT UnInit(void);

    /*!
    * Register for Chroma Link effect events.
    * @param[in] lpFunc Callback function.
    * @return <b>ERROR_INVALID_PARAMETER</b>: lpFunc is invalid.
    * @return For other error codes, please refer to [Windows System Error Codes](https://msdn.microsoft.com/en-us/library/windows/desktop/ms681381(v=vs.85).aspx) from Windows SDK (WinError.h).
    * @remark 
    * - <b>CHROMA_NONE</b> : This event will be sent when there are no more Chroma Link events. 
    *                        Clients can resume their own lighting effects.
    * - <b>CHROMA_CUSTOM</b> : This event will be sent when there is a ChromaSDK::ChromaLink::CHROMA_CUSTOM effect event.
    *                          pData is casted to ChromaSDK::ChromaLink::CUSTOM_EFFECT_TYPE.
    *                          The colors of each (5) LEDs can be accessed from the array.
    * - <b>CHROMA_STATIC</b> : This event will be sent when there is a ChromaSDK::ChromaLink::CHROMA_STATIC effect event.
    *                          pData is casted to ChromaSDK::ChromaLink::STATIC_EFFECT_TYPE.
    *                          The color can be accessed from the structure.
    */
    RZRESULT RegisterEventNotification(LPCHROMALINKPROC lpFunc);

    /*!
    * Register for Chroma Link effect events.
    * @return For other error codes, please refer to [Windows System Error Codes](https://msdn.microsoft.com/en-us/library/windows/desktop/ms681381(v=vs.85).aspx) from Windows SDK (WinError.h).
    */
    RZRESULT UnregisterEventNotification(void);

#ifdef __cplusplus
}
#endif

#endif  // _RZCHROMALINKAPI_H_
