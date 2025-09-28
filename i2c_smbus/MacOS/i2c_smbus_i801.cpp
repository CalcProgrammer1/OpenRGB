/*---------------------------------------------------------*\
| i2c_smbus_i801.cpp                                        |
|                                                           |
|   i801 SMBUS driver for MacOS                             |
|                                                           |
|   Adam Honse (CalcProgrammer1)                29 Jan 2019 |
|   Portions based on Linux source code                     |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "macUSPCIOAccess.h"

#include "Detector.h"
#include "i2c_smbus_i801.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include "SettingsManager.h"

using namespace std::chrono_literals;

i2c_smbus_i801::i2c_smbus_i801()
{
}

i2c_smbus_i801::~i2c_smbus_i801()
{
}

/* Return negative errno on error. */
s32 i2c_smbus_i801::i801_access(u16 addr, char read_write, u8 command, int size, i2c_smbus_data *data)
{
    int hwpec = 0;
    int block = 0;
    int ret = 0, xact = 0;
    //struct i801_priv *priv = i2c_get_adapdata(adap);

    //mutex_lock(&priv->acpi_lock);
    //if (priv->acpi_reserved) {
    //    mutex_unlock(&priv->acpi_lock);
    //    return -EBUSY;
    //}

    //pm_runtime_get_sync(&priv->pci_dev->dev);

    //hwpec = (priv->features & FEATURE_SMBUS_PEC) && (flags & I2C_CLIENT_PEC)
    //    && size != I2C_SMBUS_QUICK
    //    && size != I2C_SMBUS_I2C_BLOCK_DATA;

    switch (size)
    {
    case I2C_SMBUS_QUICK:
        WriteIoPortByte(SMBHSTADD, ((addr & 0x7f) << 1) | (read_write & 0x01));
        xact = I801_QUICK;
        break;
    case I2C_SMBUS_BYTE:
        WriteIoPortByte(SMBHSTADD, ((addr & 0x7f) << 1) | (read_write & 0x01));
        if (read_write == I2C_SMBUS_WRITE)
            WriteIoPortByte(SMBHSTCMD, command);
        xact = I801_BYTE;
        break;
    case I2C_SMBUS_BYTE_DATA:
        WriteIoPortByte(SMBHSTADD, ((addr & 0x7f) << 1) | (read_write & 0x01));
        WriteIoPortByte(SMBHSTCMD, command);
        if (read_write == I2C_SMBUS_WRITE)
            WriteIoPortByte(SMBHSTDAT0, data->byte);
        xact = I801_BYTE_DATA;
        break;
    case I2C_SMBUS_WORD_DATA:
        WriteIoPortByte(SMBHSTADD, ((addr & 0x7f) << 1) | (read_write & 0x01));
        WriteIoPortByte(SMBHSTCMD, command);
        if (read_write == I2C_SMBUS_WRITE)
        {
            WriteIoPortByte(SMBHSTDAT0, data->word & 0xff);
            WriteIoPortByte(SMBHSTDAT1, (data->word & 0xff00) >> 8);
        }
        xact = I801_WORD_DATA;
        break;
    case I2C_SMBUS_BLOCK_DATA:
        WriteIoPortByte(SMBHSTADD, ((addr & 0x7f) << 1) | (read_write & 0x01));
        WriteIoPortByte(SMBHSTCMD, command);
        block = 1;
        break;
    case I2C_SMBUS_I2C_BLOCK_DATA:
        /*
        * NB: page 240 of ICH5 datasheet shows that the R/#W
        * bit should be cleared here, even when reading.
        * However if SPD Write Disable is set (Lynx Point and later),
        * the read will fail if we don't set the R/#W bit.
        */
        WriteIoPortByte(SMBHSTADD, ((addr & 0x7f) << 1) | (read_write & 0x01));

        if (read_write == I2C_SMBUS_READ)
        {
            /* NB: page 240 of ICH5 datasheet also shows
            * that DATA1 is the cmd field when reading */
            WriteIoPortByte(SMBHSTDAT1, command);
        }
        else
        {
            WriteIoPortByte(SMBHSTCMD, command);
        }
        block = 1;
        break;
    default:
        ret = -EOPNOTSUPP;
        goto out;
    }

    //if (hwpec)	/* enable/disable hardware PEC */
    //    outb_p(inb_p(SMBAUXCTL(priv)) | SMBAUXCTL_CRC, SMBAUXCTL(priv));
    //else
    WriteIoPortByte(SMBAUXCTL, ReadIoPortByte(SMBAUXCTL) & (~SMBAUXCTL_CRC));

    if (block)
        ret = i801_block_transaction(data, read_write, size, hwpec);
    else
        ret = i801_transaction(xact);

    /* Some BIOSes don't like it when PEC is enabled at reboot or resume
    time, so we forcibly disable it after every transaction. Turn off
    E32B for the same reason. */
    //if (hwpec || block)
    WriteIoPortByte(SMBAUXCTL, ReadIoPortByte(SMBAUXCTL) & ~(SMBAUXCTL_CRC | SMBAUXCTL_E32B));

    if (block)
        goto out;
    if (ret)
        goto out;
    if ((read_write == I2C_SMBUS_WRITE) || (xact == I801_QUICK))
        goto out;

    switch (xact & 0x7f)
    {
    case I801_BYTE:	/* Result put in SMBHSTDAT0 */
    case I801_BYTE_DATA:
        data->byte = ReadIoPortByte(SMBHSTDAT0);
        break;
    case I801_WORD_DATA:
        data->word = ReadIoPortByte(SMBHSTDAT0) + (ReadIoPortByte(SMBHSTDAT1) << 8);
        break;
    }

out:
    //pm_runtime_mark_last_busy(&priv->pci_dev->dev);
    //pm_runtime_put_autosuspend(&priv->pci_dev->dev);
    //mutex_unlock(&priv->acpi_lock);
    return ret;
}

