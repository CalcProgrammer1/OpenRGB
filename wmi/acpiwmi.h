#ifndef ACPIWMI_H
#define ACPIWMI_H

#ifdef _WIN32

bool AsWMI_CheckSupport();
bool AsWMI_Open();
void AsWMI_Close();
bool AsWMI_NB_RegisterEvent();
bool AsWMI_NB_DeviceControl(int a1, int a2);
bool AsWMI_NB_DeviceControl_2arg(int a1, int a2, int a3);
char AsWMI_NB_GetDeviceStatus(int a1, int *out);
bool AsWMI_NB_GetDeviceStatus_MoreBYTE(int a1, int a2, int *status1, int *status2, int* status3);

#endif

#endif // ACPIWMI_H
