#ifndef _SML_CUSTOMCONTROL_H_
#define _SML_CUSTOMCONTROL_H_

#include "stdafx.h"

class InfoButton :
	public QPushButton {

	Q_OBJECT

public:
	int number;
	InfoButton(QWidget* parent);
	void paintEvent(QPaintEvent* p);
	void setSubTitle(const QString& text);

private:
	QString SubTitle = "";

public slots:
	void Clicked();

signals:
	void Number(int);
};

#endif // !_SML_CUSTOMCONTROL_H_