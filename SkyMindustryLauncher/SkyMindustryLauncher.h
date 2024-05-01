#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_SkyMindustryLauncher.h"

class SkyMindustryLauncher : public QMainWindow
{
    Q_OBJECT

public:
    SkyMindustryLauncher(QWidget *parent = nullptr);
    ~SkyMindustryLauncher();

private:
    Ui::SkyMindustryLauncherClass ui;
};
