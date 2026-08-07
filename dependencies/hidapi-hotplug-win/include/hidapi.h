/*******************************************************
 HIDAPI - Multi-Platform library for
 communication with HID devices.

 Alan Ott
 Signal 11 Software

 libusb/hidapi Team

 Copyright 2023, All Rights Reserved.

 At the discretion of the user of this library,
 this software may be licensed under the terms of the
 GNU General Public License v3, a BSD-Style license, or the
 original HIDAPI license as outlined in the LICENSE.txt,
 LICENSE-gpl3.txt, LICENSE-bsd.txt, and LICENSE-orig.txt
 files located at the root of the source distribution.
 These files may also be found in the public source
 code repository located at:
        https://github.com/libusb/hidapi .
********************************************************/

/** @file
 * @defgroup API hidapi API
 */

#ifndef HIDAPI_H__
#define HIDAPI_H__

#include <wchar.h>

/* #480: this is to be refactored properly for v1.0 */
#ifdef _WIN32
   #ifndef HID_API_NO_EXPORT_DEFINE
      #define HID_API_EXPORT __declspec(dllexport)
   #endif
#endif
#ifndef HID_API_EXPORT
   #define HID_API_EXPORT /**< API export macro */
#endif
/* To be removed in v1.0 */
#define HID_API_CALL /**< API call macro */

#define HID_API_EXPORT_CALL HID_API_EXPORT HID_API_CALL /**< API export and call macro*/

/** @brief Static/compile-time major version of the library.

	@ingroup API
*/
#define HID_API_VERSION_MAJOR 0
/** @brief Static/compile-time minor version of the library.

	@ingroup API
*/
#define HID_API_VERSION_MINOR 15
/** @brief Static/compile-time patch version of the library.

	@ingroup API
*/
#define HID_API_VERSION_PATCH 0

/* Helper macros */
#define HID_API_AS_STR_IMPL(x) #x
#define HID_API_AS_STR(x) HID_API_AS_STR_IMPL(x)
#define HID_API_TO_VERSION_STR(v1, v2, v3) HID_API_AS_STR(v1.v2.v3)

/** @brief Coverts a version as Major/Minor/Patch into a number:
	<8 bit major><16 bit minor><8 bit patch>.

	This macro was added in version 0.12.0.

	Convenient function to be used for compile-time checks, like:
	@code{.c}
	#if HID_API_VERSION >= HID_API_MAKE_VERSION(0, 12, 0)
	@endcode

	@ingroup API
*/
#define HID_API_MAKE_VERSION(mj, mn, p) (((mj) << 24) | ((mn) << 8) | (p))

/** @brief Static/compile-time version of the library.

	This macro was added in version 0.12.0.

	@see @ref HID_API_MAKE_VERSION.

	@ingroup API
*/
#define HID_API_VERSION HID_API_MAKE_VERSION(HID_API_VERSION_MAJOR, HID_API_VERSION_MINOR, HID_API_VERSION_PATCH)

/** @brief Static/compile-time string version of the library.

	@ingroup API
*/
#define HID_API_VERSION_STR HID_API_TO_VERSION_STR(HID_API_VERSION_MAJOR, HID_API_VERSION_MINOR, HID_API_VERSION_PATCH)

/** @brief Maximum expected HID Report descriptor size in bytes.

	Since version 0.13.0, @ref HID_API_VERSION >= HID_API_MAKE_VERSION(0, 13, 0)

	@ingroup API
*/
#define HID_API_MAX_REPORT_DESCRIPTOR_SIZE 4096

