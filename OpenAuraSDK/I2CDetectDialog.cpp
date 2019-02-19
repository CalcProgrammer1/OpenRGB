#include "I2CDetectDialog.h"
#include "OpenAuraSDK.h"

IMPLEMENT_DYNAMIC(I2CDetectDialog, CDialogEx)

I2CDetectDialog::I2CDetectDialog(std::vector<i2c_smbus_interface *>& bus, CWnd* pParent)
    : CDialogEx(IDD_DIALOG_I2C_DETECT, pParent), busses(bus)
{

}

I2CDetectDialog::~I2CDetectDialog()
{

}

void I2CDetectDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(I2CDetectDialog, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON_I2C_DETECT, &I2CDetectDialog::OnBnClickedButtonI2cDetect)
END_MESSAGE_MAP()

BOOL I2CDetectDialog::OnInitDialog()
{
    CComboBox* i2c_bus_box = (CComboBox*)GetDlgItem(IDC_COMBO_I2C_BUS);
    for (int i = 0; i < busses.size(); i++)
    {
        i2c_bus_box->AddString(busses[i]->device_name);
        i2c_bus_box->SetCurSel(0);
    }

    CFont* myFont = new CFont();
    myFont->CreateFontA(16, 0, 0, 0, FW_DONTCARE, false, false, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH | FF_MODERN, _T("Consolas"));

    CEdit* edit = (CEdit*)GetDlgItem(IDC_EDIT_I2C_DETECT);
    edit->SetFont(myFont);

    return TRUE;
}

void I2CDetectDialog::OnBnClickedButtonI2cDetect()
{
    i2c_smbus_interface* bus = busses[((CComboBox*)GetDlgItem(IDC_COMBO_I2C_BUS))->GetCurSel()];

    SetDlgItemText(IDC_EDIT_I2C_DETECT, DetectI2C(bus, MODE_QUICK).c_str());
}