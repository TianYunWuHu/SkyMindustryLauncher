#include "log.h"

void logger::initialize() {
	QFile initialization;
	initialization.setFileName("latest.log");
	initialization.open(QIODevice::WriteOnly | QIODevice::Truncate);
	initialization.close();
}

//输出到调试台和日志文件
void logger::log(LogLevel level, QString String ...) {
	//打开日志文件
	QFile LogFile;
	LogFile.setFileName("latest.log");
	LogFile.open(QIODevice::Append);
	QTime qtime = QTime::currentTime();
	//获取当前时间
	QString time = qtime.toString("[hh:mm:ss]");
	QTextStream logFile(&LogFile);
	//设置可变参数
	QString LogString = "";
	char ch;
	va_list list;
	va_start(list, String);
	for (int i = 0; i < String.length(); i++) {
		ch = String[i].toLatin1();
		if (ch == '%') {
			if (i + 1 == String.length()) {
				LogString += "%";
				break;
			}
			i++;
			ch = String[i].toLatin1();
			switch (ch)
			{
			case 's':
				LogString += va_arg(list, char*);
				break;
			case 'd':
				LogString += QString::number(va_arg(list, int));
				break;
			case 'f':
				LogString += QString::number(va_arg(list, double));
				break;
			case 'q':
				LogString += va_arg(list, QString);
				break;
			default:
				LogString += ("%" + String[i]);
				break;
			}
		}
		else
		{
			LogString += String[i];
		}
	}
	switch (level)
	{
	case info:
		qDebug() << time << "[Iofo]" << LogString;
		logFile << time << "[Info]" << LogString << endl;
		break;
	case warn:
		qDebug() << time + "[Warn]" + LogString;
		logFile << time << "[Warn]" << LogString << endl;
		break;
	case error:
		qDebug() << time + "[Error]" + LogString;
		logFile << time << "[Error]" << LogString << endl;
		break;
	case debug:
		qDebug() << time + "[Debug]" + LogString;
		logFile << time << "[Debug]" << LogString << endl;
		break;
	}
	//关闭日志文件
	LogFile.close();
}