/* Block transaction function */
int i2c_smbus_i801::i801_block_transaction(i2c_smbus_data *data, char read_write, int command, int hwpec)
{
    int result = 0;
    //unsigned char hostc;

    //if (command == I2C_SMBUS_I2C_BLOCK_DATA)
    //{
    //    if (read_write == I2C_SMBUS_WRITE)
    //    {
    //        /* set I2C_EN bit in configuration register */
    //        pci_read_config_byte(priv->pci_dev, SMBHSTCFG, &hostc);
    //        pci_write_config_byte(priv->pci_dev, SMBHSTCFG,
    //            hostc | SMBHSTCFG_I2C_EN);
    //    }
    //    else if (!(priv->features & FEATURE_I2C_BLOCK_READ)) {
    //        dev_err(&priv->pci_dev->dev,
    //            "I2C block read is unsupported!\n");
    //        return -EOPNOTSUPP;
    //    }
    //}

    if (read_write == I2C_SMBUS_WRITE || command == I2C_SMBUS_I2C_BLOCK_DATA)
    {
        if (data->block[0] < 1)
            data->block[0] = 1;
        if (data->block[0] > I2C_SMBUS_BLOCK_MAX)
            data->block[0] = I2C_SMBUS_BLOCK_MAX;
    }
    else
    {
        data->block[0] = 32;	/* max for SMBus block reads */
    }

    /* Experience has shown that the block buffer can only be used for
    SMBus (not I2C) block transactions, even though the datasheet
    doesn't mention this limitation. */
    //if ((priv->features & FEATURE_BLOCK_BUFFER)
    //    && command != I2C_SMBUS_I2C_BLOCK_DATA
    //    && i801_set_block_buffer_mode(priv) == 0)
    //    result = i801_block_transaction_by_block(priv, data,
    //        read_write, hwpec);
    //else
        result = i801_block_transaction_byte_by_byte(data, read_write, command, hwpec);

    //if (command == I2C_SMBUS_I2C_BLOCK_DATA
    //    && read_write == I2C_SMBUS_WRITE) {
    //    /* restore saved configuration register value */
    //    pci_write_config_byte(priv->pci_dev, SMBHSTCFG, hostc);
    //}
    return result;
}

