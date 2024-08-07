#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_SkyMindustryLauncher.h"
#include "stdafx.h"
#include "scr/log.h"
#include "scr/SMLWidgets.h"
#include "scr/SMLMessageBox.h"

class SkyMindustryLauncher : public QMainWindow
{
	Q_OBJECT

public:
	SkyMindustryLauncher(QWidget* parent = nullptr);
	~SkyMindustryLauncher();

public slots:
	void mousePressEvent(QMouseEvent* e);
	void mouseMoveEvent(QMouseEvent* e);
	void on_CloseButton_clicked();
	void on_MiniButton_clicked();
	void on_HomeButton_clicked();
	void on_ConfigButton_clicked();
	void on_DownloadButton_clicked();
	void on_SettingsButton_clicked();

private:
	Ui::SkyMindustryLauncherClass ui;
	QPoint m_CurrentPressPoint;
	QPoint m_CurrentPressPoint_global;
	SMLWidgets* CurrentWidget;
	QSettings* setting;
	QDir dir;

	void LessWindowsHint();//无边框
	void EnvironmentINIT();
};
