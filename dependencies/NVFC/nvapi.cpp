#ifdef _WIN32
    #define _WIN32_LEAN_AND_MEAN
    #include <windows.h>
#elif __linux__
    #include <dlfcn.h>
#endif

#include "nvapi.h"
#include <string.h>

typedef void * (*nvapi_QueryInterface_t)(int);

// Constructors for NvAPI structures that just zero the memory and set the right version
NV_DELTA_ENTRY::NV_DELTA_ENTRY()
{
	memset(this, 0, sizeof *this);
}

NV_GPU_PSTATES20_V2::NV_GPU_PSTATES20_V2()
{
	memset(this, 0, sizeof *this);
	version = NV_STRUCT_VERSION(NV_GPU_PSTATES20_V2, 2);
}

NV_CLOCK_FREQUENCIES_V2::NV_CLOCK_FREQUENCIES_V2()
{
	memset(this, 0, sizeof *this);
	version = NV_STRUCT_VERSION(NV_CLOCK_FREQUENCIES_V2, 2);
}

NV_GPU_PERFORMANCE_TABLE_V1::NV_GPU_PERFORMANCE_TABLE_V1()
{
	memset(this, 0, sizeof *this);
	version = NV_STRUCT_VERSION(NV_GPU_PERFORMANCE_TABLE_V1, 1);
}

NV_DYNAMIC_PSTATES_V1::NV_DYNAMIC_PSTATES_V1()
{
	memset(this, 0, sizeof *this);
	version = NV_STRUCT_VERSION(NV_DYNAMIC_PSTATES_V1, 1);
}

NV_GPU_POWER_POLICIES_INFO_V1::NV_GPU_POWER_POLICIES_INFO_V1()
{
	memset(this, 0, sizeof *this);
	version = NV_STRUCT_VERSION(NV_GPU_POWER_POLICIES_INFO_V1, 1);
}

NV_GPU_POWER_POLICIES_STATUS_V1::NV_GPU_POWER_POLICIES_STATUS_V1()
{
	memset(this, 0, sizeof *this);
	version = NV_STRUCT_VERSION(NV_GPU_POWER_POLICIES_STATUS_V1, 1);
}

NV_GPU_VOLTAGE_DOMAINS_STATUS_V1::NV_GPU_VOLTAGE_DOMAINS_STATUS_V1()
{
	memset(this, 0, sizeof *this);
	version = NV_STRUCT_VERSION(NV_GPU_VOLTAGE_DOMAINS_STATUS_V1, 1);
}

NV_GPU_THERMAL_SETTINGS_V2::NV_GPU_THERMAL_SETTINGS_V2()
{
	memset(this, 0, sizeof *this);
	version = NV_STRUCT_VERSION(NV_GPU_THERMAL_SETTINGS_V2, 2);
}

NV_GPU_THERMAL_POLICIES_INFO_V2::NV_GPU_THERMAL_POLICIES_INFO_V2()
{
	memset(this, 0, sizeof *this);
	version = NV_STRUCT_VERSION(NV_GPU_THERMAL_POLICIES_INFO_V2, 2);
}

NV_GPU_THERMAL_POLICIES_STATUS_V2::NV_GPU_THERMAL_POLICIES_STATUS_V2()
{
	memset(this, 0, sizeof *this);
	version = NV_STRUCT_VERSION(NV_GPU_THERMAL_POLICIES_STATUS_V2, 2);
}

NV_GPU_COOLER_SETTINGS_V2::NV_GPU_COOLER_SETTINGS_V2()
{
	memset(this, 0, sizeof *this);
	version = NV_STRUCT_VERSION(NV_GPU_COOLER_SETTINGS_V2, 2);
}

NV_GPU_COOLER_LEVELS_V1::NV_GPU_COOLER_LEVELS_V1()
{
	memset(this, 0, sizeof *this);
	version = NV_STRUCT_VERSION(NV_GPU_COOLER_LEVELS_V1, 1);
}

NV_MEMORY_INFO_V2::NV_MEMORY_INFO_V2()
{
	memset(this, 0, sizeof *this);
	version = NV_STRUCT_VERSION(NV_MEMORY_INFO_V2, 2);
}

