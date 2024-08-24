#include "stdafx.h"
#include "SMLWidgets.h"

SMLWidgets::~SMLWidgets() {
	next = nullptr;
	previous = nullptr;
}

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
	setting = new QSettings("./SML/settings.ini", QSettings::IniFormat);
	//设置控件属性
	TitleIcon->setGeometry(0, 0, 40, 40);
	TitleIcon->setStyleSheet("border-style:inset;");
	TitleIcon->setIcon(QIcon(":/SkyMindustryLauncher/rec/home.png"));
	TitleIcon->setIconSize(QSize(20, 20));
	TitleIcon->setDisabled(true);
	title->setGeometry(40, 0, 550, 40);
	title->setText("首页");
	LaunchBar->setGeometry(0, 340, 590, 50);
	LaunchBar->setText("   当前选择：" + GetCurrentVersion());
	LaunchBar->setStyleSheet("background-color: rgb(130, 140, 255);color: rgb(255, 255, 255);");
	LaunchButton->setGeometry(450, 340, 140, 50);
	LaunchButton->setStyleSheet("QPushButton{background-color: rgba(0, 0, 0, 30);color: rgb(255, 255, 255);border-style: inset;font-size: 20px;}QPushButton:hover{background-color: rgba(0, 0, 0, 60);}QPushButton:pressed{background-color: rgba(0, 0, 0, 90);}");
	LaunchButton->setText(setting->value("/game/isRunning").toBool() ? "停止游戏" : "启动游戏");
	connect(LaunchButton, SIGNAL(clicked()), this, SLOT(on_LaunchButton_clicked()));
	this->show();
}

QString HomeWidget::GetCurrentVersion() {
	if (setting->value("/game/CurrentVersion").toString() != "") {
		return setting->value("/game/CurrentVersion").toString();
	}
	else
	{
		return "无";
	}
}

void HomeWidget::on_LaunchButton_clicked() {
	if (LaunchButton->text() == "启动游戏") {
		if (GetCurrentVersion() != "无") {
			connect(this->parent()->parent()->parent(), SIGNAL(GameFinish()), this, SLOT(GetGameFinished()));
			isGameCanLaunch = true;
			next = new LaunchLoadingWidget(this->parentWidget(), this, GetCurrentVersion());
			next->show();
			this->hide();
		}
		else
		{
			SMLMessageBox::msgbox(this->parentWidget(), Info, "当前版本不可启动");
		}
	}
	else if (LaunchButton->text() == "停止游戏") {
		if (SMLMessageBox::msgbox(this->parentWidget(), Warn, "强制停止游戏可能会导致存档丢失，是否继续？") == 1) {
			connect(this, SIGNAL(ForceQuit()), GameMain, SLOT(ForceQuit()));
			emit ForceQuit();
		}
	}
}

void HomeWidget::launched() {
	LaunchButton->setText("停止游戏");
}

void HomeWidget::GetGameFinished() {
	LaunchButton->setText("启动游戏");
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
	InfoText = new QLabel(this);
	GVLT = new GetVersionListT();
	//设置控件属性
	TitleIcon->setGeometry(0, 0, 40, 40);
	TitleIcon->setStyleSheet("border-style:inset;background-color: rgb(255, 255, 255);");
	TitleIcon->setIcon(QIcon(":/SkyMindustryLauncher/rec/config.png"));
	TitleIcon->setIconSize(QSize(20, 20));
	TitleIcon->setDisabled(true);
	title->setGeometry(40, 0, 550, 40);
	title->setStyleSheet("background-color: rgb(255, 255, 255);");
	title->setText("配置游戏");
	VersionList->setGeometry(0, 40, 590, 350);
	VersionList->setStyleSheet("border: none;");
	VersionList->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{width: 10px;padding-top: 0px;padding-bottom: 0px;}QScrollBar::handle:vertical{background-color: rgb(140, 140, 140)}QScrollBar::handle:vertical:hover{background-color: rgb(90, 90, 90)}QScrollBar::add-line:vertical{height: 0px;width: 10px;subcontrol-position: bottom;}QScrollBar::sub-line:vertical{height: 0px;width: 10px;subcontrol-position: top;}QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical{background-color: rgba(0, 0, 0, 0);}");
	VersionList->setWidgetResizable(true);
	InfoText->setGeometry(0, 155, 590, 40);
	InfoText->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
	InfoText->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	InfoText->setText("正在加载版本列表...");
	//设置获取版本列表线程
	qRegisterMetaType<QList<VersionInfo>>("QList<VersionInfo>");
	connect(GVLT, SIGNAL(GetVersionList(QList<VersionInfo>)), this, SLOT(GotVersionList(QList<VersionInfo>)));
	//连接刷新函数
	connect(this, SIGNAL(showed()), this, SLOT(RefreshList()));
	this->show();
}

