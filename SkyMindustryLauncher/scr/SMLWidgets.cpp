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
	LauncherOption = new QPushButton(submenu);
	GameOption = new QPushButton(submenu);
	DownloadOption = new QPushButton(submenu);
	AboutOption = new QPushButton(submenu);
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
	LauncherOption->setGeometry(0, 0, 100, 40);
	LauncherOption->setStyleSheet("QPushButton{background-color: rgba(0, 0, 0, 30);color: rgb(255, 255, 255);border-style: inset;}QPushButton:hover{background-color: rgba(0, 0, 0, 60);}QPushButton:pressed{background-color: rgba(0, 0, 0, 90);}");
	LauncherOption->setText("启动器");
	connect(LauncherOption, SIGNAL(clicked()), this, SLOT(on_LauncherOption_clicked()));
	GameOption->setGeometry(0, 40, 100, 40);
	GameOption->setStyleSheet("QPushButton{background-color: rgba(0, 0, 0, 30);color: rgb(255, 255, 255);border-style: inset;}QPushButton:hover{background-color: rgba(0, 0, 0, 60);}QPushButton:pressed{background-color: rgba(0, 0, 0, 90);}");
	GameOption->setText("游戏");
	connect(GameOption, SIGNAL(clicked()), this, SLOT(on_GameOption_clicked()));
	DownloadOption->setGeometry(0, 80, 100, 40);
	DownloadOption->setStyleSheet("QPushButton{background-color: rgba(0, 0, 0, 30);color: rgb(255, 255, 255);border-style: inset;}QPushButton:hover{background-color: rgba(0, 0, 0, 60);}QPushButton:pressed{background-color: rgba(0, 0, 0, 90);}");
	DownloadOption->setText("下载");
	connect(DownloadOption, SIGNAL(clicked()), this, SLOT(on_DownloadOption_clicked()));
	AboutOption->setGeometry(0, 120, 100, 40);
	AboutOption->setStyleSheet("QPushButton{background-color: rgba(0, 0, 0, 30);color: rgb(255, 255, 255);border-style: inset;}QPushButton:hover{background-color: rgba(0, 0, 0, 60);}QPushButton:pressed{background-color: rgba(0, 0, 0, 90);}");
	AboutOption->setText("关于");
	connect(AboutOption, SIGNAL(clicked()), this, SLOT(on_AboutOption_clicked()));
	this->show();
	SwitchLauncher();
}

