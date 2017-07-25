#ifndef SSHOTER_H
#define SSHOTER_H

#include <QMainWindow>
#include <QPixmap>
#include <QScreen>
#include <QImage>
#include <QWidget>
#include <QTime>
#include <QUrl>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkAccessManager>
#include <QKeyEvent>
#include <QSettings>
#include <QDebug>
#include <QDir>
#include <QGraphicsEffect>
#include <QEvent>
#include <QMouseEvent>
#include <QPoint>
#include <QKeySequence>
#include <QtMath>
#include <QMessageBox>
#include <QPainter>
#include <QDesktopWidget>
#include <QFile>

namespace Ui {
class Sshoter;
}

class Sshoter : public QMainWindow
{
    Q_OBJECT

public:
    explicit Sshoter(QWidget *parent = 0);
    ~Sshoter();
    QPixmap ScreenShot;
    QString File_name;
    QString Ftp_Login;
    QString Ftp_Pass;
    QString Ftp_Adress;
    int Ftp_Port = 21;
    QString Dir = "ssy/";
    QString SaveButton;
    QString NewButton;
    QString UploadButton;
    QPoint MouseClicked, MouseReleased, MouseMoved, MouseMovedOne;
    bool CanClick = false, clicked = false, MouseMovedOnce = true;
    QScreen *screen;
    QWidget *wdg = new QWidget();

public slots:
    void NewScreenshot();
    void SaveScreenshot();
    void UploadScreenshot();
    void LoadSettings();
    void SetDefaultFtp();
    void ShowOptions();
    void getReleasedPos(QScreen *screen);
    void mouseReleaseEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void DrawLine();


private slots:

private:
    Ui::Sshoter *ui;
};

#endif // SSHOTER_H
