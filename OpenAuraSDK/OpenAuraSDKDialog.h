#ifndef OPENAURASDK_DIALOG_H
#define OPENAURASDK_DIALOG_H

#include <afxdialogex.h>
#include <vector>
#include "i2c_smbus.h"
#include "RGBController.h"
#include "resource.h"

class OpenAuraSDKDialog : public CDialogEx
{
    DECLARE_DYNAMIC(OpenAuraSDKDialog)

public:
    OpenAuraSDKDialog(std::vector<i2c_smbus_interface *>& bus, std::vector<RGBController *>& control, CWnd* pParent = NULL);
    virtual ~OpenAuraSDKDialog();
    virtual BOOL OnInitDialog();

private:
    void setMode(unsigned char mode_val);

protected:
    std::vector<i2c_smbus_interface *>& busses;
    std::vector<RGBController *>& controllers;
    virtual void DoDataExchange(CDataExchange* pDX);
    DECLARE_MESSAGE_MAP()

public:
    afx_msg void OnBnClickedButtonOpenaurasdkI2cdetect();
    afx_msg void OnCbnCloseupComboOpenaurasdkDevice();
    afx_msg void OnBnClickedButtonOpenaurasdkSetColorsAll();
    afx_msg void OnCbnCloseupComboOpenaurasdkMode();
	afx_msg void OnBnClickedButtonOpenaurasdkSetZone();
    afx_msg void OnBnClickedButtonOpenaurasdkSetColorsLed();
};

#endif
