#include "stdafx.h"
#include "SkyMindustryLauncher.h"

SkyMindustryLauncher::SkyMindustryLauncher(QWidget* parent)
	: QMainWindow(parent)
{
	VarINIT();
	LessWindowsHint();
	EnvironmentINIT();
	SettingsINIT();
}

SkyMindustryLauncher::~SkyMindustryLauncher()
{}

//实现窗口拖动
void SkyMindustryLauncher::mousePressEvent(QMouseEvent* e)
{
	if (ui.title->geometry().contains(this->mapFromGlobal(QCursor::pos())))
	{
		m_CurrentPressPoint_global = e->globalPos();//获取点击鼠标时的全局坐标
		m_CurrentPressPoint = e->pos();//获取点击鼠标的相对坐标
	}
}
void SkyMindustryLauncher::mouseMoveEvent(QMouseEvent* e)
{
	if (ui.title->geometry().contains(this->mapFromGlobal(QCursor::pos())))
	{
		QPoint offset = e->globalPos() - m_CurrentPressPoint_global;//计算鼠标移动位置
		this->move(e->globalPos() - m_CurrentPressPoint);
	}
}
//构造函数中初始化
void SkyMindustryLauncher::VarINIT() {
	MainWidget = this;
}
void SkyMindustryLauncher::LessWindowsHint() {
	ui.setupUi(this);
	CurrentWidget = nullptr;
	//设置无边框
	setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
	//窗口阴影
	this->setAttribute(Qt::WA_TranslucentBackground, true);
	QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect();
	effect->setBlurRadius(10);
	effect->setColor(QColor::fromRgbF(0, 0, 0, 0.7));
	effect->setOffset(0, 0);
	ui.MainFrame->setGraphicsEffect(effect);
}
void SkyMindustryLauncher::EnvironmentINIT() {
	dir.cd(QDir::currentPath());
	if (QFileInfo("./SML").isDir() == false) {
		dir.mkdir("./SML");
	}
	if (QFile::exists("./SML/settings.ini") == false) {
		QFile ini("./SML/settings.ini");
		ini.open(QIODevice::WriteOnly);
		ini.close();
		setting = new QSettings("./SML/settings.ini", QSettings::IniFormat);
	}
	else
	{
		setting = new QSettings("./SML/settings.ini", QSettings::IniFormat);
	}
	if (dir.exists("Game") == false) {
		dir.mkdir("Game");
	}
}
void SkyMindustryLauncher::SettingsINIT() {
	//启动器
	if (setting->value("/launcher/WindowTitle") == QVariant()) setting->setValue("/launcher/WindowTitle", "");
	QString title = setting->value("/launcher/WindowTitle").toString();
	if (title == "") ui.title->setText("  Sky Mindustry Launcher");
	else ui.title->setText("  " + title);

	//游戏
	if (setting->value("/game/CurrentVersion") == QVariant()) setting->setValue("/game/CurrentVersion", "");
	if (setting->value("/game/isRunning") == QVariant()) setting->setValue("/game/isRunning", false);
	if (setting->value("/game/JavaPath") == QVariant()) setting->setValue("/game/JavaPath", "");

	//下载
	if (setting->value("/download/ConcurrentNumber") == QVariant()) setting->setValue("/download/ConcurrentNumber", 8);
	if (setting->value("/download/source") == QVariant()) setting->setValue("/download/source", 1);
}

void SkyMindustryLauncher::on_MiniButton_clicked() {
	this->showMinimized();
}
void SkyMindustryLauncher::on_CloseButton_clicked() {
	this->close();
}

void SkyMindustryLauncher::on_HomeButton_clicked() {
	delete CurrentWidget;
	CurrentWidget = new HomeWidget(ui.MainFrame);
}
void SkyMindustryLauncher::on_ConfigButton_clicked() {
	delete CurrentWidget;
	CurrentWidget = new ConfigWidget(ui.MainFrame);
}
void SkyMindustryLauncher::on_DownloadButton_clicked() {
	delete CurrentWidget;
	CurrentWidget = new DownloadWidget(ui.MainFrame);
}
void SkyMindustryLauncher::on_SettingsButton_clicked() {
	delete CurrentWidget;
	CurrentWidget = new SettingsWidget(ui.MainFrame);
}

void SkyMindustryLauncher::GameFinished() {
	delete GameMain;
	GameMain = nullptr;
	QSettings setting("./SML/settings.ini", QSettings::IniFormat);
	setting.setValue("/game/isRunning", false);
	emit GameFinish();
}

void SkyMindustryLauncher::SubWidgetShowed() {
	ui.HomeButton->setEnabled(false);
	ui.ConfigButton->setEnabled(false);
	ui.DownloadButton->setEnabled(false);
	ui.SettingsButton->setEnabled(false);
}
void SkyMindustryLauncher::SubWidgetClosed() {
	ui.HomeButton->setEnabled(true);
	ui.ConfigButton->setEnabled(true);
	ui.DownloadButton->setEnabled(true);
	ui.SettingsButton->setEnabled(true);
}

void SkyMindustryLauncher::WindowTitleChanged(const QString text) {
	setting->setValue("/launcher/WindowTitle", text);
	if (text == "") ui.title->setText("  Sky Mindustry Launcher");
	else ui.title->setText("  " + text);
}