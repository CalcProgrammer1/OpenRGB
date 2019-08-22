#ifndef I2CDETECT_DIALOG_H
#define I2CDETECT_DIALOG_H

#include <afxdialogex.h>
#include <vector>
#include "i2c_smbus.h"
#include "resource.h"

class I2CDetectDialog : public CDialogEx
{
    DECLARE_DYNAMIC(I2CDetectDialog)

public:
    I2CDetectDialog(std::vector<i2c_smbus_interface *>& bus, CWnd* pParent = NULL);
    virtual ~I2CDetectDialog();
    virtual BOOL OnInitDialog();

private:

protected:
    std::vector<i2c_smbus_interface *>& busses;
    virtual void DoDataExchange(CDataExchange* pDX);
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButtonI2cDetect();
};

#endif
