#include "log.h"

void logger::initialize() {
	QFile initialization;
	initialization.setFileName("latest.log");
	initialization.open(QIODevice::WriteOnly | QIODevice::Truncate);
	initialization.close();
}

//输出到调试台和日志文件
void logger::log(LogLevel level, QString LogString) {
	//打开日志文件
	QFile LogFile;
	LogFile.setFileName("latest.log");
	LogFile.open(QIODevice::Append);
	QTime qtime = QTime::currentTime();
	//获取当前时间
	QString time = qtime.toString("[hh:mm:ss]");
	QTextStream logFile(&LogFile);
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