#include "AddDeviceDialog.h"
#include "ui_AddDeviceDialog.h"

#include "RGBController.h"
#include "ResourceManager.h"
#include "SettingsManager.h"
#include "LogManager.h"

// Device entries
#include "DMXSettingsEntry.h"
#include "E131SettingsEntry.h"
#include "ElgatoKeyLightSettingsEntry.h"
#include "ElgatoLightStripSettingsEntry.h"
#include "KasaSmartSettingsEntry.h"
#include "LIFXSettingsEntry.h"
#include "PhilipsHueSettingsEntry.h"
#include "PhilipsWizSettingsEntry.h"
#include "QMKORGBSettingsEntry.h"
#include "SerialSettingsEntry.h"
#include "YeelightSettingsEntry.h"
#include "GoveeSettingsEntry.h"
#include "NanoleafScanPage.h"

#include <QDesktopServices>

static void DetectionProgressChangedCallback(void* param)
{
    AddDeviceDialog* this_obj = (AddDeviceDialog*)(param);

    QMetaObject::invokeMethod(this_obj, "onDetectionProgressChanged", Qt::QueuedConnection);
}

static void DetectionEndedCallback(void* param)
{
    AddDeviceDialog* this_obj = (AddDeviceDialog*)(param);

    QMetaObject::invokeMethod(this_obj, "onDetectionEnded", Qt::QueuedConnection);
}

AddDeviceDialog::AddDeviceDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddDeviceDialog)
{
    ui->setupUi(this);

    ResourceManager* rm = ResourceManager::get();
    rm->RegisterDetectionEndCallback(&DetectionEndedCallback, this);
    rm->RegisterDetectionProgressCallback(&DetectionProgressChangedCallback, this);

    home();
}

AddDeviceDialog::~AddDeviceDialog()
{
    ResourceManager* rm = ResourceManager::get();
    rm->UnregisterDetectionEndCallback(&DetectionEndedCallback, this);
    rm->UnregisterDetectionProgressCallback(&DetectionProgressChangedCallback, this);

    delete ui;
}

void AddDeviceDialog::setScanButtons(bool scan)
{
    ui->progressBar->setVisible(scan);

    ui->ButtonScanBack->setVisible(!scan);
    ui->ButtonScanRescan->setVisible(!scan);
    if(scan)
    {
        ui->ButtonScanBeginning->hide();
        ui->ButtonScanClose->hide();
    }
}

void AddDeviceDialog::home()
{
    scanFromPage = -1;
    helpFromPage = -1;
    configureFromPage = -1;
    ui->stackedWidget->slideInWgt(ui->PageConnectionType, SlidingStackedWidget::LEFT2RIGHT);
}

void AddDeviceDialog::rescan()
{
    if(scanFromPage == -1)
    {
        // Save the index of the page we came from
        scanFromPage = ui->stackedWidget->currentIndex();

        // Cache the names of all devices that were found so far
        cachedDevices.clear();
        std::vector<RGBController*> controllers = ResourceManager::get()->GetRGBControllers();
        for(int i = 0; i < (int)controllers.size(); ++i)
        {
            cachedDevices.push_back(controllers[i]->name);
        }
    }
    ui->LabelScan->setText(tr("<h2>Re-scanning devices...</h2>"));
    setScanButtons(true);

    ui->stackedWidget->slideInWgt(ui->PageScan, SlidingStackedWidget::RIGHT2LEFT);

    ResourceManager::get()->DetectDevices();
}

void AddDeviceDialog::configure(QWidget* entry)
{
    configureFromPage = ui->stackedWidget->currentIndex();

    delete ui->ConfigureEntry;
    ui->ConfigureEntry = entry;
    ui->PageConfigureLayout->insertWidget(1, entry);
    ui->stackedWidget->slideInWgt(ui->PageConfigure, SlidingStackedWidget::RIGHT2LEFT);
}

void AddDeviceDialog::on_ButtonSerial_clicked()
{
    configure(new SerialSettingsEntry);
}

void AddDeviceDialog::on_ButtonDMX_clicked()
{
    configure(new DMXSettingsEntry);
}

void AddDeviceDialog::on_ButtonQMK_clicked()
{
    configure(new QMKORGBSettingsEntry);
}

void AddDeviceDialog::on_ButtonE131_clicked()
{
    configure(new E131SettingsEntry);
}

void AddDeviceDialog::on_ButtonKasaSmart_clicked()
{
    configure(new KasaSmartSettingsEntry);
}

void AddDeviceDialog::on_ButtonLIFX_clicked()
{
    configure(new LIFXSettingsEntry);
}

