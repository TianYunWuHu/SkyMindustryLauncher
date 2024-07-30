#ifndef _SML_SMLTHREAD_H_
#define _SML_SMLTHREAD_H_

#include <qthread.h>
#include <qstring.h>
#include <qlist.h>
#include <qdir.h>
#include <qfileinfo.h>
#include <qsettings.h>
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

#endif // !_SML_SMLTHREAD_H_
