#ifndef OPENAURASDK_DIALOG_H
#define OPENAURASDK_DIALOG_H

#include <afxdialogex.h>
#include <vector>
#include "i2c_smbus.h"
#include "AuraController.h"
#include "resource.h"

class OpenAuraSDKDialog : public CDialogEx
{
    DECLARE_DYNAMIC(OpenAuraSDKDialog)

public:
    OpenAuraSDKDialog(std::vector<i2c_smbus_interface *>& bus, std::vector<AuraController *>& control, CWnd* pParent = NULL);
    virtual ~OpenAuraSDKDialog();
    virtual BOOL OnInitDialog();

private:

protected:
    std::vector<i2c_smbus_interface *>& busses;
    std::vector<AuraController *>& controllers;
    virtual void DoDataExchange(CDataExchange* pDX);
    DECLARE_MESSAGE_MAP()
public:

    afx_msg void OnBnClickedButtonOpenaurasdkI2cdetect();
    afx_msg void OnBnClickedRadioOpenaurasdkDirectMode();
    afx_msg void OnBnClickedRadioOpenaurasdkEffectMode();
    afx_msg void OnBnClickedRadioOpenaurasdkEffectOff();
    afx_msg void OnBnClickedRadioOpenaurasdkEffectStatic();
    afx_msg void OnBnClickedRadioOpenaurasdkEffectBreathing();
    afx_msg void OnBnClickedRadioOpenaurasdkEffectFlashing();
    afx_msg void OnBnClickedRadioOpenaurasdkEffectSpectrumCycle();
    afx_msg void OnBnClickedRadioOpenaurasdkEffectRainbow();
    afx_msg void OnBnClickedRadioOpenaurasdkBreathingSpectrum();
};

#endif
