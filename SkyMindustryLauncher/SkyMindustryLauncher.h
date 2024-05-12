#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_SkyMindustryLauncher.h"
#include <qgraphicseffect.h>
#include <QMouseEvent>
#include <qfile.h>
#include <qdebug.h>
#include <qstyle.h>
#include <qstyleoption.h>
#include <qfontdatabase.h>

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

private:
	Ui::SkyMindustryLauncherClass ui;
	QPoint m_CurrentPressPoint;
	QPoint m_CurrentPressPoint_global;
};