void ConfigWidget::GotVersionList(QList<VersionInfo> verList) {
	this->VerList = verList;
	VersionListWidget = new QWidget(VersionList);
	VersionListWidget->setGeometry(0, 0, 590, 0);
	VersionListWidget->setStyleSheet("background-color: rgba(0, 0, 0, 0);");
	ArrangeButton(VersionListWidget);
	VersionList->setWidget(VersionListWidget);
	VersionListWidget->show();
}

void ConfigWidget::ArrangeButton(QWidget* parent) {
	if (VerList.size() != 0) {
		int w = 590;
		if (VerList.size() > 5) {
			w = 580;
		}
		for (int i = 0; i < VerList.size(); i++) {
			QLabel* tempLabel = new QLabel(parent);
			tempLabel->setGeometry(15, i * 60 + 15, 30, 30);
			tempLabel->setScaledContents(true);
			if (GetSelect() == i) {
				tempLabel->setPixmap(QPixmap(":/SkyMindustryLauncher/rec/selected.png"));
			}
			SelectBox.append(tempLabel);
		}
		for (int i = 0; i < VerList.size(); i++) {
			InfoButton* tempButton = new InfoButton(parent);
			tempButton->setText(VerList.at(i).name);
			tempButton->setGeometry(0, i * 60, w, 60);
			tempButton->setStyleSheet("QPushButton{background-color: rgba(0, 0, 0, 30);color: rgb(255, 255, 255);border-style: inset;font-size: 20px;text-align: left;border-left-width: 60px;border-left-color: rgba(0, 0, 0, 0);}QPushButton:hover{background-color: rgba(0, 0, 0, 60);}QPushButton:pressed{background-color: rgba(0, 0, 0, 90);}");
			tempButton->setSubTitle(VerList.at(i).ver);
			tempButton->number = i;
			connect(tempButton, SIGNAL(Number(int)), this, SLOT(ButtonClicked(int)));
			ButtonBox.append(tempButton);
		}
		for (int i = 0; i < VerList.size(); i++) {
			InfoButton* tempButton = new InfoButton(parent);
			tempButton->setText("");
			tempButton->setIcon(QIcon(":/SkyMindustryLauncher/rec/edit.png"));
			tempButton->setIconSize(QSize(25, 25));
			tempButton->setGeometry(w - 60, i * 60, 60, 60);
			tempButton->setStyleSheet("QPushButton{background-color: rgba(0, 0, 0, 0);border-style: inset;}QPushButton:hover{background-color: rgba(0, 0, 0, 30);}QPushButton:pressed{background-color: rgba(0, 0, 0, 60);}");
			tempButton->number = i;
			connect(tempButton, SIGNAL(Number(int)), this, SLOT(EditButtonClicked(int)));
			EditButtonBox.append(tempButton);
		}
		VersionListWidget->setMinimumSize(w, VerList.size() * 60);
		InfoText->lower();
		InfoText->setText("");
	}
	else
	{
		InfoText->raise();
		InfoText->setText("当前无版本，快去下载吧！");
	}
}

