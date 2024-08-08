#ifndef _SML_SMLWIDGETS_H_
#define _SML_SMLWIDGETS_H_

#include "stdafx.h"
#include "scr/log.h"
#include "scr/SMLMessageBox.h"
#include "scr/SMLThread.h"
#include "scr/SMLCustomControl.h"

class SMLWidgets :
	public QWidget
{

	Q_OBJECT

protected:
	SMLWidgets* previous = nullptr;//上一个界面
	SMLWidgets* next = nullptr;//下一个界面

};

class HomeWidget :
	public SMLWidgets {

	Q_OBJECT

public:
	HomeWidget(QWidget* parent);

private:
	QPushButton* TitleIcon;//图标
	QLabel* title;//标题栏
	QLabel* LaunchBar;//启动栏
	QPushButton* LaunchButton;//启动按钮
	QSettings* setting;//启动器设置
	GameT* GameThread;
	QString GetCurrentVersion();//获取当前版本

public slots:
	void on_LaunchButton_clicked();
};

class ConfigWidget :
	public SMLWidgets {

	Q_OBJECT

public:
	~ConfigWidget();
	ConfigWidget(QWidget* parent);

public slots:
	void GotVersionList(QList<VersionInfo>);
	void ButtonClicked(int);
	void RefreshList();
	void EditButtonClicked(int);

private:
	QPushButton* TitleIcon;//图标
	QLabel* title;//标题栏
	QScrollArea* VersionList;//滚动区域
	QWidget* VersionListWidget;//滚动区内的区域
	QLabel* InfoText;//提示标语
	GetVersionListT* GVLT;//获取版本列表线程
	QList<VersionInfo> VerList;//版本列表
	QList<InfoButton*> ButtonBox;//界面上的版本列表选项组
	QList<InfoButton*> EditButtonBox;//版本编辑按钮
	QList<QLabel*> SelectBox;//是否选中标识
	void ArrangeButton(QWidget* parent);//根据版本列表排列选项组
	void showEvent(QShowEvent* e) override;//重写显示事件
	int GetSelect();//获取当前选中的版本

signals:
	void showed();
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

class VersionManageWidget :
	public SMLWidgets {

	Q_OBJECT

public:
	VersionManageWidget(QWidget* parent, SMLWidgets* previous, QString SettingPath);

private:
	QLabel* title;
	QPushButton* TitleIcon;
	QLabel* VersionNameLabel;//重命名标签
	QLineEdit* VersionNameEditer;//重命名编辑框
	QPushButton* SaveButton;//保存按钮
	QSettings* setting;

public slots:
	void on_SaveButton_clicked();
	void on_TitleIcon_clicked();
};
#endif // !_SML_SMLWIDGETS_H_