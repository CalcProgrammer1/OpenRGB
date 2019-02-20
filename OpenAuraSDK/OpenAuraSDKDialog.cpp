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
    ON_BN_CLICKED(IDC_RADIO_OPENAURASDK_EFFECT_CHASE_FADE, &OpenAuraSDKDialog::OnBnClickedRadioOpenaurasdkEffectChaseFade)
    ON_BN_CLICKED(IDC_BUTTON_OPENAURASDK_SET_COLORS, &OpenAuraSDKDialog::OnBnClickedButtonOpenaurasdkSetColors)
    ON_CBN_CLOSEUP(IDC_COMBO_OPENAURASDK_DEVICE, &OpenAuraSDKDialog::OnCbnCloseupComboOpenaurasdkDevice)
    ON_BN_CLICKED(IDC_BUTTON_OPENAURASDK_SET_COLORS_ALL, &OpenAuraSDKDialog::OnBnClickedButtonOpenaurasdkSetColorsAll)
END_MESSAGE_MAP()

BOOL OpenAuraSDKDialog::OnInitDialog()
{
    CComboBox* aura_device_box = (CComboBox*)GetDlgItem(IDC_COMBO_OPENAURASDK_DEVICE);

    aura_device_box->AddString("All Devices");

    for (int i = 0; i < controllers.size(); i++)
    {
        aura_device_box->AddString(controllers[i]->GetDeviceName());
    }

    aura_device_box->SetCurSel(0);

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
    unsigned int aura_device = ((CComboBox*)GetDlgItem(IDC_COMBO_OPENAURASDK_DEVICE))->GetCurSel();
    if ( aura_device == 0)
    {
        for (int i = 0; i < controllers.size(); i++)
        {
            controllers[i]->AuraRegisterWrite(AURA_REG_DIRECT, 1);
            controllers[i]->AuraRegisterWrite(AURA_REG_APPLY, AURA_APPLY_VAL);
        }
    }
    else
    {
        controllers[ aura_device - 1 ]->AuraRegisterWrite(AURA_REG_DIRECT, 1);
        controllers[ aura_device - 1 ]->AuraRegisterWrite(AURA_REG_APPLY, AURA_APPLY_VAL);
    }
}


void OpenAuraSDKDialog::OnBnClickedRadioOpenaurasdkEffectMode()
{
    unsigned int aura_device = ((CComboBox*)GetDlgItem(IDC_COMBO_OPENAURASDK_DEVICE))->GetCurSel();
    if (aura_device == 0)
    {
        for (int i = 0; i < controllers.size(); i++)
        {
            controllers[i]->AuraRegisterWrite(AURA_REG_DIRECT, 0);
            controllers[i]->AuraRegisterWrite(AURA_REG_APPLY, AURA_APPLY_VAL);
        }
    }
    else
    {
        controllers[aura_device - 1]->AuraRegisterWrite(AURA_REG_DIRECT, 0);
        controllers[aura_device - 1]->AuraRegisterWrite(AURA_REG_APPLY, AURA_APPLY_VAL);
    }
}


void OpenAuraSDKDialog::setMode(unsigned char mode_val)
{
    unsigned int aura_device = ((CComboBox*)GetDlgItem(IDC_COMBO_OPENAURASDK_DEVICE))->GetCurSel();
    if (aura_device == 0)
    {
        for (int i = 0; i < controllers.size(); i++)
        {
            controllers[i]->SetMode(mode_val);
        }
    }
    else
    {
        controllers[aura_device - 1]->SetMode(mode_val);
    }
}


void OpenAuraSDKDialog::OnBnClickedRadioOpenaurasdkEffectOff()
{
    setMode(AURA_MODE_OFF);
}


void OpenAuraSDKDialog::OnBnClickedRadioOpenaurasdkEffectStatic()
{
    setMode(AURA_MODE_STATIC);
}


void OpenAuraSDKDialog::OnBnClickedRadioOpenaurasdkEffectBreathing()
{
    setMode(AURA_MODE_BREATHING);
}


void OpenAuraSDKDialog::OnBnClickedRadioOpenaurasdkEffectFlashing()
{
    setMode(AURA_MODE_FLASHING);
}


void OpenAuraSDKDialog::OnBnClickedRadioOpenaurasdkEffectSpectrumCycle()
{
    setMode(AURA_MODE_SPECTRUM_CYCLE);
}


