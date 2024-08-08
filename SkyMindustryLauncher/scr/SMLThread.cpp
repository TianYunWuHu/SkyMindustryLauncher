#include "stdafx.h"
#include "SMLThread.h"

void GetVersionListT::run() {
	VerList.clear();
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

GameT::GameT(QString name) {
	GameName = name;
	dir.cd(dir.currentPath());
}

void GameT::run() {
	QString DataPath(QDir::homePath() + "/AppData/Roaming/Mindustry");
	CopyDir(dir.absolutePath() + "/Game/" + GameName + "/Mindustry", DataPath);
	if (GameName != "无") {
		QString GamePath('"' + dir.absolutePath() + "/Game/" + GameName + "/" + "Mindustry.jar" + '"');
		logger::log(debug, "java -jar ");
		GameProcess.start("java -jar " + GamePath);
		GameProcess.waitForFinished();
		QThread::msleep(1000);
		CopyDir(DataPath, dir.absolutePath() + "/Game/" + GameName + "/Mindustry");
		QDir(DataPath).removeRecursively();
	}
}

void GameT::CopyDir(QString src, QString dst) {
	QDir srcDir(src);
	QDir dstDir(dst);

	if (!dstDir.exists()) {
		dstDir.mkdir(dst);
	}

	QFileInfoList list = srcDir.entryInfoList();

	foreach(QFileInfo info, list) {
		if (info.fileName() == "." || info.fileName() == "..") {
			continue;
		}
		if (info.isDir()) {
			// 创建文件夹，递归调用该函数
			CopyDir(info.filePath(), dst + "/" + info.fileName());
			continue;
		}
		// 文件拷贝
		QFile file(info.filePath());
		file.copy(dst + "/" + info.fileName());
	}
}