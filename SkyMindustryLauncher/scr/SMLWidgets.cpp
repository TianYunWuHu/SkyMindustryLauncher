#include "SMLWidgets.h"

HomeWidget::HomeWidget(QWidget* parent) {
	//创建home界面
	this->setParent(parent);
	this->setGeometry(50, 40, 590, 390);
	this->setStyleSheet("background-color: rgb(255, 255, 255);");
	//生成控件
	TitleIcon = new QPushButton(this);
	title = new QLabel(this);
	LaunchBar = new QLabel(this);
	LaunchButton = new QPushButton(this);
	//设置控件属性
	TitleIcon->setGeometry(0, 0, 40, 40);
	TitleIcon->setStyleSheet("border-style:inset;");
	TitleIcon->setIcon(QIcon(":/SkyMindustryLauncher/rec/home.png"));
	TitleIcon->setIconSize(QSize(20, 20));
	TitleIcon->setDisabled(true);
	title->setGeometry(40, 0, 550, 40);
	title->setText("首页");
	LaunchBar->setGeometry(0, 340, 590, 50);
	LaunchBar->setStyleSheet("background-color: rgb(130, 140, 255);color: rgb(255, 255, 255);");
	LaunchBar->setText("   当前选择：无");
	LaunchButton->setGeometry(450, 340, 140, 50);
	LaunchButton->setStyleSheet("QPushButton{background-color: rgba(0, 0, 0, 30);color: rgb(255, 255, 255);border-style: inset;font-size: 20px;}QPushButton:hover{background-color: rgba(0, 0, 0, 60);}QPushButton:pressed{background-color: rgba(0, 0, 0, 90);}");
	LaunchButton->setText("启动游戏");
	this->show();
}

HomeWidget::~HomeWidget() {
	delete TitleIcon;
	delete title;
	delete LaunchBar;
	delete LaunchButton;
}

ConfigWidget::ConfigWidget(QWidget* parent) {
	//创建config界面
	this->setParent(parent);
	this->setGeometry(50, 40, 590, 390);
	this->setStyleSheet("background-color: rgba(0, 0, 0, 0)");
	//生成控件
	TitleIcon = new QPushButton(this);
	title = new QLabel(this);
	VersionList = new QScrollArea(this);
	VersionListWidget = new QWidget(VersionList);
	//设置控件属性
	TitleIcon->setGeometry(0, 0, 40, 40);
	TitleIcon->setStyleSheet("border-style:inset;background-color: rgb(255, 255, 255);");
	TitleIcon->setIcon(QIcon(":/SkyMindustryLauncher/rec/config.png"));
	TitleIcon->setIconSize(QSize(20, 20));
	TitleIcon->setDisabled(true);
	title->setGeometry(40, 0, 550, 40);
	title->setStyleSheet("background-color: rgb(255, 255, 255);");
	title->setText("配置游戏");
	VersionListWidget->setGeometry(0, 0, 580, 400);
	VersionListWidget->setMinimumSize(QSize(580, 400));
	VersionListWidget->setStyleSheet("background-color: rgba(0, 0, 0, 0);");
	VersionList->setGeometry(0, 40, 590, 350);
	VersionList->setStyleSheet("border: none;");
	VersionList->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{width: 10px;padding-top: 0px;padding-bottom: 0px;}QScrollBar::handle:vertical{background-color: rgb(140, 140, 140)}QScrollBar::handle:vertical:hover{background-color: rgb(90, 90, 90)}QScrollBar::add-line:vertical{height: 0px;width: 10px;subcontrol-position: bottom;}QScrollBar::sub-line:vertical{height: 0px;width: 10px;subcontrol-position: top;}QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical{background-color: rgba(0, 0, 0, 0);}");
	VersionList->setWidget(VersionListWidget);
	VersionList->setWidgetResizable(true);
	this->show();
}

ConfigWidget::~ConfigWidget() {
	delete TitleIcon;
	delete title;
	delete VersionListWidget;
	delete VersionList;
}

