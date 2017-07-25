#include "optionsui.h"
#include "ui_optionsui.h"

Optionsui::Optionsui(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Optionsui)
{
    ui->setupUi(this);
    this->setWindowTitle("Options");
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(SaveConfig()));
    connect (ui->pushButton_2, SIGNAL(clicked()), this, SLOT(Destroy()));
    connect (ui->pushButton_3, SIGNAL(clicked()), this, SLOT(FileDialog()));
    LoadConfig();
}

Optionsui::~Optionsui()
{
    delete ui;
}

void Optionsui::LoadConfig()
{
    QSettings Settings(QString("configs/config.ini"), QSettings::IniFormat);
    shoter.Ftp_Login = Settings.value("Ftp_Login").toString();
    shoter.Ftp_Pass = Settings.value("Ftp_Password").toString();
    shoter.Ftp_Adress = Settings.value("Ftp_Adress").toString();
    shoter.Ftp_Port = Settings.value("Ftp_Port").toInt();
    shoter.Dir = Settings.value("Dir").toString();
    shoter.SaveButton = Settings.value("Save_Button").toString();
    shoter.NewButton = Settings.value("New_Button").toString();
    shoter.UploadButton = Settings.value("Upload_Button").toString();

    ui->lineEdit_2->setText(shoter.Ftp_Login);
    ui->lineEdit->setText(shoter.Ftp_Pass);
    ui->lineEdit_3->setText(shoter.Ftp_Adress);
    ui->lineEdit_4->setText(QString::number(shoter.Ftp_Port));
    ui->keySequenceEdit->setKeySequence(shoter.SaveButton);
    ui->keySequenceEdit_2->setKeySequence(shoter.NewButton);
    ui->keySequenceEdit_3->setKeySequence(shoter.UploadButton);
}

void Optionsui::SaveConfig()
{
    QSettings Settings(QString("configs/config.ini"), QSettings::IniFormat);

    Settings.setValue("Ftp_Login", ui->lineEdit_2->text());
    Settings.setValue("Ftp_Pass", ui->lineEdit->text());
    Settings.setValue("Ftp_Adress", ui->lineEdit_3->text());
    Settings.setValue("Ftp_Port", ui->lineEdit_4->text());
    Settings.setValue("Dir", Optionsui::SaveDir+"/");
    Settings.setValue("Save_Button", QKeySequence(ui->keySequenceEdit->keySequence()));
    Settings.setValue("New_Button", QKeySequence(ui->keySequenceEdit_2->keySequence()));
    Settings.setValue("Upload_Button", QKeySequence(ui->keySequenceEdit_3->keySequence()));

    QMessageBox AlertBox;
    AlertBox.setText("You have to reset app before changes will apply");
    AlertBox.exec();

    this->destroy();
}

void Optionsui::Destroy()
{
    this->destroy();


}

void Optionsui::FileDialog()
{
    Optionsui::SaveDir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "C://", QFileDialog::ShowDirsOnly
                                                           | QFileDialog::DontResolveSymlinks);

}
