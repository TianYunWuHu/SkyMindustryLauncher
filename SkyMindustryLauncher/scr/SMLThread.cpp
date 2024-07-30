#include "SMLThread.h"

void GetVersionListT::run() {
	QDir dir(QDir::currentPath());
	dir.cd("./Game");
	QFileInfoList fileInfoList = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs);
	QSettings* setting;
	VersionInfo VerInfo;
	foreach(auto FileInfo, fileInfoList) {
		if (QFileInfo(FileInfo.absoluteFilePath() + "/" + QDir(FileInfo.absoluteFilePath()).dirName() + ".ini").isFile()) {
			setting = new QSettings(FileInfo.absoluteFilePath() + "/" + QDir(FileInfo.absoluteFilePath()).dirName() + ".ini", QSettings::IniFormat);
			VerInfo.name = setting->value("/game/name").toString();
			VerInfo.ver = setting->value("/game/version").toInt();
			VerList.append(VerInfo);
			delete setting;
		}
	}
	emit GetVersionList(VerList);
	this->exit();
}