void ConfigWidget::ButtonClicked(int i) {
	QSettings* tempSetting = new QSettings("./SML/settings.ini", QSettings::IniFormat);
	if (tempSetting->value("/game/CurrentVersion").toString() != "") {
		if (VerList.at(i).name != tempSetting->value("/game/CurrentVersion").toString()) {
			SelectBox.at(GetSelect())->setPixmap(QPixmap(""));
			SelectBox.at(i)->setPixmap(QPixmap(":/SkyMindustryLauncher/rec/selected.png"));
			tempSetting->setValue("/game/CurrentVersion", VerList.at(i).name);
		}
	}
	else
	{
		SelectBox.at(i)->setPixmap(QPixmap(":/SkyMindustryLauncher/rec/selected.png"));
		tempSetting->setValue("/game/CurrentVersion", VerList.at(i).name);
	}
}

void ConfigWidget::EditButtonClicked(int i) {
	next = new VersionManageWidget(this->parentWidget(), this, "./Game/" + VerList.at(i).name + "/" + VerList.at(i).name + ".ini");
	this->hide();
}

void ConfigWidget::RefreshList() {
	InfoText->setText("正在加载版本列表...");
	ButtonBox.clear();
	EditButtonBox.clear();
	SelectBox.clear();
	VerList.clear();
	delete VersionListWidget;
	GVLT->start();
}

void ConfigWidget::showEvent(QShowEvent* e) {
	QWidget::showEvent(e);
	emit showed();
}

int ConfigWidget::GetSelect() {
	for (int i = 0; i < VerList.size(); i++) {
		QSettings* tempSetting = new QSettings("./SML/settings.ini", QSettings::IniFormat);
		if (VerList.at(i).name == tempSetting->value("/game/CurrentVersion").toString()) {
			return i;
		}
	}
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
	InfoText = new QLabel(this);
	GOGVT = new GetOnlineGameVersionT();
	//设置控件属性
	TitleIcon->setGeometry(0, 0, 40, 40);
	TitleIcon->setStyleSheet("border-style:inset;background-color: rgb(255, 255, 255);");
	TitleIcon->setIcon(QIcon(":/SkyMindustryLauncher/rec/download.png"));
	TitleIcon->setIconSize(QSize(20, 20));
	TitleIcon->setDisabled(true);
	title->setGeometry(40, 0, 550, 40);
	title->setStyleSheet("background-color: rgb(255, 255, 255);");
	title->setText("下载");
	VersionList->setGeometry(0, 40, 590, 350);
	VersionList->setStyleSheet("border: none;");
	VersionList->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{width: 10px;padding-top: 0px;padding-bottom: 0px;}QScrollBar::handle:vertical{background-color: rgb(140, 140, 140)}QScrollBar::handle:vertical:hover{background-color: rgb(90, 90, 90)}QScrollBar::add-line:vertical{height: 0px;width: 10px;subcontrol-position: bottom;}QScrollBar::sub-line:vertical{height: 0px;width: 10px;subcontrol-position: top;}QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical{background-color: rgba(0, 0, 0, 0);}");
	VersionList->setWidgetResizable(true);
	InfoText->setGeometry(0, 155, 590, 40);
	InfoText->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
	InfoText->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	InfoText->setText("正在加载游戏版本列表...");
	//设置获取版本列表线程
	qRegisterMetaType<QList<VersionInfo>>("QList<VersionInfo>");
	connect(GOGVT, SIGNAL(GetVersionList(QList<VersionInfo>)), this, SLOT(GotVersionList(QList<VersionInfo>)));
	//连接刷新函数
	connect(this, SIGNAL(showed()), this, SLOT(RefreshList()));
	this->show();
}

void DownloadWidget::showEvent(QShowEvent* e) {
	QWidget::showEvent(e);
	emit showed();
}

void DownloadWidget::GotVersionList(QList<VersionInfo> verList) {
	this->VerList = verList;
	VersionListWidget = new QWidget(VersionList);
	VersionListWidget->setGeometry(0, 0, 590, 0);
	VersionListWidget->setStyleSheet("background-color: rgba(0, 0, 0, 0);");
	ArrangeButton(VersionListWidget);
	VersionList->setWidget(VersionListWidget);
	VersionListWidget->show();
}

