#include "OpenAuraSDKDialog.h"
#include "OpenAuraSDK.h"
#include "I2CDetectDialog.h"

IMPLEMENT_DYNAMIC(OpenAuraSDKDialog, CDialogEx)

OpenAuraSDKDialog::OpenAuraSDKDialog(std::vector<i2c_smbus_interface *>& bus, std::vector<RGBController *>& control, CWnd* pParent)
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
    ON_CBN_CLOSEUP(IDC_COMBO_OPENAURASDK_DEVICE, &OpenAuraSDKDialog::OnCbnCloseupComboOpenaurasdkDevice)
    ON_BN_CLICKED(IDC_BUTTON_OPENAURASDK_SET_COLORS_ALL, &OpenAuraSDKDialog::OnBnClickedButtonOpenaurasdkSetColorsAll)
    ON_CBN_CLOSEUP(IDC_COMBO_OPENAURASDK_MODE, &OpenAuraSDKDialog::OnCbnCloseupComboOpenaurasdkMode)
    ON_BN_CLICKED(IDC_BUTTON_OPENAURASDK_SET_ZONE, &OpenAuraSDKDialog::OnBnClickedButtonOpenaurasdkSetZone)
    ON_BN_CLICKED(IDC_BUTTON_OPENAURASDK_SET_COLORS_LED, &OpenAuraSDKDialog::OnBnClickedButtonOpenaurasdkSetColorsLed)
    ON_BN_CLICKED(IDC_BUTTON_OPENAURASDK_SET_COLORS_DEVICE, &OpenAuraSDKDialog::OnBnClickedButtonOpenaurasdkSetColorsDevice)
END_MESSAGE_MAP()

BOOL OpenAuraSDKDialog::OnInitDialog()
{
    CComboBox* controller_box = (CComboBox*)GetDlgItem(IDC_COMBO_OPENAURASDK_DEVICE);
    CComboBox* mode_box = (CComboBox*)GetDlgItem(IDC_COMBO_OPENAURASDK_MODE);
    CComboBox* zone_box = (CComboBox*)GetDlgItem(IDC_COMBO_OPENAURASDK_ZONE);
    CComboBox* led_box = (CComboBox*)GetDlgItem(IDC_COMBO_OPENAURASDK_LED);

    for (int i = 0; i < controllers.size(); i++)
    {
        controller_box->AddString(controllers[i]->name.c_str());
    }

    controller_box->SetCurSel(0);

    if (controllers.size() > 0)
    {
        for (int i = 0; i < controllers[0]->modes.size(); i++)
        {
            mode_box->AddString(controllers[0]->modes[i].name.c_str());
        }

        mode_box->SetCurSel(controllers[0]->GetMode());

        for (int i = 0; i < controllers[0]->zones.size(); i++)
        {
            zone_box->AddString(controllers[0]->zones[i].name.c_str());
        }

        zone_box->SetCurSel(0);

        for (int i = 0; i < controllers[0]->leds.size(); i++)
        {
            led_box->AddString(controllers[0]->leds[i].name.c_str());
        }

        led_box->SetCurSel(0);
    }
    return TRUE;
}


void OpenAuraSDKDialog::OnBnClickedButtonOpenaurasdkI2cdetect()
{
    I2CDetectDialog dlg(busses);
    dlg.DoModal();
}


void OpenAuraSDKDialog::OnBnClickedButtonOpenaurasdkSetColorsAll()
{
    RGBColor color = ToRGBColor(
        GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_0_R),
        GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_0_G),
        GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_0_B)
    );

    for (int i = 0; i < controllers.size(); i++)
    {
        controllers[i]->SetCustomMode();
        controllers[i]->SetAllLEDs(color);
    }
}


void OpenAuraSDKDialog::OnCbnCloseupComboOpenaurasdkDevice()
{
    CComboBox* controller_box = (CComboBox*)GetDlgItem(IDC_COMBO_OPENAURASDK_DEVICE);
    CComboBox* mode_box = (CComboBox*)GetDlgItem(IDC_COMBO_OPENAURASDK_MODE);
    CComboBox* zone_box = (CComboBox*)GetDlgItem(IDC_COMBO_OPENAURASDK_ZONE);
    CComboBox* led_box = (CComboBox*)GetDlgItem(IDC_COMBO_OPENAURASDK_LED);

    if (controllers.size() > 0)
    {
        mode_box->ResetContent();

        for (int i = 0; i < controllers[controller_box->GetCurSel()]->modes.size(); i++)
        {
            mode_box->AddString(controllers[controller_box->GetCurSel()]->modes[i].name.c_str());
        }

        mode_box->SetCurSel(controllers[controller_box->GetCurSel()]->GetMode());

        zone_box->ResetContent();

        for (int i = 0; i < controllers[controller_box->GetCurSel()]->zones.size(); i++)
        {
            zone_box->AddString(controllers[controller_box->GetCurSel()]->zones[i].name.c_str());
        }

        zone_box->SetCurSel(0);

        led_box->ResetContent();

        for (int i = 0; i < controllers[controller_box->GetCurSel()]->leds.size(); i++)
        {
            led_box->AddString(controllers[controller_box->GetCurSel()]->leds[i].name.c_str());
        }

        led_box->SetCurSel(0);
    }
}

void OpenAuraSDKDialog::OnCbnCloseupComboOpenaurasdkMode()
{
    CComboBox* controller_box = (CComboBox*)GetDlgItem(IDC_COMBO_OPENAURASDK_DEVICE);
    CComboBox* mode_box = (CComboBox*)GetDlgItem(IDC_COMBO_OPENAURASDK_MODE);

    if (controllers.size() > 0)
    {
        controllers[controller_box->GetCurSel()]->SetMode(mode_box->GetCurSel());
    }
}


void OpenAuraSDKDialog::OnBnClickedButtonOpenaurasdkSetZone()
{
    CComboBox* controller_box = (CComboBox*)GetDlgItem(IDC_COMBO_OPENAURASDK_DEVICE);
    CComboBox* zone_box = (CComboBox*)GetDlgItem(IDC_COMBO_OPENAURASDK_ZONE);

    RGBColor color = ToRGBColor(
        GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_0_R),
        GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_0_G),
        GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_0_B)
    );

    if (controllers.size() > 0)
    {
        controllers[controller_box->GetCurSel()]->SetAllZoneLEDs(zone_box->GetCurSel(), color);
    }
}


void OpenAuraSDKDialog::OnBnClickedButtonOpenaurasdkSetColorsLed()
{
    CComboBox* controller_box = (CComboBox*)GetDlgItem(IDC_COMBO_OPENAURASDK_DEVICE);
    CComboBox* led_box = (CComboBox*)GetDlgItem(IDC_COMBO_OPENAURASDK_LED);

    RGBColor color = ToRGBColor(
        GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_0_R),
        GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_0_G),
        GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_0_B)
    );

    if (controllers.size() > 0)
    {
        controllers[controller_box->GetCurSel()]->SetLED(led_box->GetCurSel(), color);
    }
}


void OpenAuraSDKDialog::OnBnClickedButtonOpenaurasdkSetColorsDevice()
{
    CComboBox* controller_box = (CComboBox*)GetDlgItem(IDC_COMBO_OPENAURASDK_DEVICE);

    RGBColor color = ToRGBColor(
        GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_0_R),
        GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_0_G),
        GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_0_B)
    );

    if (controllers.size() > 0)
    {
        controllers[controller_box->GetCurSel()]->SetAllLEDs(color);
    }
}