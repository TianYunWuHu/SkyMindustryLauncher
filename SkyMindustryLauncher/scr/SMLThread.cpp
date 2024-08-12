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
	emit ProgressNumber(4);
	CurrentProgress progress1;
	progress1.matter = "处理游戏文件";
	progress1.number = 1;
	progress1.percent = 100;
	emit progress(progress1);
	QString DataPath(QDir::homePath() + "/AppData/Roaming/Mindustry");
	CopyDir(dir.absolutePath() + "/Game/" + GameName + "/Mindustry", DataPath);

	CurrentProgress progress2;
	progress2.matter = "处理java";
	progress2.number = 2;
	progress2.percent = 100;
	emit progress(progress2);
	QString GamePath('"' + dir.absolutePath() + "/Game/" + GameName + "/" + "Mindustry.jar" + '"');

	CurrentProgress progress3;
	progress3.matter = "启动游戏";
	progress3.number = 3;
	progress3.percent = 100;
	emit progress(progress3);
	GameProcess.start("java -jar " + GamePath);

	CurrentProgress progress4;
	progress4.matter = "等待游戏窗口出现";
	progress4.number = 4;
	progress4.percent = 100;
	emit progress(progress4);
	while (FindWindowEx(NULL, NULL, NULL, L"Mindustry") == NULL);
	emit launched();
	GameProcess.waitForFinished();

	QThread::msleep(1000);
	CopyDir(DataPath, dir.absolutePath() + "/Game/" + GameName + "/Mindustry");
	QDir(DataPath).removeRecursively();
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