void AddDeviceDialog::on_ButtonPhilipsHue_clicked()
{
    configure(new PhilipsHueSettingsEntry);
}

void AddDeviceDialog::on_ButtonPhilipsWiz_clicked()
{
    configure(new PhilipsWizSettingsEntry);
}

void AddDeviceDialog::on_ButtonYeelight_clicked()
{
    configure(new YeelightSettingsEntry);
}

void AddDeviceDialog::on_ButtonNanloeaf_clicked()
{
    configure(new NanoleafScanPage);
}

void AddDeviceDialog::on_ButtonElgatoKeyLight_clicked()
{
    configure(new ElgatoKeyLightSettingsEntry);
}

void AddDeviceDialog::on_ButtonElgatoLightStrip_clicked()
{
    configure(new ElgatoLightStripSettingsEntry);
}

void AddDeviceDialog::on_ButtonGovee_clicked()
{
    configure(new GoveeSettingsEntry);
}

// ===== CONFIGURE =====

void AddDeviceDialog::on_ButtonConfigureBack_clicked()
{
    ui->stackedWidget->slideInIdx(configureFromPage, SlidingStackedWidget::LEFT2RIGHT);
    configureFromPage = -1;
}

void AddDeviceDialog::on_ButtonConfigureScan_clicked()
{
    BaseManualDeviceEntry* manualEntry = dynamic_cast<BaseManualDeviceEntry*>(ui->ConfigureEntry);
    if(manualEntry)
    {
        SettingsManager* sm = ResourceManager::get()->GetSettingsManager();
        const char* section = manualEntry->settingsSection();
        json result = sm->GetSettings(section);

        if(!strcmp(section, "NanoleafDevices"))
        {
            NanoleafSettingsEntry* entry = dynamic_cast<NanoleafSettingsEntry*>(manualEntry);
            if(entry)
            {
                result["devices"][entry->getLocation()] = manualEntry->saveSettings();
            }
            else
            {
                LOG_ERROR("[AddDeviceDialog]: nanoleaf entry failed to cast, saving entry aborted");
            }
        }
        else if(!strcmp(section, "PhilipsHueDevices"))
        {
            result["bridges"].push_back(manualEntry->saveSettings());
        }
        else
        {
            result["devices"].push_back(manualEntry->saveSettings());
        }
        sm->SetSettings(section, result);
        sm->SaveSettings();
    }
    rescan();
}

// ===== SCAN =====

void AddDeviceDialog::on_ButtonScanBack_clicked()
{
    ui->stackedWidget->slideInIdx(scanFromPage, SlidingStackedWidget::LEFT2RIGHT);
    scanFromPage = -1;
}

void AddDeviceDialog::on_ButtonScanRescan_clicked()
{
    rescan();
}

void AddDeviceDialog::on_ButtonScanNotFound_clicked()
{
    // TODO: consider planning this stage further
}

void AddDeviceDialog::on_ButtonScanBeginning_clicked()
{
    home();
}

void AddDeviceDialog::on_ButtonScanClose_clicked()
{
    close();
}

void AddDeviceDialog::onDetectionProgressChanged()
{
    ui->progressBar->setValue(ResourceManager::get()->GetDetectionPercent());
    ui->progressBar->setFormat(QString::fromStdString(ResourceManager::get()->GetDetectionString()));

    if(ResourceManager::get()->GetDetectionPercent() != 100)
    {
        setScanButtons(true);
    };
}

void AddDeviceDialog::onDetectionEnded()
{
    setScanButtons(false);
    // Set text

    std::vector<RGBController*> controllers = ResourceManager::get()->GetRGBControllers();
    if(controllers.empty())
    {
        ui->LabelScan->setText(tr("<h2>Still nothing O_O</h2>"
                                  "Scan reported no devices"));
    }
    else
    {
        QString text = tr("<h2>Scan complete</h2>");
        bool foundNew = false;
        for(int i = 0; i < (int)controllers.size(); ++i)
        {
            std::vector<std::string>::iterator it = std::find(cachedDevices.begin(), cachedDevices.end(), controllers[i]->name);
            if(it == cachedDevices.end())
            {
                if(!foundNew)
                {
                    text.append(tr("New devices that were found:<ul>"));
                }
                foundNew = true;

                text.append("<li>");
                text.append(controllers[i]->name.c_str());
                text.append("</li>");
            }
        }
        if(!foundNew)
        {
            text.append(tr("No new devices were found. Please go back and check the parameters, or file a new device issue."));
        }
        else
        {
            text.append("</ul>");

            // Detection has gained results - show buttons to "complete" the hunt
            ui->ButtonScanBeginning->show();
            ui->ButtonScanClose->show();
        }
        ui->LabelScan->setText(text);
    }
}
