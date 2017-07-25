#ifndef OPTIONSUI_H
#define OPTIONSUI_H

#include <QDialog>
#include <QSettings>
#include <QDebug>
#include "sshoter.h"
#include <QString>
#include <QMessageBox>
#include <QFileDialog>
#include <QShortcut>

namespace Ui {
class Optionsui;
}

class Optionsui : public QDialog
{
    Q_OBJECT

public:
    explicit Optionsui(QWidget *parent = 0);
    ~Optionsui();
    Sshoter shoter;
    QString SaveDir;

public slots:
    void LoadConfig();
    void SaveConfig();
    void Destroy();
    void FileDialog();
private:
    Ui::Optionsui *ui;
};

#endif // OPTIONSUI_H