void DownloadWidget::ArrangeButton(QWidget* parent) {
	if (VerList.size() != 0) {
		int w = 590;
		if (VerList.size() > 5) {
			w = 580;
		}
		for (int i = 0; i < VerList.size(); i++) {
			QLabel* tempLabel = new QLabel(parent);
			tempLabel->setGeometry(w - 45, i * 60 + 15, 30, 30);
			tempLabel->setScaledContents(true);
			tempLabel->setPixmap(QPixmap(":/SkyMindustryLauncher/rec/go.png"));
		}
		for (int i = 0; i < VerList.size(); i++) {
			InfoButton* tempButton = new InfoButton(parent);
			tempButton->setText(VerList.at(i).ver);
			tempButton->setGeometry(0, i * 60, w, 60);
			tempButton->setStyleSheet("QPushButton{background-color: rgba(0, 0, 0, 30);color: rgb(255, 255, 255);border-style: inset;font-size: 20px;text-align: left;}QPushButton:hover{background-color: rgba(0, 0, 0, 60);}QPushButton:pressed{background-color: rgba(0, 0, 0, 90);}");
			tempButton->number = i;
			connect(tempButton, SIGNAL(Number(int)), this, SLOT(ButtonClicked(int)));
			ButtonBox.append(tempButton);
		}
		VersionListWidget->setMinimumSize(w, VerList.size() * 60);
		InfoText->lower();
		InfoText->setText("");
	}
	else
	{
		InfoText->raise();
		InfoText->setText("获取版本失败,请检查网络");
	}
}

void DownloadWidget::ButtonClicked(int i) {
	next = new DownloadManageWidget(this->parentWidget(), this, VerList.at(i));
	this->hide();
}

void DownloadWidget::RefreshList() {
	InfoText->setText("正在加载版本列表...");
	ButtonBox.clear();
	VerList.clear();
	delete VersionListWidget;
	GOGVT->start();
}

void DownloadWidget::GetDownloadStart(VersionInfo version) {
	next = new DownloadLoadingWidget(this->parentWidget(), this, version);
	next->show();
	this->hide();
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

VersionManageWidget::VersionManageWidget(QWidget* parent, SMLWidgets* previous, QString SettingPath) {
	//变量初始化
	setting = new QSettings(SettingPath, QSettings::IniFormat);
	this->previous = previous;
	//创建版本管理界面
	this->setParent(parent);
	this->setGeometry(50, 40, 590, 390);
	this->setStyleSheet("background-color: rgba(0, 0, 0, 0)");
	//生成控件
	TitleIcon = new QPushButton(this);
	title = new QLabel(this);
	VersionNameLabel = new QLabel(this);
	VersionNameEditer = new QLineEdit(this);
	SaveButton = new QPushButton(this);
	//设置控件属性
	TitleIcon->setGeometry(0, 0, 40, 40);
	TitleIcon->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255);color: rgb(255, 255, 255);border-style: inset;font-size: 20px;}QPushButton:hover{background-color: rgb(225, 225, 225);}QPushButton:pressed{background-color: rgb(195, 195, 195);}");
	TitleIcon->setIcon(QIcon(":/SkyMindustryLauncher/rec/back.png"));
	TitleIcon->setIconSize(QSize(20, 20));
	connect(TitleIcon, SIGNAL(clicked()), this, SLOT(on_TitleIcon_clicked()));
	title->setGeometry(40, 0, 550, 40);
	title->setStyleSheet("background-color: rgb(255, 255, 255);");
	title->setText("配置游戏-" + setting->value("/game/name").toString());
	VersionNameLabel->setGeometry(40, 60, 90, 30);
	VersionNameLabel->setText("版本名称");
	VersionNameEditer->setGeometry(130, 60, 420, 30);
	VersionNameEditer->setText(setting->value("/game/name").toString());
	VersionNameEditer->setStyleSheet("QLineEdit{border-style: inset;border-color: rgb(0, 170, 255);border-width: 1px 1px 1px 1px;}QLineEdit:focus{border-width: 1.5px 1.5px 1.5px 1.5px;}");
	VersionNameEditer->setPlaceholderText("最多64字符");
	VersionNameEditer->setMaxLength(64);
	SaveButton->setGeometry(440, 340, 110, 40);
	SaveButton->setStyleSheet("QPushButton{background-color: rgba(0, 0, 0, 30);color: rgb(255, 255, 255);border-style: inset;font-size: 20px;}QPushButton:hover{background-color: rgba(0, 0, 0, 60);}QPushButton:pressed{background-color: rgba(0, 0, 0, 90);}");
	SaveButton->setText("保存");
	connect(SaveButton, SIGNAL(clicked()), this, SLOT(on_SaveButton_clicked()));

	connect(this, SIGNAL(SubWidgetShow()), MainWidget, SLOT(SubWidgetShowed()));
	emit SubWidgetShow();
	this->show();
}

