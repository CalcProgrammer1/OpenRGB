#include "OpenRGBSoftwareInfoPage.h"

using namespace Ui;

OpenRGBSoftwareInfoPage::OpenRGBSoftwareInfoPage(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::OpenRGBSoftwareInfoPageUi)
{
    ui->setupUi(this);

    ui->VersionValue->setText(VERSION_STRING);
    ui->BuildDateValue->setText(BUILDDATE_STRING);
    ui->GitCommitIDValue->setText(GIT_COMMIT_ID);
    ui->GitCommitDateValue->setText(GIT_COMMIT_DATE);
    ui->GitBranchValue->setText(GIT_BRANCH);
}

OpenRGBSoftwareInfoPage::~OpenRGBSoftwareInfoPage()
{
    delete ui;
}

void OpenRGBSoftwareInfoPage::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}