/*
* For "byte-by-byte" block transactions:
*   I2C write uses cmd=I801_BLOCK_DATA, I2C_EN=1
*   I2C read uses cmd=I801_I2C_BLOCK_DATA
*/
int i2c_smbus_i801::i801_block_transaction_byte_by_byte(i2c_smbus_data *data, char read_write, int command, int /*hwpec*/)
{
    int i, len;
    int smbcmd;
    int status;
    int result;

    result = i801_check_pre();
    if (result < 0)
        return result;

    len = data->block[0];

    if (read_write == I2C_SMBUS_WRITE)
    {
        WriteIoPortByte(SMBHSTDAT0, len);
        WriteIoPortByte(SMBBLKDAT, data->block[1]);
    }

    if (command == I2C_SMBUS_I2C_BLOCK_DATA && read_write == I2C_SMBUS_READ)
        smbcmd = I801_I2C_BLOCK_DATA;
    else
        smbcmd = I801_BLOCK_DATA;

    //if (priv->features & FEATURE_IRQ) {
    //    priv->is_read = (read_write == I2C_SMBUS_READ);
    //    if (len == 1 && priv->is_read)
    //        smbcmd |= SMBHSTCNT_LAST_BYTE;
    //    priv->cmd = smbcmd | SMBHSTCNT_INTREN;
    //    priv->len = len;
    //    priv->count = 0;
    //    priv->data = &data->block[1];
    //
    //    outb_p(priv->cmd | SMBHSTCNT_START, SMBHSTCNT(priv));
    //    result = wait_event_timeout(priv->waitq,
    //        (status = priv->status),
    //        adap->timeout);
    //    if (!result) {
    //        status = -ETIMEDOUT;
    //        dev_warn(&priv->pci_dev->dev,
    //            "Timeout waiting for interrupt!\n");
    //    }
    //    priv->status = 0;
    //    return i801_check_post(priv, status);
    //}

    for (i = 1; i <= len; i++)
    {
        if (i == len && read_write == I2C_SMBUS_READ)
            smbcmd |= SMBHSTCNT_LAST_BYTE;
        WriteIoPortByte(SMBHSTCNT, smbcmd);

        if (i == 1)
            WriteIoPortByte(SMBHSTCNT, ReadIoPortByte(SMBHSTCNT) | SMBHSTCNT_START);

        status = i801_wait_byte_done();
        if (status)
            goto exit;

        if (i == 1 && read_write == I2C_SMBUS_READ && command != I2C_SMBUS_I2C_BLOCK_DATA)
        {
            len = ReadIoPortByte(SMBHSTDAT0);
            if (len < 1 || len > I2C_SMBUS_BLOCK_MAX)
            {
                /* Recover */
                while (ReadIoPortByte(SMBHSTSTS) & SMBHSTSTS_HOST_BUSY)
                    WriteIoPortByte(SMBHSTSTS, SMBHSTSTS_BYTE_DONE);
                WriteIoPortByte(SMBHSTSTS, SMBHSTSTS_INTR);
                return -EPROTO;
            }
            data->block[0] = len;
        }

        /* Retrieve/store value in SMBBLKDAT */
        if (read_write == I2C_SMBUS_READ)
            data->block[i] = ReadIoPortByte(SMBBLKDAT);
        if (read_write == I2C_SMBUS_WRITE && i + 1 <= len)
            WriteIoPortByte(SMBBLKDAT, data->block[i + 1]);

        /* signals SMBBLKDAT ready */
        WriteIoPortByte(SMBHSTSTS, SMBHSTSTS_BYTE_DONE);
    }

    status = i801_wait_intr();
exit:
    return i801_check_post(status);
}