void OpenAuraSDKDialog::OnBnClickedRadioOpenaurasdkEffectRainbow()
{
    setMode(AURA_MODE_RAINBOW);
}


void OpenAuraSDKDialog::OnBnClickedRadioOpenaurasdkBreathingSpectrum()
{
    setMode(AURA_MODE_SPECTRUM_CYCLE_BREATHING);
}


void OpenAuraSDKDialog::OnBnClickedRadioOpenaurasdkEffectChaseFade()
{
    setMode(AURA_MODE_CHASE_FADE);
}


void OpenAuraSDKDialog::OnBnClickedButtonOpenaurasdkSetColors()
{
    unsigned int aura_device = ((CComboBox*)GetDlgItem(IDC_COMBO_OPENAURASDK_DEVICE))->GetCurSel();

    if (aura_device == 0)
    {
        unsigned char direct = ((CButton*)GetDlgItem(IDC_RADIO_OPENAURASDK_DIRECT_MODE))->GetCheck();

        if (direct == 0)
        {
            for (int i = 0; i < controllers.size(); i++)
            {
                controllers[i]->SetLEDColorEffect( 0,
                                                    GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_0_R),
                                                    GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_0_G),
                                                    GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_0_B) );
                controllers[i]->SetLEDColorEffect( 1,
                                                    GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_1_R),
                                                    GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_1_G),
                                                    GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_1_B) );
                controllers[i]->SetLEDColorEffect( 2,
                                                    GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_2_R),
                                                    GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_2_G),
                                                    GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_2_B) );
                controllers[i]->SetLEDColorEffect( 3,
                                                    GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_3_R),
                                                    GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_3_G),
                                                    GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_3_B) );
                controllers[i]->SetLEDColorEffect( 4,
                                                    GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_4_R),
                                                    GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_4_G),
                                                    GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_4_B) );
            }
        }
        else
        {
            for (int i = 0; i < controllers.size(); i++)
            {
                controllers[i]->SetLEDColorDirect( 0,
                                                   GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_0_R),
                                                   GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_0_G),
                                                   GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_0_B) );
                controllers[i]->SetLEDColorDirect( 1,
                                                   GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_1_R),
                                                   GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_1_G),
                                                   GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_1_B) );
                controllers[i]->SetLEDColorDirect( 2,
                                                   GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_2_R),
                                                   GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_2_G),
                                                   GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_2_B) );
                controllers[i]->SetLEDColorDirect( 3,
                                                   GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_3_R),
                                                   GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_3_G),
                                                   GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_3_B) );
                controllers[i]->SetLEDColorDirect( 4,
                                                   GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_4_R),
                                                   GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_4_G),
                                                   GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_4_B) );
            }
        }
    }
    else
    {
        unsigned char direct = controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_DIRECT);

        if (direct == 0)
        {
            controllers[aura_device - 1]->SetLEDColorEffect( 0,
                                                GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_0_R),
                                                GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_0_G),
                                                GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_0_B) );
            controllers[aura_device - 1]->SetLEDColorEffect( 1,
                                                GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_1_R),
                                                GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_1_G),
                                                GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_1_B) );
            controllers[aura_device - 1]->SetLEDColorEffect( 2,
                                                GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_2_R),
                                                GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_2_G),
                                                GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_2_B) );
            controllers[aura_device - 1]->SetLEDColorEffect( 3,
                                                GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_3_R),
                                                GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_3_G),
                                                GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_3_B) );
            controllers[aura_device - 1]->SetLEDColorEffect( 4,
                                                GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_4_R),
                                                GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_4_G),
                                                GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_4_B) );
        }
        else
        {
            controllers[aura_device - 1]->SetLEDColorDirect( 0,
                                               GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_0_R),
                                               GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_0_G),
                                               GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_0_B) );
            controllers[aura_device - 1]->SetLEDColorDirect( 1,
                                               GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_1_R),
                                               GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_1_G),
                                               GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_1_B) );
            controllers[aura_device - 1]->SetLEDColorDirect( 2,
                                               GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_2_R),
                                               GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_2_G),
                                               GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_2_B) );
            controllers[aura_device - 1]->SetLEDColorDirect( 3,
                                               GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_3_R),
                                               GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_3_G),
                                               GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_3_B) );
            controllers[aura_device - 1]->SetLEDColorDirect( 4,
                                               GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_4_R),
                                               GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_4_G),
                                               GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_4_B) );
        }
    }

}


