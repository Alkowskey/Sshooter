#include "sshoter.h"
#include "ui_sshoter.h"
#include "optionsui.h"

Sshoter::Sshoter(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Sshoter)
{

    //initialization UI

    ui->setupUi(this);

    //black color

    //hiding buttons. They are unused at start

    ui->pushButton_2->hide();
    ui->pushButton_3->hide();
    ui->label_2->hide();
    ui->textBrowser->hide();
    this->setWindowOpacity(qreal(0.97));
    //Connecting buttons

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(NewScreenshot()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(SaveScreenshot()));
    connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(UploadScreenshot()));
    connect(ui->actionOpcje, SIGNAL (triggered()), this, SLOT(ShowOptions()));

    //Time for generting random numbers (Used to saving files)

    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    //Load settings function

    LoadSettings();

    //Keys shortcut

    QShortcut *_saveScreenshot = new QShortcut(this);
    _saveScreenshot->setKey(QKeySequence(Sshoter::SaveButton));
    QShortcut *_newScreenshot = new QShortcut(this);
    _newScreenshot->setKey(QKeySequence(Sshoter::NewButton));
    QShortcut *_uploadScreenshot = new QShortcut(this);
    _uploadScreenshot->setKey(QKeySequence(Sshoter::UploadButton));

    //Connecting shortcuts

    connect(_saveScreenshot, SIGNAL(activated()), this, SLOT(SaveScreenshot()));
    connect(_newScreenshot, SIGNAL(activated()), this, SLOT(NewScreenshot()));
    connect(_uploadScreenshot, SIGNAL(activated()), this, SLOT(UploadScreenshot()));
}

Sshoter::~Sshoter()
{
    delete ui;
}

//New screenshot function
void Sshoter::NewScreenshot()
{
    //Which window should be screened
    Sshoter::screen = QApplication::screens().at(0);
    //its fine there
    Sshoter::CanClick = true;

    //Setting mainWindow opacity

    this->setWindowOpacity(qreal(0.3));
    this->showFullScreen();
    ui->pushButton_2->hide();
    ui->pushButton_3->hide();
    ui->label_2->hide();
    ui->textBrowser->hide();
    ui->pushButton->hide();
    ui->label->hide();
    ui->menuOpcje->menuAction()->setVisible(false);
    ui->label_3->hide();
    ui->label_4->hide();
}

//Save screenshot function
void Sshoter::SaveScreenshot()
{
    //Saving file as png if needed
    Sshoter::ScreenShot.save(Sshoter::Dir+Sshoter::File_name, "png");
}

//Uploading screenshot
void Sshoter::UploadScreenshot()
{

    //Ftp link like "ftp://example.com"
    QString Link = Sshoter::Ftp_Adress+"/"+Sshoter::File_name;
    if(Link.at(3)!=":"||Link.at(4)!="/"||Link.at(5)!="/")Link = "ftp://"+Link;
    qDebug()<<Link;


    //initializing network class
    QNetworkAccessManager *Ftp = new QNetworkAccessManager(this);

    //QUrl class used for prepering connection stuff
    QUrl url  (Link);
    url.setUserName(Sshoter::Ftp_Login);
    url.setPassword(Sshoter::Ftp_Pass);
    url.setPort(Sshoter::Ftp_Port);

    //Saving screen as TEMP, needed for upload
    Sshoter::ScreenShot.save("TEMP.png", "png");

    //Opening TEMP file
    QFile *File = new QFile("TEMP.png");

    //if doesnt exist qDebug
    if(!File->open(QIODevice::ReadOnly))
    {
        qDebug() << "Nie udalo sie otworzyc pliku";
    }
    else
    {
        //Uploading file!
        Ftp->put(QNetworkRequest(url), File);

        //Im showing link for picture.
        ui->label_2->setText("Link: ");
        Link.remove(0, 6);
        if(Link.at(0)=="f"&&Link.at(1)=="t"&&Link.at(2)=="p"&&Link.at(3)==".")ui->textBrowser->setText(Link.remove(0, 4));
        else ui->textBrowser->setText(Link);
        ui->label_2->show();
        ui->textBrowser->show();
    }
}

//Load settings function
void Sshoter::LoadSettings()
{
    //Settings class used for saving variables used in FTP
    QSettings Settings(QString("configs/config.ini"), QSettings::IniFormat);
    Sshoter::Ftp_Login = Settings.value("Ftp_Login").toString();
    Sshoter::Ftp_Pass = Settings.value("Ftp_Pass").toString();
    Sshoter::Ftp_Adress = Settings.value("Ftp_Adress").toString();
    Sshoter::Dir = Settings.value("Dir").toString();
    Sshoter::Ftp_Port = Settings.value("Ftp_Port").toInt();

    //Loading shortcut buttons saved by user
    Sshoter::SaveButton = Settings.value("Save_Button").toString();
    Sshoter::NewButton = Settings.value("New_Button").toString();
    Sshoter::UploadButton = Settings.value("Upload_Button").toString();

    //Creating Dir where pictures will be saved
    if(!QDir(Sshoter::Dir).exists())QDir().mkdir(Sshoter::Dir);

    //if user dont have FTP server we are using mine
    if(Sshoter::Ftp_Login==NULL||Sshoter::Ftp_Adress==NULL||Sshoter::Ftp_Pass==NULL) SetDefaultFtp();
}

//Setting up my FTP
void Sshoter::SetDefaultFtp()
{
    Sshoter::Ftp_Login = "Alkowskey@opx.pl";
    Sshoter::Ftp_Pass = "******";
    Sshoter::Ftp_Adress ="ftp://Alkowskey.opx.pl/SSy";
}