/*
* Convert the status register to an error code, and clear it.
* Note that status only contains the bits we want to clear, not the
* actual register value.
*/
int i2c_smbus_i801::i801_check_post(int status)
{
    int result = 0;

    /*
    * If the SMBus is still busy, we give up
    * Note: This timeout condition only happens when using polling
    * transactions.  For interrupt operation, NAK/timeout is indicated by
    * DEV_ERR.
    */
    if (status < 0)
    {
        /* try to stop the current command */
        WriteIoPortByte(SMBHSTCNT, ReadIoPortByte(SMBHSTCNT) | SMBHSTCNT_KILL);
        //usleep_range(1000, 2000);
        std::this_thread::sleep_for(1ms);
        WriteIoPortByte(SMBHSTCNT, ReadIoPortByte(SMBHSTCNT) & (~SMBHSTCNT_KILL));

        WriteIoPortByte(SMBHSTSTS, STATUS_FLAGS);
        return -ETIMEDOUT;
    }

    if (status & SMBHSTSTS_FAILED)
    {
        result = -EIO;
    }

    if (status & SMBHSTSTS_DEV_ERR)
    {
        /*
        * This may be a PEC error, check and clear it.
        *
        * AUXSTS is handled differently from HSTSTS.
        * For HSTSTS, i801_isr() or i801_wait_intr()
        * has already cleared the error bits in hardware,
        * and we are passed a copy of the original value
        * in "status".
        * For AUXSTS, the hardware register is left
        * for us to handle here.
        * This is asymmetric, slightly iffy, but safe,
        * since all this code is serialized and the CRCE
        * bit is harmless as long as it's cleared before
        * the next operation.
        */
        //if ((priv->features & FEATURE_SMBUS_PEC) &&
        //    (inb_p(SMBAUXSTS(priv)) & SMBAUXSTS_CRCE)) {
        //    outb_p(SMBAUXSTS_CRCE, SMBAUXSTS(priv));
        //    result = -EBADMSG;
        //    dev_dbg(&priv->pci_dev->dev, "PEC error\n");
        //}
        //else {
            result = -ENXIO;
        //    dev_dbg(&priv->pci_dev->dev, "No response\n");
        //}
    }

    if (status & SMBHSTSTS_BUS_ERR)
    {
        result = -EAGAIN;
    }

    /* Clear status flags except BYTE_DONE, to be cleared by caller */
    WriteIoPortByte(SMBHSTSTS, status);

    return result;
}

/* Make sure the SMBus host is ready to start transmitting.
Return 0 if it is, -EBUSY if it is not. */
int i2c_smbus_i801::i801_check_pre()
{
    int status;

    status = ReadIoPortByte(SMBHSTSTS);
    if (status & SMBHSTSTS_HOST_BUSY)
    {
        return -EBUSY;
    }

    status &= STATUS_FLAGS;
    if (status)
    {
        WriteIoPortByte(SMBHSTSTS, status);
        status = ReadIoPortByte(SMBHSTSTS) & STATUS_FLAGS;
        if (status)
        {
            return -EBUSY;
        }
    }

    /*
    * Clear CRC status if needed.
    * During normal operation, i801_check_post() takes care
    * of it after every operation.  We do it here only in case
    * the hardware was already in this state when the driver
    * started.
    */
    //if (priv->features & FEATURE_SMBUS_PEC) {
    //    status = inb_p(SMBAUXSTS(priv)) & SMBAUXSTS_CRCE;
    //    if (status) {
    //        dev_dbg(&priv->pci_dev->dev,
    //            "Clearing aux status flags (%02x)\n", status);
    //        outb_p(status, SMBAUXSTS(priv));
    //        status = inb_p(SMBAUXSTS(priv)) & SMBAUXSTS_CRCE;
    //        if (status) {
    //            dev_err(&priv->pci_dev->dev,
    //                "Failed clearing aux status flags (%02x)\n",
    //                status);
    //            return -EBUSY;
    //        }
    //    }
    //}

    return 0;
}