void OpenAuraSDKDialog::OnBnClickedButtonOpenaurasdkSetColorsAll()
{
    unsigned int aura_device = ((CComboBox*)GetDlgItem(IDC_COMBO_OPENAURASDK_DEVICE))->GetCurSel();

    if (aura_device == 0)
    {
        unsigned char direct = ((CButton*)GetDlgItem(IDC_RADIO_OPENAURASDK_DIRECT_MODE))->GetCheck();

        if (direct == 0)
        {
            for (int i = 0; i < controllers.size(); i++)
            {
                controllers[i]->SetAllColorsEffect(GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_0_R),
                                                   GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_0_G),
                                                   GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_0_B));
            }
        }
        else
        {
            for (int i = 0; i < controllers.size(); i++)
            {
                controllers[i]->SetAllColorsDirect(GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_0_R),
                                                   GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_0_G),
                                                   GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_0_B));
            }
        }
    }
    else
    {
        unsigned char direct = controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_DIRECT);

        if (direct == 0)
        {
            controllers[aura_device - 1]->SetAllColorsEffect(GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_0_R),
                                                    GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_0_G),
                                                    GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_0_B));
        }
        else
        {
            controllers[aura_device - 1]->SetAllColorsDirect(GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_0_R),
                                                             GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_0_G),
                                                             GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_0_B));
        }
    }

}