NV_DISPLAY_DRIVER_VERSION_V1::NV_DISPLAY_DRIVER_VERSION_V1()
{
	memset(this, 0, sizeof *this);
	version = NV_STRUCT_VERSION(NV_DISPLAY_DRIVER_VERSION_V1, 1);
}

NV_I2C_INFO_V3::NV_I2C_INFO_V3()
{
	memset(this, 0, sizeof * this);
	version = NV_STRUCT_VERSION(NV_I2C_INFO_V3, 3);
}

// Interface: 0150E828
static NV_STATUS (*pNvAPI_Initialize)();

// Interface: D22BDD7E
static NV_STATUS (*pNvAPI_Unload)();

// Interface: 9ABDD40D
static NV_STATUS (*pNvAPI_EnumDisplayHandle)(
	NV_S32 this_enum,
	NV_DISPLAY_HANDLE *display_handle);

// Interface: E5AC921F
static NV_STATUS (*pNvAPI_EnumPhysicalGPUs)(
	NV_PHYSICAL_GPU_HANDLE *physical_gpu_handles,
	NV_S32 *gpu_count);

// Interface: F951A4D1
static NV_STATUS (*pNvAPI_GetDisplayDriverVersion)(
	NV_DISPLAY_HANDLE display_handle,
	NV_DISPLAY_DRIVER_VERSION_V1 *display_driver_version);

// Interface: 01053FA5
static NV_STATUS (*pNvAPI_GetInterfaceVersionString)(
	NV_SHORT_STRING version);

// Interface: 34EF9506
static NV_STATUS (*pNvAPI_GetPhysicalGPUsFromDisplay)(
	NV_DISPLAY_HANDLE display_handle,
	NV_PHYSICAL_GPU_HANDLE *gpu_handles,
	NV_U32 *gpu_count);

// Interface: 774AA982
static NV_STATUS (*pNvAPI_GetMemoryInfo)(
	NV_DISPLAY_HANDLE display_handle,
	NV_MEMORY_INFO_V2 *memory_info);

// Interface: 0CEEE8E9F
static NV_STATUS (*pNvAPI_GPU_GetFullName)(
	NV_PHYSICAL_GPU_HANDLE physical_gpu_handle,
	NV_SHORT_STRING name);

// Interface: 6FF81213
static NV_STATUS (*pNvAPI_GPU_GetPStates20)(
	NV_PHYSICAL_GPU_HANDLE physical_gpu_handle,
	NV_GPU_PSTATES20_V2 *pstates);

// Interface: 0F4DAE6B
static NV_STATUS (*pNvAPI_GPU_SetPStates20)(
	NV_PHYSICAL_GPU_HANDLE physical_gpu_handle,
	NV_GPU_PSTATES20_V2 *pstates);

// Interface: DCB616C3
static NV_STATUS (*pNvAPI_GPU_GetAllClockFrequencies)(
	NV_PHYSICAL_GPU_HANDLE physical_gpu_handle,
	NV_CLOCK_FREQUENCIES_V2 *frequencies);

// Interface: 60DED2ED
static NV_STATUS (*pNvAPI_GPU_GetDynamicPStates)(
	NV_PHYSICAL_GPU_HANDLE physical_gpu_handle,
	NV_DYNAMIC_PSTATES_V1 *dynamic_pstates);

// Interface: 34206D86
static NV_STATUS (*pNvAPI_GPU_GetPowerPoliciesInfo)(
	NV_PHYSICAL_GPU_HANDLE physical_gpu_handle,
	NV_GPU_POWER_POLICIES_INFO_V1 *policies_info);

// Interface: 70916171
static NV_STATUS (*pNvAPI_GPU_GetPowerPoliciesStatus)(
	NV_PHYSICAL_GPU_HANDLE physical_gpu_handle,
	NV_GPU_POWER_POLICIES_STATUS_V1 *policies_status);

// Interface: 0C16C7E2C
static NV_STATUS (*pNvAPI_GPU_GetVoltageDomainStatus)(
	NV_PHYSICAL_GPU_HANDLE physical_gpu_handle,
	NV_GPU_VOLTAGE_DOMAINS_STATUS_V1 *voltage_domains_status);

