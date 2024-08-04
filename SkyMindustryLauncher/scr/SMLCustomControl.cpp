﻿#include "SMLCustomControl.h"

InfoButton::InfoButton(QWidget* parent) : QPushButton(parent) {

}

void InfoButton::paintEvent(QPaintEvent* p) {
	QPushButton::paintEvent(p);
	QPainter painter(this);
	painter.setFont(QFont("黑体", 10));
	painter.setPen(QColor(105, 105, 105));
	painter.drawText(10, 50, SubTitle);
}

void InfoButton::setSubTitle(const QString& text) {
	SubTitle = text;
	this->update();
}