void OpenAuraSDKDialog::OnCbnCloseupComboOpenaurasdkDevice()
{
    unsigned int aura_device = ((CComboBox*)GetDlgItem(IDC_COMBO_OPENAURASDK_DEVICE))->GetCurSel();
    if (aura_device == 0)
    {
        /*((CButton*)GetDlgItem(IDC_RADIO_OPENAURASDK_DIRECT_MODE))->SetCheck(false);
        ((CButton*)GetDlgItem(IDC_RADIO_OPENAURASDK_EFFECT_MODE))->SetCheck(false);

        ((CButton*)GetDlgItem(IDC_RADIO_OPENAURASDK_EFFECT_OFF))->SetCheck(false);
        ((CButton*)GetDlgItem(IDC_RADIO_OPENAURASDK_EFFECT_STATIC))->SetCheck(false);
        ((CButton*)GetDlgItem(IDC_RADIO_OPENAURASDK_EFFECT_BREATHING))->SetCheck(false);
        ((CButton*)GetDlgItem(IDC_RADIO_OPENAURASDK_EFFECT_FLASHING))->SetCheck(false);
        ((CButton*)GetDlgItem(IDC_RADIO_OPENAURASDK_EFFECT_SPECTRUM_CYCLE))->SetCheck(false);
        ((CButton*)GetDlgItem(IDC_RADIO_OPENAURASDK_EFFECT_RAINBOW))->SetCheck(false);
        ((CButton*)GetDlgItem(IDC_RADIO_OPENAURASDK_BREATHING_SPECTRUM))->SetCheck(false);
        ((CButton*)GetDlgItem(IDC_RADIO_OPENAURASDK_EFFECT_CHASE_FADE))->SetCheck(false);

        SetDlgItemText(IDC_EDIT_OPENAURASDK_LED_0_R, "");
        SetDlgItemText(IDC_EDIT_OPENAURASDK_LED_0_G, "");
        SetDlgItemText(IDC_EDIT_OPENAURASDK_LED_0_B, "");
        SetDlgItemText(IDC_EDIT_OPENAURASDK_LED_1_R, "");
        SetDlgItemText(IDC_EDIT_OPENAURASDK_LED_1_G, "");
        SetDlgItemText(IDC_EDIT_OPENAURASDK_LED_1_B, "");
        SetDlgItemText(IDC_EDIT_OPENAURASDK_LED_2_R, "");
        SetDlgItemText(IDC_EDIT_OPENAURASDK_LED_2_G, "");
        SetDlgItemText(IDC_EDIT_OPENAURASDK_LED_2_B, "");
        SetDlgItemText(IDC_EDIT_OPENAURASDK_LED_3_R, "");
        SetDlgItemText(IDC_EDIT_OPENAURASDK_LED_3_G, "");
        SetDlgItemText(IDC_EDIT_OPENAURASDK_LED_3_B, "");
        SetDlgItemText(IDC_EDIT_OPENAURASDK_LED_4_R, "");
        SetDlgItemText(IDC_EDIT_OPENAURASDK_LED_4_G, "");
        SetDlgItemText(IDC_EDIT_OPENAURASDK_LED_4_B, "");*/

    }
    else
    {
        unsigned char direct = controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_DIRECT);

        ((CButton*)GetDlgItem(IDC_RADIO_OPENAURASDK_DIRECT_MODE))->SetCheck(direct != 0);
        ((CButton*)GetDlgItem(IDC_RADIO_OPENAURASDK_EFFECT_MODE))->SetCheck(direct == 0);

        unsigned char mode = controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_MODE);

        ((CButton*)GetDlgItem(IDC_RADIO_OPENAURASDK_EFFECT_OFF))->SetCheck(mode == AURA_MODE_OFF);
        ((CButton*)GetDlgItem(IDC_RADIO_OPENAURASDK_EFFECT_STATIC))->SetCheck(mode == AURA_MODE_STATIC);
        ((CButton*)GetDlgItem(IDC_RADIO_OPENAURASDK_EFFECT_BREATHING))->SetCheck(mode == AURA_MODE_BREATHING);
        ((CButton*)GetDlgItem(IDC_RADIO_OPENAURASDK_EFFECT_FLASHING))->SetCheck(mode == AURA_MODE_FLASHING);
        ((CButton*)GetDlgItem(IDC_RADIO_OPENAURASDK_EFFECT_SPECTRUM_CYCLE))->SetCheck(mode == AURA_MODE_SPECTRUM_CYCLE);
        ((CButton*)GetDlgItem(IDC_RADIO_OPENAURASDK_EFFECT_RAINBOW))->SetCheck(mode == AURA_MODE_RAINBOW);
        ((CButton*)GetDlgItem(IDC_RADIO_OPENAURASDK_BREATHING_SPECTRUM))->SetCheck(mode == AURA_MODE_SPECTRUM_CYCLE_BREATHING);
        ((CButton*)GetDlgItem(IDC_RADIO_OPENAURASDK_EFFECT_CHASE_FADE))->SetCheck(mode == AURA_MODE_CHASE_FADE);

        if (direct == 0)
        {
            SetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_0_R, controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_COLORS_EFFECT + 0));
            SetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_0_G, controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_COLORS_EFFECT + 2));
            SetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_0_B, controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_COLORS_EFFECT + 1));
            SetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_1_R, controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_COLORS_EFFECT + 3));
            SetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_1_G, controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_COLORS_EFFECT + 5));
            SetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_1_B, controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_COLORS_EFFECT + 4));
            SetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_2_R, controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_COLORS_EFFECT + 6));
            SetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_2_G, controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_COLORS_EFFECT + 8));
            SetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_2_B, controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_COLORS_EFFECT + 7));
            SetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_3_R, controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_COLORS_EFFECT + 9));
            SetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_3_G, controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_COLORS_EFFECT + 11));
            SetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_3_B, controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_COLORS_EFFECT + 10));
            SetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_4_R, controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_COLORS_EFFECT + 12));
            SetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_4_G, controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_COLORS_EFFECT + 14));
            SetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_4_B, controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_COLORS_EFFECT + 13));
        }
        else
        {
            SetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_0_R, controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_COLORS_DIRECT + 0));
            SetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_0_G, controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_COLORS_DIRECT + 2));
            SetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_0_B, controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_COLORS_DIRECT + 1));
            SetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_1_R, controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_COLORS_DIRECT + 3));
            SetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_1_G, controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_COLORS_DIRECT + 5));
            SetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_1_B, controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_COLORS_DIRECT + 4));
            SetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_2_R, controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_COLORS_DIRECT + 6));
            SetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_2_G, controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_COLORS_DIRECT + 8));
            SetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_2_B, controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_COLORS_DIRECT + 7));
            SetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_3_R, controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_COLORS_DIRECT + 9));
            SetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_3_G, controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_COLORS_DIRECT + 11));
            SetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_3_B, controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_COLORS_DIRECT + 10));
            SetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_4_R, controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_COLORS_DIRECT + 12));
            SetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_4_G, controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_COLORS_DIRECT + 14));
            SetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_4_B, controllers[aura_device - 1]->AuraRegisterRead(AURA_REG_COLORS_DIRECT + 13));
        }
    }

}