void SettingsWidget::SwitchLauncher() {
	title->setText("设置-启动器");
	delete OptionWidget;
	delete OptionScrollArea;
	OptionScrollArea = new QScrollArea(this);
	OptionWidget = new QWidget();
	OptionScrollArea->setGeometry(100, 40, 490, 350);
	OptionScrollArea->setStyleSheet("border: none;");
	OptionScrollArea->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{width: 10px;padding-top: 0px;padding-bottom: 0px;}QScrollBar::handle:vertical{background-color: rgb(140, 140, 140)}QScrollBar::handle:vertical:hover{background-color: rgb(90, 90, 90)}QScrollBar::add-line:vertical{height: 0px;width: 10px;subcontrol-position: bottom;}QScrollBar::sub-line:vertical{height: 0px;width: 10px;subcontrol-position: top;}QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical{background-color: rgba(0, 0, 0, 0);}");
	OptionWidget->setGeometry(0, 0, 470, 0);
	OptionWidget->setMinimumSize(470, 0);
	OptionWidget->setStyleSheet("background-color: rgba(0, 0, 0, 0);");

	//个性化
	QVBoxLayout* individualization = new QVBoxLayout();
	individualization->setSizeConstraint(QLayout::SetMinimumSize);
	QPushButton* individualization_title = new QPushButton();
	individualization_title->setMinimumSize(470, 40);
	individualization_title->setStyleSheet("QPushButton:disabled{color: rgb(0, 0, 0);font-size: 20px; text-align: left; border-bottom: 3px solid #1478f0;}");
	individualization_title->setText("个性化");
	individualization_title->setDisabled(true);
	individualization->addWidget(individualization_title);
	QHBoxLayout* individualization_WindowTitle = new QHBoxLayout();
	individualization->addLayout(individualization_WindowTitle);
	QLabel* individualization_WindowTitle_label = new QLabel();
	individualization_WindowTitle_label->setMinimumHeight(30);
	individualization_WindowTitle_label->adjustSize();
	individualization_WindowTitle_label->setText("自定义窗口标题");
	individualization_WindowTitle->addWidget(individualization_WindowTitle_label);
	QLineEdit* individualization_WindowTitle_editer = new QLineEdit();
	individualization_WindowTitle_editer->setMinimumHeight(30);
	individualization_WindowTitle_editer->setStyleSheet("QLineEdit{border-style: inset;border-color: rgb(0, 170, 255);border-width: 1px 1px 1px 1px;}QLineEdit:focus{border-width: 1.5px 1.5px 1.5px 1.5px;}");
	individualization_WindowTitle_editer->setPlaceholderText("仅支持英文、数字，最多40字符，留空为默认");
	individualization_WindowTitle_editer->setMaxLength(40);
	individualization_WindowTitle->addWidget(individualization_WindowTitle_editer);


	//总
	QVBoxLayout* main = new QVBoxLayout();
	main->addLayout(individualization);
	main->addSpacerItem(new QSpacerItem(0, 470, QSizePolicy::Minimum));
	OptionWidget->setLayout(main);
	OptionScrollArea->setWidget(OptionWidget);
	OptionScrollArea->setWidgetResizable(true);
	OptionWidget->setMaximumSize(470, 350);

	OptionScrollArea->show();
}
void SettingsWidget::SwitchGame() {
	title->setText("设置-游戏");
	delete OptionWidget;
	delete OptionScrollArea;
	OptionScrollArea = new QScrollArea(this);
	OptionWidget = new QWidget();
	OptionScrollArea->setGeometry(100, 40, 490, 350);
	OptionScrollArea->setStyleSheet("border: none;");
	OptionScrollArea->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{width: 10px;padding-top: 0px;padding-bottom: 0px;}QScrollBar::handle:vertical{background-color: rgb(140, 140, 140)}QScrollBar::handle:vertical:hover{background-color: rgb(90, 90, 90)}QScrollBar::add-line:vertical{height: 0px;width: 10px;subcontrol-position: bottom;}QScrollBar::sub-line:vertical{height: 0px;width: 10px;subcontrol-position: top;}QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical{background-color: rgba(0, 0, 0, 0);}");
	OptionWidget->setGeometry(0, 0, 470, 0);
	OptionWidget->setMinimumSize(470, 0);
	OptionWidget->setStyleSheet("background-color: rgba(0, 0, 0, 0);");

	//java设置
	QVBoxLayout* java = new QVBoxLayout();
	java->setSizeConstraint(QLayout::SetMinimumSize);
	QPushButton* java_title = new QPushButton();
	java_title->setMinimumSize(470, 40);
	java_title->setStyleSheet("QPushButton:disabled{color: rgb(0, 0, 0);font-size: 20px; text-align: left; border-bottom: 3px solid #1478f0;}");
	java_title->setText("java设置");
	java_title->setDisabled(true);
	java->addWidget(java_title);
	QHBoxLayout* java_JavaPath = new QHBoxLayout();
	java->addLayout(java_JavaPath);
	QLabel* java_JavaPath_label = new QLabel();
	java_JavaPath_label->setMinimumHeight(30);
	java_JavaPath_label->adjustSize();
	java_JavaPath_label->setText("java路径");
	java_JavaPath->addWidget(java_JavaPath_label);
	QLineEdit* java_JavaPath_editer = new QLineEdit();
	java_JavaPath_editer->setMinimumHeight(30);
	java_JavaPath_editer->setStyleSheet("QLineEdit{border-style: inset;border-color: rgb(0, 170, 255);border-width: 1px 1px 1px 1px;}QLineEdit:focus{border-width: 1.5px 1.5px 1.5px 1.5px;}");
	java_JavaPath_editer->setPlaceholderText("请使用java17及以上");
	java_JavaPath->addWidget(java_JavaPath_editer);

	//总
	QVBoxLayout* main = new QVBoxLayout();
	main->addLayout(java);
	main->addSpacerItem(new QSpacerItem(0, 470, QSizePolicy::Minimum));
	OptionWidget->setLayout(main);
	OptionScrollArea->setWidget(OptionWidget);
	OptionScrollArea->setWidgetResizable(true);
	OptionWidget->setMaximumSize(470, 350);

	OptionScrollArea->show();
}
void SettingsWidget::SwitchDownload() {
	title->setText("设置-下载");
	delete OptionWidget;
	delete OptionScrollArea;
	OptionScrollArea = new QScrollArea(this);
	OptionWidget = new QWidget();
	OptionScrollArea->setGeometry(100, 40, 490, 350);
	OptionScrollArea->setStyleSheet("border: none;");
	OptionScrollArea->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{width: 10px;padding-top: 0px;padding-bottom: 0px;}QScrollBar::handle:vertical{background-color: rgb(140, 140, 140)}QScrollBar::handle:vertical:hover{background-color: rgb(90, 90, 90)}QScrollBar::add-line:vertical{height: 0px;width: 10px;subcontrol-position: bottom;}QScrollBar::sub-line:vertical{height: 0px;width: 10px;subcontrol-position: top;}QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical{background-color: rgba(0, 0, 0, 0);}");
	OptionWidget->setGeometry(0, 0, 470, 0);
	OptionWidget->setMinimumSize(470, 0);
	OptionWidget->setStyleSheet("background-color: rgba(0, 0, 0, 0);");

	//下载选项
	QVBoxLayout* DownloadOption = new QVBoxLayout();
	DownloadOption->setSizeConstraint(QLayout::SetMinimumSize);
	QPushButton* DownloadOption_title = new QPushButton();
	DownloadOption_title->setMinimumSize(470, 40);
	DownloadOption_title->setStyleSheet("QPushButton:disabled{color: rgb(0, 0, 0);font-size: 20px; text-align: left; border-bottom: 3px solid #1478f0;}");
	DownloadOption_title->setText("下载选项");
	DownloadOption_title->setDisabled(true);
	DownloadOption->addWidget(DownloadOption_title);
	QHBoxLayout* DownloadOption_ConcurrentNumber = new QHBoxLayout();
	DownloadOption->addLayout(DownloadOption_ConcurrentNumber);
	QLabel* DownloadOption_ConcurrentNumber_label = new QLabel();
	DownloadOption_ConcurrentNumber_label->setMinimumHeight(30);
	DownloadOption_ConcurrentNumber_label->adjustSize();
	DownloadOption_ConcurrentNumber_label->setText("下载最大并发数");
	DownloadOption_ConcurrentNumber->addWidget(DownloadOption_ConcurrentNumber_label);
	QLineEdit* DownloadOption_ConcurrentNumber_editer = new QLineEdit();
	DownloadOption_ConcurrentNumber_editer->setMinimumHeight(30);
	DownloadOption_ConcurrentNumber_editer->setStyleSheet("QLineEdit{border-style: inset;border-color: rgb(0, 170, 255);border-width: 1px 1px 1px 1px;}QLineEdit:focus{border-width: 1.5px 1.5px 1.5px 1.5px;}");
	DownloadOption_ConcurrentNumber_editer->setPlaceholderText("该数值越大下载越快，最大16");
	DownloadOption_ConcurrentNumber_editer->setValidator(new QIntValidator(DownloadOption_ConcurrentNumber_editer));
	DownloadOption_ConcurrentNumber->addWidget(DownloadOption_ConcurrentNumber_editer);
	QHBoxLayout* DownloadOption_source = new QHBoxLayout();
	DownloadOption->addLayout(DownloadOption_source);
	QLabel* DownloadOption_source_label = new QLabel();
	DownloadOption_source_label->setMinimumHeight(30);
	DownloadOption_source_label->adjustSize();
	DownloadOption_source_label->setText("下载源");
	DownloadOption_source->addWidget(DownloadOption_source_label);
	QButtonGroup* DownloadOption_source_ButtonGroup = new QButtonGroup();
	QRadioButton* DownloadOption_source_button1 = new QRadioButton();
	DownloadOption_source_ButtonGroup->addButton(DownloadOption_source_button1);
	DownloadOption_source_button1->setText("官方源");
	DownloadOption_source_button1->setStyleSheet("QRadioButton::indicator:unchecked{border-radius: 7px;background-color: rgb(255, 255, 255);border: 2px solid white;}QRadioButton::indicator:checked{border-radius: 7px;background-color: rgb(50, 130, 255);border: 2px solid white;}");
	DownloadOption_source->addWidget(DownloadOption_source_button1);
	QRadioButton* DownloadOption_source_button2 = new QRadioButton();
	DownloadOption_source_ButtonGroup->addButton(DownloadOption_source_button2);
	DownloadOption_source_button2->setText("镜像源1");
	DownloadOption_source_button2->setStyleSheet("QRadioButton::indicator:unchecked{border-radius: 7px;background-color: rgb(255, 255, 255);border: 2px solid white;}QRadioButton::indicator:checked{border-radius: 7px;background-color: rgb(50, 130, 255);border: 2px solid white;}");
	DownloadOption_source->addWidget(DownloadOption_source_button2);
	QRadioButton* DownloadOption_source_button3 = new QRadioButton();
	DownloadOption_source_ButtonGroup->addButton(DownloadOption_source_button3);
	DownloadOption_source_button3->setText("镜像源2");
	DownloadOption_source_button3->setStyleSheet("QRadioButton::indicator:unchecked{border-radius: 7px;background-color: rgb(255, 255, 255);border: 2px solid white;}QRadioButton::indicator:checked{border-radius: 7px;background-color: rgb(50, 130, 255);border: 2px solid white;}");
	DownloadOption_source->addWidget(DownloadOption_source_button3);

	//总
	QVBoxLayout* main = new QVBoxLayout();
	main->addLayout(DownloadOption);
	main->addSpacerItem(new QSpacerItem(0, 470, QSizePolicy::Minimum));
	OptionWidget->setLayout(main);
	OptionScrollArea->setWidget(OptionWidget);
	OptionScrollArea->setWidgetResizable(true);
	OptionWidget->setMaximumSize(470, 350);

	OptionScrollArea->show();
}
void SettingsWidget::SwitchAbout() {
	title->setText("设置-关于");
	delete OptionWidget;
	delete OptionScrollArea;
	OptionScrollArea = new QScrollArea(this);
	OptionWidget = new QWidget();
	OptionScrollArea->setGeometry(100, 40, 490, 350);
	OptionScrollArea->setStyleSheet("border: none;");
	OptionScrollArea->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{width: 10px;padding-top: 0px;padding-bottom: 0px;}QScrollBar::handle:vertical{background-color: rgb(140, 140, 140)}QScrollBar::handle:vertical:hover{background-color: rgb(90, 90, 90)}QScrollBar::add-line:vertical{height: 0px;width: 10px;subcontrol-position: bottom;}QScrollBar::sub-line:vertical{height: 0px;width: 10px;subcontrol-position: top;}QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical{background-color: rgba(0, 0, 0, 0);}");
	OptionWidget->setGeometry(0, 0, 470, 0);
	OptionWidget->setMinimumSize(470, 0);
	OptionWidget->setStyleSheet("background-color: rgba(0, 0, 0, 0);");

	//关于SkyMindustryLauncher
	QVBoxLayout* AboutSkyMindustryLauncher = new QVBoxLayout();
	AboutSkyMindustryLauncher->setSizeConstraint(QLayout::SetMinimumSize);
	QPushButton* AboutSkyMindustryLauncher_title = new QPushButton();
	AboutSkyMindustryLauncher_title->setMinimumSize(470, 40);
	AboutSkyMindustryLauncher_title->setStyleSheet("QPushButton:disabled{color: rgb(0, 0, 0);font-size: 20px; text-align: left; border-bottom: 3px solid #1478f0;}");
	AboutSkyMindustryLauncher_title->setText("关于SkyMindustryLauncher");
	AboutSkyMindustryLauncher_title->setDisabled(true);
	AboutSkyMindustryLauncher->addWidget(AboutSkyMindustryLauncher_title);
	QHBoxLayout* AboutSkyMindustryLauncher_SML = new QHBoxLayout();
	AboutSkyMindustryLauncher->addLayout(AboutSkyMindustryLauncher_SML);
	QLabel* AboutSkyMindustryLauncher_SML_icon = new QLabel();
	AboutSkyMindustryLauncher_SML_icon->setScaledContents(true);
	AboutSkyMindustryLauncher_SML_icon->setPixmap(QPixmap(":/SkyMindustryLauncher/rec/icon.png"));
	AboutSkyMindustryLauncher_SML_icon->setMinimumSize(60, 60);
	AboutSkyMindustryLauncher_SML_icon->setMaximumSize(60, 60);
	AboutSkyMindustryLauncher_SML->addWidget(AboutSkyMindustryLauncher_SML_icon);
	QLabel* AboutSkyMindustryLauncher_SML_introduction = new QLabel();
	AboutSkyMindustryLauncher_SML_introduction->setMinimumHeight(60);
	AboutSkyMindustryLauncher_SML_introduction->setWordWrap(true);
	AboutSkyMindustryLauncher_SML_introduction->setStyleSheet("font-size: 12px;");
	AboutSkyMindustryLauncher_SML_introduction->setText("SkyMindustryLauncher是一个基于qt5的Mindustry启动器，包含多版本管理和下载游戏功能。");
	AboutSkyMindustryLauncher_SML->addWidget(AboutSkyMindustryLauncher_SML_introduction);
	QHBoxLayout* AboutSkyMindustryLauncher_developer = new QHBoxLayout();
	AboutSkyMindustryLauncher->addLayout(AboutSkyMindustryLauncher_developer);
	QLabel* AboutSkyMindustryLauncher_developer_icon = new QLabel();
	AboutSkyMindustryLauncher_developer_icon->setScaledContents(true);
	AboutSkyMindustryLauncher_developer_icon->setPixmap(QPixmap(":/SkyMindustryLauncher/rec/wuhu.png"));
	AboutSkyMindustryLauncher_developer_icon->setMinimumSize(60, 60);
	AboutSkyMindustryLauncher_developer_icon->setMaximumSize(60, 60);
	AboutSkyMindustryLauncher_developer->addWidget(AboutSkyMindustryLauncher_developer_icon);
	QLabel* AboutSkyMindustryLauncher_developer_introduction = new QLabel();
	AboutSkyMindustryLauncher_developer_introduction->setMinimumHeight(60);
	AboutSkyMindustryLauncher_developer_introduction->setWordWrap(true);
	AboutSkyMindustryLauncher_developer_introduction->setStyleSheet("font-size: 12px;");
	AboutSkyMindustryLauncher_developer_introduction->setText("SkyMindustryLauncher所有的代码都是TianYunWuHu一个人敲的，含有许多未知的bug :P");
	AboutSkyMindustryLauncher_developer->addWidget(AboutSkyMindustryLauncher_developer_introduction);
	QHBoxLayout* AboutSkyMindustryLauncher_mindustry = new QHBoxLayout();
	AboutSkyMindustryLauncher->addLayout(AboutSkyMindustryLauncher_mindustry);
	QLabel* AboutSkyMindustryLauncher_mindustry_icon = new QLabel();
	AboutSkyMindustryLauncher_mindustry_icon->setScaledContents(true);
	AboutSkyMindustryLauncher_mindustry_icon->setPixmap(QPixmap(":/SkyMindustryLauncher/rec/mindustry.png"));
	AboutSkyMindustryLauncher_mindustry_icon->setMinimumSize(60, 60);
	AboutSkyMindustryLauncher_mindustry_icon->setMaximumSize(60, 60);
	AboutSkyMindustryLauncher_mindustry->addWidget(AboutSkyMindustryLauncher_mindustry_icon);
	QLabel* AboutSkyMindustryLauncher_mindustry_introduction = new QLabel();
	AboutSkyMindustryLauncher_mindustry_introduction->setMinimumHeight(60);
	AboutSkyMindustryLauncher_mindustry_introduction->setWordWrap(true);
	AboutSkyMindustryLauncher_mindustry_introduction->setStyleSheet("font-size: 12px;");
	AboutSkyMindustryLauncher_mindustry_introduction->setText("Mindustry是一个RTS类的塔防游戏，喜欢玩红警和铁锈战争的绝对不能错过！");
	AboutSkyMindustryLauncher_mindustry->addWidget(AboutSkyMindustryLauncher_mindustry_introduction);

	//开源软件使用声明
	QVBoxLayout* OpenSource = new QVBoxLayout();
	OpenSource->setSizeConstraint(QLayout::SetMinimumSize);
	QPushButton* OpenSource_title = new QPushButton();
	OpenSource_title->setMinimumSize(470, 40);
	OpenSource_title->setStyleSheet("QPushButton:disabled{color: rgb(0, 0, 0);font-size: 20px; text-align: left; border-bottom: 3px solid #1478f0;}");
	OpenSource_title->setText("开源软件使用声明");
	OpenSource_title->setDisabled(true);
	OpenSource->addWidget(OpenSource_title);
	QLabel* OpenSource_tabel = new QLabel();
	OpenSource_tabel->setText("<table cellspacing=20><tr><th>名称</th><th>版本</th><th>协议</th></tr><tr><td>Mindustry</td><td>All</td><td>GPL3.0</td></tr><tr><td>Qt Framework</td><td>5.12.9</td><td>LGPL3.0|GPL2.0</td></tr><tr><td>Aria2</td><td>1.37.0</td><td>GPL2.0</td></tr></table>");
	OpenSource_tabel->adjustSize();
	OpenSource->addWidget(OpenSource_tabel);
	QPushButton* OpenSource_mindusry = new QPushButton();
	OpenSource_mindusry->setMinimumHeight(40);
	OpenSource_mindusry->setText("前往Mindustry官方仓库 ->");
	OpenSource_mindusry->setStyleSheet("QPushButton{background-color: rgba(0, 0, 0, 30);color: rgb(255, 255, 255);border-style: inset;}QPushButton:hover{background-color: rgba(0, 0, 0, 60);}QPushButton:pressed{background-color: rgba(0, 0, 0, 90);}");
	OpenSource->addWidget(OpenSource_mindusry);
	QPushButton* OpenSource_qt = new QPushButton();
	OpenSource_qt->setMinimumHeight(40);
	OpenSource_qt->setText("前往Qt Framework官网 ->");
	OpenSource_qt->setStyleSheet("QPushButton{background-color: rgba(0, 0, 0, 30);color: rgb(255, 255, 255);border-style: inset;}QPushButton:hover{background-color: rgba(0, 0, 0, 60);}QPushButton:pressed{background-color: rgba(0, 0, 0, 90);}");
	OpenSource->addWidget(OpenSource_qt);
	QPushButton* OpenSource_aria2 = new QPushButton();
	OpenSource_aria2->setMinimumHeight(40);
	OpenSource_aria2->setText("前往Aria2官方仓库 ->");
	OpenSource_aria2->setStyleSheet("QPushButton{background-color: rgba(0, 0, 0, 30);color: rgb(255, 255, 255);border-style: inset;}QPushButton:hover{background-color: rgba(0, 0, 0, 60);}QPushButton:pressed{background-color: rgba(0, 0, 0, 90);}");
	OpenSource->addWidget(OpenSource_aria2);

	//特别鸣谢
	QVBoxLayout* SpecialThanks = new QVBoxLayout();
	SpecialThanks->setSizeConstraint(QLayout::SetMinimumSize);
	QPushButton* SpecialThanks_title = new QPushButton();
	SpecialThanks_title->setMinimumSize(470, 40);
	SpecialThanks_title->setStyleSheet("QPushButton:disabled{color: rgb(0, 0, 0);font-size: 20px; text-align: left; border-bottom: 3px solid #1478f0;}");
	SpecialThanks_title->setText("特别鸣谢");
	SpecialThanks_title->setDisabled(true);
	SpecialThanks->addWidget(SpecialThanks_title);
	QLabel* SpecialThanks_1 = new QLabel();
	SpecialThanks_1->setText("icons8.com\niconfinder.com\n提供了免费的图标！");
	SpecialThanks_1->adjustSize();
	SpecialThanks->addWidget(SpecialThanks_1);

	//总
	QVBoxLayout* main = new QVBoxLayout();
	main->addLayout(AboutSkyMindustryLauncher);
	main->addSpacing(20);
	main->addLayout(OpenSource);
	main->addSpacing(20);
	main->addLayout(SpecialThanks);
	OptionWidget->setLayout(main);
	OptionScrollArea->setWidget(OptionWidget);
	OptionScrollArea->setWidgetResizable(true);

	OptionScrollArea->show();
}

