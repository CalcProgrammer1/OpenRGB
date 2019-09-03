
//! \file RzErrors.h
//! \brief Error codes for Chroma SDK. If the error is not defined here, refer to WinError.h from the Windows SDK.

#ifndef _RZERRORS_H_
#define _RZERRORS_H_

#pragma once

// Error codes
//! Invalid
#define     RZRESULT_INVALID                    -1L
//! Success
#define     RZRESULT_SUCCESS                    0L
//! Access denied
#define     RZRESULT_ACCESS_DENIED              5L
//! Invalid handle
#define     RZRESULT_INVALID_HANDLE             6L
//! Not supported
#define     RZRESULT_NOT_SUPPORTED              50L
//! Invalid parameter.
#define     RZRESULT_INVALID_PARAMETER          87L
//! The service has not been started
#define     RZRESULT_SERVICE_NOT_ACTIVE         1062L
//! Cannot start more than one instance of the specified program.
#define     RZRESULT_SINGLE_INSTANCE_APP        1152L
//! Device not connected
#define     RZRESULT_DEVICE_NOT_CONNECTED       1167L
//! Element not found.
#define     RZRESULT_NOT_FOUND                  1168L
//! Request aborted.
#define     RZRESULT_REQUEST_ABORTED            1235L
//! An attempt was made to perform an initialization operation when initialization has already been completed.
#define     RZRESULT_ALREADY_INITIALIZED        1247L
//! Resource not available or disabled
#define     RZRESULT_RESOURCE_DISABLED          4309L
//! Device not available or supported
#define     RZRESULT_DEVICE_NOT_AVAILABLE       4319L
//! The group or resource is not in the correct state to perform the requested operation.
#define     RZRESULT_NOT_VALID_STATE            5023L
//! No more items
#define     RZRESULT_NO_MORE_ITEMS              259L
//! General failure.
#define     RZRESULT_FAILED                     2147500037L

#endif
