#ifndef _SML_CUSTOMCONTROL_H_
#define _SML_CUSTOMCONTROL_H_

#include <qpushbutton.h>
#include <qpainter.h>

class InfoButton :
	public QPushButton {

	Q_OBJECT

public:
	InfoButton(QWidget* parent);
	void paintEvent(QPaintEvent* p);
	void setSubTitle(const QString& text);

private:
	QString SubTitle = "";
};

#endif // !_SML_CUSTOMCONTROL_H_