DownloadWidget::DownloadWidget(QWidget* parent) {
	//创建download界面
	this->setParent(parent);
	this->setGeometry(50, 40, 590, 390);
	this->setStyleSheet("background-color: rgba(0, 0, 0, 0)");
	//生成控件
	TitleIcon = new QPushButton(this);
	title = new QLabel(this);
	VersionList = new QScrollArea(this);
	VersionListWidget = new QWidget(VersionList);
	//设置控件属性
	TitleIcon->setGeometry(0, 0, 40, 40);
	TitleIcon->setStyleSheet("border-style:inset;background-color: rgb(255, 255, 255);");
	TitleIcon->setIcon(QIcon(":/SkyMindustryLauncher/rec/download.png"));
	TitleIcon->setIconSize(QSize(20, 20));
	TitleIcon->setDisabled(true);
	title->setGeometry(40, 0, 550, 40);
	title->setStyleSheet("background-color: rgb(255, 255, 255);");
	title->setText("下载");
	VersionListWidget->setGeometry(0, 0, 580, 400);
	VersionListWidget->setMinimumSize(QSize(580, 400));
	VersionListWidget->setStyleSheet("background-color: rgba(0, 0, 0, 0);");
	VersionList->setGeometry(0, 40, 590, 350);
	VersionList->setStyleSheet("border: none;");
	VersionList->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{width: 10px;padding-top: 0px;padding-bottom: 0px;}QScrollBar::handle:vertical{background-color: rgb(140, 140, 140)}QScrollBar::handle:vertical:hover{background-color: rgb(90, 90, 90)}QScrollBar::add-line:vertical{height: 0px;width: 10px;subcontrol-position: bottom;}QScrollBar::sub-line:vertical{height: 0px;width: 10px;subcontrol-position: top;}QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical{background-color: rgba(0, 0, 0, 0);}");
	VersionList->setWidget(VersionListWidget);
	VersionList->setWidgetResizable(true);
	this->show();
}

DownloadWidget::~DownloadWidget() {
	delete TitleIcon;
	delete title;
	delete VersionListWidget;
	delete VersionList;
}

SettingsWidget::SettingsWidget(QWidget* parent) {
	//创建settings界面
	this->setParent(parent);
	this->setGeometry(50, 40, 590, 390);
	this->setStyleSheet("background-color: rgba(0, 0, 0, 0)");
	//生成控件
	TitleIcon = new QPushButton(this);
	title = new QLabel(this);
	submenu = new QWidget(this);
	OptionScrollArea = new QScrollArea(this);
	OptionWidget = new QWidget(OptionScrollArea);
	//设置控件属性
	TitleIcon->setGeometry(0, 0, 40, 40);
	TitleIcon->setStyleSheet("border-style:inset;background-color: rgb(255, 255, 255);");
	TitleIcon->setIcon(QIcon(":/SkyMindustryLauncher/rec/settings.png"));
	TitleIcon->setIconSize(QSize(20, 20));
	TitleIcon->setDisabled(true);
	title->setGeometry(40, 0, 550, 40);
	title->setStyleSheet("background-color: rgb(255, 255, 255);");
	title->setText("设置");
	submenu->setGeometry(0, 40, 100, 350);
	submenu->setStyleSheet("background-color: rgba(170, 170, 255, 150);");
	OptionWidget->setGeometry(0, 0, 480, 400);
	OptionWidget->setMinimumSize(QSize(480, 400));
	OptionWidget->setStyleSheet("background-color: rgba(0, 0, 0, 0);");
	OptionScrollArea->setGeometry(100, 40, 490, 350);
	OptionScrollArea->setStyleSheet("border: none;");
	OptionScrollArea->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{width: 10px;padding-top: 0px;padding-bottom: 0px;}QScrollBar::handle:vertical{background-color: rgb(140, 140, 140)}QScrollBar::handle:vertical:hover{background-color: rgb(90, 90, 90)}QScrollBar::add-line:vertical{height: 0px;width: 10px;subcontrol-position: bottom;}QScrollBar::sub-line:vertical{height: 0px;width: 10px;subcontrol-position: top;}QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical{background-color: rgba(0, 0, 0, 0);}");
	OptionScrollArea->setWidget(OptionWidget);
	OptionScrollArea->setWidgetResizable(true);
	this->show();
}

SettingsWidget::~SettingsWidget() {
	delete TitleIcon;
	delete title;
	delete submenu;
	delete OptionWidget;
	delete OptionScrollArea;
}