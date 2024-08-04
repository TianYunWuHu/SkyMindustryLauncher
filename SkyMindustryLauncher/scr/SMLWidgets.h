#ifndef _SML_SMLWIDGETS_H_
#define _SML_SMLWIDGETS_H_

#include <qwidget.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qicon.h>
#include <qsize.h>
#include <qscrollarea.h>
#include <qscrollbar.h>
#include <qlist.h>
#include "scr/log.h"
#include "scr/SMLThread.h"
#include "scr/SMLCustomControl.h"

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

private:
	QPushButton* TitleIcon;
	QLabel* title;
	QLabel* LaunchBar;
	QPushButton* LaunchButton;
};

class ConfigWidget :
	public SMLWidgets {

	Q_OBJECT

public:
	ConfigWidget(QWidget* parent);

public slots:
	void GotVersionList(QList<VersionInfo>);

private:
	QPushButton* TitleIcon;//图标
	QLabel* title;//标题栏
	QScrollArea* VersionList;//滚动区域
	QWidget* VersionListWidget;//滚动区内的区域
	QLabel* InfoText;//提示标语
	GetVersionListT* GVLT;//获取版本列表线程
	QList<VersionInfo>* VerList;//版本列表
	QList<InfoButton*> ButtonBox;//界面上的版本列表选项组
	void ArrangeButton(QWidget* parent);//根据版本列表排列选项组
};

class DownloadWidget :
	public SMLWidgets {

	Q_OBJECT

public:
	DownloadWidget(QWidget* parent);

private:
	QPushButton* TitleIcon;
	QLabel* title;
	QScrollArea* VersionList;
	QWidget* VersionListWidget;
};

class SettingsWidget :
	public SMLWidgets {

	Q_OBJECT

public:
	SettingsWidget(QWidget* parent);

private:
	QPushButton* TitleIcon;
	QLabel* title;
	QWidget* submenu;
	QScrollArea* OptionScrollArea;
	QWidget* OptionWidget;
};
#endif // !_SML_SMLWIDGETS_H_