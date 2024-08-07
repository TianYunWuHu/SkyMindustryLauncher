﻿#ifndef _SML_LOG_H_
#define _SML_LOG_H_

#include "stdafx.h"

enum LogLevel { info, warn, error, debug };

class logger {

public:
	static void initialize();
	static void log(LogLevel level, QString LogString ...);
};

#endif // !_SML_LOG_H_