#ifdef __cplusplus
extern "C" {
#endif
		/** A structure to hold the version numbers. */
		struct hid_api_version {
			int major; /**< major version number */
			int minor; /**< minor version number */
			int patch; /**< patch version number */
		};

		struct hid_device_;
		typedef struct hid_device_ hid_device; /**< opaque hidapi structure */

		/** @brief HID underlying bus types.

			@ingroup API
		*/
		typedef enum {
			/** Unknown bus type */
			HID_API_BUS_UNKNOWN = 0x00,

			/** USB bus
			   Specifications:
			   https://usb.org/hid */
			HID_API_BUS_USB = 0x01,

			/** Bluetooth or Bluetooth LE bus
			   Specifications:
			   https://www.bluetooth.com/specifications/specs/human-interface-device-profile-1-1-1/
			   https://www.bluetooth.com/specifications/specs/hid-service-1-0/
			   https://www.bluetooth.com/specifications/specs/hid-over-gatt-profile-1-0/ */
			HID_API_BUS_BLUETOOTH = 0x02,

			/** I2C bus
			   Specifications:
			   https://docs.microsoft.com/previous-versions/windows/hardware/design/dn642101(v=vs.85) */
			HID_API_BUS_I2C = 0x03,

			/** SPI bus
			   Specifications:
			   https://www.microsoft.com/download/details.aspx?id=103325 */
			HID_API_BUS_SPI = 0x04,

			/** Virtual device
			    E.g.: https://elixir.bootlin.com/linux/v4.0/source/include/uapi/linux/input.h#L955 
			    
			    Since version 0.16.0, @ref HID_API_VERSION >= HID_API_MAKE_VERSION(0, 16, 0)
			*/
			HID_API_BUS_VIRTUAL = 0x05,
		} hid_bus_type;

		/** hidapi info structure */
		struct hid_device_info {
			/** Platform-specific device path */
			char *path;
			/** Device Vendor ID */
			unsigned short vendor_id;
			/** Device Product ID */
			unsigned short product_id;
			/** Serial Number */
			wchar_t *serial_number;
			/** Device Release Number in binary-coded decimal,
			    also known as Device Version Number */
			unsigned short release_number;
			/** Manufacturer String */
			wchar_t *manufacturer_string;
			/** Product string */
			wchar_t *product_string;
			/** Usage Page for this Device/Interface
			    (Windows/Mac/hidraw only) */
			unsigned short usage_page;
			/** Usage for this Device/Interface
			    (Windows/Mac/hidraw only) */
			unsigned short usage;
			/** The USB interface which this logical device
			    represents.

			    Valid only if the device is a USB HID device.
			    Set to -1 in all other cases.
			*/
			int interface_number;

			/** Pointer to the next device */
			struct hid_device_info *next;

			/** Underlying bus type
			    Since version 0.13.0, @ref HID_API_VERSION >= HID_API_MAKE_VERSION(0, 13, 0)
			*/
			hid_bus_type bus_type;
		};


		/** @brief Initialize the HIDAPI library.

			This function initializes the HIDAPI library. Calling it is not
			strictly necessary, as it will be called automatically by
			hid_enumerate() and any of the hid_open_*() functions if it is
			needed.  This function should be called at the beginning of
			execution however, if there is a chance of HIDAPI handles
			being opened by different threads simultaneously.

			@ingroup API

			@returns
				This function returns 0 on success and -1 on error.
				Call hid_error(NULL) to get the failure reason.
		*/
		int HID_API_EXPORT HID_API_CALL hid_init(void);

		/** @brief Finalize the HIDAPI library.

			This function frees all of the static data associated with
			HIDAPI. It should be called at the end of execution to avoid
			memory leaks.

			@ingroup API

			@returns
				This function returns 0 on success and -1 on error.
		*/
		int HID_API_EXPORT HID_API_CALL hid_exit(void);

		/** @brief Enumerate the HID Devices.

			This function returns a linked list of all the HID devices
			attached to the system which match vendor_id and product_id.
			If @p vendor_id is set to 0 then any vendor matches.
			If @p product_id is set to 0 then any product matches.
			If @p vendor_id and @p product_id are both set to 0, then
			all HID devices will be returned.

			@ingroup API
			@param vendor_id The Vendor ID (VID) of the types of device
				to open.
			@param product_id The Product ID (PID) of the types of
				device to open.

			@returns
				This function returns a pointer to a linked list of type
				struct #hid_device_info, containing information about the HID devices
				attached to the system,
				or NULL in the case of failure or if no HID devices present in the system.
				Call hid_error(NULL) to get the failure reason.

			@note The returned value by this function must to be freed by calling hid_free_enumeration(),
			      when not needed anymore.
		*/
		struct hid_device_info HID_API_EXPORT * HID_API_CALL hid_enumerate(unsigned short vendor_id, unsigned short product_id);

		/** @brief Free an enumeration Linked List

			This function frees a linked list created by hid_enumerate().

			@ingroup API
			@param devs Pointer to a list of struct_device returned from
			            hid_enumerate().
		*/
		void  HID_API_EXPORT HID_API_CALL hid_free_enumeration(struct hid_device_info *devs);

		/** @brief Callback handle.

			Since version 0.16.0, @ref HID_API_VERSION >= HID_API_MAKE_VERSION(0, 16, 0)

			Callback handles are generated by hid_hotplug_register_callback()
			and can be used to deregister callbacks. Callback handles are
			unique and are not reused while the library remains initialized,
			so it is safe to call hid_hotplug_deregister_callback() on an
			already deregistered callback: it fails with -1 and never
			affects another callback.

			A valid callback handle is always a positive value;
			0 is never a valid handle and may be used as a sentinel.

			@ingroup API
		 */
		typedef int hid_hotplug_callback_handle;

		/** @brief Hotplug events.

			Since version 0.16.0, @ref HID_API_VERSION >= HID_API_MAKE_VERSION(0, 16, 0)

			@ingroup API
		*/
		typedef enum {
			/** A device has been plugged in and may be opened.
				Opening can still fail, e.g. due to insufficient permissions
				or if the device has already disconnected again. */
			HID_API_HOTPLUG_EVENT_DEVICE_ARRIVED = (1 << 0),

			/** A device has left and is no longer available.
				If the application holds an open handle to the device, it is
				still the application's responsibility to close it with
				hid_close().
			*/
			HID_API_HOTPLUG_EVENT_DEVICE_LEFT = (1 << 1)
		} hid_hotplug_event;

		/** @brief Hotplug flags.

			Since version 0.16.0, @ref HID_API_VERSION >= HID_API_MAKE_VERSION(0, 16, 0)

			@ingroup API
		*/
		typedef enum {
			/** Arm the callback and fire it for all matching devices that are
				already connected at registration time.

				hid_hotplug_register_callback() takes a snapshot of the
				matching connected devices, and the callback is invoked
				asynchronously, on the same internal event context that
				delivers live events (see #hid_hotplug_callback_fn), once
				for each device in that snapshot. Devices whose arrival is
				detected after the snapshot is taken are reported as regular
				live events: each device connection is reported to the
				callback exactly once - either by the initial pass or as a
				live event, never both and never neither. (A device that
				disconnects and reconnects is a new connection and is
				reported again.)

				The initial pass is delivered before any live events for
				this callback. In particular, a callback registered with
				this flag for both event types never observes a
				#HID_API_HOTPLUG_EVENT_DEVICE_LEFT for a matching device
				whose arrival it has not been told about first.

				These synthetic "arrived" events are delivered only when
				#HID_API_HOTPLUG_EVENT_DEVICE_ARRIVED is present in the
				requested events mask.

				hid_hotplug_register_callback() returning does not imply
				the initial pass has been delivered yet: the synthetic
				events may fire before or after it returns. */
			HID_API_HOTPLUG_ENUMERATE = (1 << 0)
		} hid_hotplug_flag;

		/** @brief Hotplug callback function type.

			Since version 0.16.0, @ref HID_API_VERSION >= HID_API_MAKE_VERSION(0, 16, 0)

			Called by HIDAPI when a device matching the registration filter
			is connected or disconnected. See #hid_hotplug_register_callback.

			@par Execution context

			The callback is only ever invoked on HIDAPI's internal event
			context, never on an application thread (including the
			application's main thread). This includes the synthetic
			"arrived" events requested with #HID_API_HOTPLUG_ENUMERATE:
			they are delivered asynchronously on that same context and are
			never delivered from within the hid_hotplug_register_callback()
			call itself. (When a hotplug callback itself registers a new
			callback with #HID_API_HOTPLUG_ENUMERATE, the new registration's
			synthetic events are delivered later, on this same event
			context.)

			Every callback invocation holds an internal hotplug mutex for
			its duration: any other thread calling
			hid_hotplug_register_callback() or
			hid_hotplug_deregister_callback() will block until the callback
			returns. The mutex is re-entrant: calling those functions from
			within the callback itself (i.e. on the event context, which
			already holds the mutex) does not block and cannot deadlock -
			that is what makes them safe to call from inside the callback
			(see below). Keep the callback short.

			When multiple callbacks are registered, each event is delivered
			to every matching callback sequentially, in the order the
			callbacks were registered.

			@par What the callback may call

			The hotplug API itself is thread-safe (see "Thread safety" under
			#hid_hotplug_register_callback) and the following calls are always
			safe from within the callback:

			  - hid_hotplug_register_callback()
			  - hid_hotplug_deregister_callback()   (including on its own handle)
			  - hid_error(dev)   with a non-NULL device handle, provided no
			                     other thread uses that same handle concurrently

			HIDAPI calls made from within the callback do not update the
			global error string: the callback runs on HIDAPI's internal
			event context, and internal contexts never write that string
			(an application has no way to serialize against them, so writing
			it there would be a use-after-free waiting to happen). Failures
			are still reported through return values as usual, and
			hid_error(dev) still works for a device handle - only
			hid_error(NULL) is left untouched by calls made from the
			callback.

			Any other HIDAPI function follows HIDAPI's general thread-safety
			rule (see the Multi-threading Notes in the project wiki): it is
			the application's responsibility to serialize hid_init / hid_exit /
			hid_enumerate / hid_open* / hid_close / hid_error(NULL) across all
			threads, including the hotplug callback thread (hid_exit()
			additionally must never be called from within the callback
			itself - see below). If your application
			already calls those functions only from one thread, calling them
			from the hotplug callback adds a second thread and is therefore
			UNSAFE unless the application adds synchronisation itself. The
			recommended pattern is to copy the needed fields of @p device out
			of the callback and handle open/close on your own thread.

			Calling hid_exit() from within the callback has undefined behavior:
			hid_exit() joins the hotplug thread, which would be joining itself.

			@par The device parameter

			The @p device pointer is owned by HIDAPI and is valid only for the
			duration of the call. To keep any of its fields (path,
			serial_number, manufacturer_string, etc.) beyond the callback,
			copy them out; pointer fields must be deep-copied, as all strings
			are owned by HIDAPI.

			The @p device->next pointer is always NULL. Each callback
			invocation describes exactly one device; compound or composite
			devices that expose multiple interfaces produce multiple callback
			invocations (typically delivered in quick succession).

			For #HID_API_HOTPLUG_EVENT_DEVICE_LEFT events @p device points to
			a copy captured when the device arrived (or was enumerated): all
			fields, including the strings, are valid and describe the device
			as it was while connected. A "left" event is delivered for any
			matching device that disconnects while the callback is
			registered, including devices that were already connected
			before the registration (their arrival is reported to this
			callback only if #HID_API_HOTPLUG_ENUMERATE was used). When the
			callback has observed the device's arrival, the path field
			matches the one reported then and may be used to correlate the
			two events.

			@par Return value

			Return 0 to keep the callback registered. Return any non-zero
			value to have HIDAPI deregister the callback; no further events
			for this handle will be delivered, and the handle is freed as if
			hid_hotplug_deregister_callback() had been called. This applies
			to both live events and to the synthetic "arrived" events of
			#HID_API_HOTPLUG_ENUMERATE: a non-zero return during the
			initial pass stops the remainder of that pass and deregisters
			the callback.

			@ingroup API

			@param callback_handle The handle of this callback.
			@param device The hid_device_info of the device this event occurred on.
			@param event Event that occurred: exactly one value (not a mask)
				of \ref hid_hotplug_event.
			@param user_data User data provided when this callback was registered
				(may be NULL).

			@returns
				0 to stay registered; any non-zero value to be deregistered.
		 */
		typedef int (HID_API_CALL *hid_hotplug_callback_fn)(
			hid_hotplug_callback_handle callback_handle,
			struct hid_device_info *device,
			hid_hotplug_event event,
			void *user_data);

		/** @brief Register a HID hotplug callback function.

			Since version 0.16.0, @ref HID_API_VERSION >= HID_API_MAKE_VERSION(0, 16, 0)

			If @p vendor_id is set to 0 then any vendor matches.
			If @p product_id is set to 0 then any product matches.
			If @p vendor_id and @p product_id are both set to 0, then all HID devices will be notified.

			If HIDAPI is not initialized yet, this function initializes it
			implicitly (as if by hid_init()). On some backends this binds
			HIDAPI's device-monitoring facilities to the calling thread (for
			example, the macOS backend schedules its run loop there). An
			application that cares which thread owns those facilities should
			call hid_init() explicitly from that thread first, rather than
			relying on the implicit initialization performed here.

			When #HID_API_HOTPLUG_ENUMERATE is set, the synthetic "arrived"
			events are delivered asynchronously on HIDAPI's internal event
			context (see #hid_hotplug_flag): they may fire before or after
			this function returns, but never from within this call itself,
			and never on an application thread.
			@p callback_handle (when non-NULL) is written before any events
			can be delivered, and the callback always receives its own
			handle as a parameter, so the callback may deregister itself
			even during that initial pass.

			@par Thread safety

			hid_hotplug_register_callback() and hid_hotplug_deregister_callback()
			are thread-safe with respect to each other and to HIDAPI's
			internal hotplug machinery. They may be called from any thread,
			including from within a hotplug callback. This is a deliberate
			exception to HIDAPI's general "not thread-safe" rule (see the
			Multi-threading Notes in the project wiki).

			The one caveat is the global error string: on failure these two
			functions set it, like every other HIDAPI function that reports
			an error via hid_error(NULL). They therefore have to be
			serialized against hid_error(NULL) - which the application is
			already required to serialize across all threads - even though
			they need no serialization against each other. HIDAPI's own
			internal threads never write the global error string, so an
			application that serializes its own hid_error(NULL) calls
			against its other HIDAPI calls is safe.

			The first successful call to hid_hotplug_register_callback()
			starts HIDAPI's internal hotplug machinery (on most platforms an
			internal thread), which runs until either (a) the last callback
			is deregistered, or (b) hid_exit() is called. hid_exit()
			deregisters any callbacks that are still registered and
			invalidates their handles. hid_exit() must not be called from
			within a hotplug callback (see #hid_hotplug_callback_fn).

			@ingroup API

			@param vendor_id The Vendor ID (VID) of the types of device to notify about.
			@param product_id The Product ID (PID) of the types of device to notify about.
			@param events Bitwise or of hotplug events that will trigger this callback.
				See \ref hid_hotplug_event.
			@param flags Bitwise or of hotplug flags that affect registration.
				See \ref hid_hotplug_flag.
			@param callback The callback function that will be called on device connection/disconnection.
				See \ref hid_hotplug_callback_fn.
			@param user_data The user data you wanted to provide to your callback function.
			@param callback_handle Pointer to store the handle of the allocated callback
				(Optionally NULL). On failure, *callback_handle (when
				non-NULL) is set to 0.

			@returns
				This function returns 0 on success or -1 on error.
				Call hid_error(NULL) to get the failure reason.
				Registration fails if @p callback is NULL, if @p events
				contains no valid #hid_hotplug_event bit, or if @p events
				or @p flags contain unknown bits. When more than one argument
				is invalid, which one the failure string names is
				unspecified and may differ between backends; only the -1
				return and the zeroed @p callback_handle are guaranteed.

			@note On backends without hotplug support (e.g. NetBSD)
				this function always returns -1.
		*/
		int HID_API_EXPORT HID_API_CALL hid_hotplug_register_callback(unsigned short vendor_id, unsigned short product_id, int events, int flags, hid_hotplug_callback_fn callback, void *user_data, hid_hotplug_callback_handle *callback_handle);

		/** @brief Deregister a callback from a HID hotplug.

			Since version 0.16.0, @ref HID_API_VERSION >= HID_API_MAKE_VERSION(0, 16, 0)

			Thread-safe. May be called from any thread, including from within
			a hotplug callback (on its own handle or on another callback's
			handle). Calling it on a handle that was already deregistered,
			or on a handle that was never valid, is safe: it has no effect
			and returns -1.

			When called from any thread other than HIDAPI's internal event
			context, this function does not return until an in-progress
			invocation of the callback (if any) has completed; once it
			returns, the callback will not be invoked again - including any
			undelivered synthetic events of #HID_API_HOTPLUG_ENUMERATE -
			and it is safe to release any resources the callback uses
			(e.g. whatever @p user_data points to). When called from within
			a hotplug callback, the deregistered callback will not be
			invoked once the currently executing invocation returns.

			See "Thread safety" on #hid_hotplug_register_callback for the
			full thread-safety contract of the hotplug API.

			@ingroup API

			@param callback_handle	The handle of the callback to deregister.

			@returns
				This function returns 0 when the callback was found and
				deregistered, or -1 on error (including when
				@p callback_handle is not a registered handle).
		*/
		int HID_API_EXPORT HID_API_CALL hid_hotplug_deregister_callback(hid_hotplug_callback_handle callback_handle);

		/** @brief Open a HID device using a Vendor ID (VID), Product ID
			(PID) and optionally a serial number.

			If @p serial_number is NULL, the first device with the
			specified VID and PID is opened.

			@ingroup API
			@param vendor_id The Vendor ID (VID) of the device to open.
			@param product_id The Product ID (PID) of the device to open.
			@param serial_number The Serial Number of the device to open
			                     (Optionally NULL).

			@returns
				This function returns a pointer to a #hid_device object on
				success or NULL on failure.
				Call hid_error(NULL) to get the failure reason.

			@note The returned object must be freed by calling hid_close(),
			      when not needed anymore.
		*/
		HID_API_EXPORT hid_device * HID_API_CALL hid_open(unsigned short vendor_id, unsigned short product_id, const wchar_t *serial_number);

		/** @brief Open a HID device by its path name.

			The path name be determined by calling hid_enumerate(), or a
			platform-specific path name can be used (eg: /dev/hidraw0 on
			Linux).

			@ingroup API
			@param path The path name of the device to open

			@returns
				This function returns a pointer to a #hid_device object on
				success or NULL on failure.
				Call hid_error(NULL) to get the failure reason.

			@note The returned object must be freed by calling hid_close(),
			      when not needed anymore.
		*/
		HID_API_EXPORT hid_device * HID_API_CALL hid_open_path(const char *path);

		/** @brief Write an Output report to a HID device.

			The first byte of @p data[] must contain the Report ID. For
			devices which only support a single report, this must be set
			to 0x0. The remaining bytes contain the report data. Since
			the Report ID is mandatory, calls to hid_write() will always
			contain one more byte than the report contains. For example,
			if a hid report is 16 bytes long, 17 bytes must be passed to
			hid_write(), the Report ID (or 0x0, for devices with a
			single report), followed by the report data (16 bytes). In
			this example, the length passed in would be 17.

			hid_write() will send the data on the first interrupt OUT 
			endpoint, if one exists. If it does not the behaviour is as 
			@ref hid_send_output_report

			@ingroup API
			@param dev A device handle returned from hid_open().
			@param data The data to send, including the report number as
				the first byte.
			@param length The length in bytes of the data to send.

			@returns
				This function returns the actual number of bytes written and
				-1 on error.
				Call hid_error(dev) to get the failure reason.
		*/
		int  HID_API_EXPORT HID_API_CALL hid_write(hid_device *dev, const unsigned char *data, size_t length);

		/** @brief Read an Input report from a HID device with timeout.

			Input reports are returned
			to the host through the INTERRUPT IN endpoint. The first byte will
			contain the Report number if the device uses numbered reports.

			@ingroup API
			@param dev A device handle returned from hid_open().
			@param data A buffer to put the read data into.
			@param length The number of bytes to read. For devices with
				multiple reports, make sure to read an extra byte for
				the report number.
			@param milliseconds timeout in milliseconds or -1 for blocking wait.

			@returns
				This function returns the actual number of bytes read and
				-1 on error.
				Call hid_read_error(dev) to get the failure reason.
				If no packet was available to be read within
				the timeout period, this function returns 0.

			@note This function doesn't change the buffer returned by the hid_error(dev).
		*/
		int HID_API_EXPORT HID_API_CALL hid_read_timeout(hid_device *dev, unsigned char *data, size_t length, int milliseconds);

		/** @brief Read an Input report from a HID device.

			Input reports are returned
			to the host through the INTERRUPT IN endpoint. The first byte will
			contain the Report number if the device uses numbered reports.

			@ingroup API
			@param dev A device handle returned from hid_open().
			@param data A buffer to put the read data into.
			@param length The number of bytes to read. For devices with
				multiple reports, make sure to read an extra byte for
				the report number.

			@returns
				This function returns the actual number of bytes read and
				-1 on error.
				Call hid_read_error(dev) to get the failure reason.
				If no packet was available to be read and
				the handle is in non-blocking mode, this function returns 0.

			@note This function doesn't change the buffer returned by the hid_error(dev).
		*/
		int  HID_API_EXPORT HID_API_CALL hid_read(hid_device *dev, unsigned char *data, size_t length);

		/** @brief Get a string describing the last error which occurred during hid_read/hid_read_timeout.

			Since version 0.15.0, @ref HID_API_VERSION >= HID_API_MAKE_VERSION(0, 15, 0)

			This function is intended for logging/debugging purposes.

			This function guarantees to never return NULL for a valid @ref dev.
			If there was no error in the last call to hid_read/hid_read_error -
			the returned string clearly indicates that.

			Strings returned from hid_read_error() must not be freed by the user,
			i.e. owned by HIDAPI library.
			Device-specific error string may remain allocated at most until hid_close() is called.

			@ingroup API
			@param dev A device handle. Shall never be NULL.

			@returns
				A string describing the hid_read/hid_read_timeout error (if any).
		*/
		HID_API_EXPORT const wchar_t* HID_API_CALL hid_read_error(hid_device *dev);

		/** @brief Set the device handle to be non-blocking.

			In non-blocking mode calls to hid_read() will return
			immediately with a value of 0 if there is no data to be
			read. In blocking mode, hid_read() will wait (block) until
			there is data to read before returning.

			Nonblocking can be turned on and off at any time.

			@ingroup API
			@param dev A device handle returned from hid_open().
			@param nonblock enable or not the nonblocking reads
			 - 1 to enable nonblocking
			 - 0 to disable nonblocking.

			@returns
				This function returns 0 on success and -1 on error.
				Call hid_error(dev) to get the failure reason.
		*/
		int  HID_API_EXPORT HID_API_CALL hid_set_nonblocking(hid_device *dev, int nonblock);

		/** @brief Send a Feature report to the device.

			Feature reports are sent over the Control endpoint as a
			Set_Report transfer.  The first byte of @p data[] must
			contain the Report ID. For devices which only support a
			single report, this must be set to 0x0. The remaining bytes
			contain the report data. Since the Report ID is mandatory,
			calls to hid_send_feature_report() will always contain one
			more byte than the report contains. For example, if a hid
			report is 16 bytes long, 17 bytes must be passed to
			hid_send_feature_report(): the Report ID (or 0x0, for
			devices which do not use numbered reports), followed by the
			report data (16 bytes). In this example, the length passed
			in would be 17.

			@ingroup API
			@param dev A device handle returned from hid_open().
			@param data The data to send, including the report number as
				the first byte.
			@param length The length in bytes of the data to send, including
				the report number.

			@returns
				This function returns the actual number of bytes written and
				-1 on error.
				Call hid_error(dev) to get the failure reason.
		*/
		int HID_API_EXPORT HID_API_CALL hid_send_feature_report(hid_device *dev, const unsigned char *data, size_t length);

		/** @brief Get a feature report from a HID device.

			Set the first byte of @p data[] to the Report ID of the
			report to be read.  Make sure to allow space for this
			extra byte in @p data[]. Upon return, the first byte will
			still contain the Report ID, and the report data will
			start in data[1].

			@ingroup API
			@param dev A device handle returned from hid_open().
			@param data A buffer to put the read data into, including
				the Report ID. Set the first byte of @p data[] to the
				Report ID of the report to be read, or set it to zero
				if your device does not use numbered reports.
			@param length The number of bytes to read, including an
				extra byte for the report ID. The buffer can be longer
				than the actual report.

			@returns
				This function returns the number of bytes read plus
				one for the report ID (which is still in the first
				byte), or -1 on error.
				Call hid_error(dev) to get the failure reason.
		*/
		int HID_API_EXPORT HID_API_CALL hid_get_feature_report(hid_device *dev, unsigned char *data, size_t length);

		/** @brief Send a Output report to the device.

			Since version 0.15.0, @ref HID_API_VERSION >= HID_API_MAKE_VERSION(0, 15, 0)

			Output reports are sent over the Control endpoint as a
			Set_Report transfer.  The first byte of @p data[] must
			contain the Report ID. For devices which only support a
			single report, this must be set to 0x0. The remaining bytes
			contain the report data. Since the Report ID is mandatory,
			calls to hid_send_output_report() will always contain one
			more byte than the report contains. For example, if a hid
			report is 16 bytes long, 17 bytes must be passed to
			hid_send_output_report(): the Report ID (or 0x0, for
			devices which do not use numbered reports), followed by the
			report data (16 bytes). In this example, the length passed
			in would be 17.

			This function sets the return value of hid_error().

			@ingroup API
			@param dev A device handle returned from hid_open().
			@param data The data to send, including the report number as
				the first byte.
			@param length The length in bytes of the data to send, including
				the report number.

			@returns
				This function returns the actual number of bytes written and
				-1 on error.

			@see @ref hid_write
		*/
		int HID_API_EXPORT HID_API_CALL hid_send_output_report(hid_device* dev, const unsigned char* data, size_t length);

		/** @brief Get a input report from a HID device.

			Since version 0.10.0, @ref HID_API_VERSION >= HID_API_MAKE_VERSION(0, 10, 0)

			Set the first byte of @p data[] to the Report ID of the
			report to be read. Make sure to allow space for this
			extra byte in @p data[]. Upon return, the first byte will
			still contain the Report ID, and the report data will
			start in data[1].

			@ingroup API
			@param dev A device handle returned from hid_open().
			@param data A buffer to put the read data into, including
				the Report ID. Set the first byte of @p data[] to the
				Report ID of the report to be read, or set it to zero
				if your device does not use numbered reports.
			@param length The number of bytes to read, including an
				extra byte for the report ID. The buffer can be longer
				than the actual report.

			@returns
				This function returns the number of bytes read plus
				one for the report ID (which is still in the first
				byte), or -1 on error.
				Call hid_error(dev) to get the failure reason.
		*/
		int HID_API_EXPORT HID_API_CALL hid_get_input_report(hid_device *dev, unsigned char *data, size_t length);

		/** @brief Close a HID device.

			@ingroup API
			@param dev A device handle returned from hid_open().
		*/
		void HID_API_EXPORT HID_API_CALL hid_close(hid_device *dev);

		/** @brief Get The Manufacturer String from a HID device.

			@ingroup API
			@param dev A device handle returned from hid_open().
			@param string A wide string buffer to put the data into.
			@param maxlen The length of the buffer in multiples of wchar_t.

			@returns
				This function returns 0 on success and -1 on error.
				Call hid_error(dev) to get the failure reason.
		*/
		int HID_API_EXPORT_CALL hid_get_manufacturer_string(hid_device *dev, wchar_t *string, size_t maxlen);

		/** @brief Get The Product String from a HID device.

			@ingroup API
			@param dev A device handle returned from hid_open().
			@param string A wide string buffer to put the data into.
			@param maxlen The length of the buffer in multiples of wchar_t.

			@returns
				This function returns 0 on success and -1 on error.
				Call hid_error(dev) to get the failure reason.
		*/
		int HID_API_EXPORT_CALL hid_get_product_string(hid_device *dev, wchar_t *string, size_t maxlen);

		/** @brief Get The Serial Number String from a HID device.

			@ingroup API
			@param dev A device handle returned from hid_open().
			@param string A wide string buffer to put the data into.
			@param maxlen The length of the buffer in multiples of wchar_t.

			@returns
				This function returns 0 on success and -1 on error.
				Call hid_error(dev) to get the failure reason.
		*/
		int HID_API_EXPORT_CALL hid_get_serial_number_string(hid_device *dev, wchar_t *string, size_t maxlen);

		/** @brief Get The struct #hid_device_info from a HID device.

			Since version 0.13.0, @ref HID_API_VERSION >= HID_API_MAKE_VERSION(0, 13, 0)

			@ingroup API
			@param dev A device handle returned from hid_open().

			@returns
				This function returns a pointer to the struct #hid_device_info
				for this hid_device, or NULL in the case of failure.
				Call hid_error(dev) to get the failure reason.
				This struct is valid until the device is closed with hid_close().

			@note The returned object is owned by the @p dev, and SHOULD NOT be freed by the user.
		*/
		struct hid_device_info HID_API_EXPORT * HID_API_CALL hid_get_device_info(hid_device *dev);

		/** @brief Get a string from a HID device, based on its string index.

			@ingroup API
			@param dev A device handle returned from hid_open().
			@param string_index The index of the string to get.
			@param string A wide string buffer to put the data into.
			@param maxlen The length of the buffer in multiples of wchar_t.

			@returns
				This function returns 0 on success and -1 on error.
				Call hid_error(dev) to get the failure reason.
		*/
		int HID_API_EXPORT_CALL hid_get_indexed_string(hid_device *dev, int string_index, wchar_t *string, size_t maxlen);

		/** @brief Get a report descriptor from a HID device.

			Since version 0.14.0, @ref HID_API_VERSION >= HID_API_MAKE_VERSION(0, 14, 0)

			User has to provide a preallocated buffer where descriptor will be copied to.
			The recommended size for preallocated buffer is @ref HID_API_MAX_REPORT_DESCRIPTOR_SIZE bytes.

			@ingroup API
			@param dev A device handle returned from hid_open().
			@param buf The buffer to copy descriptor into.
			@param buf_size The size of the buffer in bytes.

			@returns
				This function returns non-negative number of bytes actually copied, or -1 on error.
		*/
		int HID_API_EXPORT_CALL hid_get_report_descriptor(hid_device *dev, unsigned char *buf, size_t buf_size);

		/** @brief Get a string describing the last error which occurred.

			This function is intended for logging/debugging purposes.

			This function guarantees to never return NULL.
			If there was no error in the last function call -
			the returned string clearly indicates that.

			Any HIDAPI function that can explicitly indicate an execution failure
			(e.g. by an error code, or by returning NULL) - may set the error string,
			to be returned by this function.

			Strings returned from hid_error() must not be freed by the user,
			i.e. owned by HIDAPI library.
			Device-specific error string may remain allocated at most until hid_close() is called.
			Global error string may remain allocated at most until hid_exit() is called.

			@ingroup API
			@param dev A device handle returned from hid_open(),
			  or NULL to get the last non-device-specific error
			  (e.g. for errors in hid_open() or hid_enumerate()).

			@returns
				A string describing the last error (if any).
		*/
		HID_API_EXPORT const wchar_t* HID_API_CALL hid_error(hid_device *dev);

		/** @brief Get a runtime version of the library.

			This function is thread-safe.

			@ingroup API

			@returns
				Pointer to statically allocated struct, that contains version.
		*/
		HID_API_EXPORT const  struct hid_api_version* HID_API_CALL hid_version(void);


		/** @brief Get a runtime version string of the library.

			This function is thread-safe.

			@ingroup API

			@returns
				Pointer to statically allocated string, that contains version string.
		*/
		HID_API_EXPORT const char* HID_API_CALL hid_version_str(void);

#ifdef __cplusplus
}
#endif

#endif
