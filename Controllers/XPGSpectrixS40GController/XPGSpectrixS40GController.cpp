/*-------------------------------------------------------------------*\
|  XPGSpectrixS40GController.cpp	                                  |
|                                                                     |
|  Driver for XPG's Spectrix S40G NVMe                                |
|                                                                     |
|  NicolasNewman          25th Mar 2021                               |
|                                                                     |
\*-------------------------------------------------------------------*/

#include "XPGSpectrixS40GController.h"
#include <malloc.h>
#include <cstring>

#ifdef _WIN32
    #include <windows.h>
    #include <fileapi.h>
#else

#endif

XPGSpectrixS40GController::XPGSpectrixS40GController()
{
    led_count = XPG_SPECTRIX_LED_COUNT;
    InitializePackets();

    active_mode = XPG_SPECTRIX_S40G_MODE_STATIC;
    active_direction = XPG_SPECTRIX_S40G_DIRECTION_DEFAULT;
    active_speed = XPG_SPECTRIX_S40G_SPEED_NORMAL;
}

#ifdef _WIN32
    int XPGSpectrixS40GController::SetHandle(wchar_t buff[MAX_PATH]) {
        wchar_t path[MAX_PATH];
        wcscpy(path, L"\\\\?\\");
        wcsncat(path, buff, MAX_PATH - 4);
		for (size_t i = 0; i < MAX_PATH && path[i] != '\0'; i++) {
			path[i] = tolower(path[i]);
		}
        wprintf(L"%s\n", path);
        hDevice = CreateFileW(path, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, (LPSECURITY_ATTRIBUTES)0x0, OPEN_EXISTING, 0x0, (HANDLE)0x0);
        if (hDevice == INVALID_HANDLE_VALUE) {
                return 0;
        }
        return 1;
    }
#else

#endif

unsigned int XPGSpectrixS40GController::GetLEDCount()
{
	return led_count;
}

void XPGSpectrixS40GController::ApplyColors() 
{
	#ifdef _WIN32
		for (unsigned int i = 0; i < BIG_PACKET_SIZE / 4; i++) {
			if (i % 10 == 0) {
			}
		}
		for (unsigned int i = 0; i < SMALL_PACKET_SIZE / 4; i++) {
			if (i % 10 == 0) {
			}
		}
		// TODO better error detection
        if (hDevice != INVALID_HANDLE_VALUE) {
            int out = DeviceIoControl(hDevice, 0x2dd3c0, packet_one, BIG_PACKET_SIZE, packet_one, BIG_PACKET_SIZE, 0x0, (LPOVERLAPPED)0x0);
            if (out > 0) {
                out = DeviceIoControl(hDevice, 0x2dd3c0, packet_two, SMALL_PACKET_SIZE, packet_two, SMALL_PACKET_SIZE, 0x0, (LPOVERLAPPED)0x0);
            }
            if (out > 0) {
                out = DeviceIoControl(hDevice, 0x2dd3c0, packet_three, SMALL_PACKET_SIZE, packet_three, SMALL_PACKET_SIZE, 0x0, (LPOVERLAPPED)0x0);
            }
        }
	#else

	#endif
}

XPGSpectrixS40GController::~XPGSpectrixS40GController()
{
    free(packet_one);
    packet_one = NULL;
    free(packet_two);
    packet_two = NULL;
    free(packet_three);
    packet_three = NULL;
}

void XPGSpectrixS40GController::InitializePackets() {
    packet_one = (uint32_t *) malloc(BIG_PACKET_SIZE);
    packet_two = (uint32_t *) malloc(SMALL_PACKET_SIZE);
    packet_three = (uint32_t *) malloc(SMALL_PACKET_SIZE);  
    memcpy(packet_one, packet_one_template, BIG_PACKET_SIZE);
    memcpy(packet_two, packet_two_template, SMALL_PACKET_SIZE);
    memcpy(packet_three, packet_three_template, SMALL_PACKET_SIZE);
}

void XPGSpectrixS40GController::SetMode(uint8_t mode) {
	*((uint8_t *) (packet_two+52)+1) = mode;
}

void XPGSpectrixS40GController::SetSpeed(uint8_t speed) {
	*((uint8_t *) (packet_two+52)+2) = speed;
}

void XPGSpectrixS40GController::SetDirection(uint8_t direction) {
    *((uint8_t *) (packet_three+52)+3) = direction;
}

void XPGSpectrixS40GController::SetAllColors(uint8_t r, uint8_t g, uint8_t b) {
    for (unsigned int i = 0; i < led_count; i++) {
        SetLEDColor(i, r, g, b);
    }
}

void XPGSpectrixS40GController::SetLEDColor(unsigned int led, uint8_t r, uint8_t g, uint8_t b) {
    switch(led) {
		case 0x0:
			*((uint8_t *) (packet_one+52)+0) = r;
			*((uint8_t *) (packet_one+52)+1) = b;
			*((uint8_t *) (packet_one+52)+2) = g;
			break;
		case 0x1:
			*((uint8_t *) (packet_one+52)+3) = r;
			*((uint8_t *) (packet_one+53)+0) = b;
			*((uint8_t *) (packet_one+53)+1) = g;
			break;
		case 0x2:
			*((uint8_t *) (packet_one+53)+2) = r;
			*((uint8_t *) (packet_one+53)+3) = b;
			*((uint8_t *) (packet_one+54)+0) = g;
			break;
		case 0x3:
			*((uint8_t *) (packet_one+54)+1) = r;
			*((uint8_t *) (packet_one+54)+2) = b;
			*((uint8_t *) (packet_one+54)+3) = g;
			break;
		case 0x4:
			*((uint8_t *) (packet_one+55)+0) = r;
			*((uint8_t *) (packet_one+55)+1) = b;
			*((uint8_t *) (packet_one+55)+2) = g;
			break;
		case 0x5:
			*((uint8_t *) (packet_one+55)+3) = r;
			*((uint8_t *) (packet_one+56)+0) = b;
			*((uint8_t *) (packet_one+56)+1) = g;
			break;
		case 0x6:
			*((uint8_t *) (packet_one+56)+2) = r;
			*((uint8_t *) (packet_one+56)+3) = b;
			*((uint8_t *) (packet_one+57)+0) = g;
			break;
		case 0x7:
			*((uint8_t *) (packet_one+57)+1) = r;
			*((uint8_t *) (packet_one+57)+2) = b;
			*((uint8_t *) (packet_one+57)+3) = g;
			break;
		default:
			return;
    }
}
