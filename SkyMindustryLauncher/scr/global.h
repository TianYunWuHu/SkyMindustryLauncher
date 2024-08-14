#ifndef _SML_GLOBAL_H_
#define _SML_GLOBAL_H_

#include "stdafx.h"
#include "scr/SMLCustomControl.h"
#include "scr/SMLMessageBox.h"
#include "scr/SMLThread.h"
#include "scr/SMLWidgets.h"

//全局变量
extern GameT* GameMain;
extern bool isGameCanLaunch;
extern QMutex MUTEX;

#endif // !_SML_GLOBAL_H_