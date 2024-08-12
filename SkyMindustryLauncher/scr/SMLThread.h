#ifndef _SML_SMLTHREAD_H_
#define _SML_SMLTHREAD_H_

#include "stdafx.h"
#include "scr/log.h"

struct VersionInfo
{
	QString name;
	int ver;
};

struct CurrentProgress
{
	QString matter;//当前事项名称
	double number;//当前事项排位
	double percent;//当前事项进度
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

signals:
	void ProgressNumber(double);//总事件数量
	void progress(CurrentProgress);//每件事的进度
	void launched();
};

#endif // !_SML_SMLTHREAD_H_
