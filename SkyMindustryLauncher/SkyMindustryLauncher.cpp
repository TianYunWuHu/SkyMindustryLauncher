#include "SkyMindustryLauncher.h"

SkyMindustryLauncher::SkyMindustryLauncher(QWidget* parent)
	: QMainWindow(parent)
{
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