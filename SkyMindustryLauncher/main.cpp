﻿#include "SkyMindustryLauncher.h"
#include <QtWidgets/QApplication>

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	// 加载外部字体文件
	QFontDatabase::addApplicationFont(":/SkyMindustryLauncher/rec/QuicksandboldRegular.otf");
	//设置字体
	QFont f("黑体", 12);
	a.setFont(f);
	SkyMindustryLauncher w;
	w.show();
	return a.exec();
}