void VersionManageWidget::on_SaveButton_clicked() {
	//重命名检测
	if (VersionNameEditer->text() == "") {
		SMLMessageBox::msgbox(this->parentWidget(), Error, "输入内容不能为空（E0001）");
	}
	else
	{
		if (VersionNameEditer->text() == "无") {
			SMLMessageBox::msgbox(this->parentWidget(), Info, "你在开玩笑吗:P");
		}
		else
		{
			//普通版本重命名
			QString OldName(setting->value("/game/name").toString());
			setting->setValue("/game/name", VersionNameEditer->text());
			delete setting;
			QFile::rename(QDir::currentPath() + "/Game/" + OldName + "/" + OldName + ".ini", QDir::currentPath() + "/Game/" + OldName + "/" + VersionNameEditer->text() + ".ini");
			QDir dir(QDir::currentPath() + "/Game");
			dir.rename(QDir::currentPath() + "/Game/" + OldName, QDir::currentPath() + "/Game/" + VersionNameEditer->text());
			setting = new QSettings("./SML/settings.ini", QSettings::IniFormat);
			setting->setValue("/game/CurrentVersion", VersionNameEditer->text());
			delete setting;
			//返回配置界面
			connect(this, SIGNAL(SubWidgetClose()), MainWidget, SLOT(SubWidgetClosed()));
			emit SubWidgetClose();
			previous->show();
			this->close();
		}
	}
}

void VersionManageWidget::on_TitleIcon_clicked() {
	connect(this, SIGNAL(SubWidgetClose()), MainWidget, SLOT(SubWidgetClosed()));
	emit SubWidgetClose();
	previous->show();
	this->close();
}

