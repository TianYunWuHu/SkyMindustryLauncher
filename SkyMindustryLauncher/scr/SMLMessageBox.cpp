#include "SMLMessageBox.h"

int SMLMessageBox::msgbox(QWidget* parent, MessageBoxType type, QString text, QString Button3Text) {
	MsgWidget* MessageBox = new MsgWidget(parent, type, text, Button3Text);
	return MessageBox->exec();
}

MsgWidget::MsgWidget(QWidget* parent, MessageBoxType type, QString text, QString Button3Text) {
	this->setAttribute(Qt::WA_DeleteOnClose);
	//无边框
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog | Qt::WindowStaysOnTopHint);
	this->setAttribute(Qt::WA_TranslucentBackground, true);
	//阴影
	QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect();
	effect->setBlurRadius(10);
	effect->setColor(QColor::fromRgbF(0, 0, 0, 0.7));
	effect->setOffset(0, 0);
	this->setGraphicsEffect(effect);
	//窗口控件
	this->MainFrame = new QFrame(this);
	this->TitleBar = new QLabel(this->MainFrame);
	this->TextArea = new QLabel(this->MainFrame);
	this->TitleIcon = new QLabel(this->MainFrame);
	this->button1 = new QPushButton(this->MainFrame);//确定
	//初始化
	this->setGeometry(parent->x() + 140, parent->y() + 85, 350, 250);
	this->setParent(nullptr);
	this->MainFrame->setGeometry(5, 5, 340, 240);
	this->MainFrame->setStyleSheet("background-color: rgb(255, 255, 255);");
	this->MainFrame->setGraphicsEffect(effect);
	this->TitleBar->setGeometry(30, 0, 310, 30);
	this->TitleIcon->setGeometry(0, 0, 30, 30);
	this->TitleIcon->setScaledContents(true);
	this->TextArea->setGeometry(20, 50, 300, 130);
	this->TextArea->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	this->TextArea->setText(text);
	this->button1->setGeometry(255, 200, 85, 40);
	this->button1->setStyleSheet("QPushButton{background-color: rgba(0, 0, 0, 30);color: rgb(255, 255, 255);border-style: inset;font-size: 20px;}QPushButton:hover{background-color: rgba(0, 0, 0, 60);}QPushButton:pressed{background-color: rgba(0, 0, 0, 90);}");
	this->button1->setText("确定");
	connect(this->button1, &QPushButton::clicked, this, &MsgWidget::Button1);
	switch (type)
	{
	case Info:
		this->TitleBar->setText("提示");
		this->TitleBar->setStyleSheet("background-color: rgb(140, 140, 255);");
		this->TitleIcon->setPixmap(QPixmap(":/SkyMindustryLauncher/rec/info.png"));
		this->TitleIcon->setStyleSheet("background-color: rgb(140, 140, 255);");
		if (Button3Text != "") {
			this->button3 = new QPushButton(this->MainFrame);
			this->button3->setGeometry(170, 200, 85, 40);
			this->button3->setStyleSheet("QPushButton{background-color: rgba(0, 0, 0, 30);color: rgb(255, 255, 255);border-style: inset;font-size: 20px;}QPushButton:hover{background-color: rgba(0, 0, 0, 60);}QPushButton:pressed{background-color: rgba(0, 0, 0, 90);}");
			this->button3->setText(Button3Text);
			connect(this->button3, &QPushButton::clicked, this, &MsgWidget::Button3);
		}
		break;
	case Warn:
		this->TitleBar->setText("警告");
		this->button2 = new QPushButton(this->MainFrame);
		this->button2->setGeometry(170, 200, 85, 40);
		this->button2->setStyleSheet("QPushButton{background-color: rgba(0, 0, 0, 30);color: rgb(255, 255, 255);border-style: inset;font-size: 20px;}QPushButton:hover{background-color: rgba(0, 0, 0, 60);}QPushButton:pressed{background-color: rgba(0, 0, 0, 90);}");
		this->button2->setText("取消");
		this->TitleBar->setStyleSheet("background-color : rgb(255, 255, 127);");
		this->TitleIcon->setPixmap(QPixmap(":/SkyMindustryLauncher/rec/warn.png"));
		this->TitleIcon->setStyleSheet("background-color : rgb(255, 255, 127);");
		connect(this->button2, &QPushButton::clicked, this, &MsgWidget::Button2);
		if (Button3Text != "") {
			this->button3 = new QPushButton(this->MainFrame);
			this->button3->setGeometry(85, 200, 85, 40);
			this->button3->setStyleSheet("QPushButton{background-color: rgba(0, 0, 0, 30);color: rgb(255, 255, 255);border-style: inset;font-size: 20px;}QPushButton:hover{background-color: rgba(0, 0, 0, 60);}QPushButton:pressed{background-color: rgba(0, 0, 0, 90);}");
			this->button3->setText(Button3Text);
			connect(this->button3, &QPushButton::clicked, this, &MsgWidget::Button3);
		}
		break;
	case Error:
		this->TitleBar->setText("错误");
		this->TitleBar->setStyleSheet("background-color : rgb(255, 0, 0);");
		this->TitleIcon->setPixmap(QPixmap(":/SkyMindustryLauncher/rec/error.png"));
		this->TitleIcon->setStyleSheet("background-color : rgb(255, 0, 0);");
		if (Button3Text != "") {
			this->button3 = new QPushButton(this->MainFrame);
			this->button3->setGeometry(170, 200, 85, 40);
			this->button3->setStyleSheet("QPushButton{background-color: rgba(0, 0, 0, 30);color: rgb(255, 255, 255);border-style: inset;font-size: 20px;}QPushButton:hover{background-color: rgba(0, 0, 0, 60);}QPushButton:pressed{background-color: rgba(0, 0, 0, 90);}");
			this->button3->setText(Button3Text);
			connect(this->button3, &QPushButton::clicked, this, &MsgWidget::Button3);
		}
		break;
	}
}

//槽函数
void MsgWidget::Button1() {
	this->done(1);
}
void MsgWidget::Button2() {
	this->done(0);
}
void MsgWidget::Button3() {
	this->done(2);
}