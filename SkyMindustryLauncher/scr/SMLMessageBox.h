#ifndef _SML_SMLMESSAGEBOX_H_
#define _SML_SMLMESSAGEBOX_H_

#include "stdafx.h"
#include "scr/log.h"

enum MessageBoxType { Info, Warn, Error };

class SMLMessageBox {

public:
	static int msgbox(QWidget* parent, MessageBoxType type, QString text, QString Button3Text = "");

};

class MsgWidget :
	public QDialog {

	Q_OBJECT

public:
	MsgWidget(QWidget* parent, MessageBoxType type, QString text, QString Button3Text);

public slots:
	void Button1();
	void Button2();
	void Button3();

private:
	QPoint m_zPos;
	QFrame* MainFrame;
	QLabel* TitleBar;
	QLabel* TextArea;
	QLabel* TitleIcon;
	QPushButton* button1;//确定
	QPushButton* button2;//取消
	QPushButton* button3;//自定义

};
#endif // !_SML_SMLMESSAGEBOX_H_