LaunchLoadingWidget::LaunchLoadingWidget(QWidget* parent, SMLWidgets* previous, QString GameName) {
	//变量初始化
	this->previous = previous;
	//创建启动时加载界面
	this->setParent(parent);
	this->setGeometry(50, 40, 590, 390);
	this->setStyleSheet("background-color: rgba(0, 0, 0, 0)");
	//生成控件
	TitleIcon = new QPushButton(this);
	title = new QLabel(this);
	LaunchIcon = new QLabel(this);
	LaunchTitle = new QLabel(this);
	ProgressBar = new QProgressBar(this);
	rate = new QLabel(this);
	LaunchSchedule = new QLabel(this);
	//设置控件属性
	TitleIcon->setGeometry(0, 0, 40, 40);
	TitleIcon->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255);color: rgb(255, 255, 255);border-style: inset;}QPushButton:hover{background-color: rgb(225, 225, 225);}QPushButton:pressed{background-color: rgb(195, 195, 195);}");
	TitleIcon->setIcon(QIcon(":/SkyMindustryLauncher/rec/back.png"));
	TitleIcon->setIconSize(QSize(20, 20));
	connect(TitleIcon, SIGNAL(clicked()), this, SLOT(on_TitleIcon_clicked()));
	title->setGeometry(40, 0, 550, 40);
	title->setStyleSheet("background-color: rgb(255, 255, 255);");
	title->setText("启动游戏-" + GameName);
	LaunchIcon->setGeometry(245, 140, 100, 100);
	LaunchIcon->setPixmap(QPixmap(":/SkyMindustryLauncher/rec/launch.png"));
	LaunchIcon->setScaledContents(true);
	LaunchTitle->setGeometry(0, 240, 590, 40);
	LaunchTitle->setStyleSheet("font-size: 20px;");
	LaunchTitle->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	LaunchTitle->setText("正在启动：" + GameName);
	ProgressBar->setGeometry(0, 380, 590, 10);
	ProgressBar->setStyleSheet("QProgressBar{background-color: rgba(0, 0, 0, 30);color: rgb(85, 255, 255);border: 0px}QProgressBar::chunk{background-color: rgb(110, 210, 230);}");
	ProgressBar->setTextVisible(false);
	ProgressBar->setValue(0);
	rate->setGeometry(535, 360, 50, 20);
	rate->setStyleSheet("color: rgb(120, 120, 120);");
	rate->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	rate->setText("0%");
	LaunchSchedule->setGeometry(0, 360, 250, 20);
	LaunchSchedule->setStyleSheet("color: rgb(120, 120, 120);");
	LaunchSchedule->setText("准备工作...");
	//启动游戏进程
	GameMain = new GameT(GameName);
	//连接游戏进程
	qRegisterMetaType<CurrentProgress>("CurrentProgress");
	connect(GameMain, SIGNAL(ProgressNumber(double)), this, SLOT(GetProgressNumber(double)));
	connect(GameMain, SIGNAL(progress(CurrentProgress)), this, SLOT(GetCurrentProgress(CurrentProgress)));
	connect(GameMain, SIGNAL(launched()), this, SLOT(GetLaunched()));
	connect(GameMain, SIGNAL(finished()), parent->parent()->parent(), SLOT(GameFinished()));

	connect(this, SIGNAL(SubWidgetShow()), MainWidget, SLOT(SubWidgetShowed()));
	emit SubWidgetShow();
	GameMain->start();
	this->show();
}

void LaunchLoadingWidget::on_TitleIcon_clicked() {
	connect(this, SIGNAL(SubWidgetClose()), MainWidget, SLOT(SubWidgetClosed()));
	emit SubWidgetClose();
	MUTEX.lock();
	isGameCanLaunch = false;
	MUTEX.unlock();
	previous->show();
	this->close();
}

void LaunchLoadingWidget::GetProgressNumber(double i) {
	ProgressNumber = i;
}

void LaunchLoadingWidget::GetCurrentProgress(CurrentProgress c) {
	ProgressBar->setValue(qRound((1 / ProgressNumber * 100 * c.number) - ((100 - c.percent) / (1 / ProgressNumber * 100))));
	rate->setText(QString::number(ProgressBar->value()) + "%");
	LaunchSchedule->setText(c.matter + "(" + QString::number(c.number) + "/" + QString::number(ProgressNumber) + ")");
}

void LaunchLoadingWidget::GetLaunched() {
	connect(this, SIGNAL(SubWidgetClose()), MainWidget, SLOT(SubWidgetClosed()));
	emit SubWidgetClose();
	connect(this, SIGNAL(GameLaunched()), previous, SLOT(launched()));
	emit GameLaunched();
	previous->show();
	this->close();
}

