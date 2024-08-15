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
			VerInfo.ver = setting->value("/game/version").toString();
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

	MUTEX.lock();
	if (isGameCanLaunch) {
		MUTEX.unlock();

		CurrentProgress progress2;
		progress2.matter = "处理java";
		progress2.number = 2;
		progress2.percent = 100;
		emit progress(progress2);
		QString GamePath('"' + dir.absolutePath() + "/Game/" + GameName + "/" + "Mindustry.jar" + '"');

		MUTEX.lock();
		if (isGameCanLaunch) {
			MUTEX.unlock();

			CurrentProgress progress3;
			progress3.matter = "启动游戏";
			progress3.number = 3;
			progress3.percent = 100;
			emit progress(progress3);
			QSettings setting("./SML/settings.ini", QSettings::IniFormat);
			setting.setValue("/game/isRunning", true);
			GameProcess.start("java -jar " + GamePath);

			CurrentProgress progress4;
			progress4.matter = "等待游戏窗口出现";
			progress4.number = 4;
			progress4.percent = 100;
			emit progress(progress4);
			while (FindWindowEx(NULL, NULL, NULL, L"Mindustry") == NULL);
			emit launched();

			while (isProcessExist("java.exe"));

			QThread::msleep(1000);
		}
		else
		{
			MUTEX.unlock();
		}
	}
	else
	{
		MUTEX.unlock();
	}
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

bool GameT::isProcessExist(QString ProcessName) {
	QProcess process;
	process.start("tasklist");
	process.waitForFinished();

	QByteArray result = process.readAllStandardOutput();
	QString str = result;
	if (str.contains(ProcessName))
		return true;
	else
		return false;
}

void GameT::ForceQuit() {
	QProcess::execute("taskkill /IM java.exe /F");
}

void GetOnlineGameVersionT::run() {
	logger::log(debug, "%q", QString::number(QSslSocket::supportsSsl()));
	logger::log(debug, "%q", QSslSocket::sslLibraryBuildVersionString());
	VerList.clear();
	QUrl url = QUrl::fromUserInput("https://api.github.com/repos/Anuken/Mindustry/releases");
	QNetworkAccessManager manager;
	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
	QNetworkReply* reply = manager.get(request);
	QEventLoop loop;
	connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	QTimer::singleShot(10000, &loop, &QEventLoop::quit);
	loop.exec();
	QByteArray array;
	if (reply->isFinished())
	{
		if (reply->error() == QNetworkReply::NoError)
		{
			array = reply->readAll();
			QJsonDocument doc = QJsonDocument::fromJson(array);
			QJsonArray JsonArray = doc.array();
			for (int i = 0; i < JsonArray.size(); i++) {
				QJsonObject object = JsonArray.at(i).toObject();
				VersionInfo VerInfo;
				VerInfo.name = "";
				VerInfo.ver = object.value("tag_name").toString();
				QJsonArray assets = object.value("assets").toArray();
				QJsonObject download = assets.at(0).toObject();
				VerInfo.DownloadURL = download.value("browser_download_url").toString().toUtf8();
				VerList.append(VerInfo);
			}
			emit GetVersionList(VerList);
		}
	}
	else
	{
		disconnect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
	}
}