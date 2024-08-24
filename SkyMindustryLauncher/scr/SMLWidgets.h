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

public:
	~SMLWidgets();

protected:
	SMLWidgets* previous = nullptr;//上一个界面
	SMLWidgets* next = nullptr;//下一个界面

signals:
	void SubWidgetShow();
	void SubWidgetClose();
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
	QString GetCurrentVersion();//获取当前版本

public slots:
	void on_LaunchButton_clicked();
	void launched();
	void GetGameFinished();

signals:
	void ForceQuit();
};

class ConfigWidget :
	public SMLWidgets {

	Q_OBJECT

public:
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
	QPushButton* TitleIcon;//图标
	QLabel* title;//标题栏
	QScrollArea* VersionList;//滚动区域
	QWidget* VersionListWidget;//滚动区内的区域
	QLabel* InfoText;//提示标语
	GetOnlineGameVersionT* GOGVT;//获取版本列表线程
	QList<VersionInfo> VerList;//版本列表
	QList<InfoButton*> ButtonBox;//界面上的版本列表选项组
	void ArrangeButton(QWidget* parent);//根据版本列表排列选项组
	void showEvent(QShowEvent* e) override;//重写显示事件

public slots:
	void GotVersionList(QList<VersionInfo>);
	void ButtonClicked(int);
	void RefreshList();
	void GetDownloadStart(VersionInfo);

signals:
	void showed();
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

class LaunchLoadingWidget :
	public SMLWidgets {

	Q_OBJECT

public:
	LaunchLoadingWidget(QWidget* parent, SMLWidgets* previous, QString GameName);
private:
	QLabel* title;
	QPushButton* TitleIcon;
	QLabel* LaunchIcon;//启动图标
	QLabel* LaunchTitle;//具体启动信息
	QProgressBar* ProgressBar;//进度条
	QLabel* rate;//进度
	QLabel* LaunchSchedule;//当前处理事项
	double ProgressNumber;

public slots:
	void on_TitleIcon_clicked();
	void GetProgressNumber(double);
	void GetCurrentProgress(CurrentProgress);
	void GetLaunched();

signals:
	void GameLaunched();
};

class DownloadManageWidget :
	public SMLWidgets {

	Q_OBJECT

public:
	DownloadManageWidget(QWidget* parent, SMLWidgets* previous, VersionInfo version);

private:
	QLabel* title;
	QPushButton* TitleIcon;
	QLabel* VersionNameLabel;//命名标签
	QLineEdit* VersionNameEditer;//命名编辑框
	QPushButton* SaveButton;//保存按钮
	VersionInfo Version;

public slots:
	void on_SaveButton_clicked();
	void on_TitleIcon_clicked();

signals:
	void DownloadStart(VersionInfo);
};

class DownloadLoadingWidget :
	public SMLWidgets {

	Q_OBJECT

public:
	DownloadLoadingWidget(QWidget* parent, SMLWidgets* previous, VersionInfo version);
private:
	QLabel* title;
	QPushButton* TitleIcon;
	QLabel* DownloadIcon;//下载图标
	QLabel* DownloadTitle;//具体下载信息
	QProgressBar* ProgressBar;//进度条
	QLabel* rate;//进度
	QLabel* DownloadSchedule;//当前处理事项
	double ProgressNumber;

public slots:
	void on_TitleIcon_clicked();
	void GetProgressNumber(double);
	void GetCurrentProgress(CurrentProgress);
	void GetDownloadFinished();

signals:
	void DownloadPaused();
};
#endif // !_SML_SMLWIDGETS_H_