// Interface: 0E3640A56
static NV_STATUS (*pNvAPI_GPU_GetThermalSettings)(
	NV_PHYSICAL_GPU_HANDLE physical_gpu_handle,
	NV_THERMAL_TARGET sensor_index,
	NV_GPU_THERMAL_SETTINGS_V2 *thermal_settings);

// Interface: 014B83A5F
static NV_STATUS (*pNvAPI_GPU_GetSerialNumber)(
	NV_PHYSICAL_GPU_HANDLE physical_gpu_handle,
	NV_SHORT_STRING serial_number);

// Interface: 0AD95F5ED
static NV_STATUS (*pNvAPI_GPU_SetPowerPoliciesStatus)(
	NV_PHYSICAL_GPU_HANDLE physical_gpu_handle,
	NV_GPU_POWER_POLICIES_STATUS_V1* policies_status);

// Interface: 00D258BB5
static NV_STATUS (*pNvAPI_GPU_GetThermalPoliciesInfo)(
	NV_PHYSICAL_GPU_HANDLE physical_gpu_handle,
	NV_GPU_THERMAL_POLICIES_INFO_V2* thermal_info);

// Interface: 0E9C425A1
static NV_STATUS (*pNvAPI_GPU_GetThermalPoliciesStatus)(
	NV_PHYSICAL_GPU_HANDLE physical_gpu_handle,
	NV_GPU_THERMAL_POLICIES_STATUS_V2* thermal_status);

// Interface: 034C0B13D
static NV_STATUS (*pNvAPI_GPU_SetThermalPoliciesStatus)(
	NV_PHYSICAL_GPU_HANDLE physical_gpu_handle,
	NV_GPU_THERMAL_POLICIES_STATUS_V2* thermal_status);

// Interface: DA141340
static NV_STATUS (*pNvAPI_GPU_GetCoolerSettings)(
	NV_PHYSICAL_GPU_HANDLE physical_gpu_handle,
	NV_S32 cooler_index,
	NV_GPU_COOLER_SETTINGS_V2 *cooler_settings);

// Interface: 891FA0AE
static NV_STATUS (*pNvAPI_GPU_SetCoolerLevels)(
	NV_PHYSICAL_GPU_HANDLE physical_gpu_handle,
	NV_S32 cooler_index,
	NV_GPU_COOLER_LEVELS_V1 *cooler_levels);

// Interface: 2DDFB66E
static NV_STATUS (*pNvAPI_GPU_GetPCIIdentifiers)(
	NV_PHYSICAL_GPU_HANDLE physical_gpu_handle,
	NV_U32 *device_id,
	NV_U32 *sub_system_id,
	NV_U32 *revision_id,
	NV_U32 *ext_device_id);

// Interface: 283AC65A
static NV_STATUS (*pNvAPI_I2CWriteEx)(
	NV_PHYSICAL_GPU_HANDLE physical_gpu_handle,
	NV_I2C_INFO_V3* i2c_info,
	NV_U32 *unknown);

// Interface: 4D7B0709
static NV_STATUS(*pNvAPI_I2CReadEx)(
	NV_PHYSICAL_GPU_HANDLE physical_gpu_handle,
	NV_I2C_INFO_V3* i2c_info,
	NV_U32 *unknown);

// Interface: 3DBF5764
static NV_STATUS(*pNvAPI_GPU_ClientIllumZonesGetControl)(
	NV_PHYSICAL_GPU_HANDLE physical_gpu_handle,
	NV_GPU_CLIENT_ILLUM_ZONE_CONTROL_PARAMS* pIllumZonesControl);

// Interface: 197D065E
static NV_STATUS(*pNvAPI_GPU_ClientIllumZonesSetControl)(
	NV_PHYSICAL_GPU_HANDLE physical_gpu_handle,
	NV_GPU_CLIENT_ILLUM_ZONE_CONTROL_PARAMS* pIllumZonesControl);

static bool QueryInterfaceOpaque(nvapi_QueryInterface_t query_interface, NV_U32 id, void **result)
{
	void *address = ((void *(*)(NV_U32))query_interface)(id);
	if (address) {
		*result = address;
		return true;
	}
	return false;
}

template<typename F>
static void QueryInterfaceCast(nvapi_QueryInterface_t query_interface, NV_U32 id, const char */*function_name*/, F &function_pointer)
{
	const bool result = QueryInterfaceOpaque(query_interface, id, (void **)&function_pointer);
    ////Log::write("%s querying interface '0x%08x' '%s'", result ? "success" : "failure", id, function_name);
}