int i2c_smbus_i801::i801_transaction(int xact)
{
    int status;
    int result;

    result = i801_check_pre();
    if (result < 0)
        return result;

    WriteIoPortByte(SMBHSTCNT, ReadIoPortByte(SMBHSTCNT) & ~SMBHSTCNT_INTREN);
    //if (priv->features & FEATURE_IRQ)
    //{
    //    outb_p(xact | SMBHSTCNT_INTREN | SMBHSTCNT_START,
    //        SMBHSTCNT(priv));
    //    result = wait_event_timeout(priv->waitq,
    //        (status = priv->status),
    //        adap->timeout);
    //    if (!result) {
    //        status = -ETIMEDOUT;
    //        dev_warn(&priv->pci_dev->dev,
    //            "Timeout waiting for interrupt!\n");
    //    }
    //    priv->status = 0;
    //    return i801_check_post(priv, status);
    //}

    /* the current contents of SMBHSTCNT can be overwritten, since PEC,
    * SMBSCMD are passed in xact */
    WriteIoPortByte(SMBHSTCNT, xact | SMBHSTCNT_START);

    status = i801_wait_intr();
    return i801_check_post(status);
}

/* Wait for either BYTE_DONE or an error flag being set */
int i2c_smbus_i801::i801_wait_byte_done()
{

    int timeout = 0;
    int status;

    /* We will always wait for a fraction of a second! */
    do
    {
        std::this_thread::sleep_for(1ms);
        //usleep_range(250, 500);
        status = ReadIoPortByte(SMBHSTSTS);
    } while (!(status & (STATUS_ERROR_FLAGS | SMBHSTSTS_BYTE_DONE)) && (timeout++ < MAX_RETRIES));

    if (timeout > MAX_RETRIES)
    {
        return -ETIMEDOUT;
    }
    return status & STATUS_ERROR_FLAGS;
}

/* Wait for BUSY being cleared and either INTR or an error flag being set */
int i2c_smbus_i801::i801_wait_intr()
{
    int timeout = 0;
    int status;

    /* We will always wait for a fraction of a second! */
    do
    {
        //usleep_range(250, 500);
        status = ReadIoPortByte(SMBHSTSTS);
    } while (((status & SMBHSTSTS_HOST_BUSY) || !(status & (STATUS_ERROR_FLAGS | SMBHSTSTS_INTR))) && (timeout++ < MAX_RETRIES));

    if (timeout > MAX_RETRIES)
    {
        return -ETIMEDOUT;
    }
    return status & (STATUS_ERROR_FLAGS | SMBHSTSTS_INTR);
}

s32 i2c_smbus_i801::i2c_smbus_xfer(u8 addr, char read_write, u8 command, int size, i2c_smbus_data* data)
{
    s32 result = i801_access(addr, read_write, command, size, data);

    return result;
}

s32 i2c_smbus_i801::i2c_xfer(u8 /*addr*/, char /*read_write*/, int* /*size*/, u8* /*data*/)
{
    return -1;
}

bool i2c_smbus_i801_detect()
{
    if(!GetMacUSPCIODriverStatus())
    {
        LOG_INFO("macUSPCIO is not loaded, i801 I2C bus detection aborted");
        return(false);
    }

    uint8_t host_config = ReadConfigPortByte(SMBHSTCFG);
    if ((host_config & SMBHSTCFG_HST_EN) == 0)
    {
        LOG_INFO("i801 SMBus Disabled");
        return(false);
    }

    i2c_smbus_interface * bus;
    bus                         = new i2c_smbus_i801();
    // addresses are referenced from: https://opensource.apple.com/source/IOPCIFamily/IOPCIFamily-146/IOKit/pci/IOPCIDevice.h.auto.html
    bus->pci_vendor             = ReadConfigPortWord(0x00);
    bus->pci_device             = ReadConfigPortWord(0x02);
    bus->pci_subsystem_vendor   = ReadConfigPortWord(0x2c);
    bus->pci_subsystem_device   = ReadConfigPortWord(0x2e);

    if(!bus->pci_vendor || !bus->pci_device || !bus->pci_subsystem_vendor || !bus->pci_subsystem_device)
    {
        return(false);
    }

    snprintf(bus->device_name, 512, "Intel(R) SMBus - %X", bus->pci_device);
    ((i2c_smbus_i801 *)bus)->i801_smba = ReadConfigPortWord(0x20) & 0xFFFE;
    ResourceManager::get()->RegisterI2CBus(bus);

    return(true);
}

REGISTER_I2C_BUS_DETECTOR(i2c_smbus_i801_detect);