DownloadManageWidget::DownloadManageWidget(QWidget* parent, SMLWidgets* previous, VersionInfo version) {
	//变量初始化
	this->previous = previous;
	Version = version;
	//创建下载信息界面
	this->setParent(parent);
	this->setGeometry(50, 40, 590, 390);
	this->setStyleSheet("background-color: rgba(0, 0, 0, 0)");
	//生成控件
	TitleIcon = new QPushButton(this);
	title = new QLabel(this);
	VersionNameLabel = new QLabel(this);
	VersionNameEditer = new QLineEdit(this);
	SaveButton = new QPushButton(this);
	//设置控件属性
	TitleIcon->setGeometry(0, 0, 40, 40);
	TitleIcon->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255);color: rgb(255, 255, 255);border-style: inset;font-size: 20px;}QPushButton:hover{background-color: rgb(225, 225, 225);}QPushButton:pressed{background-color: rgb(195, 195, 195);}");
	TitleIcon->setIcon(QIcon(":/SkyMindustryLauncher/rec/back.png"));
	TitleIcon->setIconSize(QSize(20, 20));
	connect(TitleIcon, SIGNAL(clicked()), this, SLOT(on_TitleIcon_clicked()));
	title->setGeometry(40, 0, 550, 40);
	title->setStyleSheet("background-color: rgb(255, 255, 255);");
	title->setText("下载游戏-" + version.ver);
	VersionNameLabel->setGeometry(40, 60, 90, 30);
	VersionNameLabel->setText("版本名称");
	VersionNameEditer->setGeometry(130, 60, 420, 30);
	VersionNameEditer->setText(version.ver);
	VersionNameEditer->setStyleSheet("QLineEdit{border-style: inset;border-color: rgb(0, 170, 255);border-width: 1px 1px 1px 1px;}QLineEdit:focus{border-width: 1.5px 1.5px 1.5px 1.5px;}");
	VersionNameEditer->setPlaceholderText("最多64字符");
	VersionNameEditer->setMaxLength(64);
	SaveButton->setGeometry(440, 340, 110, 40);
	SaveButton->setStyleSheet("QPushButton{background-color: rgba(0, 0, 0, 30);color: rgb(255, 255, 255);border-style: inset;font-size: 20px;}QPushButton:hover{background-color: rgba(0, 0, 0, 60);}QPushButton:pressed{background-color: rgba(0, 0, 0, 90);}");
	SaveButton->setText("下载");
	connect(SaveButton, SIGNAL(clicked()), this, SLOT(on_SaveButton_clicked()));
	//连接下载下载时加载界面
	qRegisterMetaType<VersionInfo>("VersionInfo");
	connect(this, SIGNAL(DownloadStart(VersionInfo)), previous, SLOT(GetDownloadStart(VersionInfo)));

	connect(this, SIGNAL(SubWidgetShow()), MainWidget, SLOT(SubWidgetShowed()));
	emit SubWidgetShow();
	this->show();
}

void DownloadManageWidget::on_SaveButton_clicked() {
	QDir dir;
	if (dir.mkdir("./Game/" + VersionNameEditer->text())) {
		connect(this, SIGNAL(SubWidgetClose()), MainWidget, SLOT(SubWidgetClosed()));
		emit SubWidgetClose();
		Version.name = VersionNameEditer->text();
		emit DownloadStart(Version);
		this->close();
	}
	else {
		SMLMessageBox::msgbox(this->parentWidget()->parentWidget()->parentWidget(), Error, "名称已存在，请重新命名（E0002）");
	}
}

void DownloadManageWidget::on_TitleIcon_clicked() {
	connect(this, SIGNAL(SubWidgetClose()), MainWidget, SLOT(SubWidgetClosed()));
	emit SubWidgetClose();
	previous->show();
	this->close();
}

