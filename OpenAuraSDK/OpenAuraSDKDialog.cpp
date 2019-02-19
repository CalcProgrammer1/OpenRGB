#include "OpenAuraSDKDialog.h"
#include "OpenAuraSDK.h"
#include "I2CDetectDialog.h"

IMPLEMENT_DYNAMIC(OpenAuraSDKDialog, CDialogEx)

OpenAuraSDKDialog::OpenAuraSDKDialog(std::vector<i2c_smbus_interface *>& bus, std::vector<AuraController *>& control, CWnd* pParent)
    : CDialogEx(IDD_DIALOG_OPENAURASDK, pParent), busses(bus), controllers (control)
{

}

OpenAuraSDKDialog::~OpenAuraSDKDialog()
{

}

void OpenAuraSDKDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(OpenAuraSDKDialog, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON_OPENAURASDK_I2CDETECT, &OpenAuraSDKDialog::OnBnClickedButtonOpenaurasdkI2cdetect)
    ON_BN_CLICKED(IDC_RADIO_OPENAURASDK_DIRECT_MODE, &OpenAuraSDKDialog::OnBnClickedRadioOpenaurasdkDirectMode)
    ON_BN_CLICKED(IDC_RADIO_OPENAURASDK_EFFECT_MODE, &OpenAuraSDKDialog::OnBnClickedRadioOpenaurasdkEffectMode)
    ON_BN_CLICKED(IDC_RADIO_OPENAURASDK_EFFECT_OFF, &OpenAuraSDKDialog::OnBnClickedRadioOpenaurasdkEffectOff)
    ON_BN_CLICKED(IDC_RADIO_OPENAURASDK_EFFECT_STATIC, &OpenAuraSDKDialog::OnBnClickedRadioOpenaurasdkEffectStatic)
    ON_BN_CLICKED(IDC_RADIO_OPENAURASDK_EFFECT_BREATHING, &OpenAuraSDKDialog::OnBnClickedRadioOpenaurasdkEffectBreathing)
    ON_BN_CLICKED(IDC_RADIO_OPENAURASDK_EFFECT_FLASHING, &OpenAuraSDKDialog::OnBnClickedRadioOpenaurasdkEffectFlashing)
    ON_BN_CLICKED(IDC_RADIO_OPENAURASDK_EFFECT_SPECTRUM_CYCLE, &OpenAuraSDKDialog::OnBnClickedRadioOpenaurasdkEffectSpectrumCycle)
    ON_BN_CLICKED(IDC_RADIO_OPENAURASDK_EFFECT_RAINBOW, &OpenAuraSDKDialog::OnBnClickedRadioOpenaurasdkEffectRainbow)
    ON_BN_CLICKED(IDC_RADIO_OPENAURASDK_BREATHING_SPECTRUM, &OpenAuraSDKDialog::OnBnClickedRadioOpenaurasdkBreathingSpectrum)
END_MESSAGE_MAP()

BOOL OpenAuraSDKDialog::OnInitDialog()
{
    CFont* myFont = new CFont();
    myFont->CreateFontA(16, 0, 0, 0, FW_DONTCARE, false, false, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH | FF_MODERN, _T("Consolas"));

    return TRUE;
}


void OpenAuraSDKDialog::OnBnClickedButtonOpenaurasdkI2cdetect()
{
    I2CDetectDialog dlg(busses);
    dlg.DoModal();
}


void OpenAuraSDKDialog::OnBnClickedRadioOpenaurasdkDirectMode()
{
    for (int i = 0; i < controllers.size(); i++)
    {
        controllers[i]->AuraRegisterWrite(AURA_REG_DIRECT, 1);
        controllers[i]->AuraRegisterWrite(AURA_REG_APPLY, AURA_APPLY_VAL);
    }
}


void OpenAuraSDKDialog::OnBnClickedRadioOpenaurasdkEffectMode()
{
    for (int i = 0; i < controllers.size(); i++)
    {
        controllers[i]->AuraRegisterWrite(AURA_REG_DIRECT, 0);
        controllers[i]->AuraRegisterWrite(AURA_REG_APPLY, AURA_APPLY_VAL);
    }
}


void OpenAuraSDKDialog::OnBnClickedRadioOpenaurasdkEffectOff()
{
    for (int i = 0; i < controllers.size(); i++)
    {
        controllers[i]->AuraRegisterWrite(AURA_REG_MODE, AURA_MODE_OFF);
        controllers[i]->AuraRegisterWrite(AURA_REG_APPLY, AURA_APPLY_VAL);
    }
}


void OpenAuraSDKDialog::OnBnClickedRadioOpenaurasdkEffectStatic()
{
    for (int i = 0; i < controllers.size(); i++)
    {
        controllers[i]->AuraRegisterWrite(AURA_REG_MODE, AURA_MODE_STATIC);
        controllers[i]->AuraRegisterWrite(AURA_REG_APPLY, AURA_APPLY_VAL);
    }
}


void OpenAuraSDKDialog::OnBnClickedRadioOpenaurasdkEffectBreathing()
{
    for (int i = 0; i < controllers.size(); i++)
    {
        controllers[i]->AuraRegisterWrite(AURA_REG_MODE, AURA_MODE_BREATHING);
        controllers[i]->AuraRegisterWrite(AURA_REG_APPLY, AURA_APPLY_VAL);
    }
}


void OpenAuraSDKDialog::OnBnClickedRadioOpenaurasdkEffectFlashing()
{
    for (int i = 0; i < controllers.size(); i++)
    {
        controllers[i]->AuraRegisterWrite(AURA_REG_MODE, AURA_MODE_FLASHING);
        controllers[i]->AuraRegisterWrite(AURA_REG_APPLY, AURA_APPLY_VAL);
    }
}


void OpenAuraSDKDialog::OnBnClickedRadioOpenaurasdkEffectSpectrumCycle()
{
    for (int i = 0; i < controllers.size(); i++)
    {
        controllers[i]->AuraRegisterWrite(AURA_REG_MODE, AURA_MODE_SPECTRUM_CYCLE);
        controllers[i]->AuraRegisterWrite(AURA_REG_APPLY, AURA_APPLY_VAL);
    }
}


void OpenAuraSDKDialog::OnBnClickedRadioOpenaurasdkEffectRainbow()
{
    for (int i = 0; i < controllers.size(); i++)
    {
        controllers[i]->AuraRegisterWrite(AURA_REG_MODE, AURA_MODE_RAINBOW);
        controllers[i]->AuraRegisterWrite(AURA_REG_APPLY, AURA_APPLY_VAL);
    }
}


void OpenAuraSDKDialog::OnBnClickedRadioOpenaurasdkBreathingSpectrum()
{
    for (int i = 0; i < controllers.size(); i++)
    {
        controllers[i]->AuraRegisterWrite(AURA_REG_MODE, AURA_MODE_SPECTRUM_CYCLE_BREATHING);
        controllers[i]->AuraRegisterWrite(AURA_REG_APPLY, AURA_APPLY_VAL);
    }
}
