﻿#ifndef _SML_SMLWIDGETS_H_
#define _SML_SMLWIDGETS_H_

#include <qwidget.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qicon.h>
#include <qsize.h>
#include "log.h"

class SMLWidgets :
	public QWidget
{

	Q_OBJECT

};

class HomeWidget :
	public SMLWidgets {

	Q_OBJECT

public:
	HomeWidget(QWidget* parent);
	~HomeWidget();

private:
	QPushButton* TitleIcon;
	QLabel* title;
	QLabel* LaunchBar;
	QPushButton* LaunchButton;
};

#endif // !_SML_SMLWIDGETS_H_