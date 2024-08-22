#include "global.h"
#include "stdafx.h"

GameT* GameMain = nullptr;
bool isGameCanLaunch = true;
QMutex MUTEX;
QWidget* MainWidget;
DownloadMainT* download = nullptr;