DownloadLoadingWidget::DownloadLoadingWidget(QWidget* parent, SMLWidgets* previous, VersionInfo version) {
	//变量初始化
	this->previous = previous;
	//创建下载时加载界面
	this->setParent(parent);
	this->setGeometry(50, 40, 590, 390);
	this->setStyleSheet("background-color: rgba(0, 0, 0, 0)");
	//生成控件
	TitleIcon = new QPushButton(this);
	title = new QLabel(this);
	DownloadIcon = new QLabel(this);
	DownloadTitle = new QLabel(this);
	ProgressBar = new QProgressBar(this);
	rate = new QLabel(this);
	DownloadSchedule = new QLabel(this);
	//设置控件属性
	TitleIcon->setGeometry(0, 0, 40, 40);
	TitleIcon->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255);color: rgb(255, 255, 255);border-style: inset;}QPushButton:hover{background-color: rgb(225, 225, 225);}QPushButton:pressed{background-color: rgb(195, 195, 195);}");
	TitleIcon->setIcon(QIcon(":/SkyMindustryLauncher/rec/back.png"));
	TitleIcon->setIconSize(QSize(20, 20));
	connect(TitleIcon, SIGNAL(clicked()), this, SLOT(on_TitleIcon_clicked()));
	title->setGeometry(40, 0, 550, 40);
	title->setStyleSheet("background-color: rgb(255, 255, 255);");
	title->setText("下载游戏-" + version.name);
	DownloadIcon->setGeometry(245, 140, 100, 100);
	DownloadIcon->setPixmap(QPixmap(":/SkyMindustryLauncher/rec/DownloadLoading.png"));
	DownloadIcon->setScaledContents(true);
	DownloadTitle->setGeometry(0, 240, 590, 40);
	DownloadTitle->setStyleSheet("font-size: 20px;");
	DownloadTitle->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	DownloadTitle->setText("正在下载：" + version.ver);
	ProgressBar->setGeometry(0, 380, 590, 10);
	ProgressBar->setStyleSheet("QProgressBar{background-color: rgba(0, 0, 0, 30);color: rgb(85, 255, 255);border: 0px}QProgressBar::chunk{background-color: rgb(110, 210, 230);}");
	ProgressBar->setTextVisible(false);
	ProgressBar->setValue(0);
	rate->setGeometry(535, 360, 50, 20);
	rate->setStyleSheet("color: rgb(120, 120, 120);");
	rate->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	rate->setText("0%");
	DownloadSchedule->setGeometry(0, 360, 250, 20);
	DownloadSchedule->setStyleSheet("color: rgb(120, 120, 120);");
	DownloadSchedule->setText("准备工作...");
	//启动下载进程
	download = new DownloadMainT(version);
	//下载游戏进程
	qRegisterMetaType<CurrentProgress>("CurrentProgress");
	connect(download, SIGNAL(ProgressNumber(double)), this, SLOT(GetProgressNumber(double)));
	connect(download, SIGNAL(progress(CurrentProgress)), this, SLOT(GetCurrentProgress(CurrentProgress)));
	connect(download, SIGNAL(DownloadFinished()), this, SLOT(GetDownloadFinished()));
	connect(this, SIGNAL(DownloadPaused()), download, SLOT(GetDownloadPaused()));

	connect(this, SIGNAL(SubWidgetShow()), MainWidget, SLOT(SubWidgetShowed()));
	emit SubWidgetShow();
	download->start();
	this->show();
}

void DownloadLoadingWidget::on_TitleIcon_clicked() {
	if (SMLMessageBox::msgbox(MainWidget, Warn, "退出下载将取消，是否继续？") == 1) {
		connect(this, SIGNAL(SubWidgetClose()), MainWidget, SLOT(SubWidgetClosed()));
		emit SubWidgetClose();
		emit DownloadPaused();
		previous->show();
		this->close();
	}
}

void DownloadLoadingWidget::GetProgressNumber(double i) {
	ProgressNumber = i;
}

void DownloadLoadingWidget::GetCurrentProgress(CurrentProgress c) {
	ProgressBar->setValue(qRound((c.number - 1) / ProgressNumber + (c.percent / ProgressNumber)));
	rate->setText(QString::number(ProgressBar->value()) + "%");
	DownloadSchedule->setText(c.matter + "(" + QString::number(c.number) + "/" + QString::number(ProgressNumber) + ")");
}

void DownloadLoadingWidget::GetDownloadFinished() {
	connect(this, SIGNAL(SubWidgetClose()), MainWidget, SLOT(SubWidgetClosed()));
	emit SubWidgetClose();
	previous->show();
	this->close();
}