//Show options window
void Sshoter::ShowOptions()
{
    qDebug()<<"ShowOptions clicked";
    Optionsui options;
    options.setModal(true);
    options.exec();
}

//Getting position of released click
void Sshoter::getReleasedPos(QScreen *screen)
{
    Sshoter::MouseReleased = this->mapFromGlobal(QCursor::pos());

    qDebug()<<Sshoter::MouseReleased;

    Sshoter::CanClick=false;
    this->setWindowOpacity(0);
    if(Sshoter::MouseClicked.x()<Sshoter::MouseReleased.x())
    {
        if(Sshoter::MouseClicked.y()<Sshoter::MouseReleased.y())Sshoter::ScreenShot = screen->grabWindow(0, Sshoter::MouseClicked.x(), Sshoter::MouseClicked.y(), abs(Sshoter::MouseClicked.x() - Sshoter::MouseReleased.x()), abs(Sshoter::MouseClicked.y() - Sshoter::MouseReleased.y()));

        else Sshoter::ScreenShot = screen->grabWindow(0, Sshoter::MouseClicked.x(), Sshoter::MouseReleased.y(), abs(Sshoter::MouseClicked.x() - Sshoter::MouseReleased.x()), abs(Sshoter::MouseReleased.y() - Sshoter::MouseClicked.y()));
    }
    else if(Sshoter::MouseClicked.x()>Sshoter::MouseReleased.x())
    {
        if(Sshoter::MouseClicked.y()<Sshoter::MouseReleased.y())Sshoter::ScreenShot = screen->grabWindow(0, Sshoter::MouseReleased.x(), Sshoter::MouseClicked.y(), abs(Sshoter::MouseClicked.x() - Sshoter::MouseReleased.x()), abs(Sshoter::MouseClicked.y() - Sshoter::MouseReleased.y()));

        else Sshoter::ScreenShot = screen->grabWindow(0, Sshoter::MouseReleased.x(), Sshoter::MouseReleased.y(), abs(Sshoter::MouseClicked.x() - Sshoter::MouseReleased.x()), abs(Sshoter::MouseReleased.y() - Sshoter::MouseClicked.y()));
    }

    if(Sshoter::ScreenShot.height()>251||Sshoter::ScreenShot.width()>481)ui->label->setPixmap(Sshoter::ScreenShot.scaled(481, 251));
    else ui->label->setPixmap(Sshoter::ScreenShot);
    ui->pushButton_2->show();
    ui->pushButton_3->show();
    ui->menuOpcje->menuAction()->setVisible(true);
    Sshoter::File_name = "SS_"+QString::number(qrand())+".png";
    ui->pushButton->show();
    ui->label->show();
    ui->label_3->show();
    ui->label_4->show();
    this->setWindowOpacity(qreal(1));
    this->showNormal();
}

//if mouse pressed
void Sshoter::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons()==Qt::LeftButton&&Sshoter::CanClick)
    {
        qDebug() << "Left button is pressed";
        Sshoter::MouseClicked = this->mapFromGlobal(QCursor::pos());
        Sshoter::clicked = true;
        Sshoter::wdg->setWindowFlags(Qt::FramelessWindowHint);
        Sshoter::wdg->setWindowOpacity(qreal(0.6));
        Sshoter::wdg->setStyleSheet("background-color: black;");
        Sshoter::wdg->show();
    }
}

//if mouse moved
void Sshoter::mouseMoveEvent(QMouseEvent *event)
{
    if(Sshoter::clicked)
    {
        Sshoter::MouseMoved = this->mapFromGlobal(QCursor::pos());
        if(Sshoter::MouseMovedOnce)Sshoter::MouseMovedOne = this->mapFromGlobal(QCursor::pos());
        DrawLine();
    }
}

void Sshoter::DrawLine()
{
    Sshoter::MouseMovedOnce = false;
    if(Sshoter::MouseClicked.x()<Sshoter::MouseMoved.x())
    {
        if(Sshoter::MouseClicked.y()<Sshoter::MouseMoved.y()) Sshoter::wdg->setGeometry(Sshoter::MouseClicked.x(), Sshoter::MouseClicked.y(), abs(Sshoter::MouseClicked.x() - Sshoter::MouseMoved.x()), abs(Sshoter::MouseClicked.y() - Sshoter::MouseMoved.y()));

        else Sshoter::wdg->setGeometry(Sshoter::MouseClicked.x(), Sshoter::MouseMoved.y(), abs(Sshoter::MouseClicked.x() - Sshoter::MouseMoved.x()), abs(Sshoter::MouseMoved.y() - Sshoter::MouseClicked.y()));
    }
    else if(Sshoter::MouseClicked.x()>Sshoter::MouseMoved.x())
    {
        if(Sshoter::MouseClicked.y()<Sshoter::MouseMoved.y())Sshoter::wdg->setGeometry(Sshoter::MouseMoved.x(), Sshoter::MouseClicked.y(), abs(Sshoter::MouseClicked.x() - Sshoter::MouseMoved.x()), abs(Sshoter::MouseClicked.y() - Sshoter::MouseMoved.y()));

        else Sshoter::wdg->setGeometry(Sshoter::MouseMoved.x(), Sshoter::MouseMoved.y(), abs(Sshoter::MouseClicked.x() - Sshoter::MouseMoved.x()), abs(Sshoter::MouseMoved.y() - Sshoter::MouseClicked.y()));
    }
}
//if mouse released
void Sshoter::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button()==Qt::LeftButton&&Sshoter::CanClick)
    {
        qDebug() << "Left button is Released";
        Sshoter::wdg->close();
        getReleasedPos(Sshoter::screen);
        Sshoter::clicked = false;
        Sshoter::MouseMovedOnce = true;
    }
}