#define QueryInterface(query_interface, id, function) \
	QueryInterfaceCast((query_interface), (id), #function, p ## function)

static void QueryInterfaces(nvapi_QueryInterface_t query_interface)
{
    //Log::write("querying interfaces with '0x%p'", query_interface);

	QueryInterface(query_interface, 0x0150E828, NvAPI_Initialize);
	QueryInterface(query_interface, 0xD22BDD7E, NvAPI_Unload);
	QueryInterface(query_interface, 0x9ABDD40D, NvAPI_EnumDisplayHandle);
	QueryInterface(query_interface, 0xE5AC921F, NvAPI_EnumPhysicalGPUs);
	QueryInterface(query_interface, 0xF951A4D1, NvAPI_GetDisplayDriverVersion);
	QueryInterface(query_interface, 0x01053FA5, NvAPI_GetInterfaceVersionString);
	QueryInterface(query_interface, 0x34EF9506, NvAPI_GetPhysicalGPUsFromDisplay);
	QueryInterface(query_interface, 0x774AA982, NvAPI_GetMemoryInfo);

	QueryInterface(query_interface, 0x0CEEE8E9F, NvAPI_GPU_GetFullName);
	QueryInterface(query_interface, 0x6FF81213, NvAPI_GPU_GetPStates20);
	QueryInterface(query_interface, 0x0F4DAE6B, NvAPI_GPU_SetPStates20);
	QueryInterface(query_interface, 0xDCB616C3, NvAPI_GPU_GetAllClockFrequencies);
	QueryInterface(query_interface, 0x60DED2ED, NvAPI_GPU_GetDynamicPStates);
	QueryInterface(query_interface, 0x34206D86, NvAPI_GPU_GetPowerPoliciesInfo);
	QueryInterface(query_interface, 0x70916171, NvAPI_GPU_GetPowerPoliciesStatus);
	QueryInterface(query_interface, 0x0C16C7E2C, NvAPI_GPU_GetVoltageDomainStatus);
	QueryInterface(query_interface, 0x0E3640A56, NvAPI_GPU_GetThermalSettings);
	QueryInterface(query_interface, 0x014B83A5F, NvAPI_GPU_GetSerialNumber);
	QueryInterface(query_interface, 0x0AD95F5ED, NvAPI_GPU_SetPowerPoliciesStatus);
	QueryInterface(query_interface, 0x00D258BB5, NvAPI_GPU_GetThermalPoliciesInfo);
	QueryInterface(query_interface, 0x0E9C425A1, NvAPI_GPU_GetThermalPoliciesStatus);
	QueryInterface(query_interface, 0x034C0B13D, NvAPI_GPU_SetThermalPoliciesStatus);
	QueryInterface(query_interface, 0xDA141340, NvAPI_GPU_GetCoolerSettings);
	QueryInterface(query_interface, 0x891FA0AE, NvAPI_GPU_SetCoolerLevels);
	QueryInterface(query_interface, 0x2DDFB66E, NvAPI_GPU_GetPCIIdentifiers);

	QueryInterface(query_interface, 0x283AC65A, NvAPI_I2CWriteEx);
	QueryInterface(query_interface, 0x4D7B0709, NvAPI_I2CReadEx);
	QueryInterface(query_interface, 0x3DBF5764, NvAPI_GPU_ClientIllumZonesGetControl);
	QueryInterface(query_interface, 0x197D065E, NvAPI_GPU_ClientIllumZonesSetControl);
}

NV_STATUS NvAPI_Initialize()
{
	if (!pNvAPI_Initialize) {
#ifdef _WIN32
		const char *name = sizeof(void*) == 4 ? "nvapi.dll" : "nvapi64.dll";
		HMODULE nvapi = LoadLibraryA(name);
		if (!nvapi) {
            //Log::write("failed to load '%s'", name);
			return -1;
		}
        //Log::write("loaded '%s' '0x%p'", name, nvapi);
		nvapi_QueryInterface_t query_interface = (nvapi_QueryInterface_t) GetProcAddress(nvapi, "nvapi_QueryInterface");
		if (!query_interface) {
            //Log::write("failed to find 'nvapi_QueryInterface'");
			return -1;
		}
#elif __linux__
        void* nvapi = nullptr;
        if (!nvapi) nvapi = dlopen("libnvidia-api.so.1", RTLD_LAZY);
        if (!nvapi) nvapi = dlopen("libnvidia-api.so", RTLD_LAZY);
        if (!nvapi) {
            // NVIDIA Driver is not installed
            //Log::write("failed to load libnvidia-api.so, NVIDIA Driver is not installed");
            return -1;
        }
        nvapi_QueryInterface_t query_interface = (nvapi_QueryInterface_t) dlsym(nvapi, "nvapi_QueryInterface");
        if (!query_interface) {
            // NVIDIA Driver is probably not up to date, requires at least driver version 525
            //Log::write("failed to load QueryInterface from libnvidia-api.so, NVIDIA Driver is not up to date");
            return -1;
        }
#endif

		QueryInterfaces(query_interface);
	}

	return pNvAPI_Initialize
		? (*pNvAPI_Initialize)()
		: -1;
}

NV_STATUS NvAPI_Unload()
{
	return pNvAPI_Unload
		? (*pNvAPI_Unload)()
		: -1;
}

NV_STATUS NvAPI_EnumDisplayHandle(
	NV_S32 this_enum,
	NV_DISPLAY_HANDLE *display_handle)
{
	return pNvAPI_EnumDisplayHandle
		? (*pNvAPI_EnumDisplayHandle)(this_enum, display_handle)
		: -1;
}

NV_STATUS NvAPI_EnumPhysicalGPUs(
	NV_PHYSICAL_GPU_HANDLE *physical_gpu_handles,
	NV_S32 *gpu_count)
{
	return pNvAPI_EnumPhysicalGPUs
		? (*pNvAPI_EnumPhysicalGPUs)(physical_gpu_handles, gpu_count)
		: -1;
}

NV_STATUS NvAPI_GetDisplayDriverVersion(
	NV_DISPLAY_HANDLE display_handle,
	NV_DISPLAY_DRIVER_VERSION_V1 *display_driver_version)
{
	return pNvAPI_GetDisplayDriverVersion
		? (*pNvAPI_GetDisplayDriverVersion)(display_handle, display_driver_version)
		: -1;
}

NV_STATUS NvAPI_GetInterfaceVersionString(
	NV_SHORT_STRING version)
{
	return pNvAPI_GetInterfaceVersionString
		? (*pNvAPI_GetInterfaceVersionString)(version)
		: -1;
}

NV_STATUS NvAPI_GetPhysicalGPUsFromDisplay(
	NV_DISPLAY_HANDLE display_handle,
	NV_PHYSICAL_GPU_HANDLE *gpu_handles,
	NV_U32 *gpu_count)
{
	return pNvAPI_GetPhysicalGPUsFromDisplay
		? (*pNvAPI_GetPhysicalGPUsFromDisplay)(display_handle, gpu_handles, gpu_count)
		: -1;
}

NV_STATUS NvAPI_GetMemoryInfo(
	NV_DISPLAY_HANDLE display_handle,
	NV_MEMORY_INFO_V2 *memory_info)
{
	return pNvAPI_GetMemoryInfo
		? (*pNvAPI_GetMemoryInfo)(display_handle, memory_info)
		: -1;
}

NV_STATUS NvAPI_GPU_GetFullName(
	NV_PHYSICAL_GPU_HANDLE physical_gpu_handle,
	NV_SHORT_STRING name)
{
	return pNvAPI_GPU_GetFullName
		? (*pNvAPI_GPU_GetFullName)(physical_gpu_handle, name)
		: -1;
}

NV_STATUS NvAPI_GPU_GetPStates20(
	NV_PHYSICAL_GPU_HANDLE physical_gpu_handle,
	NV_GPU_PSTATES20_V2 *pstates)
{
	return pNvAPI_GPU_GetPStates20
		? (*pNvAPI_GPU_GetPStates20)(physical_gpu_handle, pstates)
		: -1;
}

NV_STATUS NvAPI_GPU_SetPStates20(
	NV_PHYSICAL_GPU_HANDLE physical_gpu_handle,
	NV_GPU_PSTATES20_V2 *pstates)
{
	return pNvAPI_GPU_SetPStates20
		? (*pNvAPI_GPU_GetPStates20)(physical_gpu_handle, pstates)
		: -1;
}

NV_STATUS NvAPI_GPU_GetAllClockFrequencies(
	NV_PHYSICAL_GPU_HANDLE physical_gpu_handle,
	NV_CLOCK_FREQUENCIES_V2 *frequencies)
{
	return pNvAPI_GPU_GetAllClockFrequencies
		? (*pNvAPI_GPU_GetAllClockFrequencies)(physical_gpu_handle, frequencies)
		: -1;
}

NV_STATUS NvAPI_GPU_GetDynamicPStates(
	NV_PHYSICAL_GPU_HANDLE physical_gpu_handle,
	NV_DYNAMIC_PSTATES_V1 *dynamic_pstates)
{
	return pNvAPI_GPU_GetDynamicPStates
		? (*pNvAPI_GPU_GetDynamicPStates)(physical_gpu_handle, dynamic_pstates)
		: -1;
}

NV_STATUS NvAPI_GPU_GetPowerPoliciesInfo(
	NV_PHYSICAL_GPU_HANDLE physical_gpu_handle,
	NV_GPU_POWER_POLICIES_INFO_V1 *policies_info)
{
	return pNvAPI_GPU_GetPowerPoliciesInfo
		? (*pNvAPI_GPU_GetPowerPoliciesInfo)(physical_gpu_handle, policies_info)
		: -1;
}

NV_STATUS NvAPI_GPU_GetPowerPoliciesStatus(
	NV_PHYSICAL_GPU_HANDLE physical_gpu_handle,
	NV_GPU_POWER_POLICIES_STATUS_V1 *policies_status)
{
	return pNvAPI_GPU_GetPowerPoliciesStatus
		? (*NvAPI_GPU_GetPowerPoliciesStatus)(physical_gpu_handle, policies_status)
		: -1;
}

NV_STATUS NvAPI_GPU_GetVoltageDomainStatus(
	NV_PHYSICAL_GPU_HANDLE physical_gpu_handle,
	NV_GPU_VOLTAGE_DOMAINS_STATUS_V1 *voltage_domains_status)
{
	return pNvAPI_GPU_GetVoltageDomainStatus
		? (*pNvAPI_GPU_GetVoltageDomainStatus)(physical_gpu_handle, voltage_domains_status)
		: -1;
}

NV_STATUS NvAPI_GPU_GetThermalSettings(
	NV_PHYSICAL_GPU_HANDLE physical_gpu_handle,
	NV_THERMAL_TARGET sensor_index,
	NV_GPU_THERMAL_SETTINGS_V2 *thermal_settings)
{
	return pNvAPI_GPU_GetThermalSettings
		? (*pNvAPI_GPU_GetThermalSettings)(physical_gpu_handle, sensor_index, thermal_settings)
		: -1;
}

NV_STATUS NvAPI_GPU_GetSerialNumber(
	NV_PHYSICAL_GPU_HANDLE physical_gpu_handle,
	NV_SHORT_STRING serial_number)
{
	return pNvAPI_GPU_GetSerialNumber
		? (*pNvAPI_GPU_GetSerialNumber)(physical_gpu_handle, serial_number)
		: -1;
}

NV_STATUS NvAPI_GPU_SetPowerPoliciesStatus(
	NV_PHYSICAL_GPU_HANDLE physical_gpu_handle,
	NV_GPU_POWER_POLICIES_STATUS_V1* policies_status)
{
	return pNvAPI_GPU_SetPowerPoliciesStatus
		? (*pNvAPI_GPU_SetPowerPoliciesStatus)(physical_gpu_handle, policies_status)
		: -1;
}

NV_STATUS NvAPI_GPU_GetThermalPoliciesInfo(
	NV_PHYSICAL_GPU_HANDLE physical_gpu_handle,
	NV_GPU_THERMAL_POLICIES_INFO_V2* thermal_info)
{
	return pNvAPI_GPU_GetThermalPoliciesInfo
		? (*pNvAPI_GPU_GetThermalPoliciesInfo)(physical_gpu_handle, thermal_info)
		: -1;
}

NV_STATUS NvAPI_GPU_GetThermalPoliciesStatus(
	NV_PHYSICAL_GPU_HANDLE physical_gpu_handle,
	NV_GPU_THERMAL_POLICIES_STATUS_V2* thermal_status)
{
	return pNvAPI_GPU_GetThermalPoliciesStatus
		? (*pNvAPI_GPU_GetThermalPoliciesStatus)(physical_gpu_handle, thermal_status)
		: -1;
}

NV_STATUS NvAPI_GPU_SetThermalPoliciesStatus(
	NV_PHYSICAL_GPU_HANDLE physical_gpu_handle,
	NV_GPU_THERMAL_POLICIES_STATUS_V2* thermal_status)
{
	return pNvAPI_GPU_SetThermalPoliciesStatus
		? (*pNvAPI_GPU_SetThermalPoliciesStatus)(physical_gpu_handle, thermal_status)
		: -1;
}

NV_STATUS NvAPI_GPU_GetCoolerSettings(
	NV_PHYSICAL_GPU_HANDLE physical_gpu_handle,
	NV_S32 cooler_index,
	NV_GPU_COOLER_SETTINGS_V2 *cooler_settings)
{
	return pNvAPI_GPU_GetCoolerSettings
		? (*pNvAPI_GPU_GetCoolerSettings)(physical_gpu_handle, cooler_index, cooler_settings)
		: -1;
}

NV_STATUS NvAPI_GPU_SetCoolerLevels(
	NV_PHYSICAL_GPU_HANDLE physical_gpu_handle,
	NV_S32 cooler_index,
	NV_GPU_COOLER_LEVELS_V1 *cooler_levels)
{
	return pNvAPI_GPU_SetCoolerLevels
		? (*pNvAPI_GPU_SetCoolerLevels)(physical_gpu_handle, cooler_index, cooler_levels)
		: -1;
}

NV_STATUS NvAPI_GPU_GetPCIIdentifiers(
	NV_PHYSICAL_GPU_HANDLE physical_gpu_handle,
	NV_U32 *device_id,
	NV_U32 *sub_system_id,
	NV_U32 *revision_id,
	NV_U32 *ext_device_id)
{
	return pNvAPI_GPU_GetPCIIdentifiers
		? (*pNvAPI_GPU_GetPCIIdentifiers)(physical_gpu_handle, device_id, sub_system_id, revision_id, ext_device_id)
		: -1;
}

NV_STATUS NvAPI_I2CWriteEx(
	NV_PHYSICAL_GPU_HANDLE physical_gpu_handle,
	NV_I2C_INFO_V3* i2c_info,
	NV_U32 *unknown)
{
	return pNvAPI_I2CWriteEx
		? (*pNvAPI_I2CWriteEx)(physical_gpu_handle, i2c_info, unknown)
		: -1;
}

NV_STATUS NvAPI_I2CReadEx(
	NV_PHYSICAL_GPU_HANDLE physical_gpu_handle,
	NV_I2C_INFO_V3* i2c_info,
	NV_U32 *unknown)
{
	return pNvAPI_I2CReadEx
		? (*pNvAPI_I2CReadEx)(physical_gpu_handle, i2c_info, unknown)
		: -1;
}

NV_STATUS NvAPI_GPU_ClientIllumZonesGetControl(
	NV_PHYSICAL_GPU_HANDLE physical_gpu_handle,
	NV_GPU_CLIENT_ILLUM_ZONE_CONTROL_PARAMS* pIllumZonesControl)
{
	return pNvAPI_GPU_ClientIllumZonesGetControl
		? (*pNvAPI_GPU_ClientIllumZonesGetControl)(physical_gpu_handle, pIllumZonesControl)
		: -1;
}

NV_STATUS NvAPI_GPU_ClientIllumZonesSetControl(
	NV_PHYSICAL_GPU_HANDLE physical_gpu_handle,
	NV_GPU_CLIENT_ILLUM_ZONE_CONTROL_PARAMS* pIllumZonesControl)
{
	return pNvAPI_GPU_ClientIllumZonesSetControl
		? (*pNvAPI_GPU_ClientIllumZonesSetControl)(physical_gpu_handle, pIllumZonesControl)
		: -1;
}
