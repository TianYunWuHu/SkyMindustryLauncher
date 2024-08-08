#ifndef _SML_SMLTHREAD_H_
#define _SML_SMLTHREAD_H_

#include "stdafx.h"
#include "scr/log.h"

struct VersionInfo
{
	QString name;
	int ver;
};

class GetVersionListT :
	public QThread
{

	Q_OBJECT

private:
	QList<VersionInfo> VerList;
	void run();

signals:
	void GetVersionList(QList<VersionInfo> verList);
};

class GameT :
	public QThread {

	Q_OBJECT

public:
	GameT(QString name);

private:
	QString GameName;
	QDir dir;
	QProcess GameProcess;
	void run();
	void CopyDir(QString src, QString dst);
};

#endif // !_SML_SMLTHREAD_H_