void SettingsWidget::on_LauncherOption_clicked() {
	SwitchLauncher();
}
void SettingsWidget::on_GameOption_clicked() {
	SwitchGame();
}
void SettingsWidget::on_DownloadOption_clicked() {
	SwitchDownload();
}
void SettingsWidget::on_AboutOption_clicked() {
	SwitchAbout();
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
	DeleteButton = new QPushButton(this);
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
	DeleteButton->setGeometry(40, 340, 110, 40);
	DeleteButton->setStyleSheet("QPushButton{background-color: rgba(0, 0, 0, 30);color: rgb(255, 255, 255);border-style: inset;font-size: 20px;}QPushButton:hover{background-color: rgba(0, 0, 0, 60);}QPushButton:pressed{background-color: rgba(0, 0, 0, 90);}");
	DeleteButton->setText("删除");
	connect(DeleteButton, SIGNAL(clicked()), this, SLOT(on_DeleteButton_clicked()));

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

void VersionManageWidget::on_DeleteButton_clicked() {
	QString VersionName(setting->value("/game/name").toString());
	QSettings MainSetting("./SML/settings.ini", QSettings::IniFormat);
	if (SMLMessageBox::msgbox(MainWidget, Warn, "你会永久失去这个版本（真的很久），是否继续？") == 1) {
		if (MainSetting.value("/game/CurrentVersion").toString() == VersionName) {
			MainSetting.setValue("/game/CurrentVersion", "");
		}
		delete setting;
		QDir("./Game/" + VersionName).removeRecursively();
		connect(this, SIGNAL(SubWidgetClose()), MainWidget, SLOT(SubWidgetClosed()));
		emit SubWidgetClose();
		previous->show();
		this->close();
	}
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
	if (QSettings("./SML/settings.ini", QSettings::IniFormat).value("/game/isRunning").toBool() == false) {
		connect(this, SIGNAL(SubWidgetClose()), MainWidget, SLOT(SubWidgetClosed()));
		emit SubWidgetClose();
		MUTEX.lock();
		isGameCanLaunch = false;
		MUTEX.unlock();
		previous->show();
		this->close();
	}
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